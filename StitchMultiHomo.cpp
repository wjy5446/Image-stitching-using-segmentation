#include "stdafx.h"
#include "MultiHomoStitcing_jy.h"
#include "Image.h"
#include "Mesh.h"
#include "meanshift\MeanShift.h"
#include <cmath>
#include "time.h"
#include "opencv2\core\core.hpp"
#include "opencv2\imgproc\imgproc.hpp"
#include "opencv2\features2d\features2d.hpp"
#include "opencv2\nonfree\nonfree.hpp"
#include "opencv2\nonfree\features2d.hpp"
#include "opencv2\calib3d\calib3d.hpp"

#include<vector>
using namespace cv;

#include "StitchMultiHomo.h"

CImageStitchMultiHomo::CImageStitchMultiHomo()
{
	homoAll = new Mat*[imgR.nH];
	
	for (int i = 0; i < imgR.nH; i++)
		homoAll[i] = new Mat[imgR.nW];
	
	arrClusterR = new vector<Point2f>[2];
	arrClusterL = new vector<Point2f>[2];
	arrInitKmean = new Point2f[2];
	arrCenter = new Point2f[2];
	labelCluster = NULL;
	arrMesh = NULL;
}

CImageStitchMultiHomo::~CImageStitchMultiHomo()
{
	if (arrMesh != NULL)
		delete[] arrMesh;
		
	if (labelCluster != NULL)
		delete[] labelCluster;
		
	for (int i = 0; i < imgR.nH; i++)
		delete[] homoAll[i];
	delete[] homoAll;
		
	delete[] arrClusterR;
	delete[] arrClusterL;
	delete[] arrInitKmean;
	delete[] arrCenter;
}

void CImageStitchMultiHomo::KMeansCluster(bool bForgy)
{
	int i, j;
	double dTempDis;
	int* arrNumData;
	int nTempX, nTempY;
	int nLabel;
	bool bChange = true;

	arrNumData = new int[2];

	//Kmean초기값 설정.
	if (bForgy == true)
	{
		RNG rng;
		int nRanSed;

		for (int i = 0; i < 2; i++)
		{
			nRanSed = rng.uniform(0, nNumPoint - 1);
			arrInitKmean[i] = GodPointR[nRanSed];
		}
	}

	if (bForgy == false)
	{
		arrInitKmean[0].y = 0;
		arrInitKmean[0].x = double(imgR.nW) / double(2);
		arrInitKmean[1].y = imgR.nH;
		arrInitKmean[1].x = double(imgR.nW) / double(2);
	}

	//k-means clustering
	for (i = 0; i < 2; i++)
	{
		arrCenter[i].y = arrInitKmean[i].y;
		arrCenter[i].x = arrInitKmean[i].x;
	}

	while (bChange)
	{
		bChange = false;

		// 중심과 cluster의 거리
		for (int i = 0; i < nNumPoint; i++)
		{
			nTempX = GodPointR[i].x;
			nTempY = GodPointR[i].y;


			for (int j = 0; j < 2; j++)
			{
				if (j == 0)
				{
					//dTempDis = GetDistance(nTempX, nTempY, arrCenter[j].x, arrCenter[j].y);
					dTempDis = abs(nTempY - arrCenter[j].y);
					nLabel = j;
				}
				else
				{
					if (dTempDis > abs(nTempY - arrCenter[j].y))
					{
						nLabel = j;
						dTempDis = abs(nTempY - arrCenter[j].y);
					}
				}
			}

			//변화가 있는지 확인한다.
			if (labelCluster[i] != nLabel)
			{
				bChange = true;
			}

			labelCluster[i] = nLabel;
		}

		// 다시 중심값을 구한다
		// Array 초기화
		for (int i = 0; i < 2; i++)
		{
			arrCenter[i].y = 0;
			arrCenter[i].x = 0;
			arrNumData[i] = 0;
		}

		for (int i = 0; i < nNumPoint; i++)
		{
			arrCenter[labelCluster[i]].y += GodPointR[i].y;
			arrCenter[labelCluster[i]].x += GodPointR[i].x;
			arrNumData[labelCluster[i]] ++;
		}

		for (int i = 0; i < 2; i++)
		{
			arrCenter[i].y /= arrNumData[i];
			arrCenter[i].x /= arrNumData[i];
		}
	}

	//Make arrClusterR
	for (int i = 0; i < nNumPoint; i++)
	{
		arrClusterR[labelCluster[i]].push_back(GodPointR[i]);
		arrClusterL[labelCluster[i]].push_back(GodPointL[i]);
	}

	delete[] arrNumData;
}

void CImageStitchMultiHomo::MakeMesh()
{
	int i, j;

	// value for meshed warp
	int nNumVertH, nNumVertW;
	int nRestVertH, nRestVertW;

	// mesh warp

	nRestVertH = (imgR.nH - 1) % 10;
	nRestVertW = (imgR.nW - 1) % 10;

	if (nRestVertH == 0)
		nNumVertH = (imgR.nH - 1) / 10;
	else
		nNumVertH = (imgR.nH - 1) / 10 + 1;

	if (nRestVertW == 0)
		nNumVertW = (imgR.nW - 1) / 10;
	else
		nNumVertW = (imgR.nW - 1) / 10 + 1;

	nNumVert = nNumVertH * nNumVertW;
	arrMesh = new CMesh[nNumVert];

	// Set vertice

	for (i = 0; i < nNumVertH - 1; i++)
		for (j = 0; j < nNumVertW - 1; j++)
		{
			arrMesh[i * nNumVertW + j] = CMesh(10 * i, 10 * j, 10);
		}

	for (i = 0; i < nNumVertH - 1; i++)
	{
		if (nRestVertW == 0)
		{
			nRestVertW = 10;
		}

		arrMesh[i * nNumVertW + nNumVertW - 1] = CMesh(10 * i, 10 * (nNumVertW - 1), 10, nRestVertW);
	}

	for (i = 0; i < nNumVertW - 1; i++)
	{
		if (nRestVertH == 0)
		{
			nRestVertH = 10;
		}

		arrMesh[(nNumVertH - 1)* nNumVertW + i] = CMesh(10 * (nNumVertH - 1), 10 * i, nRestVertH, 10);
	}

	arrMesh[(nNumVertH - 1)* nNumVertW + nNumVertW - 1] = CMesh(10 * (nNumVertH - 1), 10 * (nNumVertW - 1), nRestVertH, nRestVertW);
}


void CImageStitchMultiHomo::WarpMeshHomography()
{
	int i, j, x, y;

	// value for get Image size
	int offsetX = 0, offsetY = 0;
	int maxX_totalR = 0, maxY_totalR = 0, minX_totalR = imgR.nW, minY_totalR = imgR.nH;
	double maxX_transR = 0, maxY_transR = 0, minX_transR = imgR.nW, minY_transR = imgR.nH;
	int nH_total, nW_total;
	Point2f inputPoint, transPoint;

	// get image's size
	for (y = 0; y < imgR.nH; y++)
		for (x = 0; x < imgR.nW; x++)
		{
			inputPoint = Point2f(x, y);
			GetPerspectivePoint(inputPoint, transPoint, homoAll[y][x]);

			if (transPoint.y > maxY_transR) maxY_transR = transPoint.y;
			if (transPoint.x > maxX_transR) maxX_transR = transPoint.x;
			if (transPoint.y < minY_transR) minY_transR = transPoint.y;
			if (transPoint.x < minX_transR) minX_transR = transPoint.x;
		}

	//Set transformed Right Image
	if (minY_transR < 0) minY_totalR = minY_transR; else minY_totalR = 0;
	if (maxY_transR > imgR.nH - 1) maxY_totalR = maxY_transR; else maxY_totalR = imgR.nH - 1;

	if (minX_transR < 0) minX_totalR = minX_transR; else minX_totalR = 0;
	if (maxX_transR > imgR.nW - 1) maxX_totalR = maxX_transR; else maxX_totalR = imgR.nW - 1;

	nH_total = maxY_totalR - minY_totalR + 1;
	nW_total = maxX_totalR - minX_totalR + 1;

	if (minY_transR < 0) offsetY = abs(minY_transR);
	if (minX_transR < 0) offsetX = abs(minX_transR);

	//Set total Image's nH, nW

	imgTotal.imgRed = cmatrix(nH_total, nW_total); imgTotal.imgGreen = cmatrix(nH_total, nW_total); imgTotal.imgBlue = cmatrix(nH_total, nW_total);
	imgLtrans.imgRed = cmatrix(nH_total, nW_total); imgLtrans.imgGreen = cmatrix(nH_total, nW_total); imgLtrans.imgBlue = cmatrix(nH_total, nW_total);
	imgRtrans.imgRed = cmatrix(nH_total, nW_total); imgRtrans.imgGreen = cmatrix(nH_total, nW_total); imgRtrans.imgBlue = cmatrix(nH_total, nW_total);
	weightRtrans = dmatrix(nH_total, nW_total); weightLtrans = dmatrix(nH_total, nW_total);

	imgTotal.nH = nH_total;	imgTotal.nW = nW_total;
	imgLtrans.nH = nH_total; imgLtrans.nW = nW_total;
	imgRtrans.nH = nH_total; imgRtrans.nW = nW_total;


	//WarpSeamWeight

	for (int y = 0; y < nH_total; y++)
		for (int x = 0; x < nW_total; x++)
		{
			imgLtrans.imgRed[y][x] = 0; imgLtrans.imgGreen[y][x] = 0; imgLtrans.imgBlue[y][x] = 0;
			imgRtrans.imgRed[y][x] = 0; imgRtrans.imgGreen[y][x] = 0; imgRtrans.imgBlue[y][x] = 0;
			weightRtrans[y][x] = 0; weightLtrans[y][x] = 0;
		}

	for (int y = 0; y < imgL.nH; y++)
		for (int x = 0; x < imgL.nW; x++)
		{
			weightLtrans[y + offsetY][x + offsetX] = weightSeamL[y][x];
			imgLtrans.imgRed[y + offsetY][x + offsetX] = imgL.imgRed[y][x];
			imgLtrans.imgGreen[y + offsetY][x + offsetX] = imgL.imgGreen[y][x];
			imgLtrans.imgBlue[y + offsetY][x + offsetX] = imgL.imgBlue[y][x];
		}

	//----------------------------------------------------------------------------------------
	// Set deformed vertice
	int nTmpX, nTmpY;

	for (i = 0; i < nNumVert; i++)
	{
		arrMesh[i].DeformMesh(false, homoAll);
	}

	//inverse mesh warp
	for (i = 0; i < nNumVert; i++)
	{
		// inverse warping
		for (y = arrMesh[i].nMinY; y <= arrMesh[i].nMaxY; y++)
			for (x = arrMesh[i].nMinX; x <= arrMesh[i].nMaxX; x++) {

				inputPoint = Point2f(x, y);

				Mat inverH = arrMesh[i].Homo.inv();
				GetPerspectivePoint(inputPoint, transPoint, inverH);

				if (transPoint.x >= arrMesh[i].vertex[0].x
					&& transPoint.x <= arrMesh[i].vertex[3].x
					&& transPoint.y >= arrMesh[i].vertex[0].y
					&& transPoint.y <= arrMesh[i].vertex[3].y)
				{
					imgRtrans.imgRed[y + offsetY][x + offsetX] = imgR.imgRed[int(transPoint.y)][int(transPoint.x)];
					imgRtrans.imgGreen[y + offsetY][x + offsetX] = imgR.imgGreen[int(transPoint.y)][int(transPoint.x)];
					imgRtrans.imgBlue[y + offsetY][x + offsetX] = imgR.imgBlue[int(transPoint.y)][int(transPoint.x)];
					weightRtrans[y + offsetY][x + offsetX] = weightSeamR[int(transPoint.y)][int(transPoint.x)];
				}
			}
	}

	for (y = 0; y < nH_total; y++)
		for (x = 0; x < nW_total; x++)
		{
			if (weightRtrans[y][x] > weightLtrans[y][x])
			{
				weightRtrans[y][x] = 1;
				weightLtrans[y][x] = 0;
			}

			if (weightLtrans[y][x] > weightRtrans[y][x])
			{
				weightLtrans[y][x] = 1;
				weightRtrans[y][x] = 0;
			}
		}

	this->MultiBandBlending(nH_total, nW_total, imgTotal.imgRed, imgTotal.imgGreen, imgTotal.imgBlue);

	DisplayCimage2DColor(imgTotal.imgRed, imgTotal.imgGreen, imgTotal.imgBlue, nW_total, nH_total, 0, 0);
}

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

#include "StitchKMeans.h"


CImageStitchKMeans::CImageStitchKMeans()
{
	arrHomo = new Mat[2];
	weightMap = dmatrix(imgR.nH, imgR.nW);
}

CImageStitchKMeans::~CImageStitchKMeans()
{
	delete[] arrHomo;
	free_dmatrix(weightMap, imgR.nH, imgR.nW);
}

void CImageStitchKMeans::RemoveOutlier(vector<Point2f>& pointR, vector<Point2f>& pointL, double thresh)
{
	int i;
	int nTmp = 0, nNum;
	vector<Point2f> tmpPointR, tmpPointL;
	double distance;
	Point2f inputPoint, outputPoint;
	Mat homo;

	nNum = pointR.size();

	homo = findHomography(pointR, pointL, CV_RANSAC);

	for (i = 0; i < nNum; i++)
	{
		inputPoint.x = pointR[i].x;
		inputPoint.y = pointR[i].y;

		GetPerspectivePoint(inputPoint, outputPoint, homo);

		distance = GetDistance(pointL[i].x, pointL[i].y, outputPoint.x, outputPoint.y);

		if (distance < thresh)
		{
			tmpPointR.push_back(pointR[i]);
			tmpPointL.push_back(pointL[i]);
			nTmp++;
		}
	}

	pointR.clear();
	pointL.clear();

	for (i = 0; i < nTmp; i++)
	{
		pointR.push_back(tmpPointR[i]);
		pointL.push_back(tmpPointL[i]);
	}
}

double CImageStitchKMeans::GetInlierRate(vector<Point2f>& pointR, vector<Point2f>& pointL, Mat& homo, double thresh)
{
	int i;
	int nNum = 0;
	int nNumInlier = 0;
	double distance;
	double dInlierRate = 0;
	Point2f inputPoint, outputPoint;

	nNum = pointR.size();

	homo = findHomography(pointR, pointL, CV_RANSAC);

	for (i = 0; i < nNum; i++)
	{
		inputPoint.x = pointR[i].x;
		inputPoint.y = pointR[i].y;

		GetPerspectivePoint(inputPoint, outputPoint, homo);

		distance = GetDistance(pointL[i].x, pointL[i].y, outputPoint.x, outputPoint.y);

		if (distance < thresh)
		{
			nNumInlier++;
		}
	}

	dInlierRate = double(nNumInlier) / double(nNum);

	return dInlierRate;
}

void CImageStitchKMeans::DrawFeature()
{
	int x, y, i, j;

	//-----------------------------------------
	//-------------<Layer 확인>---------------
	int ** arrColor;
	int nNum;

	arrColor = imatrix(2, 3);

	GetRandColorArray(arrColor, 2);

	for (i = 0; i < 2; i++)
	{
		nNum = arrClusterR[i].size();

		for (j = 0; j < nNum; j++)
		{
			y = arrClusterR[i][j].y;
			x = arrClusterR[i][j].x;
			DrawCross(imgMark.imgRed, imgR.nW, imgR.nH, CPoint(x, y), arrColor[i][0]);
			DrawCross(imgMark.imgGreen, imgR.nW, imgR.nH, CPoint(x, y), arrColor[i][1]);
			DrawCross(imgMark.imgBlue, imgR.nW, imgR.nH, CPoint(x, y), arrColor[i][2]);
		}
	}

	DisplayCimage2DColor(imgMark.imgRed, imgMark.imgGreen, imgMark.imgBlue, imgR.nW, imgR.nH, 0, 0);

	free_imatrix(arrColor, 2, 3);
}

void CImageStitchKMeans::RemoveOutlier1st(double dThresh)
{
	RemoveOutlier(GodPointR, GodPointL, dThresh);
}

void CImageStitchKMeans::KMeansCluster()
{
	int i, j;
	double dTempDis, dTmp;
	int* arrNumData;
	double dTempX, dTempY;
	int nLabel;
	int nNum;
	bool bChange = true;

	nNum = GodPointR.size();

	// 메모리 할당
	arrNumData = new int[2];
	labelCluster = new int[nNum];

	//Kmean초기값 설정.
	RNG rng;
	int nRanSed;

	for (int i = 0; i < 2; i++)
	{
		nRanSed = rng.uniform(0, nNum - 1);
		arrInitKmean[i] = GodPointR[nRanSed];
	}

	//k-means clustering
	for (i = 0; i < 2; i++)
	{
		arrCenter[i] = arrInitKmean[i];
	}

	while (bChange)
	{
		bChange = false;

		// 중심과 cluster의 거리
		for (i = 0; i < nNum; i++)
		{
			dTempX = GodPointR[i].x;
			dTempY = GodPointR[i].y;

			for (j = 0; j < 2; j++)
			{
				dTmp = GetDistance(dTempX, dTempY, double(arrCenter[j].x), double(arrCenter[j].y));

				if (j == 0)
				{
					dTempDis = dTmp;
					nLabel = j;
				}
				else
				{
					if (dTempDis > dTmp)
					{
						nLabel = j;
						dTempDis = dTmp;
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
		for (i = 0; i < 2; i++)
		{
			arrCenter[i].y = 0;
			arrCenter[i].x = 0;
			arrNumData[i] = 0;
		}

		for (i = 0; i < nNum; i++)
		{
			arrCenter[labelCluster[i]].y += GodPointR[i].y;
			arrCenter[labelCluster[i]].x += GodPointR[i].x;
			arrNumData[labelCluster[i]] ++;
		}

		for (i = 0; i < 2; i++)
		{
			arrCenter[i].y /= arrNumData[i];
			arrCenter[i].x /= arrNumData[i];
		}
	}

	//Make arrClusterR
	for (i = 0; i < nNum; i++)
	{
		arrClusterR[labelCluster[i]].push_back(GodPointR[i]);
		arrClusterL[labelCluster[i]].push_back(GodPointL[i]);
	}

	delete[] arrNumData;
}

void CImageStitchKMeans::RemoveOutlier2nd(double dThresh)
{
	int i;
	bool bBreak = true;
	double dInlierRate;
	double* arrThresh;
	Mat tmpHomo;

	arrThresh = new double[2];

	for (i = 0; i < 2; i++)
	{
		arrThresh[i] = dThresh;
	}

	for (i = 0; i < 2; i++)
	{
		while (bBreak)
		{
			bBreak = false;

			dInlierRate = GetInlierRate(arrClusterR[i], arrClusterL[i], tmpHomo, dThresh);

			if (dInlierRate < 0.2)
			{
				arrThresh[i] += 5;
				bBreak = true;
			}
			else
			{
				arrHomo[i] = tmpHomo;
			}
		}

		RemoveOutlier(arrClusterR[i], arrClusterL[i], arrThresh[i]);
		bBreak = true;
	}

	delete[] arrThresh;

	//DrawFeature();
}

void CImageStitchKMeans::BlendingHomo()
{
	int y, x, i, j;
	int tmp;
	double TopDistance, BtmDistance;

	for (y = 0; y < imgR.nH; y++)
		for (x = 0; x < imgR.nW; x++)
		{
			// Top Set와 제일 가까운 값 구하기
			for (i = 0; i < arrClusterR[0].size(); i++) {
				if (i == 0) { TopDistance = GetDistance(double(x), double(y), double(arrClusterR[0][i].x), double(arrClusterR[0][i].y)); }
				else{
					tmp = GetDistance(double(x), double(y), double(arrClusterR[0][i].x), double(arrClusterR[0][i].y));
					if (tmp < TopDistance) TopDistance = tmp;
				}
			}

			// Botttom Set와 제일 가까운 값 구하기
			for (i = 0; i < arrClusterR[1].size(); i++) {
				if (i == 0) { BtmDistance = GetDistance(double(x), double(y), double(arrClusterR[1][i].x), double(arrClusterR[1][i].y)); }
				else{
					tmp = GetDistance(double(x), double(y), double(arrClusterR[1][i].x), double(arrClusterR[1][i].y));
					if (tmp < BtmDistance) BtmDistance = tmp;
				}
			}

			weightMap[y][x] = (TopDistance) / (TopDistance + BtmDistance);
		}

	// homography blending
	for (int y = 0; y < imgR.nH; y++)
		for (int x = 0; x < imgR.nW; x++)
		{
			homoAll[y][x] = Mat(cvSize(3, 3), CV_64FC1, cvScalar(0));
			for (int i = 0; i < 3; i++)
				for (int j = 0; j < 3; j++)
				{
					homoAll[y][x].at<double>(i, j) = (weightMap[y][x] * arrHomo[1].at<double>(i, j)) + ((1 - weightMap[y][x]) * arrHomo[0].at<double>(i, j));
				}
		}
}
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

#include "StitchLayer.h"
CImageStitchLayer::CImageStitchLayer()
{
}

CImageStitchLayer::~CImageStitchLayer()
{
	delete[] nNumSetPoint;
}

void CImageStitchLayer::FindLayer(int nThresh)
{
	int i, j, x, y;
	vector < Point2f > tmpVecL, tmpVecR;
	vector <Point2f> inlierL, inlierR;
	vector <Point2f> outlierL, outlierR;
	vector <Point2f> emptyVec;
	Mat H;
	Point2f input, output, nullPoint(0, 0);
	double dTmp;
	int nSize;

	for (i = 0; i < nNumPoint; i++) {
		tmpVecR.push_back(GodPointR[i]);
		tmpVecL.push_back(GodPointL[i]);
	}

	nSize = nNumPoint;

	while (nSize > nThresh)
	{
		H = findHomography(tmpVecR, tmpVecL, CV_RANSAC);

		// inlier, outlier 판단.
		for (i = 0; i < nSize; i++)
		{
			input = tmpVecR[i];
			GetPerspectivePoint(input, output, H);
			dTmp = GetDistance(tmpVecL[i].x, tmpVecL[i].y, output.x, output.y);

			if (dTmp < 3)
			{
				inlierL.push_back(tmpVecL[i]);
				inlierR.push_back(tmpVecR[i]);
			}
			else
			{
				outlierL.push_back(tmpVecL[i]);
				outlierR.push_back(tmpVecR[i]);
			}
		}

		// Layer의 inlier set과 homography 배열에 저장
		if (inlierL.size() > nThresh)
		{
			SetPointLayerL.push_back(inlierL);
			SetPointLayerR.push_back(inlierR);
			setHomoLayer.push_back(H);
		}

		tmpVecR.clear();
		tmpVecL.clear();

		// 나머지 outlier에서 새로운 호모그래피를 구하기 위해
		// tmpVec과 nSize 다시 재정의
		nSize = outlierL.size();

		for (i = 0; i < nSize; i++){
			tmpVecL.push_back(outlierL[i]);
			tmpVecR.push_back(outlierR[i]);
		}

		inlierR.clear();
		inlierL.clear();
		outlierR.clear();
		outlierL.clear();
	}

	//this->DrawLayer();
}

void CImageStitchLayer::BlendingHomo(double sigma)
{
	int i, j, y, x;
	//Blending homography.

	double* dNearDis;
	double* dWeight;
	double tmp, alpa, weightSum = 0;
	Point2f tmpPoint, tmpCen;
	Mat tmpH;

	nNumLayer = SetPointLayerR.size();

	dNearDis = new double[nNumLayer];
	dWeight = new double[nNumLayer];
	nNumSetPoint = new int[nNumLayer];

	for (i = 0; i < nNumLayer; i++)
	{
		nNumSetPoint[i] = SetPointLayerR[i].size();
		dNearDis[i] = 999;
		dWeight[i] = 0;
	}


	for (i = 0; i < nNumVert; i++)
	{
		tmpH = Mat(cvSize(3, 3), CV_64FC1, cvScalar(0));
		tmpCen = arrMesh[i].central;

		//각 Layer별 최소거리 구하기.
		for (j = 0; j < nNumLayer; j++)
		{
			for (int k = 0; k < nNumSetPoint[j]; k++)
			{
				tmpPoint.x = SetPointLayerR[j].at(k).x;
				tmpPoint.y = SetPointLayerR[j].at(k).y;

				tmp = GetDistance(tmpCen.x, tmpCen.y, tmpPoint.x, tmpPoint.y);

				if (tmp < dNearDis[j])
				{
					dNearDis[j] = tmp;
				}
			}

			//Get Weight
			tmp = -1 * dNearDis[j] * dNearDis[j] / (sigma * sigma);
			dWeight[j] = exp(tmp);
			weightSum += dWeight[j];
		}

		//Blending 
		for (j = 0; j < nNumLayer; j++)
		{
			dWeight[j] /= weightSum;
			for (int a = 0; a < 3; a++)
				for (int b = 0; b < 3; b++)
				{
					tmpH.at<double>(a, b) += (dWeight[j] * setHomoLayer[j].at<double>(a, b));
				}
		}

		for (y = arrMesh[i].vertex[0].y; y <= arrMesh[i].vertex[3].y; y++)
			for (x = arrMesh[i].vertex[0].x; x <= arrMesh[i].vertex[3].x; x++)
			{
				homoAll[y][x] = tmpH;
			}

		for (j = 0; j < nNumLayer; j++)
		{
			dNearDis[j] = 999;
		}

		weightSum = 0;
	}

	delete[] dNearDis;
	delete[] dWeight;
}

void CImageStitchLayer::DrawLayer()
{
	int x, y, i, j;

	//-----------------------------------------
	//-------------<Layer 확인>---------------
	int ** arrColor;
	int nNumPoint;
	int nSize;

	nSize = SetPointLayerL.size();
	arrColor = imatrix(nSize, 3);

	GetRandColorArray(arrColor, nSize);

	for (i = 0; i < nSize; i++)
	{
		nNumPoint = SetPointLayerR[i].size();

		for (j = 0; j < nNumPoint; j++)
		{
			y = SetPointLayerR[i].at(j).y;
			x = SetPointLayerR[i].at(j).x;
			DrawCross(imgMark.imgRed, imgR.nW, imgR.nH, CPoint(x, y), arrColor[i][0]);
			DrawCross(imgMark.imgGreen, imgR.nW, imgR.nH, CPoint(x, y), arrColor[i][1]);
			DrawCross(imgMark.imgBlue, imgR.nW, imgR.nH, CPoint(x, y), arrColor[i][2]);
		}
	}

	DisplayCimage2DColor(imgMark.imgRed, imgMark.imgGreen, imgMark.imgBlue, imgR.nW, imgR.nH, 0, 0);

	free_imatrix(arrColor, nSize, 3);
}
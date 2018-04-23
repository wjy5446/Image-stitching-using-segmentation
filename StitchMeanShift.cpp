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

#include "StitchMeanShift.h"

//=================================================================================================================================================
//=====================================================<Meanshift Image Stitching>=================================================================
//=================================================================================================================================================

CImageStitchMeanShift::CImageStitchMeanShift()
{
	arrErr = NULL;
	arrInfo = NULL;
	arrFeatureNum = NULL;
	arrRegion = NULL;
	arrHomo = NULL;

	LabelMeanShift = imatrix(imgR.nH, imgR.nW);
	weightObj = cmatrix(imgR.nH, imgR.nW);

	imgSegment.imgRed = cmatrix(imgR.nH, imgR.nW);
	imgSegment.imgGreen = cmatrix(imgR.nH, imgR.nW);
	imgSegment.imgBlue = cmatrix(imgR.nH, imgR.nW);
	imgSegment.nH = imgR.nH;
	imgSegment.nW = imgR.nW;
}

CImageStitchMeanShift::CImageStitchMeanShift(BYTE* img1D_L, BYTE* img1D_R, int nHL, int nWL, int nHR, int nWR, CString fileName) : CImageStitchBasic(img1D_L, img1D_R, nHL, nWL, nHR, nWR)
{
	arrErr = NULL;
	arrInfo = NULL;
	arrFeatureNum = NULL;
	arrRegion = NULL;
	arrHomo = NULL;

	strFileName = fileName;
	LabelMeanShift = imatrix(imgR.nH, imgR.nW);
	weightObj = cmatrix(imgR.nH, imgR.nW);

	imgSegment.imgRed = cmatrix(imgR.nH, imgR.nW);
	imgSegment.imgGreen = cmatrix(imgR.nH, imgR.nW);
	imgSegment.imgBlue = cmatrix(imgR.nH, imgR.nW);
	imgSegment.nH = imgR.nH;
	imgSegment.nW = imgR.nW;
}

CImageStitchMeanShift::~CImageStitchMeanShift()
{
	if (arrErr != NULL)
		delete[] arrErr;

	if (arrInfo != NULL)
		delete[] arrInfo;

	if (arrFeatureNum != NULL)
		delete[] arrFeatureNum;

	if (arrRegion != NULL)
		delete[] arrRegion;

	if (arrHomo != NULL)
		delete[] arrHomo;

	free_imatrix(LabelMeanShift, imgR.nH, imgR.nW);
	free_cmatrix(weightObj, imgR.nH, imgR.nW);
	delete[] arrObjIdx;

	free_cmatrix(imgSegment.imgRed, imgR.nH, imgR.nW);
	free_cmatrix(imgSegment.imgGreen, imgR.nH, imgR.nW);
	free_cmatrix(imgSegment.imgBlue, imgR.nH, imgR.nW);
}

/****************************************************
<SegmentMeanshift>

Byte Image를 IplImage로 바꾸고 MeanShift segmentation를 해준다.

minFeature 이상의 특징점을 가지고 있는 Region Number와
minArea 이상의 영역을 가지고 있는 Region Number을 골라준다.
그리고 그 영역에 대한 Index 배열을 가진다.

------------------------------------
Pre : imgR.nW, imgR.nH, imgR.imgRed, imgR.imgBlue, imgR.imgGreen, minrRegion, minFeature, minArea
Post : nNumSeg, LabelMeanShift

*******************************************************/
void CImageStitchMeanShift::SegmentMeanshift(int minRegion)
{
	int i, j, x, y;

	IplImage* inputIplImg;
	IplImage* resultIplImg;

	int nIdx;
	int tmpX, tmpY;
	int tmp = 0;

	inputIplImg = cvCreateImage(cvSize(imgR.nW, imgR.nH), IPL_DEPTH_8U, 3);
	resultIplImg = cvCreateImage(cvSize(imgR.nW, imgR.nH), IPL_DEPTH_8U, 3);

	for (y = 0; y < imgR.nH; y++)
		for (x = 0; x < imgR.nW; x++)
		{
			nIdx = y * inputIplImg->widthStep + x * inputIplImg->nChannels;
			inputIplImg->imageData[nIdx] = imgR.imgBlue[y][x];
			inputIplImg->imageData[nIdx + 1] = imgR.imgGreen[y][x];
			inputIplImg->imageData[nIdx + 2] = imgR.imgRed[y][x];
		}

	nNumSeg = MeanShift(inputIplImg, LabelMeanShift, resultIplImg, minRegion);

	// 전체 Region를 Obj후보군으로 설정
	nNumObjCandi = nNumSeg;
	arrObjIdx = new int[nNumSeg];
	arrErr = new int[nNumSeg];
	arrInfo = new double[nNumSeg];
	arrFeatureNum = new int[nNumSeg];
	arrRegion = new int[nNumSeg];
	arrHomo = new Mat[nNumSeg];
	
	for (i = 0; i < nNumSeg; i++)
	{
		arrObjIdx[i] = i;
		arrErr[i] = -1;
		arrInfo[i] = -1;
		arrFeatureNum[i] = 0;
		arrRegion[i] = 0;
		arrHomo[i] = 0;
	}

	// Set FeatreNum, Region, Homo
	GetArrFeatureNum();
	GetArrRegion();
	GetArrHomo();

	// Set Global Homography
	homoGlo = findHomography(GodPointR, GodPointL, CV_RANSAC);

	//DrawSegment();

	cvReleaseImage(&inputIplImg);
	cvReleaseImage(&resultIplImg);
}


void CImageStitchMeanShift::FindSingleObject(float fDiffInlierRate, bool bDrawErrInfo)
{
	int i, j;
	double dTmp;
	double dMaxInlier=0;
	int* arrTmp;
	Mat tmpHomo;
	vector <Point2f> objPointR, objPointL;

	// Inlier Rate가 가장 높은 Index, Background Homography 구하기
	for (i = 0; i < nNumSeg; i++)
	{
		if (arrObjIdx[i] != -1)
		{
			arrTmp = new int[1];
			arrTmp[0] = arrObjIdx[i];

			arrInfo[i] = GetExceptInlierRate(arrTmp, 1, 0, tmpHomo);

			if (dMaxInlier < arrInfo[i])
			{
				dMaxInlier = arrInfo[i];
				nIdxObj = arrObjIdx[i];
				homoBack = tmpHomo;
			}

			delete arrTmp;
		}
	}

	// Set Object Homography
	homoObj = arrHomo[nIdxObj];

	// 최고 물체와 나머지의 평균의 차이를 비교
	double meanInlier=0;
	int tmpNum = 0;

	for (i = 0; i < nNumSeg; i++)
	{
		if (arrObjIdx[i] != -1)
		{
			if (arrInfo[i] != dMaxInlier)
			{
				meanInlier += arrInfo[i];
				tmpNum++;
			}
		}
	}

	meanInlier /= tmpNum;

	if (bDrawErrInfo)
		DrawActiveErr();

	if (dMaxInlier - meanInlier < fDiffInlierRate)
		bExeDualHomo = false;
}

/****************************************************
<DetermineActive>

여러 기준을 통해 Object 후보군을 판별한다.

후보군 판별 후 새로운 후보군 array를 작성한다.
------------------------------------
Pre : 
Post : arrObjIdx, nNumObjCandi, arrObjInlierRate

*******************************************************/
void CImageStitchMeanShift::DetermineActive(int minFeature, int minArea, double minFeatureRate, int minKLdiver, bool bDrawErrInfo)
{
	int i;
	int tmpNum = 0;
	bool bOnFeatureRate;

	ActiveFeatureNum(minFeature);
	ActiveRegion(minArea);
	bOnFeatureRate = ActiveFeatureRate(minFeatureRate);
	ActiveMisMatch();
	ActiveKLdivergence(minKLdiver);

	//featureRate가 높을 때에는 바로 빠져나가게 한다.
	if (bOnFeatureRate)
	{
		bExeDualHomo = false;

		if (bDrawErrInfo)
			DrawActiveErr();
			
		return;
	}

	// arrObjIdx, nNumObjCandi 다시 판별
	// tmpNum 구하기
	for (i = 0; i < nNumObjCandi; i++)
	{
		if (arrObjIdx[i] != -1)
		{
			tmpNum++;
		}
	}

	// tmpIdx 구하기
	if (tmpNum <= 1)
	{
		bExeDualHomo = false;
		return;
	}                                                              
	else
	{
		bExeDualHomo = true;
	}
}

void CImageStitchMeanShift::MakeWeightMap(int dilation, bool bOnDraw)
{
	int x, y, i, j;

	BYTE ** tmpWeight;
	int** tmpLabel;
	int windowSize;

	windowSize = imgR.nW / 10;

	tmpWeight = cmatrix(imgR.nH, imgR.nW);
	tmpLabel = imatrix(imgR.nH, imgR.nW);

	for (i = 0; i < imgR.nH; i++)
		for (j = 0; j < imgR.nW; j++)
		{
			if (LabelMeanShift[i][j] == nIdxObj)
				weightObj[i][j] = 255;
			else
				weightObj[i][j] = 0;
		}

	// open
	ErosionBinary(weightObj, tmpWeight, imgR.nW, imgR.nH, 0, 10);
	DilationBinary(tmpWeight, weightObj, imgR.nW, imgR.nH, 0, 10);

	// Labeling
	Labeling(weightObj, tmpLabel, imgR.nW, imgR.nH, 1000);

	for (y = 0; y < imgR.nH; y++)
		for (x = 0; x < imgR.nW; x++)
		{
			if (tmpLabel[y][x] == 0)
			{
				weightObj[y][x] = 255;
			}
			else
				weightObj[y][x] = 0;
		}


	//Fill Algorithm
	Fill(weightObj, imgR.nW, imgR.nH);


	// dilation
	DilationBinary(weightObj, tmpWeight, imgR.nW, imgR.nH, 0, dilation);

	for (y = 0; y < imgR.nH; y++)
		for (x = 0; x < imgR.nW; x++)
		{
			weightObj[y][x] = tmpWeight[y][x];
		}

	if (bOnDraw)
	{
		DisplayCimage2D(weightObj, imgR.nW, imgR.nH, 0, 0);
	}

	free_cmatrix(tmpWeight, imgR.nH, imgR.nW);
	free_imatrix(tmpLabel, imgR.nH, imgR.nW);
}

void CImageStitchMeanShift::WarpHomography(bool bWrite)
{
	int x, y, i;

	Image2D imgBack;

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
			inputPoint = cvPoint(x, y);

			if (bExeDualHomo)
				GetPerspectivePoint(inputPoint, transPoint, homoBack);
			else
				GetPerspectivePoint(inputPoint, transPoint, homoGlo);

			if (transPoint.y > maxY_transR) maxY_transR = transPoint.y;
			if (transPoint.x > maxX_transR) maxX_transR = transPoint.x;
			if (transPoint.y < minY_transR) minY_transR = transPoint.y;
			if (transPoint.x < minX_transR) minX_transR = transPoint.x;
		}

	if (minY_transR < 0) minY_totalR = minY_transR; else minY_totalR = 0;
	if (maxY_transR > imgR.nH - 1) maxY_totalR = maxY_transR; else maxY_totalR = imgR.nH - 1;

	if (minX_transR < 0) minX_totalR = minX_transR; else minX_totalR = 0;
	if (maxX_transR > imgR.nW - 1) maxX_totalR = maxX_transR; else maxX_totalR = imgR.nW - 1;

	nH_total = maxY_totalR - minY_totalR + 1;
	nW_total = maxX_totalR - minX_totalR + 1;

	if (minY_transR < 0) offsetY = abs(minY_transR);
	if (minX_totalR < 0) offsetX = abs(minX_totalR);

	if (nH_total > 10000 || nW_total > 10000)
	{
		//// 글씨 넣기
		FILE *ferr;
		CString errPos;

		// 파일 열기
		errPos = _T("C:\\Users\\vision_jy\\Desktop\\1. Image DB\\Image Stitching\\실험폴더\\no\\");
		errPos += strFileName;
		errPos += _T("err.txt");

		ferr = fopen(errPos, "w");

		fprintf(ferr, "Homography is error!!!");
		
		fclose(ferr);

		return;
	}

	//Set total Image's nH, nW
	imgTotal.imgRed = cmatrix(nH_total, nW_total); imgTotal.imgGreen = cmatrix(nH_total, nW_total); imgTotal.imgBlue = cmatrix(nH_total, nW_total);
	imgBack.imgRed = cmatrix(nH_total, nW_total); imgBack.imgGreen = cmatrix(nH_total, nW_total); imgBack.imgBlue = cmatrix(nH_total, nW_total);
	imgLtrans.imgRed = cmatrix(nH_total, nW_total); imgLtrans.imgGreen = cmatrix(nH_total, nW_total); imgLtrans.imgBlue = cmatrix(nH_total, nW_total);
	imgRtrans.imgRed = cmatrix(nH_total, nW_total); imgRtrans.imgGreen = cmatrix(nH_total, nW_total); imgRtrans.imgBlue = cmatrix(nH_total, nW_total);
	weightLtrans = dmatrix(nH_total, nW_total); weightRtrans = dmatrix(nH_total, nW_total);

	imgTotal.nH = nH_total;	imgTotal.nW = nW_total;
	imgLtrans.nH = nH_total; imgLtrans.nW = nW_total;
	imgRtrans.nH = nH_total; imgRtrans.nW = nW_total;

	for (int y = 0; y < nH_total; y++)
		for (int x = 0; x < nW_total; x++)
		{
			imgTotal.imgRed[y][x] = 0; imgTotal.imgGreen[y][x] = 0; imgTotal.imgBlue[y][x] = 0;
			imgBack.imgRed[y][x] = 0; imgBack.imgGreen[y][x] = 0; imgBack.imgBlue[y][x] = 0;

			imgLtrans.imgRed[y][x] = 0; imgLtrans.imgGreen[y][x] = 0; imgLtrans.imgBlue[y][x] = 0;
			imgRtrans.imgRed[y][x] = 0; imgRtrans.imgGreen[y][x] = 0; imgRtrans.imgBlue[y][x] = 0;

			weightLtrans[y][x] = 0; weightRtrans[y][x] = 0;
		}

	//=====================================================================================================================================================
	// 싱글 호모그래피
	if (!bExeDualHomo)
	{
		// 왼쪽
		for (y = 0; y < imgL.nH; y++)
			for (x = 0; x < imgL.nW; x++)
			{
				imgLtrans.imgRed[y + offsetY][x + offsetX] = imgL.imgRed[y][x];
				imgLtrans.imgGreen[y + offsetY][x + offsetX] = imgL.imgGreen[y][x];
				imgLtrans.imgBlue[y + offsetY][x + offsetX] = imgL.imgBlue[y][x];
				weightLtrans[y + offsetY][x + offsetX] = weightSeamL[y][x];
			}

		// 오른쪽
		Mat inverH = homoGlo.inv();

		for (y = minY_transR; y < maxY_transR; y++)
			for (x = minX_transR; x < maxX_transR; x++) {

				inputPoint = Point2f(x, y);
				GetPerspectivePoint(inputPoint, transPoint, inverH);
				if (transPoint.x >= 0 && transPoint.x < imgR.nW && transPoint.y >= 0 && transPoint.y < imgR.nH)
				{
					imgRtrans.imgRed[y + offsetY][x + offsetX] = BilinearInterpolation(transPoint, imgR.imgRed, imgR.nH, imgR.nW);
					imgRtrans.imgGreen[y + offsetY][x + offsetX] = BilinearInterpolation(transPoint, imgR.imgGreen, imgR.nH, imgR.nW);;
					imgRtrans.imgBlue[y + offsetY][x + offsetX] = BilinearInterpolation(transPoint, imgR.imgBlue, imgR.nH, imgR.nW);;
					weightRtrans[y + offsetY][x + offsetX] = weightSeamR[int(transPoint.y)][int(transPoint.x)];
				}
			}

		// Seam 만들기
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
					weightRtrans[y][x] = 0;
					weightLtrans[y][x] = 1;
				}
			}

		// multiband blending
		this->MultiBandBlending(imgTotal.nH, imgTotal.nW, imgTotal.imgRed, imgTotal.imgGreen, imgTotal.imgBlue);
	}

	//==================================================================================================================================
	// 듀얼 호모그래피
	if (bExeDualHomo)
	{
		// 배경이미지 합성.
		for (int y = 0; y < imgL.nH; y++)
			for (int x = 0; x < imgL.nW; x++)
			{
				imgLtrans.imgRed[y + offsetY][x + offsetX] = imgL.imgRed[y][x];
				imgLtrans.imgGreen[y + offsetY][x + offsetX] = imgL.imgGreen[y][x];
				imgLtrans.imgBlue[y + offsetY][x + offsetX] = imgL.imgBlue[y][x];

				weightLtrans[y + offsetY][x + offsetX] = weightSeamL[y][x];
			}

		Mat inverBackH = homoBack.inv();

		for (int y = minY_transR; y < maxY_transR; y++)
			for (int x = minX_transR; x < maxX_transR; x++) {

				inputPoint = cvPoint(x, y);
				GetPerspectivePoint(inputPoint, transPoint, inverBackH);

				if (transPoint.x >= 0 && transPoint.x < imgR.nW && transPoint.y >= 0 && transPoint.y < imgR.nH)
				{
					imgRtrans.imgRed[y + offsetY][x + offsetX] = BilinearInterpolation(transPoint, imgR.imgRed, imgR.nH, imgR.nW);
					imgRtrans.imgGreen[y + offsetY][x + offsetX] = BilinearInterpolation(transPoint, imgR.imgGreen, imgR.nH, imgR.nW);
					imgRtrans.imgBlue[y + offsetY][x + offsetX] = BilinearInterpolation(transPoint, imgR.imgBlue, imgR.nH, imgR.nW);

					if (weightObj[int(transPoint.y)][int(transPoint.x)] == 0){
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
					weightRtrans[y][x] = 0;
					weightLtrans[y][x] = 1;
				}
			}

		this->MultiBandBlending(nH_total, nW_total,imgBack.imgRed, imgBack.imgGreen, imgBack.imgBlue);

		//------------------------------------------------------------------------
		// 물체이미지 합성.
		// transL : 배경이미지, transR : 물체이미지

		Mat inverObjH = homoObj.inv();

		// 배경 웨이트맵 형성
		for (int y = 0; y < imgL.nH; y++)
			for (int x = 0; x < imgL.nW; x++)
			{
				weightLtrans[y + offsetY][x + offsetX] = 1;
			}

		for (int y = minY_transR; y < maxY_transR; y++)
			for (int x = minX_transR; x < maxX_transR; x++) {

				inputPoint = cvPoint(x, y);
				GetPerspectivePoint(inputPoint, transPoint, inverBackH);

				if (transPoint.x >= 0 && transPoint.x < imgR.nW && transPoint.y >= 0 && transPoint.y < imgR.nH)
				{
					if (weightObj[int(transPoint.y)][int(transPoint.x)] == 0)
					{
						weightLtrans[y + offsetY][x + offsetX] = 1;
					}
				}
			}

		
		// 배경 이미지 형성 및 물체 이미지 초기화
		for (int y = 0; y < nH_total; y++)
			for (int x = 0; x < nW_total; x++)
			{
				imgLtrans.imgRed[y][x] = imgBack.imgRed[y][x];
				imgLtrans.imgGreen[y][x] = imgBack.imgGreen[y][x];
				imgLtrans.imgBlue[y][x] = imgBack.imgBlue[y][x];

				imgRtrans.imgRed[y][x] = 0;
				imgRtrans.imgGreen[y][x] = 0;
				imgRtrans.imgBlue[y][x] = 0;

				weightRtrans[y][x] = 0;
			}

		// 물체 이미지, 웨이트맵 형성

		for (int y = minY_totalR; y < maxY_totalR; y++)
			for (int x = minX_totalR; x < maxX_totalR; x++) {

				inputPoint = cvPoint(x, y);
				GetPerspectivePoint(inputPoint, transPoint, inverObjH);

				if (transPoint.x >= 0 && transPoint.x < imgR.nW && transPoint.y >= 0 && transPoint.y < imgR.nH)
				{

					imgRtrans.imgRed[y + offsetY][x + offsetX] = BilinearInterpolation(transPoint, imgR.imgRed, imgR.nH, imgR.nW);
					imgRtrans.imgGreen[y + offsetY][x + offsetX] = BilinearInterpolation(transPoint, imgR.imgGreen, imgR.nH, imgR.nW);
					imgRtrans.imgBlue[y + offsetY][x + offsetX] = BilinearInterpolation(transPoint, imgR.imgBlue, imgR.nH, imgR.nW);

					if (weightObj[int(transPoint.y)][int(transPoint.x)] == 255)
					{
						weightRtrans[y + offsetY][x + offsetX] = 1;
						weightLtrans[y + offsetY][x + offsetX] = 0;
					}
				}
			}

		this->MultiBandBlending(nH_total, nW_total, imgTotal.imgRed, imgTotal.imgGreen, imgTotal.imgBlue);

		//DisplayCimage2DColor(imgBack.imgRed, imgBack.imgGreen, imgBack.imgBlue, nW_total, nH_total, 0, 0);

	}

	// WriteImage
	if (bWrite)
	{
		BYTE* img1D_Total = new BYTE[GetBmp24Size(imgTotal.nW, imgTotal.nH)];
		BYTE* img1D_Seg = new BYTE[GetBmp24Size(imgR.nW, imgR.nH)];

		int Pos;
		CString FilePos;
		CString FileMarkPos;
		CString FileErrPos;

		for (y = 0; y < imgTotal.nH; y++)
			for (x = 0; x < imgTotal.nW; x++)
			{
				Pos = GetBmp24Pos(imgTotal.nW, imgTotal.nH, x, y);

				img1D_Total[Pos++] = imgTotal.imgBlue[y][x];
				img1D_Total[Pos++] = imgTotal.imgGreen[y][x];
				img1D_Total[Pos] = imgTotal.imgRed[y][x];
			}
		
		for (y = 0; y < imgR.nH; y++)
			for (x = 0; x < imgR.nW; x++)
			{
				Pos = GetBmp24Pos(imgR.nW, imgR.nH, x, y);

				img1D_Seg[Pos++] = imgSegment.imgBlue[y][x];
				img1D_Seg[Pos++] = imgSegment.imgGreen[y][x];
				img1D_Seg[Pos] = imgSegment.imgRed[y][x];
			}

		if (bExeDualHomo)
		{
			FilePos = _T("C:\\Users\\vision_jy\\Desktop\\1. Image DB\\Image Stitching\\실험폴더\\dual\\");
			FilePos += strFileName;
			FilePos += _T(".bmp");

			FileMarkPos = _T("C:\\Users\\vision_jy\\Desktop\\1. Image DB\\Image Stitching\\실험폴더\\dual\\");
			FileMarkPos += strFileName;
			FileMarkPos += _T("seg.bmp");

			FileErrPos = _T("C:\\Users\\vision_jy\\Desktop\\1. Image DB\\Image Stitching\\실험폴더\\dual\\");
			FileErrPos += strFileName;
			FileErrPos += _T("err.txt");
		}
		else
		{
			FilePos = _T("C:\\Users\\vision_jy\\Desktop\\1. Image DB\\Image Stitching\\실험폴더\\single\\");
			FilePos += strFileName;
			FilePos += _T(".bmp");

			FileMarkPos = _T("C:\\Users\\vision_jy\\Desktop\\1. Image DB\\Image Stitching\\실험폴더\\single\\");
			FileMarkPos += strFileName;
			FileMarkPos += _T("seg.bmp");

			FileErrPos = _T("C:\\Users\\vision_jy\\Desktop\\1. Image DB\\Image Stitching\\실험폴더\\single\\");
			FileErrPos += strFileName;
			FileErrPos += _T("err.txt");
		}

		SaveBmp(FilePos, img1D_Total, imgTotal.nW, imgTotal.nH);
		SaveBmp(FileMarkPos, img1D_Seg, imgR.nW, imgR.nH);

		//// 글씨 넣기
		FILE *ferr;

		// 파일 열기
		ferr = fopen(FileErrPos, "w");

		// 헤더 쓰기
		int tmp = 0;
		for (i = 0; i < nNumSeg; i++)
		{
			fprintf(ferr, "%d\t", i);
			
			switch (arrErr[i])
			{
			case 0:
				fprintf(ferr, "%s\t\t", "feature number");
				fprintf(ferr, "%d\n", int(arrInfo[i]));
				break;

			case 1:
				fprintf(ferr, "%s\t\t", "region area");
				fprintf(ferr, "%d\n", int(arrInfo[i]));
				break;

			case 2:
				fprintf(ferr, "%s\t\t", "feature rate");
				fprintf(ferr, "%f\n", arrInfo[i]);
				break;

			case 3:
				fprintf(ferr, "%s\t\t", "mismatch");
				fprintf(ferr, "%s\n", " ");
				break;

			case 4:
				fprintf(ferr, "%s\t\t", "kl-divergence");
				fprintf(ferr, "%f\n", arrInfo[i]);
				break;

			default:
				fprintf(ferr, "%s\t\t", "obj candidate");
				fprintf(ferr, "%s\t", " ");
				if (nNumObjCandi > 1)
					fprintf(ferr, "%f\n", arrInfo[i]);
				else
					fprintf(ferr, "%s\n", " ");
				break;
			}
		}

		fclose(ferr);

		delete[] img1D_Seg;
		delete[] img1D_Total;
	}

	DisplayCimage2DColor(imgTotal.imgRed, imgTotal.imgGreen, imgTotal.imgBlue, nW_total, nH_total, 0, 0);

	free_cmatrix(imgBack.imgRed, nH_total, nW_total);
	free_cmatrix(imgBack.imgGreen, nH_total, nW_total);
	free_cmatrix(imgBack.imgBlue, nH_total, nW_total);

	free_cmatrix(imgTotal.imgRed, imgTotal.nH, imgTotal.nW);
	free_cmatrix(imgTotal.imgGreen, imgTotal.nH, imgTotal.nW);
	free_cmatrix(imgTotal.imgBlue, imgTotal.nH, imgTotal.nW);

	free_dmatrix(weightRtrans, imgTotal.nH, imgTotal.nW);
	free_dmatrix(weightLtrans, imgTotal.nH, imgTotal.nW);

	free_cmatrix(imgRtrans.imgRed, imgTotal.nH, imgTotal.nW);
	free_cmatrix(imgRtrans.imgGreen, imgTotal.nH, imgTotal.nW);
	free_cmatrix(imgRtrans.imgBlue, imgTotal.nH, imgTotal.nW);

	free_cmatrix(imgLtrans.imgRed, imgTotal.nH, imgTotal.nW);
	free_cmatrix(imgLtrans.imgGreen, imgTotal.nH, imgTotal.nW);
	free_cmatrix(imgLtrans.imgBlue, imgTotal.nH, imgTotal.nW);
}

/****************************************************
<GetExcepInlierRate>

해당 index의 영역을 제외한 부분의 Inlier Rate를 구해준다.

Inlier의 판별은 거리 값으로 한다. (default :3)
Inlier Rate는 (Inlier 갯수) / (제외된 특징점의 갯수)

*Mode
0 : 제외한 부분의 특징점에서 구한 Homography를 활용한다.
1 : 정해진 Homography를 활용한다.

------------------------------------
Pre : arrIdx, numIdx
Post :

*******************************************************/
double CImageStitchMeanShift::GetExceptInlierRate(int* arrIdx, int numIdx, int mode, Mat& homo)
{
	int i, j;
	vector<Point2f> tmpPointR, tmpPointL;
	Mat tmpHomo;
	bool bCheck = false;

	// 영역 이외의 특징점 분포를 구한다.
	// 영역 이외의 호모그래피를 구한다.
	for (int i = 0; i < nNumPoint; i++)
	{
		for (int j = 0; j < numIdx; j++)
		{
			if (LabelMeanShift[int(GodPointR[i].y)][int(GodPointR[i].x)] == arrIdx[j])
			{
				bCheck = true;
			}
		}

		if (bCheck)
		{
			bCheck = false;
		}
		else
		{
			tmpPointR.push_back(GodPointR[i]);
			tmpPointL.push_back(GodPointL[i]);
		}
	}

	if (mode == 0)
		homo = findHomography(tmpPointR, tmpPointL, CV_RANSAC);

	// Inlier 비율을 구한다.
	int numInlier;
	int numSegPoint;
	double dTmp;
	double inlierRate;
	Point2f tmpInput, tmpOutput;


	numInlier = 0;
	numSegPoint = tmpPointR.size();

	for (i = 0; i < numSegPoint; i++)
	{
		tmpInput.x = tmpPointR[i].x;
		tmpInput.y = tmpPointR[i].y;

		GetPerspectivePoint(tmpInput, tmpOutput, homo);

		dTmp = GetDistance(tmpOutput.x, tmpOutput.y, tmpPointL[i].x, tmpPointL[i].y);

		if (dTmp < 3)
		{
			numInlier++;
		}
	}

	inlierRate = float(numInlier) / float(numSegPoint);

	return inlierRate;
}

double CImageStitchMeanShift::GetInlierRate(int Idx, Mat& homo)
{
	int i, j;
	vector<Point2f> tmpPointR, tmpPointL;

	for (i = 0; i < nNumPoint; i++)
	{
		if (LabelMeanShift[int(GodPointR[i].y)][int(GodPointR[i].x)] == Idx)
		{
			tmpPointR.push_back(GodPointR[i]);
			tmpPointL.push_back(GodPointL[i]);
		}
	}

	// Inlier 비율을 구한다.
	int numInlier;
	int numSegPoint;
	double dTmp;
	double inlierRate;
	Point2f tmpInput, tmpOutput;

	numInlier = 0;
	numSegPoint = tmpPointR.size();

	for (i = 0; i < numSegPoint; i++)
	{
		tmpInput.x = tmpPointR[i].x;
		tmpInput.y = tmpPointR[i].y;

		GetPerspectivePoint(tmpInput, tmpOutput, homo);

		dTmp = GetDistance(tmpOutput.x, tmpOutput.y, tmpPointL[i].x, tmpPointL[i].y);

		if (dTmp < 3)
		{
			numInlier++;
		}
	}

	inlierRate = float(numInlier) / float(numSegPoint);

	return inlierRate;
}

// 모든 영역의 특징점 갯수를 구한다. 
void CImageStitchMeanShift::GetArrFeatureNum()
{
	int i;
	int nTmp;

	// Set Region's number for getting homography
	for (i = 0; i < nNumPoint; i++)
	{
		nTmp = LabelMeanShift[int(GodPointR[i].y)][int(GodPointR[i].x)];
		arrFeatureNum[nTmp]++;
	}
}

// 모든 영역의 영역넓이를 구한다. 
void CImageStitchMeanShift::GetArrRegion()
{
	int y, x;
	int nTmp;

	for (y = 0; y < imgR.nH; y++)
		for (x = 0; x < imgR.nW; x++)
		{
			nTmp = LabelMeanShift[y][x];
			arrRegion[nTmp]++;
		}
}

// 모든 영역의 호모그래피를 구한다.
// 단 먼저 GetFeatureNum이 구해져 있어야 한다.
void CImageStitchMeanShift::GetArrHomo()
{
	int i;
	vector<Point2f> tmpPointR, tmpPointL;

	for (i = 0; i < nNumSeg; i++)
	{
		if (arrFeatureNum[i] > 4)
		{
			GetHomography(i, arrHomo[i]);
		}
		else
		{
			arrHomo[i] = -1;
		}

	}
}

void CImageStitchMeanShift::GetHomography(int Idx, Mat& homo)
{
	int i, j;
	vector<Point2f> tmpPointR, tmpPointL;

	for (i = 0; i < nNumPoint; i++)
	{
		if (LabelMeanShift[int(GodPointR[i].y)][int(GodPointR[i].x)] == Idx)
		{
			tmpPointR.push_back(GodPointR[i]);
			tmpPointL.push_back(GodPointL[i]);
		}
	}

	homo = findHomography(tmpPointR, tmpPointL, CV_RANSAC);
}

void CImageStitchMeanShift::DrawSegment()
{
	int i, j, x, y;

	CString strNum;

	Point2f* idxPos;

	int** arrColor;
	int tmp;
	int markColor[3];
	bool bBigMark;
	int charColor;
	int nH_img;

	

	arrColor = imatrix(nNumSeg, 3);

	idxPos = new Point2f[nNumSeg];

	GetRandColorArray(arrColor, nNumSeg);
	//----------------------------------------------

	// Region에 영역별 색깔 부여
	for (y = 0; y < imgR.nH; y++)
		for (x = 0; x < imgR.nW; x++)
		{
			tmp = LabelMeanShift[y][x];
			imgSegment.imgRed[y][x] = arrColor[tmp][2];
			imgSegment.imgGreen[y][x] = arrColor[tmp][1];
			imgSegment.imgBlue[y][x] = arrColor[tmp][0];
		}

	// Region의 Index 출력 위치 구하기
	for (i = 0; i < nNumObjCandi; i++)
	{
		for (y = 0; y < imgR.nH; y++)
			for (x = 0; x < imgR.nW; x++)
			{
				if (LabelMeanShift[y][x] == arrObjIdx[i])
				{
					idxPos[i].y = y;
					idxPos[i].x = x;
					goto outsegmark;
				}
			}

	outsegmark:
		int a;
	}

	tmp = 0;

	// Region's Mark 표시
	for (i = 0; i < nNumObjCandi; i++)
	{
		strNum.Format(_T("%d"), arrObjIdx[i]);

		// 크기 설정
		if (arrObjIdx[i] == nIdxObj)
			bBigMark = true;
		else
			bBigMark = false;

		// 글자 체크
		DrawTextOnImag2DGray(imgSegment.imgRed, imgR.nW, imgR.nH, idxPos[i].x, idxPos[i].y, strNum, DT_LEFT, 255, bBigMark);
		DrawTextOnImag2DGray(imgSegment.imgGreen, imgR.nW, imgR.nH, idxPos[i].x, idxPos[i].y, strNum, DT_LEFT, 0, bBigMark);
		DrawTextOnImag2DGray(imgSegment.imgBlue, imgR.nW, imgR.nH, idxPos[i].x, idxPos[i].y, strNum, DT_LEFT, 0, bBigMark);
	}

	// Feature point
	for (int i = 0; i < nNumPoint; i++)
	{
		DrawCross(imgSegment.imgRed, imgR.nW, imgR.nH, CPoint(GodPointR[i].x, GodPointR[i].y), 0);
		DrawCross(imgSegment.imgGreen, imgR.nW, imgR.nH, CPoint(GodPointR[i].x, GodPointR[i].y), 0);
		DrawCross(imgSegment.imgBlue, imgR.nW, imgR.nH, CPoint(GodPointR[i].x, GodPointR[i].y), 0);
	}

	DisplayCimage2DColor(imgSegment.imgRed, imgSegment.imgGreen, imgSegment.imgBlue, imgR.nW, imgR.nH, 0, 0);
	
	delete[] idxPos;
	delete[] arrColor;
}

double CImageStitchMeanShift::KLdivergenceForBivariateGaussiian(double* mean0, double* mean1, double** covMat0, double** covMat1)
{
	double tmp;
	double** invCov1;
	double** tmpMat;
	double* tmpVec;
	double* difMean;
	double detCov0, detCov1;

	invCov1 = dmatrix(2, 2);
	tmpMat = dmatrix(2, 2);
	tmpVec = new double[2];
	difMean = new double[2];

	invCov1 = InverseMatrix2D(covMat1);

	//tr(cov1^-1*cov0)
	tmpMat = MultipleMatrix2D(invCov1, covMat0);

	tmp = tmpMat[0][0] + tmpMat[1][1];

	//(u1-u0)^T * cov1^-1 * (u1-u0)
	difMean[0] = mean1[0] - mean0[0];
	difMean[1] = mean1[1] - mean0[1];

	tmpVec[0] = (difMean[0] * invCov1[0][0]) + (difMean[1] * invCov1[1][0]);
	tmpVec[1] = (difMean[0] * invCov1[0][1]) + (difMean[1] * invCov1[1][1]);

	tmp += ((tmpVec[0] * difMean[0]) + (tmpVec[1] * difMean[1]));

	//ln(det(cov1)/det(cov0))
	detCov0 = DetermineMat2D(covMat0);
	detCov1 = DetermineMat2D(covMat1);

	tmp += log(detCov1 / detCov0);

	tmp -= 2;
	tmp /= 2;

	free_dmatrix(invCov1, 2, 2);
	free_dmatrix(tmpMat, 2, 2);
	delete[]tmpVec;
	delete[]difMean;

	return tmp;
}

double CImageStitchMeanShift::DissimilarHomo(int nH, int nW, Mat& homo0, Mat& homo1, bool bOnDraw)
{
	int i, j, y, x;
	double dTmp=0;
	Point2f input[4];
	Point2f output0[4], output1[4];

	BYTE** drawSquare;

	int offsetX, offsetY;
	int nH_img, nW_img;
	double minX=20, minY=20;
	double maxX=0, maxY=0;

	input[0].x = 0; input[0].y = 0;
	input[1].x = nW; input[1].y = 0;
	input[2].x = nW; input[2].y = nH;
	input[3].x = 0; input[3].y = nH;

 
	for (i = 0; i < 4; i++)
	{
		GetPerspectivePoint(input[i], output0[i], homo0);
		GetPerspectivePoint(input[i], output1[i], homo1);

		dTmp += GetDistance(output0[i].x, output0[i].y, output1[i].x, output1[i].y);
	}

	if (bOnDraw)
	{
		for (i = 0; i < 4; i++)
		{
			input[i].y = input[i].y / 2;
			input[i].x = input[i].x / 2;

			output0[i].y = output0[i].y / 2;
			output0[i].x = output0[i].x / 2;

			output1[i].y = output1[i].y / 2;
			output1[i].x = output1[i].x / 2;
		}

		for (i = 0; i < 4; i++)
		{
			if (minX > input[i].x) minX = input[i].x;
			if (minY > input[i].y) minY = input[i].y;
			if (maxX < input[i].x) maxX = input[i].x;
			if (maxY < input[i].y) maxY = input[i].y;

			if (minX > output0[i].x) minX = output0[i].x;
			if (minY > output0[i].y) minY = output0[i].y;
			if (maxX < output0[i].x) maxX = output0[i].x;
			if (maxY < output0[i].y) maxY = output0[i].y;

			if (minX > output1[i].x) minX = output1[i].x;
			if (minY > output1[i].y) minY = output1[i].y;
			if (maxX < output1[i].x) maxX = output1[i].x;
			if (maxY < output1[i].y) maxY = output1[i].y;
		}

		if (minX < 0) offsetX = -1 * minX; else offsetX = 0;
		if (minY < 0) offsetY = -1 * minY; else offsetY = 0;

		nH_img = maxY - minY + 1;
		nW_img = maxX - minX + 1;

		drawSquare = cmatrix(nH_img, nW_img);

		for (y = 0; y < nH_img; y++)
			for (x = 0; x < nW_img; x++)
			{
				drawSquare[y][x] = 0;
			}

		DrawLine(drawSquare, nW_img, nH_img, input[0].x + offsetX, input[0].y + offsetY, input[1].x + offsetX, input[1].y + offsetY, 50);
		DrawLine(drawSquare, nW_img, nH_img, input[1].x + offsetX, input[1].y + offsetY, input[2].x + offsetX, input[2].y + offsetY, 50);
		DrawLine(drawSquare, nW_img, nH_img, input[2].x + offsetX, input[2].y + offsetY, input[3].x + offsetX, input[3].y + offsetY, 50);
		DrawLine(drawSquare, nW_img, nH_img, input[3].x + offsetX, input[3].y + offsetY, input[0].x + offsetX, input[0].y + offsetY, 50);

		DrawLine(drawSquare, nW_img, nH_img, output0[0].x + offsetX, output0[0].y + offsetY, output0[1].x + offsetX, output0[1].y + offsetY, 255);
		DrawLine(drawSquare, nW_img, nH_img, output0[1].x + offsetX, output0[1].y + offsetY, output0[2].x + offsetX, output0[2].y + offsetY, 255);
		DrawLine(drawSquare, nW_img, nH_img, output0[2].x + offsetX, output0[2].y + offsetY, output0[3].x + offsetX, output0[3].y + offsetY, 255);
		DrawLine(drawSquare, nW_img, nH_img, output0[3].x + offsetX, output0[3].y + offsetY, output0[0].x + offsetX, output0[0].y + offsetY, 255);

		DrawLine(drawSquare, nW_img, nH_img, output1[0].x + offsetX, output1[0].y + offsetY, output1[1].x + offsetX, output1[1].y + offsetY, 200);
		DrawLine(drawSquare, nW_img, nH_img, output1[1].x + offsetX, output1[1].y + offsetY, output1[2].x + offsetX, output1[2].y + offsetY, 200);
		DrawLine(drawSquare, nW_img, nH_img, output1[2].x + offsetX, output1[2].y + offsetY, output1[3].x + offsetX, output1[3].y + offsetY, 200);
		DrawLine(drawSquare, nW_img, nH_img, output1[3].x + offsetX, output1[3].y + offsetY, output1[0].x + offsetX, output1[0].y + offsetY, 200);

		DisplayCimage2D(drawSquare, nW_img, nH_img, 0, 0);

		free_cmatrix(drawSquare, nH_img, nW_img);
	}

	return dTmp;
}

/****************************************************
<ActiveFeatureNum>

각 영역의 FeatureNum를 통해 Object 후보군을 쓸지 말지 판단한다.

*이전에 GetArrFeatureNum를 통해 arrFeatureNum가 준비되어 있어야 한다.
*******************************************************/
void CImageStitchMeanShift::ActiveFeatureNum(int minFeature)
{
	int i;

	// Set Region's number for getting homography
	for (i = 0; i < nNumSeg; i++)
	{
		if (arrObjIdx[i] != -1)
		{
			if (arrFeatureNum[i] < minFeature)
			{
				arrObjIdx[i] = -1;
				arrErr[i] = 0;
				arrInfo[i] = arrFeatureNum[i];
			}
		}
	}
}

/****************************************************
<ActiveRegion>

각 영역의 Region Area를 통해 Object 후보군을 쓸지 말지 판단한다.

*이전에 GetArrRegion를 통해 arrRegion가 준비되어 있어야 한다.
*******************************************************/
void CImageStitchMeanShift::ActiveRegion(int minArea)
{
	int i;
	
	// Set Region's number for getting homography
	for (i = 0; i < nNumSeg; i++)
	{
		if (arrObjIdx[i] != -1)
		{
			if (arrRegion[i] < minArea)
			{
				arrObjIdx[i] = -1;
				arrErr[i] = 1;
				arrInfo[i] = arrRegion[i];
			}
		}
	}
}

/****************************************************
<ActiveFeatureRate>

각 영역의 FeatureRate를 통해 Object 후보군을 쓸지 말지 판단한다.

*이전에 GetArrFeatureNum를 통해 arrFeatureNum가 준비되어 있어야 한다.
*******************************************************/
bool CImageStitchMeanShift::ActiveFeatureRate(double minFeatureRate)
{
	int i, j, y, x;
	int tmpX, tmpY;
	int regionFeature=0;
	double featureRate;
	bool bOnFeatureRate = false;

	// Set Region's number for getting homography
	for (i = 0; i < nNumSeg; i++)
	{
		if (arrObjIdx[i] != -1)
		{
			featureRate = double(arrFeatureNum[i]) / double(nNumPoint);

			if (featureRate > minFeatureRate)
			{
				arrObjIdx[i] = -1;
				arrErr[i] = 2;
				arrInfo[i] = featureRate;
				bOnFeatureRate = true;
			}
		}
	}

	return bOnFeatureRate;
}

/****************************************************
<ActiveMisMatch>

각 영역의 호모그래피를 통해 Object 후보군을 쓸지 말지 판단한다.

*이전에 GetArrHomo를 통해 arrHomo가 준비되어 있어야 한다.
*******************************************************/
void CImageStitchMeanShift::ActiveMisMatch()
{
	int i, j, y, x;
	int tmpX, tmpY;
	double sumVec=0;
	bool bMisMatch = false;

	Point2f inputPt[4], outputPt[4];
	Point2f outputVecR[4], outputVecL[4];

	inputPt[0].y = 0; inputPt[0].x = 0;
	inputPt[1].y = 0; inputPt[1].x = imgR.nW;
	inputPt[2].y = imgR.nH; inputPt[2].x = 0;
	inputPt[3].y = imgR.nH; inputPt[3].x = imgR.nW;

	// Set Region's number for getting homography
	for (i = 0; i < nNumSeg; i++)
	{
		if (arrObjIdx[i] != -1)
		{
			for (j = 0; j < 4; j++)
				GetPerspectivePoint(inputPt[j], outputPt[j], arrHomo[i]);

			// 각도 angle 구하기
			outputVecR[0].y = outputPt[1].y - outputPt[0].y;
			outputVecR[0].x = outputPt[1].x - outputPt[0].x;

			outputVecR[1].y = outputPt[3].y - outputPt[1].y;
			outputVecR[1].x = outputPt[3].x - outputPt[1].x;

			outputVecR[2].y = outputPt[2].y - outputPt[3].y;
			outputVecR[2].x = outputPt[2].x - outputPt[3].x;

			outputVecR[3].y = outputPt[0].y - outputPt[2].y;
			outputVecR[3].x = outputPt[0].x - outputPt[2].x;

			outputVecL[0].y = outputPt[0].y - outputPt[1].y;
			outputVecL[0].x = outputPt[0].x - outputPt[1].x;

			outputVecL[1].y = outputPt[1].y - outputPt[3].y;
			outputVecL[1].x = outputPt[1].x - outputPt[3].x;

			outputVecL[2].y = outputPt[3].y - outputPt[2].y;
			outputVecL[2].x = outputPt[3].x - outputPt[2].x;

			outputVecL[3].y = outputPt[2].y - outputPt[0].y;
			outputVecL[3].x = outputPt[2].x - outputPt[0].x;

			sumVec += GetAngleVecter(outputVecR[0], outputVecL[3]);
			sumVec += GetAngleVecter(outputVecR[1], outputVecL[0]);
			sumVec += GetAngleVecter(outputVecR[2], outputVecL[1]);
			sumVec += GetAngleVecter(outputVecR[3], outputVecL[2]);

			// 사각형인지 판별
			if (sumVec < 350)
				bMisMatch = true;
		
			// 변형된 영상의 가로, 세로 길이 얼마나 큰지 판별
			int minX = imgR.nW, maxX=0, minY= imgR.nH, maxY=0;
			int nHT, nWT;

			for (j = 0; j < 4; j++)
			{
				if (minX > outputPt[j].x) minX = outputPt[j].x;
				if (maxX < outputPt[j].x) maxX = outputPt[j].x;
				if (minY > outputPt[j].y) minY = outputPt[j].y;
				if (maxY < outputPt[j].y) maxY = outputPt[j].y;
			}

			nHT = maxY - minY;
			nWT = maxX - minX;

			if (nHT > 10000 || nWT > 10000 || nHT == 0 || nWT == 0)
			{
				bMisMatch = true;
			}

			if (bMisMatch)
			{
				arrObjIdx[i] = -1;
				arrErr[i] = 3;
				arrInfo[i] = NULL;
				bMisMatch = false;
			}
			sumVec = 0;
		}
	}
}

void CImageStitchMeanShift::ActiveKLdivergence(double minKLdivergence)
{
	int y, x, i, j;
	double** covMatR, **covMatF;
	double* meanR, *meanF;
	double similarity;
	vector <Point2f> regionPoint;
	vector <Point2f> objPointR, objPointL;

	covMatR = dmatrix(2, 2);
	covMatF = dmatrix(2, 2);
	meanR = new double[2];
	meanF = new double[2];

	// Set Region's number for getting homography
	for (i = 0; i < nNumSeg; i++)
	{
		if (arrObjIdx[i] != -1)
		{
			// Get Region Mean, covMat
			for (y = 0; y < imgR.nH; y++)
				for (x = 0; x < imgR.nW; x++)
				{
					if (LabelMeanShift[y][x] == arrObjIdx[i])
					{
						regionPoint.push_back(Point2f(x, y));
					}
				}

			GetCovAndMean2D(covMatR, meanR, regionPoint);

			// Get Feature in Region Mean, covMat
			for (j = 0; j < nNumPoint; j++)
			{
				if (LabelMeanShift[int(GodPointR[j].y)][int(GodPointR[j].x)] == arrObjIdx[i])
				{
					objPointR.push_back(GodPointR[j]);
					objPointL.push_back(GodPointL[j]);
				}
			}

			GetCovAndMean2D(covMatF, meanF, objPointR);

			// KL-divergence
			similarity = KLdivergenceForBivariateGaussiian(meanF, meanR, covMatF, covMatR);
			similarity += KLdivergenceForBivariateGaussiian(meanR, meanF, covMatR, covMatF);

			similarity /= 2.;

			if (similarity > minKLdivergence)
			{
				arrObjIdx[i] = -1;
				arrErr[i] = 4;
				arrInfo[i] = similarity;
			}
		}

		regionPoint.clear();
		objPointR.clear();
		objPointL.clear();
		
	}

	free_dmatrix(covMatR, 2, 2);
	free_dmatrix(covMatF, 2, 2);

	delete[] meanR;
	delete[] meanF;
}

void CImageStitchMeanShift::DrawActiveErr()
{
	int i, j, x, y;
	CString strNum, strErr, strErrInfo;
	BYTE** imgErr;

	imgErr = cmatrix(15*nNumSeg, 250);

	for (y = 0; y < 15 * nNumSeg; y++)
		for (x = 0; x < 250; x++)
		{
			imgErr[y][x] = 0;
		}

	for (i = 0; i < nNumSeg; i++)
	{
		strNum.Format(_T("%d"), i);
		DrawTextOnImag2DGray(imgErr, 250, 15 * nNumSeg, 0, i * 15, strNum, DT_LEFT, 255, false);

		switch (arrErr[i])
		{
		case 0:
			strErr="feature point";
			strErrInfo.Format(_T("%d"), int(arrInfo[i]));
			break;

		case 1:
			strErr = "region area";
			strErrInfo.Format(_T("%d"), int(arrInfo[i]));
			break;

		case 2:
			strErr = "feature rate";
			strErrInfo.Format(_T("%f"), arrInfo[i]);
			break;

		case 3:
			strErr = "misMatch";
			strErrInfo = " ";
			break;

		case 4:
			strErr = "kl-divergence";
			strErrInfo.Format(_T("%f"), arrInfo[i]);
			break;

		default:
			strErr = "obj candidate";
			if (arrInfo[i] == -1)
				strErrInfo = " ";
			else
				strErrInfo.Format(_T("%f"), arrInfo[i]);
		}
		
		DrawTextOnImag2DGray(imgErr, 250, 15 * nNumSeg, 40, i * 15, strErr, DT_LEFT, 255, false);
		DrawTextOnImag2DGray(imgErr, 250, 15 * nNumSeg, 160, i * 15, strErrInfo, DT_LEFT, 255, false);
	}

	DisplayCimage2D(imgErr, 250, 15 * nNumSeg, 0, 0);

	delete[] imgErr;
}

//===============================================================================================================================================================================================================================================
//===============================================================================================================================================================================================================================================

void CImageStitchMeanShift::FindMultiObject()
{
	int i, j;
	double inlierRate;
	int* index;
	double thresh = 0.7;
	int tmp;
	bool bDone = true;

	// 모든 object 후보군을 제외한 특징점의 inlier 비율 구하기
	inlierRate = GetExceptInlierRate(arrObjIdx, nNumObjCandi, 0, homoBack);

	if (inlierRate > thresh)
	{
		while (nNumObjCandi > 1)
		{
			index = new int[nNumObjCandi - 1];

			for (i = 0; i < nNumObjCandi; i++)
			{
				// 새로운 object 후보군 생성
				for (j = 0; j < nNumObjCandi - 1; j++)
				{
					tmp = i + j;
					if (tmp >= nNumObjCandi) tmp -= nNumObjCandi;
					index[j] = arrObjIdx[tmp];
				}

				inlierRate = GetExceptInlierRate(index, nNumObjCandi - 1, 1, homoBack);

				if (inlierRate > thresh)
				{
					bDone = false;
					break;
				}
			}

			// while문을 멈출지 판별
			if (bDone)
			{
				break;
			}
			else
			{
				// object 후보군 업데이트
				nNumObjCandi--;

				delete[] arrObjIdx;
				arrObjIdx = new int[nNumObjCandi];
				for (i = 0; i < nNumObjCandi; i++)
					arrObjIdx[i] = index[i];

				delete[] index;
			}
		}
	}
	else
	{
		bExeDualHomo = true;	// 싱글 호모그래피를 사용한다.
	}
}

void CImageStitchMeanShift::FindObject()
{
	int i, j;
	int nTmp;
	vector<Point2f> PointTransR;
	Point2f tmpPoint;
	
	double* arrDist;

	arrDist = new double[nNumObjCandi];

	homoGlo = findHomography(GodPointR, GodPointL, CV_RANSAC);

	// PointTransR
	for (i = 0; i < nNumPoint; i++)
	{
		GetPerspectivePoint(GodPointR[i], tmpPoint, homoGlo);
		PointTransR.push_back(tmpPoint);
	}

	// distance by regions
	for (i = 0; i < nNumObjCandi; i++)
	{
		arrDist[i] = 0;
		nTmp = 0;

		for (j = 0; j < nNumPoint; j++)
		{
			if (LabelMeanShift[int(GodPointR[j].y)][int(GodPointR[j].x)] == arrObjIdx[i])
			{
				arrDist[i] += GetDistance(int(GodPointL[j].x), int(GodPointL[j].y), int(PointTransR[j].x), int(PointTransR[j].y));
				nTmp++;
			}
		}

		arrDist[i] /= nTmp;
	}

	delete[] arrDist;
}

void CImageStitchMeanShift::Run()
{
	int x, y;

	GetMatchingPoint();
	SegmentMeanshift(2000);

	DetermineActive(10, 6000, 0.5, 20);

	if (bExeDualHomo)
	{
		FindSingleObject();
		MakeWeightMap(10);
	}

	WarpHomography(true);
}
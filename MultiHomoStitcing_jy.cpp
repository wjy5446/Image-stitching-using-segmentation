//#include "stdafx.h"
//#include "MultiHomoStitcing_jy.h"
//#include "Image.h"
//#include "Mesh.h"
//#include "meanshift\MeanShift.h"
//#include <cmath>
//#include "time.h"
//#include "opencv2\core\core.hpp"
//#include "opencv2\imgproc\imgproc.hpp"
//#include "opencv2\features2d\features2d.hpp"
//#include "opencv2\nonfree\nonfree.hpp"
//#include "opencv2\nonfree\features2d.hpp"
//#include "opencv2\calib3d\calib3d.hpp"
//#include<vector>
//using namespace cv;
//
//CImageStitchBasic::CImageStitchBasic()
//{
//	// 영상 정보 읽기
//	if (!GetCurrentImageInfo(&imgL.nW, &imgL.nH, &imgL.nPosX, &imgL.nPosY)) return;
//	if (!GetCurrentImageInfo(&imgR.nW, &imgR.nH, &imgR.nPosX, &imgR.nPosY, 1)) return;
//
//	imgL.imgGray = cmatrix(imgL.nH, imgL.nW);
//	imgR.imgGray = cmatrix(imgR.nH, imgR.nW);
//
//	imgL.imgRed = cmatrix(imgL.nH, imgL.nW);
//	imgL.imgGreen = cmatrix(imgL.nH, imgL.nW);
//	imgL.imgBlue = cmatrix(imgL.nH, imgL.nW);
//
//	imgR.imgRed = cmatrix(imgR.nH, imgR.nW);
//	imgR.imgGreen = cmatrix(imgR.nH, imgR.nW);
//	imgR.imgBlue = cmatrix(imgR.nH, imgR.nW);
//
//	imgMark.imgRed = cmatrix(imgR.nH, imgR.nW);
//	imgMark.imgGreen = cmatrix(imgR.nH, imgR.nW);
//	imgMark.imgBlue = cmatrix(imgR.nH, imgR.nW);
//
//	// 회색조 영상 읽기
//	GetCurrentImageGray(imgL.imgGray);
//	GetCurrentImageGray(imgR.imgGray, 1);
//
//	// 컬러 영상 읽기
//	GetCurrentImageColor(imgL.imgRed, imgL.imgGreen, imgL.imgBlue);
//	GetCurrentImageColor(imgR.imgRed, imgR.imgGreen, imgR.imgBlue, 1);
//	GetCurrentImageColor(imgMark.imgRed, imgMark.imgGreen, imgMark.imgBlue, 1);
//
//	//Set weightmap for Seam
//	weightSeamR = dmatrix(imgR.nH, imgR.nW);
//	weightSeamL = dmatrix(imgL.nH, imgL.nW);
//
//	MakeWeightSeam(weightSeamR, imgR.nH, imgR.nW);
//	MakeWeightSeam(weightSeamL, imgL.nH, imgL.nW);
//}
//
//CImageStitchBasic::CImageStitchBasic(BYTE* img1D_L, BYTE* img1D_R, int nHL, int nWL, int nHR, int nWR)
//{
//	int y, x;
//	imgL.nH = nHL, imgL.nW = nWL;
//	imgR.nH = nHR, imgR.nW = nWR;
//
//	imgL.imgGray = cmatrix(imgL.nH, imgL.nW);
//	imgR.imgGray = cmatrix(imgR.nH, imgR.nW);
//
//	imgL.imgRed = cmatrix(imgL.nH, imgL.nW);
//	imgL.imgGreen = cmatrix(imgL.nH, imgL.nW);
//	imgL.imgBlue = cmatrix(imgL.nH, imgL.nW);
//
//	imgR.imgRed = cmatrix(imgR.nH, imgR.nW);
//	imgR.imgGreen = cmatrix(imgR.nH, imgR.nW);
//	imgR.imgBlue = cmatrix(imgR.nH, imgR.nW);
//
//	imgMark.imgRed = cmatrix(imgR.nH, imgR.nW);
//	imgMark.imgGreen = cmatrix(imgR.nH, imgR.nW);
//	imgMark.imgBlue = cmatrix(imgR.nH, imgR.nW);
//
//	// Set weightmap for Seam
//	weightSeamR = dmatrix(imgR.nH, imgR.nW);
//	weightSeamL = dmatrix(imgL.nH, imgL.nW);
//
//	MakeWeightSeam(weightSeamR, imgR.nH, imgR.nW);
//	MakeWeightSeam(weightSeamL, imgL.nH, imgL.nW);
//
//	// 컬러영상 읽기
//	// - 왼쪽 -
//	int Pos;
//
//	for (y = 0; y < nHL; y++)
//		for (x = 0; x < nWL; x++)
//		{
//			Pos = GetBmp24Pos(nWL, nHL, x, y);
//			imgL.imgBlue[y][x] = img1D_L[Pos++];
//			imgL.imgGreen[y][x] = img1D_L[Pos++];
//			imgL.imgRed[y][x] = img1D_L[Pos];
//		}
//	// - 오른쪽 -
//	for (y = 0; y < nHR; y++)
//		for (x = 0; x < nWR; x++)
//		{
//			Pos = GetBmp24Pos(nWR, nHR, x, y);
//			imgR.imgBlue[y][x] = img1D_R[Pos++];
//			imgR.imgGreen[y][x] = img1D_R[Pos++];
//			imgR.imgRed[y][x] = img1D_R[Pos];
//		}
//
//	for (y = 0; y < nHR; y++)
//		for (x = 0; x < nWR; x++)
//		{
//			Pos = GetBmp24Pos(nWR, nHR, x, y);
//			imgMark.imgBlue[y][x] = img1D_R[Pos++];
//			imgMark.imgGreen[y][x] = img1D_R[Pos++];
//			imgMark.imgRed[y][x] = img1D_R[Pos];
//		}
//
//	// 회색영상 읽기
//	// - 왼쪽 -
//
//	for (y = 0; y < nHL; y++)
//		for (x = 0; x < nWL; x++)
//		{
//			Pos = GetBmp24Pos(nWL, nHL, x, y);
//			imgL.imgGray[y][x] = (img1D_L[Pos] + img1D_L[Pos + 1] + img1D_L[Pos + 2]) / 3;
//		}
//
//	// - 오른쪽 -
//	for (y = 0; y < nHR; y++)
//		for (x = 0; x < nWR; x++)
//		{
//			Pos = GetBmp24Pos(nWR, nHR, x, y);
//			imgR.imgGray[y][x] = (img1D_R[Pos] + img1D_R[Pos + 1] + img1D_R[Pos + 2]) / 3;
//		}
//}
//
//CImageStitchBasic::~CImageStitchBasic()
//{
//	free_cmatrix(imgL.imgGray, imgL.nH, imgL.nW);
//	free_cmatrix(imgR.imgGray, imgR.nH, imgR.nW);
//	
//	free_cmatrix(imgL.imgRed, imgL.nH, imgL.nW);
//	free_cmatrix(imgL.imgGreen, imgL.nH, imgL.nW);
//	free_cmatrix(imgL.imgBlue, imgL.nH, imgL.nW);
//	
//	free_cmatrix(imgR.imgRed, imgR.nH, imgR.nW);
//	free_cmatrix(imgR.imgGreen, imgR.nH, imgR.nW);
//	free_cmatrix(imgR.imgBlue, imgR.nH, imgR.nW);
//	
//	free_cmatrix(imgMark.imgRed, imgR.nH, imgR.nW);
//	free_cmatrix(imgMark.imgGreen, imgR.nH, imgR.nW);
//	free_cmatrix(imgMark.imgBlue, imgR.nH, imgR.nW);
//
//	free_dmatrix(weightSeamR, imgR.nH, imgR.nW);
//	free_dmatrix(weightSeamL, imgL.nH, imgL.nW);
//}
//
//void CImageStitchBasic::GetMatchingPoint(int nPara, bool bOnFeature, bool bOnMatch)
//{
//	Mat M_ImageL(cvSize(imgL.nW, imgL.nH), CV_8UC1, cvScalar(0));
//	Mat M_ImageR(cvSize(imgR.nW, imgR.nH), CV_8UC1, cvScalar(0));
//
//	// Byte -> Mat
//
//	ByteToMatGray(imgL.imgGray, M_ImageL.data, imgL.nH, imgL.nW);
//	ByteToMatGray(imgR.imgGray, M_ImageR.data, imgR.nH, imgR.nW);
//
//	//Detect the keypoints using SURF Detector
//
//	SiftFeatureDetector detector(nPara);
//	std::vector< KeyPoint > keypoints_L, keypoints_R;
//	detector.detect(M_ImageL, keypoints_L);
//	detector.detect(M_ImageR, keypoints_R);
//
//	//-- Step 2: Calculate descriptors (feature vectors)
//	
//	SiftDescriptorExtractor extractor;
//	Mat descriptors_L, descriptors_R;
//	extractor.compute(M_ImageL, keypoints_L, descriptors_L);
//	extractor.compute(M_ImageR, keypoints_R, descriptors_R);
//
//	//-- Step 3: Matching descriptor vectors using FLANN matcher
//
//	FlannBasedMatcher matcher;
//	std::vector< DMatch > matches;
//	matcher.match(descriptors_L, descriptors_R, matches);
//	double max_dist = 0; double min_dist = 100;
//
//
//	//-- Quick calculation of max and min distances between keypoints
//	for (int i = 0; i < descriptors_L.rows; i++)
//	{
//		double dist = matches[i].distance;
//		if (dist < min_dist) min_dist = dist;
//		if (dist > max_dist) max_dist = dist;
//	}
//
//	//-- Use only "good" matches (i.e. whose distance is less than 3*min_dist )
//
//	std::vector< DMatch > good_matches;
//	for (int i = 0; i < descriptors_L.rows; i++)
//		if (matches[i].distance <  4 * min_dist)
//			good_matches.push_back(matches[i]);
//
//	//--특징점 갯수 확인
//	if (good_matches.size()<10)
//	{
//		AfxMessageBox(_T("특징점의 갯수가 부족합니다."));
//	}
//
//
//	for (unsigned int i = 0; i < good_matches.size(); i++)
//	{
//		//-- Get the keypoints from the good matches
//		GodPointL.push_back(keypoints_L[good_matches[i].queryIdx].pt);
//		GodPointR.push_back(keypoints_R[good_matches[i].trainIdx].pt);
//	}
//
//	nNumPoint = GodPointR.size();
//
//	// Option
//
//	if (bOnFeature)
//	{
//		DrawFeature();
//	}
//
//	if (bOnMatch)
//	{
//		DrawMatch();
//	}
//}
//
//void CImageStitchBasic::GetHomography()
//{
//	homo = findHomography(GodPointR, GodPointL, CV_RANSAC);
//}
//
//void CImageStitchBasic::WarpHomography(bool bOnLR)
//{
//	int y, x;
//	int i;
//
//	//--GetSize
//	int offsetX = 0, offsetY = 0;
//	int nH_transR, nW_transR;
//	int maxX_totalR = 0, maxY_totalR = 0, minX_totalR = imgR.nW, minY_totalR = imgR.nH;
//	double maxX_transR = 0, maxY_transR = 0, minX_transR = imgR.nW, minY_transR = imgR.nH;
//	int nH_total, nW_total;
//	Point2f inputPoint, transPoint;
//
//	for (y = 0; y < imgR.nH; y++)
//		for (x = 0; x < imgR.nW; x++)
//		{
//			inputPoint = Point2f(x, y);
//			GetPerspectivePoint(inputPoint, transPoint, homo);
//
//			if (transPoint.y > maxY_transR) maxY_transR = transPoint.y;
//			if (transPoint.x > maxX_transR) maxX_transR = transPoint.x;
//			if (transPoint.y < minY_transR) minY_transR = transPoint.y;
//			if (transPoint.x < minX_transR) minX_transR = transPoint.x;
//		}
//
//	if (minY_transR < 0) minY_totalR = minY_transR; else minY_totalR = 0;
//	if (maxY_transR > imgR.nH - 1) maxY_totalR = maxY_transR; else maxY_totalR = imgR.nH - 1;
//
//	if (minX_transR < 0) minX_totalR = minX_transR; else minX_totalR = 0;
//	if (maxX_transR > imgR.nW - 1) maxX_totalR = maxX_transR; else maxX_totalR = imgR.nW - 1;
//
//	nH_total = maxY_totalR - minY_totalR + 1;
//	nW_total = maxX_totalR - minX_totalR + 1;
//
//	if (minY_transR < 0) offsetY = abs(minY_transR);
//	if (minX_transR < 0) offsetX = abs(minX_transR);
//
//	imgTotal.imgRed = cmatrix(nH_total, nW_total); imgTotal.imgGreen = cmatrix(nH_total, nW_total); imgTotal.imgBlue = cmatrix(nH_total, nW_total);
//	imgLtrans.imgRed = cmatrix(nH_total, nW_total); imgLtrans.imgGreen = cmatrix(nH_total, nW_total); imgLtrans.imgBlue = cmatrix(nH_total, nW_total);
//	imgRtrans.imgRed = cmatrix(nH_total, nW_total); imgRtrans.imgGreen = cmatrix(nH_total, nW_total); imgRtrans.imgBlue = cmatrix(nH_total, nW_total);
//	weightRtrans = dmatrix(nH_total, nW_total); weightLtrans = dmatrix(nH_total, nW_total);
//
//	imgTotal.nH = nH_total;	imgTotal.nW = nW_total;
//	imgLtrans.nH = nH_total; imgLtrans.nW = nW_total;
//	imgRtrans.nH = nH_total; imgRtrans.nW = nW_total;
//
//
//	//---image warping
//
//	// 초기화
//	for (y = 0; y < nH_total; y++)
//		for (x = 0; x < nW_total; x++)
//		{
//			weightRtrans[y][x] = 0;	weightLtrans[y][x] = 0;
//			imgLtrans.imgRed[y][x] = 0; imgLtrans.imgGreen[y][x] = 0; imgLtrans.imgBlue[y][x] = 0;
//			imgRtrans.imgRed[y][x] = 0; imgRtrans.imgGreen[y][x] = 0; imgRtrans.imgBlue[y][x] = 0;
//		}
//
//	// 왼쪽
//	for (y = 0; y < imgL.nH; y++)
//		for (x = 0; x < imgL.nW; x++)
//		{
//			imgLtrans.imgRed[y + offsetY][x + offsetX] = imgL.imgRed[y][x];
//			imgLtrans.imgGreen[y + offsetY][x + offsetX] = imgL.imgGreen[y][x];
//			imgLtrans.imgBlue[y + offsetY][x + offsetX] = imgL.imgBlue[y][x];
//			weightLtrans[y + offsetY][x + offsetX] = weightSeamL[y][x];
//		}
//
//	// 오른쪽
//	Mat inverH = homo.inv();
//
//	for (y = minY_transR; y < maxY_transR; y++)
//		for (x = minX_transR; x < maxX_transR; x++) {
//
//			inputPoint = Point2f(x, y);
//			GetPerspectivePoint(inputPoint, transPoint, inverH);
//			if (transPoint.x >= 0 && transPoint.x < imgR.nW && transPoint.y >= 0 && transPoint.y < imgR.nH)
//			{
//				imgRtrans.imgRed[y + offsetY][x + offsetX] = BilinearInterpolation(transPoint, imgR.imgRed, imgR.nH, imgR.nW);
//				imgRtrans.imgGreen[y + offsetY][x + offsetX] = BilinearInterpolation(transPoint, imgR.imgGreen, imgR.nH, imgR.nW);;
//				imgRtrans.imgBlue[y + offsetY][x + offsetX] = BilinearInterpolation(transPoint, imgR.imgBlue, imgR.nH, imgR.nW);;
//				weightRtrans[y + offsetY][x + offsetX] = weightSeamR[int(transPoint.y)][int(transPoint.x)];
//			}
//		}
//
//	// Seam 만들기
//	for (y = 0; y < nH_total; y++)
//		for (x = 0; x < nW_total; x++)
//		{
//			if (weightRtrans[y][x] > weightLtrans[y][x])
//			{
//				weightRtrans[y][x] = 1;
//				weightLtrans[y][x] = 0;
//			}
//
//			if (weightLtrans[y][x] > weightRtrans[y][x])
//			{
//				weightRtrans[y][x] = 0;
//				weightLtrans[y][x] = 1;
//			}
//		}
//
//	// multiband blending
//	this->MultiBandBlending(imgTotal.nH, imgTotal.nW, imgTotal.imgRed, imgTotal.imgGreen, imgTotal.imgBlue);
//
//	DisplayCimage2DColor(imgTotal.imgRed, imgTotal.imgGreen, imgTotal.imgBlue, imgTotal.nW, imgTotal.nH,0,0);
//
//	// Option
//	if (bOnLR)
//	{
//		DrawLR();
//	}
//
//	free_cmatrix(imgTotal.imgRed, imgTotal.nH, imgTotal.nW);
//	free_cmatrix(imgTotal.imgGreen, imgTotal.nH, imgTotal.nW);
//	free_cmatrix(imgTotal.imgBlue, imgTotal.nH, imgTotal.nW);
//
//	free_dmatrix(weightRtrans, imgTotal.nH, imgTotal.nW);
//	free_dmatrix(weightLtrans, imgTotal.nH, imgTotal.nW);
//
//	free_cmatrix(imgRtrans.imgRed, imgTotal.nH, imgTotal.nW);
//	free_cmatrix(imgRtrans.imgGreen, imgTotal.nH, imgTotal.nW);
//	free_cmatrix(imgRtrans.imgBlue, imgTotal.nH, imgTotal.nW);
//
//	free_cmatrix(imgLtrans.imgRed, imgTotal.nH, imgTotal.nW);
//	free_cmatrix(imgLtrans.imgGreen, imgTotal.nH, imgTotal.nW);
//	free_cmatrix(imgLtrans.imgBlue, imgTotal.nH, imgTotal.nW);
//}
//
//void CImageStitchBasic::DrawFeature()
//{
//	int nRand;
//	int arrColor[3];
//
//	// 색깔 랜덤으로 받기
//	nRand = rand() % 255;	arrColor[0] = nRand;
//	nRand = rand() % 255;   arrColor[1] = nRand;
//	nRand = rand() % 255;   arrColor[2] = nRand;
//
//	// 특징점 표시
//	for (int i = 0; i < nNumPoint; i++)
//	{
//		DrawCross(imgMark.imgRed, imgR.nW, imgR.nH, CPoint(GodPointR[i].x, GodPointR[i].y), arrColor[0]);
//		DrawCross(imgMark.imgGreen, imgR.nW, imgR.nH, CPoint(GodPointR[i].x, GodPointR[i].y), arrColor[1]);
//		DrawCross(imgMark.imgBlue, imgR.nW, imgR.nH, CPoint(GodPointR[i].x, GodPointR[i].y), arrColor[2]);
//	}
//
//	DisplayCimage2DColor(imgMark.imgRed, imgMark.imgGreen, imgMark.imgBlue, imgR.nW, imgR.nH, imgR.nPosX, imgR.nPosY);
//}
//
//void CImageStitchBasic::DrawMatch()
//{
//	Image2D imgMatch;
//
//	imgMatch.nH = imgR.nH;
//	imgMatch.nW = imgR.nW + imgL.nW;
//
//	imgMatch.imgRed = cmatrix(imgMatch.nH, imgMatch.nW);
//	imgMatch.imgGreen = cmatrix(imgMatch.nH, imgMatch.nW);
//	imgMatch.imgBlue = cmatrix(imgMatch.nH, imgMatch.nW);
//
//	for (int i = 0; i < imgL.nH; i++)
//		for (int j = 0; j < imgL.nW; j++)
//		{
//			imgMatch.imgRed[i][j] = imgL.imgRed[i][j];
//			imgMatch.imgGreen[i][j] = imgL.imgGreen[i][j];
//			imgMatch.imgBlue[i][j] = imgL.imgBlue[i][j];
//		}
//
//	for (int i = 0; i < imgR.nH; i++)
//		for (int j = 0; j < imgR.nW; j++)
//		{
//			imgMatch.imgRed[i][j + imgL.nW] = imgR.imgRed[i][j];
//			imgMatch.imgGreen[i][j + imgL.nW] = imgR.imgGreen[i][j];
//			imgMatch.imgBlue[i][j + imgL.nW] = imgR.imgBlue[i][j];
//		}
//
//	for (int i = 0; i < GodPointR.size(); i++)
//	{
//		DrawLine(imgMatch.imgRed, imgR.nW + imgL.nW, imgR.nH, GodPointL[i].x, GodPointL[i].y, GodPointR[i].x + imgL.nW, GodPointR[i].y, 0);
//		DrawLine(imgMatch.imgGreen, imgR.nW + imgL.nW, imgR.nH, GodPointL[i].x, GodPointL[i].y, GodPointR[i].x + imgL.nW, GodPointR[i].y, 255);
//		DrawLine(imgMatch.imgBlue, imgR.nW + imgL.nW, imgR.nH, GodPointL[i].x, GodPointL[i].y, GodPointR[i].x + imgL.nW, GodPointR[i].y, 0);
//	}
//
//	DisplayCimage2DColor(imgMatch.imgRed, imgMatch.imgGreen, imgMatch.imgBlue, imgR.nW + imgL.nW, imgL.nH, 0, 0);
//
//	free_cmatrix(imgMatch.imgRed, imgMatch.nH, imgMatch.nW);
//	free_cmatrix(imgMatch.imgGreen, imgMatch.nH, imgMatch.nW);
//	free_cmatrix(imgMatch.imgBlue, imgMatch.nH, imgMatch.nW);
//}
//
//void CImageStitchBasic::DrawLR()
//{
//	DisplayCimage2DColor(imgLtrans.imgRed, imgLtrans.imgGreen, imgLtrans.imgBlue, imgTotal.nW, imgTotal.nH, 0, 0);
//	DisplayCimage2DColor(imgRtrans.imgRed, imgRtrans.imgGreen, imgRtrans.imgBlue, imgTotal.nW, imgTotal.nH, 0, 0);
//}
//
//void CImageStitchBasic::MultiBandBlending(int nH, int nW, BYTE** Result_R, BYTE** Result_G, BYTE** Result_B)
//{
//	int x, y;
//	int nGSize = 11; // Gaussian Filter Size
//	double dSigma = 20; // Gaussian's sigma
//	double dSigma1, dSigma2;
//	double** Gfilter = dmatrix(nGSize, nGSize);
//
//
//	// 가우시안 필터링한 이미지
//	double** GL_Ch1_R, **GL_Ch1_G, **GL_Ch1_B; //왼쪽 영상
//	double** GR_Ch1_R, **GR_Ch1_G, **GR_Ch1_B; //오른쪽 영상
//
//	double** GL_Ch2_R, **GL_Ch2_G, **GL_Ch2_B;
//	double** GR_Ch2_R, **GR_Ch2_G, **GR_Ch2_B;
//
//	double** GL_Ch3_R, **GL_Ch3_G, **GL_Ch3_B;
//	double** GR_Ch3_R, **GR_Ch3_G, **GR_Ch3_B;
//
//	// 원본이미지와 가우시안한 이미지의 차이(즉 이미지의 주파수 대역)
//	double** BL_Ch1_R, **BL_Ch1_G, **BL_Ch1_B; //왼쪽 영상
//	double** BR_Ch1_R, **BR_Ch1_G, **BR_Ch1_B; //오른쪽 영상
//
//	double** BL_Ch2_R, **BL_Ch2_G, **BL_Ch2_B;
//	double** BR_Ch2_R, **BR_Ch2_G, **BR_Ch2_B;
//
//	double** BL_Ch3_R, **BL_Ch3_G, **BL_Ch3_B;
//	double** BR_Ch3_R, **BR_Ch3_G, **BR_Ch3_B;
//
//	// 가우시안 필터링한 웨이트 값
//	double** WL_Ch1, **WL_Ch2, **WL_Ch3; //왼쪽 영상
//	double** WR_Ch1, **WR_Ch2, **WR_Ch3; //오른쪽 영상
//
//
//	double** Out_Ch1_R, **Out_Ch1_G, **Out_Ch1_B;
//	double** Out_Ch2_R, **Out_Ch2_G, **Out_Ch2_B;
//	double** Out_Ch3_R, **Out_Ch3_G, **Out_Ch3_B;
//
//
//	//메모리 할당
//
//	WL_Ch1 = dmatrix(nH, nW); WL_Ch2 = dmatrix(nH, nW); WL_Ch3 = dmatrix(nH, nW);
//	WR_Ch1 = dmatrix(nH, nW); WR_Ch2 = dmatrix(nH, nW); WR_Ch3 = dmatrix(nH, nW);
//
//	GL_Ch1_R = dmatrix(nH, nW);	GL_Ch1_G = dmatrix(nH, nW);	GL_Ch1_B = dmatrix(nH, nW);
//	GR_Ch1_R = dmatrix(nH, nW);	GR_Ch1_G = dmatrix(nH, nW);	GR_Ch1_B = dmatrix(nH, nW);
//
//	GL_Ch2_R = dmatrix(nH, nW);	GL_Ch2_G = dmatrix(nH, nW);	GL_Ch2_B = dmatrix(nH, nW);
//	GR_Ch2_R = dmatrix(nH, nW);	GR_Ch2_G = dmatrix(nH, nW);	GR_Ch2_B = dmatrix(nH, nW);
//
//	GL_Ch3_R = dmatrix(nH, nW);	GL_Ch3_G = dmatrix(nH, nW);	GL_Ch3_B = dmatrix(nH, nW);
//	GR_Ch3_R = dmatrix(nH, nW);	GR_Ch3_G = dmatrix(nH, nW);	GR_Ch3_B = dmatrix(nH, nW);
//
//	BL_Ch1_R = dmatrix(nH, nW); BL_Ch1_G = dmatrix(nH, nW);	BL_Ch1_B = dmatrix(nH, nW);
//	BR_Ch1_R = dmatrix(nH, nW);	BR_Ch1_G = dmatrix(nH, nW);	BR_Ch1_B = dmatrix(nH, nW);
//
//	BL_Ch2_R = dmatrix(nH, nW);	BL_Ch2_G = dmatrix(nH, nW);	BL_Ch2_B = dmatrix(nH, nW);
//	BR_Ch2_R = dmatrix(nH, nW);	BR_Ch2_G = dmatrix(nH, nW);	BR_Ch2_B = dmatrix(nH, nW);
//
//	BL_Ch3_R = dmatrix(nH, nW);	BL_Ch3_G = dmatrix(nH, nW);	BL_Ch3_B = dmatrix(nH, nW);
//	BR_Ch3_R = dmatrix(nH, nW);	BR_Ch3_G = dmatrix(nH, nW);	BR_Ch3_B = dmatrix(nH, nW);
//
//	Out_Ch1_R = dmatrix(nH, nW); Out_Ch1_G = dmatrix(nH, nW); Out_Ch1_B = dmatrix(nH, nW);
//	Out_Ch2_R = dmatrix(nH, nW); Out_Ch2_G = dmatrix(nH, nW); Out_Ch2_B = dmatrix(nH, nW);
//	Out_Ch3_R = dmatrix(nH, nW); Out_Ch3_G = dmatrix(nH, nW); Out_Ch3_B = dmatrix(nH, nW);
//
//	//-----------------------------------------------------------------------------------------
//
//	//Ch1(high frequency) 이미지 필터링
//	GaussianFilter(Gfilter, dSigma, nGSize);
//
//	//이미지 가우시안 필터링
//	MirrorPadding(GL_Ch1_R, Gfilter, imgLtrans.imgRed, nGSize, nW, nH); MirrorPadding(GL_Ch1_G, Gfilter, imgLtrans.imgGreen, nGSize, nW, nH); MirrorPadding(GL_Ch1_B, Gfilter, imgLtrans.imgBlue, nGSize, nW, nH);
//	MirrorPadding(GR_Ch1_R, Gfilter, imgRtrans.imgRed, nGSize, nW, nH); MirrorPadding(GR_Ch1_G, Gfilter, imgRtrans.imgGreen, nGSize, nW, nH); MirrorPadding(GR_Ch1_B, Gfilter, imgRtrans.imgBlue, nGSize, nW, nH);
//
//	//웨이트 가우시안 필터링
//	MirrorPadding(WL_Ch1, Gfilter, weightLtrans, nGSize, nW, nH); MirrorPadding(WR_Ch1, Gfilter, weightRtrans, nGSize, nW, nH);
//
//	double val;
//	for (int y = 0; y<nH; y++)
//		for (int x = 0; x<nW; x++)
//		{
//			BL_Ch1_R[y][x] = imgLtrans.imgRed[y][x] - GL_Ch1_R[y][x];
//			BL_Ch1_G[y][x] = imgLtrans.imgGreen[y][x] - GL_Ch1_G[y][x];
//			BL_Ch1_B[y][x] = imgLtrans.imgBlue[y][x] - GL_Ch1_B[y][x];
//
//			BR_Ch1_R[y][x] = imgRtrans.imgRed[y][x] - GR_Ch1_R[y][x];
//			BR_Ch1_G[y][x] = imgRtrans.imgGreen[y][x] - GR_Ch1_G[y][x];
//			BR_Ch1_B[y][x] = imgRtrans.imgBlue[y][x] - GR_Ch1_B[y][x];
//
//			val = (BL_Ch1_R[y][x] * WL_Ch1[y][x] + BR_Ch1_R[y][x] * WR_Ch1[y][x]) / (WL_Ch1[y][x] + WR_Ch1[y][x]);
//			Out_Ch1_R[y][x] = val;
//
//			val = (BL_Ch1_G[y][x] * WL_Ch1[y][x] + BR_Ch1_G[y][x] * WR_Ch1[y][x]) / (WL_Ch1[y][x] + WR_Ch1[y][x]);
//			Out_Ch1_G[y][x] = val;
//
//			val = (BL_Ch1_B[y][x] * WL_Ch1[y][x] + BR_Ch1_B[y][x] * WR_Ch1[y][x]) / (WL_Ch1[y][x] + WR_Ch1[y][x]);
//			Out_Ch1_B[y][x] = val;
//		}
//
//	//Ch2(middle frequency) 이미지 필터링
//	dSigma1 = sqrt(3.)*dSigma;
//	GaussianFilter(Gfilter, dSigma1, nGSize);
//
//	MirrorPadding(GL_Ch2_R, Gfilter, GL_Ch1_R, nGSize, nW, nH); MirrorPadding(GL_Ch2_G, Gfilter, GL_Ch1_R, nGSize, nW, nH); MirrorPadding(GL_Ch2_B, Gfilter, GL_Ch1_R, nGSize, nW, nH);
//	MirrorPadding(GR_Ch2_R, Gfilter, GR_Ch1_R, nGSize, nW, nH); MirrorPadding(GR_Ch2_G, Gfilter, GR_Ch1_G, nGSize, nW, nH); MirrorPadding(GR_Ch2_B, Gfilter, GR_Ch1_B, nGSize, nW, nH);
//
//	MirrorPadding(WL_Ch2, Gfilter, WL_Ch1, nGSize, nW, nH); MirrorPadding(WR_Ch2, Gfilter, WR_Ch1, nGSize, nW, nH);
//
//	for (y = 0; y<nH; y++)
//		for (x = 0; x<nW; x++)
//		{
//			BL_Ch2_R[y][x] = GL_Ch1_R[y][x] - GL_Ch2_R[y][x];
//			BL_Ch2_G[y][x] = GL_Ch1_G[y][x] - GL_Ch2_G[y][x];
//			BL_Ch2_B[y][x] = GL_Ch1_B[y][x] - GL_Ch2_B[y][x];
//
//			BR_Ch2_R[y][x] = GR_Ch1_R[y][x] - GR_Ch2_R[y][x];
//			BR_Ch2_G[y][x] = GR_Ch1_G[y][x] - GR_Ch2_G[y][x];
//			BR_Ch2_B[y][x] = GR_Ch1_B[y][x] - GR_Ch2_B[y][x];
//
//			val = (BL_Ch2_R[y][x] * WL_Ch2[y][x] + BR_Ch2_R[y][x] * WR_Ch2[y][x]) / (WL_Ch2[y][x] + WR_Ch2[y][x]);
//			Out_Ch2_R[y][x] = val;
//
//			val = (BL_Ch2_G[y][x] * WL_Ch2[y][x] + BR_Ch2_G[y][x] * WR_Ch2[y][x]) / (WL_Ch2[y][x] + WR_Ch2[y][x]);
//			Out_Ch2_G[y][x] = val;
//
//			val = (BL_Ch2_B[y][x] * WL_Ch2[y][x] + BR_Ch2_B[y][x] * WR_Ch2[y][x]) / (WL_Ch2[y][x] + WR_Ch2[y][x]);
//			Out_Ch2_B[y][x] = val;
//		}
//
//	//Ch3(low frequency) 이미지 필터링
//	dSigma2 = sqrt(5.)*dSigma;
//	GaussianFilter(Gfilter, dSigma2, nGSize);
//
//	MirrorPadding(GL_Ch3_R, Gfilter, GL_Ch2_R, nGSize, nW, nH); MirrorPadding(GL_Ch3_G, Gfilter, GL_Ch2_R, nGSize, nW, nH); MirrorPadding(GL_Ch3_B, Gfilter, GL_Ch2_R, nGSize, nW, nH);
//	MirrorPadding(GR_Ch3_R, Gfilter, GR_Ch2_R, nGSize, nW, nH); MirrorPadding(GR_Ch3_G, Gfilter, GR_Ch2_G, nGSize, nW, nH); MirrorPadding(GR_Ch3_B, Gfilter, GR_Ch2_B, nGSize, nW, nH);
//
//	MirrorPadding(WL_Ch3, Gfilter, WL_Ch2, nGSize, nW, nH); MirrorPadding(WR_Ch3, Gfilter, WR_Ch2, nGSize, nW, nH);
//
//	for (y = 0; y<nH; y++)
//		for (x = 0; x<nW; x++)
//		{
//			BL_Ch3_R[y][x] = GL_Ch2_R[y][x];// -GL_Ch3_R[y][x];
//			BL_Ch3_G[y][x] = GL_Ch2_G[y][x];// -GL_Ch3_G[y][x];
//			BL_Ch3_B[y][x] = GL_Ch2_B[y][x];// -GL_Ch3_B[y][x];
//
//			BR_Ch3_R[y][x] = GR_Ch2_R[y][x];// -GR_Ch3_R[y][x];
//			BR_Ch3_G[y][x] = GR_Ch2_G[y][x];// -GR_Ch3_G[y][x];
//			BR_Ch3_B[y][x] = GR_Ch2_B[y][x];// -GR_Ch3_B[y][x];
//
//			val = (BL_Ch3_R[y][x] * WL_Ch3[y][x] + BR_Ch3_R[y][x] * WR_Ch3[y][x]) / (WL_Ch3[y][x] + WR_Ch3[y][x]);
//			Out_Ch3_R[y][x] = val;
//
//			val = (BL_Ch3_G[y][x] * WL_Ch3[y][x] + BR_Ch3_G[y][x] * WR_Ch3[y][x]) / (WL_Ch3[y][x] + WR_Ch3[y][x]);
//			Out_Ch3_G[y][x] = val;
//
//			val = (BL_Ch3_B[y][x] * WL_Ch3[y][x] + BR_Ch3_B[y][x] * WR_Ch3[y][x]) / (WL_Ch3[y][x] + WR_Ch3[y][x]);
//			Out_Ch3_B[y][x] = val;
//		}
//
//	for (y = 0; y < nH; y++)
//		for (x = 0; x < nW; x++)
//		{
//			if (WR_Ch3[y][x] == 0)
//			{
//				WR_Ch3[y][x] = 1;
//			}
//			else
//				WR_Ch3[y][x] = 0;
//		}
//
//	// 블렌딩 합치기
//	for (int y = 0; y<nH; y++)
//		for (int x = 0; x<nW; x++)
//		{
//			val = Out_Ch1_R[y][x] + Out_Ch2_R[y][x] + Out_Ch3_R[y][x];
//			val = val < 0 ? 0 : val;
//			val = val > 255 ? 255 : val;
//			Result_R[y][x] = val;
//
//			val = Out_Ch1_G[y][x] + Out_Ch2_G[y][x] + Out_Ch3_G[y][x];
//			val = val < 0 ? 0 : val;
//			val = val > 255 ? 255 : val;
//			Result_G[y][x] = val;
//
//			val = Out_Ch1_B[y][x] + Out_Ch2_B[y][x] + Out_Ch3_B[y][x];
//			val = val < 0 ? 0 : val;
//			val = val > 255 ? 255 : val;
//			Result_B[y][x] = val;
//		}
//
//
//	//메모리 해제
//	free_dmatrix(Gfilter, nGSize, nGSize);
//
//	free_dmatrix(WL_Ch1, nH, nW); free_dmatrix(WL_Ch2, nH, nW); free_dmatrix(WL_Ch3, nH, nW);
//	free_dmatrix(WR_Ch1, nH, nW); free_dmatrix(WR_Ch2, nH, nW); free_dmatrix(WR_Ch3, nH, nW);
//
//	free_dmatrix(GL_Ch1_R, nH, nW); free_dmatrix(GL_Ch1_G, nH, nW); free_dmatrix(GL_Ch1_B, nH, nW);
//	free_dmatrix(GR_Ch1_R, nH, nW); free_dmatrix(GR_Ch1_G, nH, nW); free_dmatrix(GR_Ch1_B, nH, nW);
//
//	free_dmatrix(GL_Ch2_R, nH, nW); free_dmatrix(GL_Ch2_G, nH, nW); free_dmatrix(GL_Ch2_B, nH, nW);
//	free_dmatrix(GR_Ch2_R, nH, nW); free_dmatrix(GR_Ch2_G, nH, nW); free_dmatrix(GR_Ch2_B, nH, nW);
//
//	free_dmatrix(GL_Ch3_R, nH, nW); free_dmatrix(GL_Ch3_G, nH, nW); free_dmatrix(GL_Ch3_B, nH, nW);
//	free_dmatrix(GR_Ch3_R, nH, nW); free_dmatrix(GR_Ch3_G, nH, nW); free_dmatrix(GR_Ch3_B, nH, nW);
//
//	free_dmatrix(BL_Ch1_R, nH, nW); free_dmatrix(BL_Ch1_G, nH, nW); free_dmatrix(BL_Ch1_B, nH, nW);
//	free_dmatrix(BR_Ch1_R, nH, nW); free_dmatrix(BR_Ch1_G, nH, nW); free_dmatrix(BR_Ch1_B, nH, nW);
//
//	free_dmatrix(BL_Ch2_R, nH, nW); free_dmatrix(BL_Ch2_G, nH, nW); free_dmatrix(BL_Ch2_B, nH, nW);
//	free_dmatrix(BR_Ch2_R, nH, nW); free_dmatrix(BR_Ch2_G, nH, nW); free_dmatrix(BR_Ch2_B, nH, nW);
//
//	free_dmatrix(BL_Ch3_R, nH, nW); free_dmatrix(BL_Ch3_G, nH, nW); free_dmatrix(BL_Ch3_B, nH, nW);
//	free_dmatrix(BR_Ch3_R, nH, nW); free_dmatrix(BR_Ch3_G, nH, nW); free_dmatrix(BR_Ch3_B, nH, nW);
//
//	free_dmatrix(Out_Ch1_R, nH, nW); free_dmatrix(Out_Ch1_G, nH, nW); free_dmatrix(Out_Ch1_B, nH, nW);
//	free_dmatrix(Out_Ch2_R, nH, nW); free_dmatrix(Out_Ch2_G, nH, nW); free_dmatrix(Out_Ch2_B, nH, nW);
//	free_dmatrix(Out_Ch3_R, nH, nW); free_dmatrix(Out_Ch3_G, nH, nW); free_dmatrix(Out_Ch3_B, nH, nW);
//}
//
//void CImageStitchBasic::MakeWeightSeam(double** weightSeam, int nH, int nW)
//{
//	int i, j;
//	double dCenH, dCenW;
//	double dTmp;
//
//	dCenH = double(nH) / 2.;
//	dCenW = double(nW) / 2.;
//
//	for (i = 0; i < nH; i++)
//		for (j = 0; j < nW; j++)
//		{
//			dTmp = 1. - (1. / dCenH)*(abs(dCenH - double(i)));
//			dTmp = dTmp - (dTmp / dCenW)*(abs(dCenW - double(j)));
//
//			if (dTmp > 0)
//			{
//				dTmp = dTmp;
//			}
//
//			weightSeam[i][j] = dTmp;
//		}
//}
//
//int CImageStitchBasic::BilinearInterpolation(Point2f point, BYTE** image, int nH, int nW)
//{
//	int result;
//	double dX1, dX2;
//	double dY1, dY2;
//	double IA, IB, IC, ID;
//	double Itmp1, Itmp2;
//	CvPoint A;
//
//	A.x = int(point.x);
//	A.y = int(point.y);
//
//	if (A.x >= nW - 1 || A.y >= nH - 1)
//	{
//		result = image[int(point.y)][int(point.x)];
//		return result;
//	}
//
//
//	IA = image[A.y][A.x];		IB = image[A.y][A.x + 1];
//	IC = image[A.y + 1][A.x];	ID = image[A.y + 1][A.x + 1];
//
//	dX1 = point.x - A.x;		dX2 = A.x + 1 - point.x;
//	dY1 = point.y - A.y;		dY2 = A.y + 1 - point.y;
//
//	// I'1,2 값 구하기
//	Itmp1 = ((dX1 * IB) + (dX2 * IA));
//	Itmp2 = ((dX1 * ID) + (dX2 * IC));
//
//	// result 값 구하기
//	result = int(((dY1 * Itmp2) + (dY2 * Itmp1)));
//
//	return result;
//}
////=================================================================================================================================================
////=====================================================<Meanshift Image Stitching>=================================================================
////=================================================================================================================================================
//
//CImageStitchMeanShift::CImageStitchMeanShift()
//{
//	arrErr = NULL;
//	arrInfo = NULL;
//	arrFeatureNum = NULL;
//	arrRegion = NULL;
//	arrHomo = NULL;
//
//	LabelMeanShift = imatrix(imgR.nH, imgR.nW);
//	weightObj = cmatrix(imgR.nH, imgR.nW);
//
//	imgSegment.imgRed = cmatrix(imgR.nH, imgR.nW);
//	imgSegment.imgGreen = cmatrix(imgR.nH, imgR.nW);
//	imgSegment.imgBlue = cmatrix(imgR.nH, imgR.nW);
//	imgSegment.nH = imgR.nH;
//	imgSegment.nW = imgR.nW;
//}
//
//CImageStitchMeanShift::CImageStitchMeanShift(BYTE* img1D_L, BYTE* img1D_R, int nHL, int nWL, int nHR, int nWR, CString fileName) : CImageStitchBasic(img1D_L, img1D_R, nHL, nWL, nHR, nWR)
//{
//	arrErr = NULL;
//	arrInfo = NULL;
//	arrFeatureNum = NULL;
//	arrRegion = NULL;
//	arrHomo = NULL;
//
//	strFileName = fileName;
//	LabelMeanShift = imatrix(imgR.nH, imgR.nW);
//	weightObj = cmatrix(imgR.nH, imgR.nW);
//
//	imgSegment.imgRed = cmatrix(imgR.nH, imgR.nW);
//	imgSegment.imgGreen = cmatrix(imgR.nH, imgR.nW);
//	imgSegment.imgBlue = cmatrix(imgR.nH, imgR.nW);
//	imgSegment.nH = imgR.nH;
//	imgSegment.nW = imgR.nW;
//}
//
//CImageStitchMeanShift::~CImageStitchMeanShift()
//{
//	if (arrErr != NULL)
//		delete[] arrErr;
//
//	if (arrInfo != NULL)
//		delete[] arrInfo;
//
//	if (arrFeatureNum != NULL)
//		delete[] arrFeatureNum;
//
//	if (arrRegion != NULL)
//		delete[] arrRegion;
//
//	if (arrHomo != NULL)
//		delete[] arrHomo;
//
//	free_imatrix(LabelMeanShift, imgR.nH, imgR.nW);
//	free_cmatrix(weightObj, imgR.nH, imgR.nW);
//	delete[] arrObjIdx;
//
//	free_cmatrix(imgSegment.imgRed, imgR.nH, imgR.nW);
//	free_cmatrix(imgSegment.imgGreen, imgR.nH, imgR.nW);
//	free_cmatrix(imgSegment.imgBlue, imgR.nH, imgR.nW);
//}
//
///****************************************************
//<SegmentMeanshift>
//
//Byte Image를 IplImage로 바꾸고 MeanShift segmentation를 해준다.
//
//minFeature 이상의 특징점을 가지고 있는 Region Number와
//minArea 이상의 영역을 가지고 있는 Region Number을 골라준다.
//그리고 그 영역에 대한 Index 배열을 가진다.
//
//------------------------------------
//Pre : imgR.nW, imgR.nH, imgR.imgRed, imgR.imgBlue, imgR.imgGreen, minrRegion, minFeature, minArea
//Post : nNumSeg, LabelMeanShift
//
//*******************************************************/
//void CImageStitchMeanShift::SegmentMeanshift(int minRegion)
//{
//	int i, j, x, y;
//
//	IplImage* inputIplImg;
//	IplImage* resultIplImg;
//
//	int nIdx;
//	int tmpX, tmpY;
//	int tmp = 0;
//
//	inputIplImg = cvCreateImage(cvSize(imgR.nW, imgR.nH), IPL_DEPTH_8U, 3);
//	resultIplImg = cvCreateImage(cvSize(imgR.nW, imgR.nH), IPL_DEPTH_8U, 3);
//
//	for (y = 0; y < imgR.nH; y++)
//		for (x = 0; x < imgR.nW; x++)
//		{
//			nIdx = y * inputIplImg->widthStep + x * inputIplImg->nChannels;
//			inputIplImg->imageData[nIdx] = imgR.imgBlue[y][x];
//			inputIplImg->imageData[nIdx + 1] = imgR.imgGreen[y][x];
//			inputIplImg->imageData[nIdx + 2] = imgR.imgRed[y][x];
//		}
//
//	nNumSeg = MeanShift(inputIplImg, LabelMeanShift, resultIplImg, minRegion);
//
//	// 전체 Region를 Obj후보군으로 설정
//	nNumObjCandi = nNumSeg;
//	arrObjIdx = new int[nNumSeg];
//	arrErr = new int[nNumSeg];
//	arrInfo = new double[nNumSeg];
//	arrFeatureNum = new int[nNumSeg];
//	arrRegion = new int[nNumSeg];
//	arrHomo = new Mat[nNumSeg];
//	
//	for (i = 0; i < nNumSeg; i++)
//	{
//		arrObjIdx[i] = i;
//		arrErr[i] = -1;
//		arrInfo[i] = -1;
//		arrFeatureNum[i] = 0;
//		arrRegion[i] = 0;
//		arrHomo[i] = 0;
//	}
//
//	// Set FeatreNum, Region, Homo
//	GetArrFeatureNum();
//	GetArrRegion();
//	GetArrHomo();
//
//	// Set Global Homography
//	homoGlo = findHomography(GodPointR, GodPointL, CV_RANSAC);
//
//	DrawSegment();
//
//	cvReleaseImage(&inputIplImg);
//	cvReleaseImage(&resultIplImg);
//}
//
//
//void CImageStitchMeanShift::FindSingleObject(float fDiffInlierRate, bool bDrawErrInfo)
//{
//	int i, j;
//	double dTmp;
//	double dMaxInlier=0;
//	int* arrTmp;
//	Mat tmpHomo;
//	vector <Point2f> objPointR, objPointL;
//
//	// Inlier Rate가 가장 높은 Index, Background Homography 구하기
//	for (i = 0; i < nNumSeg; i++)
//	{
//		if (arrObjIdx[i] != -1)
//		{
//			arrTmp = new int[1];
//			arrTmp[0] = arrObjIdx[i];
//
//			arrInfo[i] = GetExceptInlierRate(arrTmp, 1, 0, tmpHomo);
//
//			if (dMaxInlier < arrInfo[i])
//			{
//				dMaxInlier = arrInfo[i];
//				nIdxObj = arrObjIdx[i];
//				homoBack = tmpHomo;
//			}
//
//			delete arrTmp;
//		}
//	}
//
//	// Set Object Homography
//	homoObj = arrHomo[nIdxObj];
//
//	// 최고 물체와 나머지의 평균의 차이를 비교
//	double meanInlier=0;
//	int tmpNum = 0;
//
//	for (i = 0; i < nNumSeg; i++)
//	{
//		if (arrObjIdx[i] != -1)
//		{
//			if (arrInfo[i] != dMaxInlier)
//			{
//				meanInlier += arrInfo[i];
//				tmpNum++;
//			}
//		}
//	}
//
//	meanInlier /= tmpNum;
//
//	if (bDrawErrInfo)
//		DrawActiveErr();
//
//	if (dMaxInlier - meanInlier < fDiffInlierRate)
//		bExeDualHomo = false;
//}
//
///****************************************************
//<DetermineActive>
//
//여러 기준을 통해 Object 후보군을 판별한다.
//
//후보군 판별 후 새로운 후보군 array를 작성한다.
//------------------------------------
//Pre : 
//Post : arrObjIdx, nNumObjCandi, arrObjInlierRate
//
//*******************************************************/
//void CImageStitchMeanShift::DetermineActive(int minFeature, int minArea, double minFeatureRate, int minKLdiver, bool bDrawErrInfo)
//{
//	int i;
//	int tmpNum = 0;
//	bool bOnFeatureRate;
//
//	ActiveFeatureNum(minFeature);
//	ActiveRegion(minArea);
//	bOnFeatureRate = ActiveFeatureRate(minFeatureRate);
//	ActiveMisMatch();
//	ActiveKLdivergence(minKLdiver);
//
//	//featureRate가 높을 때에는 바로 빠져나가게 한다.
//	if (bOnFeatureRate)
//	{
//		bExeDualHomo = false;
//
//		if (bDrawErrInfo)
//			DrawActiveErr();
//			
//		return;
//	}
//
//	// arrObjIdx, nNumObjCandi 다시 판별
//	// tmpNum 구하기
//	for (i = 0; i < nNumObjCandi; i++)
//	{
//		if (arrObjIdx[i] != -1)
//		{
//			tmpNum++;
//		}
//	}
//
//	// tmpIdx 구하기
//	if (tmpNum <= 1)
//	{
//		bExeDualHomo = false;
//		return;
//	}                                                              
//	else
//	{
//		bExeDualHomo = true;
//	}
//}
//
//void CImageStitchMeanShift::MakeWeightMap(int dilation, bool bOnDraw)
//{
//	int x, y, i, j;
//
//	BYTE ** tmpWeight;
//	int** tmpLabel;
//	int windowSize;
//
//	windowSize = imgR.nW / 10;
//
//	tmpWeight = cmatrix(imgR.nH, imgR.nW);
//	tmpLabel = imatrix(imgR.nH, imgR.nW);
//
//	for (i = 0; i < imgR.nH; i++)
//		for (j = 0; j < imgR.nW; j++)
//		{
//			if (LabelMeanShift[i][j] == nIdxObj)
//				weightObj[i][j] = 255;
//			else
//				weightObj[i][j] = 0;
//		}
//
//	// open
//	ErosionBinary(weightObj, tmpWeight, imgR.nW, imgR.nH, 0, 10);
//	DilationBinary(tmpWeight, weightObj, imgR.nW, imgR.nH, 0, 10);
//
//	// Labeling
//	Labeling(weightObj, tmpLabel, imgR.nW, imgR.nH, 1000);
//
//	for (y = 0; y < imgR.nH; y++)
//		for (x = 0; x < imgR.nW; x++)
//		{
//			if (tmpLabel[y][x] == 0)
//			{
//				weightObj[y][x] = 255;
//			}
//			else
//				weightObj[y][x] = 0;
//		}
//
//
//	//Fill Algorithm
//	Fill(weightObj, imgR.nW, imgR.nH);
//
//
//	// dilation
//	DilationBinary(weightObj, tmpWeight, imgR.nW, imgR.nH, 0, dilation);
//
//	for (y = 0; y < imgR.nH; y++)
//		for (x = 0; x < imgR.nW; x++)
//		{
//			weightObj[y][x] = tmpWeight[y][x];
//		}
//
//	if (bOnDraw)
//	{
//		DisplayCimage2D(weightObj, imgR.nW, imgR.nH, 0, 0);
//	}
//
//	free_cmatrix(tmpWeight, imgR.nH, imgR.nW);
//	free_imatrix(tmpLabel, imgR.nH, imgR.nW);
//}
//
//void CImageStitchMeanShift::WarpHomography(bool bWrite)
//{
//	int x, y, i;
//
//	Image2D imgBack;
//
//	// value for get Image size
//	int offsetX = 0, offsetY = 0;
//	int maxX_totalR = 0, maxY_totalR = 0, minX_totalR = imgR.nW, minY_totalR = imgR.nH;
//	double maxX_transR = 0, maxY_transR = 0, minX_transR = imgR.nW, minY_transR = imgR.nH;
//	int nH_total, nW_total;
//	Point2f inputPoint, transPoint;
//
//	// get image's size
//	for (y = 0; y < imgR.nH; y++)
//		for (x = 0; x < imgR.nW; x++)
//		{
//			inputPoint = cvPoint(x, y);
//
//			if (bExeDualHomo)
//				GetPerspectivePoint(inputPoint, transPoint, homoBack);
//			else
//				GetPerspectivePoint(inputPoint, transPoint, homoGlo);
//
//			if (transPoint.y > maxY_transR) maxY_transR = transPoint.y;
//			if (transPoint.x > maxX_transR) maxX_transR = transPoint.x;
//			if (transPoint.y < minY_transR) minY_transR = transPoint.y;
//			if (transPoint.x < minX_transR) minX_transR = transPoint.x;
//		}
//
//	if (minY_transR < 0) minY_totalR = minY_transR; else minY_totalR = 0;
//	if (maxY_transR > imgR.nH - 1) maxY_totalR = maxY_transR; else maxY_totalR = imgR.nH - 1;
//
//	if (minX_transR < 0) minX_totalR = minX_transR; else minX_totalR = 0;
//	if (maxX_transR > imgR.nW - 1) maxX_totalR = maxX_transR; else maxX_totalR = imgR.nW - 1;
//
//	nH_total = maxY_totalR - minY_totalR + 1;
//	nW_total = maxX_totalR - minX_totalR + 1;
//
//	if (minY_transR < 0) offsetY = abs(minY_transR);
//	if (minX_totalR < 0) offsetX = abs(minX_totalR);
//
//	if (nH_total > 10000 || nW_total > 10000)
//	{
//		//// 글씨 넣기
//		FILE *ferr;
//		CString errPos;
//
//		// 파일 열기
//		errPos = _T("C:\\Users\\vision_jy\\Desktop\\1. Image DB\\Image Stitching\\실험폴더\\no\\");
//		errPos += strFileName;
//		errPos += _T("err.txt");
//
//		ferr = fopen(errPos, "w");
//
//		fprintf(ferr, "Homography is error!!!");
//		
//		fclose(ferr);
//
//		return;
//	}
//
//	//Set total Image's nH, nW
//	imgTotal.imgRed = cmatrix(nH_total, nW_total); imgTotal.imgGreen = cmatrix(nH_total, nW_total); imgTotal.imgBlue = cmatrix(nH_total, nW_total);
//	imgBack.imgRed = cmatrix(nH_total, nW_total); imgBack.imgGreen = cmatrix(nH_total, nW_total); imgBack.imgBlue = cmatrix(nH_total, nW_total);
//	imgLtrans.imgRed = cmatrix(nH_total, nW_total); imgLtrans.imgGreen = cmatrix(nH_total, nW_total); imgLtrans.imgBlue = cmatrix(nH_total, nW_total);
//	imgRtrans.imgRed = cmatrix(nH_total, nW_total); imgRtrans.imgGreen = cmatrix(nH_total, nW_total); imgRtrans.imgBlue = cmatrix(nH_total, nW_total);
//	weightLtrans = dmatrix(nH_total, nW_total); weightRtrans = dmatrix(nH_total, nW_total);
//
//	imgTotal.nH = nH_total;	imgTotal.nW = nW_total;
//	imgLtrans.nH = nH_total; imgLtrans.nW = nW_total;
//	imgRtrans.nH = nH_total; imgRtrans.nW = nW_total;
//
//	for (int y = 0; y < nH_total; y++)
//		for (int x = 0; x < nW_total; x++)
//		{
//			imgTotal.imgRed[y][x] = 0; imgTotal.imgGreen[y][x] = 0; imgTotal.imgBlue[y][x] = 0;
//			imgBack.imgRed[y][x] = 0; imgBack.imgGreen[y][x] = 0; imgBack.imgBlue[y][x] = 0;
//
//			imgLtrans.imgRed[y][x] = 0; imgLtrans.imgGreen[y][x] = 0; imgLtrans.imgBlue[y][x] = 0;
//			imgRtrans.imgRed[y][x] = 0; imgRtrans.imgGreen[y][x] = 0; imgRtrans.imgBlue[y][x] = 0;
//
//			weightLtrans[y][x] = 0; weightRtrans[y][x] = 0;
//		}
//
//	//=====================================================================================================================================================
//	// 싱글 호모그래피
//	if (!bExeDualHomo)
//	{
//		// 왼쪽
//		for (y = 0; y < imgL.nH; y++)
//			for (x = 0; x < imgL.nW; x++)
//			{
//				imgLtrans.imgRed[y + offsetY][x + offsetX] = imgL.imgRed[y][x];
//				imgLtrans.imgGreen[y + offsetY][x + offsetX] = imgL.imgGreen[y][x];
//				imgLtrans.imgBlue[y + offsetY][x + offsetX] = imgL.imgBlue[y][x];
//				weightLtrans[y + offsetY][x + offsetX] = weightSeamL[y][x];
//			}
//
//		// 오른쪽
//		Mat inverH = homoGlo.inv();
//
//		for (y = minY_transR; y < maxY_transR; y++)
//			for (x = minX_transR; x < maxX_transR; x++) {
//
//				inputPoint = Point2f(x, y);
//				GetPerspectivePoint(inputPoint, transPoint, inverH);
//				if (transPoint.x >= 0 && transPoint.x < imgR.nW && transPoint.y >= 0 && transPoint.y < imgR.nH)
//				{
//					imgRtrans.imgRed[y + offsetY][x + offsetX] = BilinearInterpolation(transPoint, imgR.imgRed, imgR.nH, imgR.nW);
//					imgRtrans.imgGreen[y + offsetY][x + offsetX] = BilinearInterpolation(transPoint, imgR.imgGreen, imgR.nH, imgR.nW);;
//					imgRtrans.imgBlue[y + offsetY][x + offsetX] = BilinearInterpolation(transPoint, imgR.imgBlue, imgR.nH, imgR.nW);;
//					weightRtrans[y + offsetY][x + offsetX] = weightSeamR[int(transPoint.y)][int(transPoint.x)];
//				}
//			}
//
//		// Seam 만들기
//		for (y = 0; y < nH_total; y++)
//			for (x = 0; x < nW_total; x++)
//			{
//				if (weightRtrans[y][x] > weightLtrans[y][x])
//				{
//					weightRtrans[y][x] = 1;
//					weightLtrans[y][x] = 0;
//				}
//
//				if (weightLtrans[y][x] > weightRtrans[y][x])
//				{
//					weightRtrans[y][x] = 0;
//					weightLtrans[y][x] = 1;
//				}
//			}
//
//		// multiband blending
//		this->MultiBandBlending(imgTotal.nH, imgTotal.nW, imgTotal.imgRed, imgTotal.imgGreen, imgTotal.imgBlue);
//	}
//
//	//==================================================================================================================================
//	// 듀얼 호모그래피
//	if (bExeDualHomo)
//	{
//		// 배경이미지 합성.
//		for (int y = 0; y < imgL.nH; y++)
//			for (int x = 0; x < imgL.nW; x++)
//			{
//				imgLtrans.imgRed[y + offsetY][x + offsetX] = imgL.imgRed[y][x];
//				imgLtrans.imgGreen[y + offsetY][x + offsetX] = imgL.imgGreen[y][x];
//				imgLtrans.imgBlue[y + offsetY][x + offsetX] = imgL.imgBlue[y][x];
//
//				weightLtrans[y + offsetY][x + offsetX] = weightSeamL[y][x];
//			}
//
//		Mat inverBackH = homoBack.inv();
//
//		for (int y = minY_transR; y < maxY_transR; y++)
//			for (int x = minX_transR; x < maxX_transR; x++) {
//
//				inputPoint = cvPoint(x, y);
//				GetPerspectivePoint(inputPoint, transPoint, inverBackH);
//
//				if (transPoint.x >= 0 && transPoint.x < imgR.nW && transPoint.y >= 0 && transPoint.y < imgR.nH)
//				{
//					imgRtrans.imgRed[y + offsetY][x + offsetX] = BilinearInterpolation(transPoint, imgR.imgRed, imgR.nH, imgR.nW);
//					imgRtrans.imgGreen[y + offsetY][x + offsetX] = BilinearInterpolation(transPoint, imgR.imgGreen, imgR.nH, imgR.nW);
//					imgRtrans.imgBlue[y + offsetY][x + offsetX] = BilinearInterpolation(transPoint, imgR.imgBlue, imgR.nH, imgR.nW);
//
//					if (weightObj[int(transPoint.y)][int(transPoint.x)] == 0){
//						weightRtrans[y + offsetY][x + offsetX] = weightSeamR[int(transPoint.y)][int(transPoint.x)];
//					}
//				}
//			}
//
//		for (y = 0; y < nH_total; y++)
//			for (x = 0; x < nW_total; x++)
//			{
//				if (weightRtrans[y][x] > weightLtrans[y][x])
//				{
//					weightRtrans[y][x] = 1;
//					weightLtrans[y][x] = 0;
//				}
//
//				if (weightLtrans[y][x] > weightRtrans[y][x])
//				{
//					weightRtrans[y][x] = 0;
//					weightLtrans[y][x] = 1;
//				}
//			}
//
//		this->MultiBandBlending(nH_total, nW_total,imgBack.imgRed, imgBack.imgGreen, imgBack.imgBlue);
//
//		//------------------------------------------------------------------------
//		// 물체이미지 합성.
//		// transL : 배경이미지, transR : 물체이미지
//
//		Mat inverObjH = homoObj.inv();
//
//		// 배경 웨이트맵 형성
//		for (int y = 0; y < imgL.nH; y++)
//			for (int x = 0; x < imgL.nW; x++)
//			{
//				weightLtrans[y + offsetY][x + offsetX] = 1;
//			}
//
//		for (int y = minY_transR; y < maxY_transR; y++)
//			for (int x = minX_transR; x < maxX_transR; x++) {
//
//				inputPoint = cvPoint(x, y);
//				GetPerspectivePoint(inputPoint, transPoint, inverBackH);
//
//				if (transPoint.x >= 0 && transPoint.x < imgR.nW && transPoint.y >= 0 && transPoint.y < imgR.nH)
//				{
//					if (weightObj[int(transPoint.y)][int(transPoint.x)] == 0)
//					{
//						weightLtrans[y + offsetY][x + offsetX] = 1;
//					}
//				}
//			}
//
//		
//		// 배경 이미지 형성 및 물체 이미지 초기화
//		for (int y = 0; y < nH_total; y++)
//			for (int x = 0; x < nW_total; x++)
//			{
//				imgLtrans.imgRed[y][x] = imgBack.imgRed[y][x];
//				imgLtrans.imgGreen[y][x] = imgBack.imgGreen[y][x];
//				imgLtrans.imgBlue[y][x] = imgBack.imgBlue[y][x];
//
//				imgRtrans.imgRed[y][x] = 0;
//				imgRtrans.imgGreen[y][x] = 0;
//				imgRtrans.imgBlue[y][x] = 0;
//
//				weightRtrans[y][x] = 0;
//			}
//
//		// 물체 이미지, 웨이트맵 형성
//
//		for (int y = minY_totalR; y < maxY_totalR; y++)
//			for (int x = minX_totalR; x < maxX_totalR; x++) {
//
//				inputPoint = cvPoint(x, y);
//				GetPerspectivePoint(inputPoint, transPoint, inverObjH);
//
//				if (transPoint.x >= 0 && transPoint.x < imgR.nW && transPoint.y >= 0 && transPoint.y < imgR.nH)
//				{
//
//					imgRtrans.imgRed[y + offsetY][x + offsetX] = BilinearInterpolation(transPoint, imgR.imgRed, imgR.nH, imgR.nW);
//					imgRtrans.imgGreen[y + offsetY][x + offsetX] = BilinearInterpolation(transPoint, imgR.imgGreen, imgR.nH, imgR.nW);
//					imgRtrans.imgBlue[y + offsetY][x + offsetX] = BilinearInterpolation(transPoint, imgR.imgBlue, imgR.nH, imgR.nW);
//
//					if (weightObj[int(transPoint.y)][int(transPoint.x)] == 255)
//					{
//						weightRtrans[y + offsetY][x + offsetX] = 1;
//						weightLtrans[y + offsetY][x + offsetX] = 0;
//					}
//				}
//			}
//
//		this->MultiBandBlending(nH_total, nW_total, imgTotal.imgRed, imgTotal.imgGreen, imgTotal.imgBlue);
//
//		//DisplayCimage2DColor(imgBack.imgRed, imgBack.imgGreen, imgBack.imgBlue, nW_total, nH_total, 0, 0);
//
//	}
//
//	// WriteImage
//	if (bWrite)
//	{
//		BYTE* img1D_Total = new BYTE[GetBmp24Size(imgTotal.nW, imgTotal.nH)];
//		BYTE* img1D_Seg = new BYTE[GetBmp24Size(imgR.nW, imgR.nH)];
//
//		int Pos;
//		CString FilePos;
//		CString FileMarkPos;
//		CString FileErrPos;
//
//		for (y = 0; y < imgTotal.nH; y++)
//			for (x = 0; x < imgTotal.nW; x++)
//			{
//				Pos = GetBmp24Pos(imgTotal.nW, imgTotal.nH, x, y);
//
//				img1D_Total[Pos++] = imgTotal.imgBlue[y][x];
//				img1D_Total[Pos++] = imgTotal.imgGreen[y][x];
//				img1D_Total[Pos] = imgTotal.imgRed[y][x];
//			}
//		
//		for (y = 0; y < imgR.nH; y++)
//			for (x = 0; x < imgR.nW; x++)
//			{
//				Pos = GetBmp24Pos(imgR.nW, imgR.nH, x, y);
//
//				img1D_Seg[Pos++] = imgSegment.imgBlue[y][x];
//				img1D_Seg[Pos++] = imgSegment.imgGreen[y][x];
//				img1D_Seg[Pos] = imgSegment.imgRed[y][x];
//			}
//
//		if (bExeDualHomo)
//		{
//			FilePos = _T("C:\\Users\\vision_jy\\Desktop\\1. Image DB\\Image Stitching\\실험폴더\\dual\\");
//			FilePos += strFileName;
//			FilePos += _T(".bmp");
//
//			FileMarkPos = _T("C:\\Users\\vision_jy\\Desktop\\1. Image DB\\Image Stitching\\실험폴더\\dual\\");
//			FileMarkPos += strFileName;
//			FileMarkPos += _T("seg.bmp");
//
//			FileErrPos = _T("C:\\Users\\vision_jy\\Desktop\\1. Image DB\\Image Stitching\\실험폴더\\dual\\");
//			FileErrPos += strFileName;
//			FileErrPos += _T("err.txt");
//		}
//		else
//		{
//			FilePos = _T("C:\\Users\\vision_jy\\Desktop\\1. Image DB\\Image Stitching\\실험폴더\\single\\");
//			FilePos += strFileName;
//			FilePos += _T(".bmp");
//
//			FileMarkPos = _T("C:\\Users\\vision_jy\\Desktop\\1. Image DB\\Image Stitching\\실험폴더\\single\\");
//			FileMarkPos += strFileName;
//			FileMarkPos += _T("seg.bmp");
//
//			FileErrPos = _T("C:\\Users\\vision_jy\\Desktop\\1. Image DB\\Image Stitching\\실험폴더\\single\\");
//			FileErrPos += strFileName;
//			FileErrPos += _T("err.txt");
//		}
//
//		SaveBmp(FilePos, img1D_Total, imgTotal.nW, imgTotal.nH);
//		SaveBmp(FileMarkPos, img1D_Seg, imgR.nW, imgR.nH);
//
//		//// 글씨 넣기
//		FILE *ferr;
//
//		// 파일 열기
//		ferr = fopen(FileErrPos, "w");
//
//		// 헤더 쓰기
//		int tmp = 0;
//		for (i = 0; i < nNumSeg; i++)
//		{
//			fprintf(ferr, "%d\t", i);
//			
//			switch (arrErr[i])
//			{
//			case 0:
//				fprintf(ferr, "%s\t\t", "feature number");
//				fprintf(ferr, "%d\n", int(arrInfo[i]));
//				break;
//
//			case 1:
//				fprintf(ferr, "%s\t\t", "region area");
//				fprintf(ferr, "%d\n", int(arrInfo[i]));
//				break;
//
//			case 2:
//				fprintf(ferr, "%s\t\t", "feature rate");
//				fprintf(ferr, "%f\n", arrInfo[i]);
//				break;
//
//			case 3:
//				fprintf(ferr, "%s\t\t", "mismatch");
//				fprintf(ferr, "%s\n", " ");
//				break;
//
//			case 4:
//				fprintf(ferr, "%s\t\t", "kl-divergence");
//				fprintf(ferr, "%f\n", arrInfo[i]);
//				break;
//
//			default:
//				fprintf(ferr, "%s\t\t", "obj candidate");
//				fprintf(ferr, "%s\t", " ");
//				if (nNumObjCandi > 1)
//					fprintf(ferr, "%f\n", arrInfo[i]);
//				else
//					fprintf(ferr, "%s\n", " ");
//				break;
//			}
//		}
//
//		fclose(ferr);
//
//		delete[] img1D_Seg;
//		delete[] img1D_Total;
//	}
//
//	DisplayCimage2DColor(imgTotal.imgRed, imgTotal.imgGreen, imgTotal.imgBlue, nW_total, nH_total, 0, 0);
//
//	free_cmatrix(imgBack.imgRed, nH_total, nW_total);
//	free_cmatrix(imgBack.imgGreen, nH_total, nW_total);
//	free_cmatrix(imgBack.imgBlue, nH_total, nW_total);
//
//	free_cmatrix(imgTotal.imgRed, imgTotal.nH, imgTotal.nW);
//	free_cmatrix(imgTotal.imgGreen, imgTotal.nH, imgTotal.nW);
//	free_cmatrix(imgTotal.imgBlue, imgTotal.nH, imgTotal.nW);
//
//	free_dmatrix(weightRtrans, imgTotal.nH, imgTotal.nW);
//	free_dmatrix(weightLtrans, imgTotal.nH, imgTotal.nW);
//
//	free_cmatrix(imgRtrans.imgRed, imgTotal.nH, imgTotal.nW);
//	free_cmatrix(imgRtrans.imgGreen, imgTotal.nH, imgTotal.nW);
//	free_cmatrix(imgRtrans.imgBlue, imgTotal.nH, imgTotal.nW);
//
//	free_cmatrix(imgLtrans.imgRed, imgTotal.nH, imgTotal.nW);
//	free_cmatrix(imgLtrans.imgGreen, imgTotal.nH, imgTotal.nW);
//	free_cmatrix(imgLtrans.imgBlue, imgTotal.nH, imgTotal.nW);
//}
//
///****************************************************
//<GetExcepInlierRate>
//
//해당 index의 영역을 제외한 부분의 Inlier Rate를 구해준다.
//
//Inlier의 판별은 거리 값으로 한다. (default :3)
//Inlier Rate는 (Inlier 갯수) / (제외된 특징점의 갯수)
//
//*Mode
//0 : 제외한 부분의 특징점에서 구한 Homography를 활용한다.
//1 : 정해진 Homography를 활용한다.
//
//------------------------------------
//Pre : arrIdx, numIdx
//Post :
//
//*******************************************************/
//double CImageStitchMeanShift::GetExceptInlierRate(int* arrIdx, int numIdx, int mode, Mat& homo)
//{
//	int i, j;
//	vector<Point2f> tmpPointR, tmpPointL;
//	Mat tmpHomo;
//	bool bCheck = false;
//
//	// 영역 이외의 특징점 분포를 구한다.
//	// 영역 이외의 호모그래피를 구한다.
//	for (int i = 0; i < nNumPoint; i++)
//	{
//		for (int j = 0; j < numIdx; j++)
//		{
//			if (LabelMeanShift[int(GodPointR[i].y)][int(GodPointR[i].x)] == arrIdx[j])
//			{
//				bCheck = true;
//			}
//		}
//
//		if (bCheck)
//		{
//			bCheck = false;
//		}
//		else
//		{
//			tmpPointR.push_back(GodPointR[i]);
//			tmpPointL.push_back(GodPointL[i]);
//		}
//	}
//
//	if (mode == 0)
//		homo = findHomography(tmpPointR, tmpPointL, CV_RANSAC);
//
//	// Inlier 비율을 구한다.
//	int numInlier;
//	int numSegPoint;
//	double dTmp;
//	double inlierRate;
//	Point2f tmpInput, tmpOutput;
//
//
//	numInlier = 0;
//	numSegPoint = tmpPointR.size();
//
//	for (i = 0; i < numSegPoint; i++)
//	{
//		tmpInput.x = tmpPointR[i].x;
//		tmpInput.y = tmpPointR[i].y;
//
//		GetPerspectivePoint(tmpInput, tmpOutput, homo);
//
//		dTmp = GetDistance(tmpOutput.x, tmpOutput.y, tmpPointL[i].x, tmpPointL[i].y);
//
//		if (dTmp < 3)
//		{
//			numInlier++;
//		}
//	}
//
//	inlierRate = float(numInlier) / float(numSegPoint);
//
//	return inlierRate;
//}
//
//double CImageStitchMeanShift::GetInlierRate(int Idx, Mat& homo)
//{
//	int i, j;
//	vector<Point2f> tmpPointR, tmpPointL;
//
//	for (i = 0; i < nNumPoint; i++)
//	{
//		if (LabelMeanShift[int(GodPointR[i].y)][int(GodPointR[i].x)] == Idx)
//		{
//			tmpPointR.push_back(GodPointR[i]);
//			tmpPointL.push_back(GodPointL[i]);
//		}
//	}
//
//	// Inlier 비율을 구한다.
//	int numInlier;
//	int numSegPoint;
//	double dTmp;
//	double inlierRate;
//	Point2f tmpInput, tmpOutput;
//
//	numInlier = 0;
//	numSegPoint = tmpPointR.size();
//
//	for (i = 0; i < numSegPoint; i++)
//	{
//		tmpInput.x = tmpPointR[i].x;
//		tmpInput.y = tmpPointR[i].y;
//
//		GetPerspectivePoint(tmpInput, tmpOutput, homo);
//
//		dTmp = GetDistance(tmpOutput.x, tmpOutput.y, tmpPointL[i].x, tmpPointL[i].y);
//
//		if (dTmp < 3)
//		{
//			numInlier++;
//		}
//	}
//
//	inlierRate = float(numInlier) / float(numSegPoint);
//
//	return inlierRate;
//}
//
//// 모든 영역의 특징점 갯수를 구한다. 
//void CImageStitchMeanShift::GetArrFeatureNum()
//{
//	int i;
//	int nTmp;
//
//	// Set Region's number for getting homography
//	for (i = 0; i < nNumPoint; i++)
//	{
//		nTmp = LabelMeanShift[int(GodPointR[i].y)][int(GodPointR[i].x)];
//		arrFeatureNum[nTmp]++;
//	}
//}
//
//// 모든 영역의 영역넓이를 구한다. 
//void CImageStitchMeanShift::GetArrRegion()
//{
//	int y, x;
//	int nTmp;
//
//	for (y = 0; y < imgR.nH; y++)
//		for (x = 0; x < imgR.nW; x++)
//		{
//			nTmp = LabelMeanShift[y][x];
//			arrRegion[nTmp]++;
//		}
//}
//
//// 모든 영역의 호모그래피를 구한다.
//// 단 먼저 GetFeatureNum이 구해져 있어야 한다.
//void CImageStitchMeanShift::GetArrHomo()
//{
//	int i;
//	vector<Point2f> tmpPointR, tmpPointL;
//
//	for (i = 0; i < nNumSeg; i++)
//	{
//		if (arrFeatureNum[i] > 4)
//		{
//			GetHomography(i, arrHomo[i]);
//		}
//		else
//		{
//			arrHomo[i] = -1;
//		}
//
//	}
//}
//
//void CImageStitchMeanShift::GetHomography(int Idx, Mat& homo)
//{
//	int i, j;
//	vector<Point2f> tmpPointR, tmpPointL;
//
//	for (i = 0; i < nNumPoint; i++)
//	{
//		if (LabelMeanShift[int(GodPointR[i].y)][int(GodPointR[i].x)] == Idx)
//		{
//			tmpPointR.push_back(GodPointR[i]);
//			tmpPointL.push_back(GodPointL[i]);
//		}
//	}
//
//	homo = findHomography(tmpPointR, tmpPointL, CV_RANSAC);
//}
//
//void CImageStitchMeanShift::DrawSegment()
//{
//	int i, j, x, y;
//
//	CString strNum;
//
//	Point2f* idxPos;
//
//	int** arrColor;
//	int tmp;
//	int markColor[3];
//	bool bBigMark;
//	int charColor;
//	int nH_img;
//
//	
//
//	arrColor = imatrix(nNumSeg, 3);
//
//	idxPos = new Point2f[nNumSeg];
//
//	GetRandColorArray(arrColor, nNumSeg);
//	//----------------------------------------------
//
//	// Region에 영역별 색깔 부여
//	for (y = 0; y < imgR.nH; y++)
//		for (x = 0; x < imgR.nW; x++)
//		{
//			tmp = LabelMeanShift[y][x];
//			imgSegment.imgRed[y][x] = arrColor[tmp][2];
//			imgSegment.imgGreen[y][x] = arrColor[tmp][1];
//			imgSegment.imgBlue[y][x] = arrColor[tmp][0];
//		}
//
//	// Region의 Index 출력 위치 구하기
//	for (i = 0; i < nNumObjCandi; i++)
//	{
//		for (y = 0; y < imgR.nH; y++)
//			for (x = 0; x < imgR.nW; x++)
//			{
//				if (LabelMeanShift[y][x] == arrObjIdx[i])
//				{
//					idxPos[i].y = y;
//					idxPos[i].x = x;
//					goto outsegmark;
//				}
//			}
//
//	outsegmark:
//		int a;
//	}
//
//	tmp = 0;
//
//	// Region's Mark 표시
//	for (i = 0; i < nNumObjCandi; i++)
//	{
//		strNum.Format(_T("%d"), arrObjIdx[i]);
//
//		// 크기 설정
//		if (arrObjIdx[i] == nIdxObj)
//			bBigMark = true;
//		else
//			bBigMark = false;
//
//		// 글자 체크
//		DrawTextOnImag2DGray(imgSegment.imgRed, imgR.nW, imgR.nH, idxPos[i].x, idxPos[i].y, strNum, DT_LEFT, 255, bBigMark);
//		DrawTextOnImag2DGray(imgSegment.imgGreen, imgR.nW, imgR.nH, idxPos[i].x, idxPos[i].y, strNum, DT_LEFT, 0, bBigMark);
//		DrawTextOnImag2DGray(imgSegment.imgBlue, imgR.nW, imgR.nH, idxPos[i].x, idxPos[i].y, strNum, DT_LEFT, 0, bBigMark);
//	}
//
//	// Feature point
//	for (int i = 0; i < nNumPoint; i++)
//	{
//		DrawCross(imgSegment.imgRed, imgR.nW, imgR.nH, CPoint(GodPointR[i].x, GodPointR[i].y), 0);
//		DrawCross(imgSegment.imgGreen, imgR.nW, imgR.nH, CPoint(GodPointR[i].x, GodPointR[i].y), 0);
//		DrawCross(imgSegment.imgBlue, imgR.nW, imgR.nH, CPoint(GodPointR[i].x, GodPointR[i].y), 0);
//	}
//
//	DisplayCimage2DColor(imgSegment.imgRed, imgSegment.imgGreen, imgSegment.imgBlue, imgR.nW, imgR.nH, 0, 0);
//	
//	delete[] idxPos;
//	delete[] arrColor;
//}
//
//double CImageStitchMeanShift::KLdivergenceForBivariateGaussiian(double* mean0, double* mean1, double** covMat0, double** covMat1)
//{
//	double tmp;
//	double** invCov1;
//	double** tmpMat;
//	double* tmpVec;
//	double* difMean;
//	double detCov0, detCov1;
//
//	invCov1 = dmatrix(2, 2);
//	tmpMat = dmatrix(2, 2);
//	tmpVec = new double[2];
//	difMean = new double[2];
//
//	invCov1 = InverseMatrix2D(covMat1);
//
//	//tr(cov1^-1*cov0)
//	tmpMat = MultipleMatrix2D(invCov1, covMat0);
//
//	tmp = tmpMat[0][0] + tmpMat[1][1];
//
//	//(u1-u0)^T * cov1^-1 * (u1-u0)
//	difMean[0] = mean1[0] - mean0[0];
//	difMean[1] = mean1[1] - mean0[1];
//
//	tmpVec[0] = (difMean[0] * invCov1[0][0]) + (difMean[1] * invCov1[1][0]);
//	tmpVec[1] = (difMean[0] * invCov1[0][1]) + (difMean[1] * invCov1[1][1]);
//
//	tmp += ((tmpVec[0] * difMean[0]) + (tmpVec[1] * difMean[1]));
//
//	//ln(det(cov1)/det(cov0))
//	detCov0 = DetermineMat2D(covMat0);
//	detCov1 = DetermineMat2D(covMat1);
//
//	tmp += log(detCov1 / detCov0);
//
//	tmp -= 2;
//	tmp /= 2;
//
//	free_dmatrix(invCov1, 2, 2);
//	free_dmatrix(tmpMat, 2, 2);
//	delete[]tmpVec;
//	delete[]difMean;
//
//	return tmp;
//}
//
//double CImageStitchMeanShift::DissimilarHomo(int nH, int nW, Mat& homo0, Mat& homo1, bool bOnDraw)
//{
//	int i, j, y, x;
//	double dTmp=0;
//	Point2f input[4];
//	Point2f output0[4], output1[4];
//
//	BYTE** drawSquare;
//
//	int offsetX, offsetY;
//	int nH_img, nW_img;
//	double minX=20, minY=20;
//	double maxX=0, maxY=0;
//
//	input[0].x = 0; input[0].y = 0;
//	input[1].x = nW; input[1].y = 0;
//	input[2].x = nW; input[2].y = nH;
//	input[3].x = 0; input[3].y = nH;
//
// 
//	for (i = 0; i < 4; i++)
//	{
//		GetPerspectivePoint(input[i], output0[i], homo0);
//		GetPerspectivePoint(input[i], output1[i], homo1);
//
//		dTmp += GetDistance(output0[i].x, output0[i].y, output1[i].x, output1[i].y);
//	}
//
//	if (bOnDraw)
//	{
//		for (i = 0; i < 4; i++)
//		{
//			input[i].y = input[i].y / 2;
//			input[i].x = input[i].x / 2;
//
//			output0[i].y = output0[i].y / 2;
//			output0[i].x = output0[i].x / 2;
//
//			output1[i].y = output1[i].y / 2;
//			output1[i].x = output1[i].x / 2;
//		}
//
//		for (i = 0; i < 4; i++)
//		{
//			if (minX > input[i].x) minX = input[i].x;
//			if (minY > input[i].y) minY = input[i].y;
//			if (maxX < input[i].x) maxX = input[i].x;
//			if (maxY < input[i].y) maxY = input[i].y;
//
//			if (minX > output0[i].x) minX = output0[i].x;
//			if (minY > output0[i].y) minY = output0[i].y;
//			if (maxX < output0[i].x) maxX = output0[i].x;
//			if (maxY < output0[i].y) maxY = output0[i].y;
//
//			if (minX > output1[i].x) minX = output1[i].x;
//			if (minY > output1[i].y) minY = output1[i].y;
//			if (maxX < output1[i].x) maxX = output1[i].x;
//			if (maxY < output1[i].y) maxY = output1[i].y;
//		}
//
//		if (minX < 0) offsetX = -1 * minX; else offsetX = 0;
//		if (minY < 0) offsetY = -1 * minY; else offsetY = 0;
//
//		nH_img = maxY - minY + 1;
//		nW_img = maxX - minX + 1;
//
//		drawSquare = cmatrix(nH_img, nW_img);
//
//		for (y = 0; y < nH_img; y++)
//			for (x = 0; x < nW_img; x++)
//			{
//				drawSquare[y][x] = 0;
//			}
//
//		DrawLine(drawSquare, nW_img, nH_img, input[0].x + offsetX, input[0].y + offsetY, input[1].x + offsetX, input[1].y + offsetY, 50);
//		DrawLine(drawSquare, nW_img, nH_img, input[1].x + offsetX, input[1].y + offsetY, input[2].x + offsetX, input[2].y + offsetY, 50);
//		DrawLine(drawSquare, nW_img, nH_img, input[2].x + offsetX, input[2].y + offsetY, input[3].x + offsetX, input[3].y + offsetY, 50);
//		DrawLine(drawSquare, nW_img, nH_img, input[3].x + offsetX, input[3].y + offsetY, input[0].x + offsetX, input[0].y + offsetY, 50);
//
//		DrawLine(drawSquare, nW_img, nH_img, output0[0].x + offsetX, output0[0].y + offsetY, output0[1].x + offsetX, output0[1].y + offsetY, 255);
//		DrawLine(drawSquare, nW_img, nH_img, output0[1].x + offsetX, output0[1].y + offsetY, output0[2].x + offsetX, output0[2].y + offsetY, 255);
//		DrawLine(drawSquare, nW_img, nH_img, output0[2].x + offsetX, output0[2].y + offsetY, output0[3].x + offsetX, output0[3].y + offsetY, 255);
//		DrawLine(drawSquare, nW_img, nH_img, output0[3].x + offsetX, output0[3].y + offsetY, output0[0].x + offsetX, output0[0].y + offsetY, 255);
//
//		DrawLine(drawSquare, nW_img, nH_img, output1[0].x + offsetX, output1[0].y + offsetY, output1[1].x + offsetX, output1[1].y + offsetY, 200);
//		DrawLine(drawSquare, nW_img, nH_img, output1[1].x + offsetX, output1[1].y + offsetY, output1[2].x + offsetX, output1[2].y + offsetY, 200);
//		DrawLine(drawSquare, nW_img, nH_img, output1[2].x + offsetX, output1[2].y + offsetY, output1[3].x + offsetX, output1[3].y + offsetY, 200);
//		DrawLine(drawSquare, nW_img, nH_img, output1[3].x + offsetX, output1[3].y + offsetY, output1[0].x + offsetX, output1[0].y + offsetY, 200);
//
//		DisplayCimage2D(drawSquare, nW_img, nH_img, 0, 0);
//
//		free_cmatrix(drawSquare, nH_img, nW_img);
//	}
//
//	return dTmp;
//}
//
///****************************************************
//<ActiveFeatureNum>
//
//각 영역의 FeatureNum를 통해 Object 후보군을 쓸지 말지 판단한다.
//
//*이전에 GetArrFeatureNum를 통해 arrFeatureNum가 준비되어 있어야 한다.
//*******************************************************/
//void CImageStitchMeanShift::ActiveFeatureNum(int minFeature)
//{
//	int i;
//
//	// Set Region's number for getting homography
//	for (i = 0; i < nNumSeg; i++)
//	{
//		if (arrObjIdx[i] != -1)
//		{
//			if (arrFeatureNum[i] < minFeature)
//			{
//				arrObjIdx[i] = -1;
//				arrErr[i] = 0;
//				arrInfo[i] = arrFeatureNum[i];
//			}
//		}
//	}
//}
//
///****************************************************
//<ActiveRegion>
//
//각 영역의 Region Area를 통해 Object 후보군을 쓸지 말지 판단한다.
//
//*이전에 GetArrRegion를 통해 arrRegion가 준비되어 있어야 한다.
//*******************************************************/
//void CImageStitchMeanShift::ActiveRegion(int minArea)
//{
//	int i;
//	
//	// Set Region's number for getting homography
//	for (i = 0; i < nNumSeg; i++)
//	{
//		if (arrObjIdx[i] != -1)
//		{
//			if (arrRegion[i] < minArea)
//			{
//				arrObjIdx[i] = -1;
//				arrErr[i] = 1;
//				arrInfo[i] = arrRegion[i];
//			}
//		}
//	}
//}
//
///****************************************************
//<ActiveFeatureRate>
//
//각 영역의 FeatureRate를 통해 Object 후보군을 쓸지 말지 판단한다.
//
//*이전에 GetArrFeatureNum를 통해 arrFeatureNum가 준비되어 있어야 한다.
//*******************************************************/
//bool CImageStitchMeanShift::ActiveFeatureRate(double minFeatureRate)
//{
//	int i, j, y, x;
//	int tmpX, tmpY;
//	int regionFeature=0;
//	double featureRate;
//	bool bOnFeatureRate = false;
//
//	// Set Region's number for getting homography
//	for (i = 0; i < nNumSeg; i++)
//	{
//		if (arrObjIdx[i] != -1)
//		{
//			featureRate = double(arrFeatureNum[i]) / double(nNumPoint);
//
//			if (featureRate > minFeatureRate)
//			{
//				arrObjIdx[i] = -1;
//				arrErr[i] = 2;
//				arrInfo[i] = featureRate;
//				bOnFeatureRate = true;
//			}
//		}
//	}
//
//	return bOnFeatureRate;
//}
//
///****************************************************
//<ActiveMisMatch>
//
//각 영역의 호모그래피를 통해 Object 후보군을 쓸지 말지 판단한다.
//
//*이전에 GetArrHomo를 통해 arrHomo가 준비되어 있어야 한다.
//*******************************************************/
//void CImageStitchMeanShift::ActiveMisMatch()
//{
//	int i, j, y, x;
//	int tmpX, tmpY;
//	double sumVec=0;
//	bool bMisMatch = false;
//
//	Point2f inputPt[4], outputPt[4];
//	Point2f outputVecR[4], outputVecL[4];
//
//	inputPt[0].y = 0; inputPt[0].x = 0;
//	inputPt[1].y = 0; inputPt[1].x = imgR.nW;
//	inputPt[2].y = imgR.nH; inputPt[2].x = 0;
//	inputPt[3].y = imgR.nH; inputPt[3].x = imgR.nW;
//
//	// Set Region's number for getting homography
//	for (i = 0; i < nNumSeg; i++)
//	{
//		if (arrObjIdx[i] != -1)
//		{
//			for (j = 0; j < 4; j++)
//				GetPerspectivePoint(inputPt[j], outputPt[j], arrHomo[i]);
//
//			// 각도 angle 구하기
//			outputVecR[0].y = outputPt[1].y - outputPt[0].y;
//			outputVecR[0].x = outputPt[1].x - outputPt[0].x;
//
//			outputVecR[1].y = outputPt[3].y - outputPt[1].y;
//			outputVecR[1].x = outputPt[3].x - outputPt[1].x;
//
//			outputVecR[2].y = outputPt[2].y - outputPt[3].y;
//			outputVecR[2].x = outputPt[2].x - outputPt[3].x;
//
//			outputVecR[3].y = outputPt[0].y - outputPt[2].y;
//			outputVecR[3].x = outputPt[0].x - outputPt[2].x;
//
//			outputVecL[0].y = outputPt[0].y - outputPt[1].y;
//			outputVecL[0].x = outputPt[0].x - outputPt[1].x;
//
//			outputVecL[1].y = outputPt[1].y - outputPt[3].y;
//			outputVecL[1].x = outputPt[1].x - outputPt[3].x;
//
//			outputVecL[2].y = outputPt[3].y - outputPt[2].y;
//			outputVecL[2].x = outputPt[3].x - outputPt[2].x;
//
//			outputVecL[3].y = outputPt[2].y - outputPt[0].y;
//			outputVecL[3].x = outputPt[2].x - outputPt[0].x;
//
//			sumVec += GetAngleVecter(outputVecR[0], outputVecL[3]);
//			sumVec += GetAngleVecter(outputVecR[1], outputVecL[0]);
//			sumVec += GetAngleVecter(outputVecR[2], outputVecL[1]);
//			sumVec += GetAngleVecter(outputVecR[3], outputVecL[2]);
//
//			// 사각형인지 판별
//			if (sumVec < 350)
//				bMisMatch = true;
//		
//			// 변형된 영상의 가로, 세로 길이 얼마나 큰지 판별
//			int minX = imgR.nW, maxX=0, minY= imgR.nH, maxY=0;
//			int nHT, nWT;
//
//			for (j = 0; j < 4; j++)
//			{
//				if (minX > outputPt[j].x) minX = outputPt[j].x;
//				if (maxX < outputPt[j].x) maxX = outputPt[j].x;
//				if (minY > outputPt[j].y) minY = outputPt[j].y;
//				if (maxY < outputPt[j].y) maxY = outputPt[j].y;
//			}
//
//			nHT = maxY - minY;
//			nWT = maxX - minX;
//
//			if (nHT > 10000 || nWT > 10000 || nHT == 0 || nWT == 0)
//			{
//				bMisMatch = true;
//			}
//
//			if (bMisMatch)
//			{
//				arrObjIdx[i] = -1;
//				arrErr[i] = 3;
//				arrInfo[i] = NULL;
//				bMisMatch = false;
//			}
//			sumVec = 0;
//		}
//	}
//}
//
//void CImageStitchMeanShift::ActiveKLdivergence(double minKLdivergence)
//{
//	int y, x, i, j;
//	double** covMatR, **covMatF;
//	double* meanR, *meanF;
//	double similarity;
//	vector <Point2f> regionPoint;
//	vector <Point2f> objPointR, objPointL;
//
//	covMatR = dmatrix(2, 2);
//	covMatF = dmatrix(2, 2);
//	meanR = new double[2];
//	meanF = new double[2];
//
//	// Set Region's number for getting homography
//	for (i = 0; i < nNumSeg; i++)
//	{
//		if (arrObjIdx[i] != -1)
//		{
//			// Get Region Mean, covMat
//			for (y = 0; y < imgR.nH; y++)
//				for (x = 0; x < imgR.nW; x++)
//				{
//					if (LabelMeanShift[y][x] == arrObjIdx[i])
//					{
//						regionPoint.push_back(Point2f(x, y));
//					}
//				}
//
//			GetCovAndMean2D(covMatR, meanR, regionPoint);
//
//			// Get Feature in Region Mean, covMat
//			for (j = 0; j < nNumPoint; j++)
//			{
//				if (LabelMeanShift[int(GodPointR[j].y)][int(GodPointR[j].x)] == arrObjIdx[i])
//				{
//					objPointR.push_back(GodPointR[j]);
//					objPointL.push_back(GodPointL[j]);
//				}
//			}
//
//			GetCovAndMean2D(covMatF, meanF, objPointR);
//
//			// KL-divergence
//			similarity = KLdivergenceForBivariateGaussiian(meanF, meanR, covMatF, covMatR);
//			similarity += KLdivergenceForBivariateGaussiian(meanR, meanF, covMatR, covMatF);
//
//			similarity /= 2.;
//
//			if (similarity > minKLdivergence)
//			{
//				arrObjIdx[i] = -1;
//				arrErr[i] = 4;
//				arrInfo[i] = similarity;
//			}
//		}
//
//		regionPoint.clear();
//		objPointR.clear();
//		objPointL.clear();
//		
//	}
//
//	free_dmatrix(covMatR, 2, 2);
//	free_dmatrix(covMatF, 2, 2);
//
//	delete[] meanR;
//	delete[] meanF;
//}
//
//void CImageStitchMeanShift::DrawActiveErr()
//{
//	int i, j, x, y;
//	CString strNum, strErr, strErrInfo;
//	BYTE** imgErr;
//
//	imgErr = cmatrix(15*nNumSeg, 250);
//
//	for (y = 0; y < 15 * nNumSeg; y++)
//		for (x = 0; x < 250; x++)
//		{
//			imgErr[y][x] = 0;
//		}
//
//	for (i = 0; i < nNumSeg; i++)
//	{
//		strNum.Format(_T("%d"), i);
//		DrawTextOnImag2DGray(imgErr, 250, 15 * nNumSeg, 0, i * 15, strNum, DT_LEFT, 255, false);
//
//		switch (arrErr[i])
//		{
//		case 0:
//			strErr="feature point";
//			strErrInfo.Format(_T("%d"), int(arrInfo[i]));
//			break;
//
//		case 1:
//			strErr = "region area";
//			strErrInfo.Format(_T("%d"), int(arrInfo[i]));
//			break;
//
//		case 2:
//			strErr = "feature rate";
//			strErrInfo.Format(_T("%f"), arrInfo[i]);
//			break;
//
//		case 3:
//			strErr = "misMatch";
//			strErrInfo = " ";
//			break;
//
//		case 4:
//			strErr = "kl-divergence";
//			strErrInfo.Format(_T("%f"), arrInfo[i]);
//			break;
//
//		default:
//			strErr = "obj candidate";
//			if (arrInfo[i] == -1)
//				strErrInfo = " ";
//			else
//				strErrInfo.Format(_T("%f"), arrInfo[i]);
//		}
//		
//		DrawTextOnImag2DGray(imgErr, 250, 15 * nNumSeg, 40, i * 15, strErr, DT_LEFT, 255, false);
//		DrawTextOnImag2DGray(imgErr, 250, 15 * nNumSeg, 160, i * 15, strErrInfo, DT_LEFT, 255, false);
//	}
//
//	DisplayCimage2D(imgErr, 250, 15 * nNumSeg, 0, 0);
//
//	delete[] imgErr;
//}
//
////===============================================================================================================================================================================================================================================
////===============================================================================================================================================================================================================================================
//
//void CImageStitchMeanShift::FindMultiObject()
//{
//	int i, j;
//	double inlierRate;
//	int* index;
//	double thresh = 0.7;
//	int tmp;
//	bool bDone = true;
//
//	// 모든 object 후보군을 제외한 특징점의 inlier 비율 구하기
//	inlierRate = GetExceptInlierRate(arrObjIdx, nNumObjCandi, 0, homoBack);
//
//	if (inlierRate > thresh)
//	{
//		while (nNumObjCandi > 1)
//		{
//			index = new int[nNumObjCandi - 1];
//
//			for (i = 0; i < nNumObjCandi; i++)
//			{
//				// 새로운 object 후보군 생성
//				for (j = 0; j < nNumObjCandi - 1; j++)
//				{
//					tmp = i + j;
//					if (tmp >= nNumObjCandi) tmp -= nNumObjCandi;
//					index[j] = arrObjIdx[tmp];
//				}
//
//				inlierRate = GetExceptInlierRate(index, nNumObjCandi - 1, 1, homoBack);
//
//				if (inlierRate > thresh)
//				{
//					bDone = false;
//					break;
//				}
//			}
//
//			// while문을 멈출지 판별
//			if (bDone)
//			{
//				break;
//			}
//			else
//			{
//				// object 후보군 업데이트
//				nNumObjCandi--;
//
//				delete[] arrObjIdx;
//				arrObjIdx = new int[nNumObjCandi];
//				for (i = 0; i < nNumObjCandi; i++)
//					arrObjIdx[i] = index[i];
//
//				delete[] index;
//			}
//		}
//	}
//	else
//	{
//		bExeDualHomo = true;	// 싱글 호모그래피를 사용한다.
//	}
//}
//
//void CImageStitchMeanShift::FindObject()
//{
//	int i, j;
//	int nTmp;
//	vector<Point2f> PointTransR;
//	Point2f tmpPoint;
//	
//	double* arrDist;
//
//	arrDist = new double[nNumObjCandi];
//
//	homoGlo = findHomography(GodPointR, GodPointL, CV_RANSAC);
//
//	// PointTransR
//	for (i = 0; i < nNumPoint; i++)
//	{
//		GetPerspectivePoint(GodPointR[i], tmpPoint, homoGlo);
//		PointTransR.push_back(tmpPoint);
//	}
//
//	// distance by regions
//	for (i = 0; i < nNumObjCandi; i++)
//	{
//		arrDist[i] = 0;
//		nTmp = 0;
//
//		for (j = 0; j < nNumPoint; j++)
//		{
//			if (LabelMeanShift[int(GodPointR[j].y)][int(GodPointR[j].x)] == arrObjIdx[i])
//			{
//				arrDist[i] += GetDistance(int(GodPointL[j].x), int(GodPointL[j].y), int(PointTransR[j].x), int(PointTransR[j].y));
//				nTmp++;
//			}
//		}
//
//		arrDist[i] /= nTmp;
//	}
//
//	delete[] arrDist;
//}
//
//void CImageStitchMeanShift::Run()
//{
//	int x, y;
//
//	GetMatchingPoint();
//	SegmentMeanshift(2000);
//
//	DetermineActive(10, 6000, 0.5, 20);
//
//	if (bExeDualHomo)
//	{
//		FindSingleObject();
//		MakeWeightMap(10);
//	}
//
//	WarpHomography(true);
//}
////===============================================================================================================================================================================================================================================
////===============================================================================================================================================================================================================================================
//
//CImageStitchMultiHomo::CImageStitchMultiHomo()
//{
//	homoAll = new Mat*[imgR.nH];
//
//	for (int i = 0; i < imgR.nH; i++)
//		homoAll[i] = new Mat[imgR.nW];
//
//	arrClusterR = new vector<Point2f>[2];
//	arrClusterL = new vector<Point2f>[2];
//	arrInitKmean = new Point2f[2];
//	arrCenter = new Point2f[2];
//	labelCluster = NULL;
//	arrMesh = NULL;
//}
//
//CImageStitchMultiHomo::~CImageStitchMultiHomo()
//{
//	if (arrMesh!=NULL)
//		delete[] arrMesh;
//
//	if (labelCluster != NULL)
//		delete[] labelCluster;
//
//	for (int i = 0; i < imgR.nH; i++)
//		delete[] homoAll[i];
//	delete[] homoAll;
//
//	delete[] arrClusterR;
//	delete[] arrClusterL;
//	delete[] arrInitKmean;
//	delete[] arrCenter;
//}
//
//void CImageStitchMultiHomo::KMeansCluster(bool bForgy)
//{
//	int i, j;
//	double dTempDis;
//	int* arrNumData;
//	int nTempX, nTempY;
//	int nLabel;
//	bool bChange = true;
//
//	arrNumData = new int[2];
//
//	//Kmean초기값 설정.
//	if (bForgy == true)
//	{
//		RNG rng;
//		int nRanSed;
//
//		for (int i = 0; i < 2; i++)
//		{
//			nRanSed = rng.uniform(0, nNumPoint - 1);
//			arrInitKmean[i] = GodPointR[nRanSed];
//		}
//	}
//
//	if (bForgy == false)
//	{
//		arrInitKmean[0].y = 0;
//		arrInitKmean[0].x = double(imgR.nW) / double(2);
//		arrInitKmean[1].y = imgR.nH;
//		arrInitKmean[1].x = double(imgR.nW) / double(2);
//	}
//
//	//k-means clustering
//	for (i = 0; i < 2; i++)
//	{
//		arrCenter[i].y = arrInitKmean[i].y;
//		arrCenter[i].x = arrInitKmean[i].x;
//	}
//
//	while (bChange)
//	{
//		bChange = false;
//
//		// 중심과 cluster의 거리
//		for (int i = 0; i < nNumPoint; i++)
//		{
//			nTempX = GodPointR[i].x;
//			nTempY = GodPointR[i].y;
//
//
//			for (int j = 0; j < 2; j++)
//			{
//				if (j == 0)
//				{
//					//dTempDis = GetDistance(nTempX, nTempY, arrCenter[j].x, arrCenter[j].y);
//					dTempDis = abs(nTempY - arrCenter[j].y);
//					nLabel = j;
//				}
//				else
//				{
//					if (dTempDis > abs(nTempY - arrCenter[j].y))
//					{
//						nLabel = j;
//						dTempDis = abs(nTempY - arrCenter[j].y);
//					}
//				}
//			}
//
//			//변화가 있는지 확인한다.
//			if (labelCluster[i] != nLabel)
//			{
//				bChange = true;
//			}
//
//			labelCluster[i] = nLabel;
//		}
//
//		// 다시 중심값을 구한다
//		// Array 초기화
//		for (int i = 0; i < 2; i++)
//		{
//			arrCenter[i].y = 0;
//			arrCenter[i].x = 0;
//			arrNumData[i] = 0;
//		}
//
//		for (int i = 0; i < nNumPoint; i++)
//		{
//			arrCenter[labelCluster[i]].y += GodPointR[i].y;
//			arrCenter[labelCluster[i]].x += GodPointR[i].x;
//			arrNumData[labelCluster[i]] ++;
//		}
//
//		for (int i = 0; i < 2; i++)
//		{
//			arrCenter[i].y /= arrNumData[i];
//			arrCenter[i].x /= arrNumData[i];
//		}
//	}
//
//	//Make arrClusterR
//	for (int i = 0; i < nNumPoint; i++)
//	{
//		arrClusterR[labelCluster[i]].push_back(GodPointR[i]);
//		arrClusterL[labelCluster[i]].push_back(GodPointL[i]);
//	}
//
//	delete[] arrNumData;
//}
//
//void CImageStitchMultiHomo::MakeMesh()
//{
//	int i, j;
//
//	// value for meshed warp
//	int nNumVertH, nNumVertW;
//	int nRestVertH, nRestVertW;
//
//	// mesh warp
//
//	nRestVertH = (imgR.nH - 1) % 10;
//	nRestVertW = (imgR.nW - 1) % 10;
//
//	if (nRestVertH == 0)
//		nNumVertH = (imgR.nH - 1) / 10;
//	else
//		nNumVertH = (imgR.nH - 1) / 10 + 1;
//
//	if (nRestVertW == 0)
//		nNumVertW = (imgR.nW - 1) / 10;
//	else
//		nNumVertW = (imgR.nW - 1) / 10 + 1;
//
//	nNumVert = nNumVertH * nNumVertW;
//	arrMesh = new CMesh[nNumVert];
//
//	// Set vertice
//
//	for (i = 0; i < nNumVertH - 1; i++)
//		for (j = 0; j < nNumVertW - 1; j++)
//		{
//			arrMesh[i * nNumVertW + j] = CMesh(10 * i, 10 * j, 10);
//		}
//
//	for (i = 0; i < nNumVertH - 1; i++)
//	{
//		if (nRestVertW == 0)
//		{
//			nRestVertW = 10;
//		}
//
//		arrMesh[i * nNumVertW + nNumVertW - 1] = CMesh(10 * i, 10 * (nNumVertW - 1), 10, nRestVertW);
//	}
//
//	for (i = 0; i < nNumVertW - 1; i++)
//	{
//		if (nRestVertH == 0)
//		{
//			nRestVertH = 10;
//		}
//
//		arrMesh[(nNumVertH - 1)* nNumVertW + i] = CMesh(10 * (nNumVertH - 1), 10 * i, nRestVertH, 10);
//	}
//
//	arrMesh[(nNumVertH - 1)* nNumVertW + nNumVertW - 1] = CMesh(10 * (nNumVertH - 1), 10 * (nNumVertW - 1), nRestVertH, nRestVertW);
//}
//
//
//void CImageStitchMultiHomo::WarpMeshHomography()
//{
//	int i, j, x, y;
//
//	// value for get Image size
//	int offsetX = 0, offsetY = 0;
//	int maxX_totalR = 0, maxY_totalR = 0, minX_totalR = imgR.nW, minY_totalR = imgR.nH;
//	double maxX_transR = 0, maxY_transR = 0, minX_transR = imgR.nW, minY_transR = imgR.nH;
//	int nH_total, nW_total;
//	Point2f inputPoint, transPoint;
//
//	// get image's size
//	for (y = 0; y < imgR.nH; y++)
//		for (x = 0; x < imgR.nW; x++)
//		{
//			inputPoint = Point2f(x, y);
//			GetPerspectivePoint(inputPoint, transPoint, homoAll[y][x]);
//
//			if (transPoint.y > maxY_transR) maxY_transR = transPoint.y;
//			if (transPoint.x > maxX_transR) maxX_transR = transPoint.x;
//			if (transPoint.y < minY_transR) minY_transR = transPoint.y;
//			if (transPoint.x < minX_transR) minX_transR = transPoint.x;
//		}
//
//	//Set transformed Right Image
//	if (minY_transR < 0) minY_totalR = minY_transR; else minY_totalR = 0;
//	if (maxY_transR > imgR.nH - 1) maxY_totalR = maxY_transR; else maxY_totalR = imgR.nH - 1;
//
//	if (minX_transR < 0) minX_totalR = minX_transR; else minX_totalR = 0;
//	if (maxX_transR > imgR.nW - 1) maxX_totalR = maxX_transR; else maxX_totalR = imgR.nW - 1;
//
//	nH_total = maxY_totalR - minY_totalR + 1;
//	nW_total = maxX_totalR - minX_totalR + 1;
//
//	if (minY_transR < 0) offsetY = abs(minY_transR);
//	if (minX_transR < 0) offsetX = abs(minX_transR);
//
//	//Set total Image's nH, nW
//
//	imgTotal.imgRed = cmatrix(nH_total, nW_total); imgTotal.imgGreen = cmatrix(nH_total, nW_total); imgTotal.imgBlue = cmatrix(nH_total, nW_total);
//	imgLtrans.imgRed = cmatrix(nH_total, nW_total); imgLtrans.imgGreen = cmatrix(nH_total, nW_total); imgLtrans.imgBlue = cmatrix(nH_total, nW_total);
//	imgRtrans.imgRed = cmatrix(nH_total, nW_total); imgRtrans.imgGreen = cmatrix(nH_total, nW_total); imgRtrans.imgBlue = cmatrix(nH_total, nW_total);
//	weightRtrans = dmatrix(nH_total, nW_total); weightLtrans = dmatrix(nH_total, nW_total);
//	
//	imgTotal.nH = nH_total;	imgTotal.nW = nW_total;
//	imgLtrans.nH = nH_total; imgLtrans.nW = nW_total;
//	imgRtrans.nH = nH_total; imgRtrans.nW = nW_total;
//
//
//	//WarpSeamWeight
//
//	for (int y = 0; y < nH_total; y++)
//		for (int x = 0; x < nW_total; x++)
//		{
//			imgLtrans.imgRed[y][x] = 0; imgLtrans.imgGreen[y][x] = 0; imgLtrans.imgBlue[y][x] = 0;
//			imgRtrans.imgRed[y][x] = 0; imgRtrans.imgGreen[y][x] = 0; imgRtrans.imgBlue[y][x] = 0;
//			weightRtrans[y][x] = 0; weightLtrans[y][x] = 0;
//		}
//
//	for (int y = 0; y < imgL.nH; y++)
//		for (int x = 0; x < imgL.nW; x++)
//		{
//			weightLtrans[y + offsetY][x + offsetX] = weightSeamL[y][x];
//			imgLtrans.imgRed[y + offsetY][x + offsetX] = imgL.imgRed[y][x];
//			imgLtrans.imgGreen[y + offsetY][x + offsetX] = imgL.imgGreen[y][x];
//			imgLtrans.imgBlue[y + offsetY][x + offsetX] = imgL.imgBlue[y][x];
//		}
//
//	//----------------------------------------------------------------------------------------
//	// Set deformed vertice
//	int nTmpX, nTmpY;
//
//	for (i = 0; i < nNumVert; i++)
//	{
//		arrMesh[i].DeformMesh(false, homoAll);
//	}
//
//	//inverse mesh warp
//	for (i = 0; i < nNumVert; i++)
//	{
//		// inverse warping
//		for (y = arrMesh[i].nMinY; y <= arrMesh[i].nMaxY; y++)
//			for (x = arrMesh[i].nMinX; x <= arrMesh[i].nMaxX; x++) {
//
//				inputPoint = Point2f(x, y);
//
//				Mat inverH = arrMesh[i].Homo.inv();
//				GetPerspectivePoint(inputPoint, transPoint, inverH);
//
//				if (transPoint.x >= arrMesh[i].vertex[0].x
//					&& transPoint.x <= arrMesh[i].vertex[3].x
//					&& transPoint.y >= arrMesh[i].vertex[0].y
//					&& transPoint.y <= arrMesh[i].vertex[3].y)
//				{
//					imgRtrans.imgRed[y + offsetY][x + offsetX] = BilinearInterpolation(transPoint, imgR.imgRed, imgR.nH, imgR.nW);;
//					imgRtrans.imgGreen[y + offsetY][x + offsetX] = BilinearInterpolation(transPoint, imgR.imgGreen, imgR.nH, imgR.nW);
//					imgRtrans.imgBlue[y + offsetY][x + offsetX] = BilinearInterpolation(transPoint, imgR.imgBlue, imgR.nH, imgR.nW);;
//					weightRtrans[y + offsetY][x + offsetX] = weightSeamR[int(transPoint.y)][int(transPoint.x)];
//				}
//			}
//	}
//
//	for (y = 0; y < nH_total; y++)
//		for (x = 0; x < nW_total; x++)
//		{
//			if (weightRtrans[y][x] > weightLtrans[y][x])
//			{
//				weightRtrans[y][x] = 1;
//				weightLtrans[y][x] = 0;
//			}
//
//			if (weightLtrans[y][x] > weightRtrans[y][x])
//			{
//				weightLtrans[y][x] = 1;
//				weightRtrans[y][x] = 0;
//			}
//		}
//
//	this->MultiBandBlending(nH_total, nW_total, imgTotal.imgRed, imgTotal.imgGreen, imgTotal.imgBlue);
//
//	DisplayCimage2DColor(imgTotal.imgRed, imgTotal.imgGreen, imgTotal.imgBlue, nW_total, nH_total, 0, 0);
//}
//
////===============================================================================================================================================================================================================================================
////===============================================================================================================================================================================================================================================
//CImageStitchLayer::CImageStitchLayer()
//{
//}
//
//CImageStitchLayer::~CImageStitchLayer()
//{
//	delete[] nNumSetPoint;
//} 
//
//void CImageStitchLayer::FindLayer(int nThresh)
//{
//	int i, j, x, y;
//	vector < Point2f > tmpVecL, tmpVecR;
//	vector <Point2f> inlierL, inlierR;
//	vector <Point2f> outlierL, outlierR;
//	vector <Point2f> emptyVec;
//	Mat H;
//	Point2f input, output, nullPoint(0, 0);
//	double dTmp;
//	int nSize;
//
//	for (i = 0; i < nNumPoint; i++) {
//		tmpVecR.push_back(GodPointR[i]);
//		tmpVecL.push_back(GodPointL[i]);
//	}
//
//	nSize = nNumPoint;
//
//	while (nSize > nThresh)
//	{
//		H = findHomography(tmpVecR, tmpVecL, CV_RANSAC);
//
//		// inlier, outlier 판단.
//		for (i = 0; i < nSize; i++)
//		{
//			input = tmpVecR[i];
//			GetPerspectivePoint(input, output, H);
//			dTmp = GetDistance(tmpVecL[i].x, tmpVecL[i].y, output.x, output.y);
//
//			if (dTmp < 3)
//			{
//				inlierL.push_back(tmpVecL[i]);
//				inlierR.push_back(tmpVecR[i]);
//			}
//			else
//			{
//				outlierL.push_back(tmpVecL[i]);
//				outlierR.push_back(tmpVecR[i]);
//			}
//		}
//
//		// Layer의 inlier set과 homography 배열에 저장
//		if (inlierL.size() > nThresh)
//		{
//			SetPointLayerL.push_back(inlierL);
//			SetPointLayerR.push_back(inlierR);
//			setHomoLayer.push_back(H);
//		}
//
//		tmpVecR.clear();
//		tmpVecL.clear();
//
//		// 나머지 outlier에서 새로운 호모그래피를 구하기 위해
//		// tmpVec과 nSize 다시 재정의
//		nSize = outlierL.size();
//
//		for (i = 0; i < nSize; i++){
//			tmpVecL.push_back(outlierL[i]);
//			tmpVecR.push_back(outlierR[i]);
//		}
//
//		inlierR.clear();
//		inlierL.clear();
//		outlierR.clear();
//		outlierL.clear();
//	}
//
//	this->DrawLayer();
//}
//
//void CImageStitchLayer::BlendingHomo(double sigma)
//{
//	int i, j, y, x;
//	//Blending homography.
//	
//	double* dNearDis;
//	double* dWeight;
//	double tmp, alpa, weightSum = 0;
//	Point2f tmpPoint, tmpCen;
//	Mat tmpH;
//
//	nNumLayer = SetPointLayerR.size();
//
//	dNearDis = new double[nNumLayer];
//	dWeight = new double[nNumLayer];
//	nNumSetPoint = new int[nNumLayer];
//
//	for (i = 0; i < nNumLayer; i++)
//	{
//		nNumSetPoint[i] = SetPointLayerR[i].size();
//		dNearDis[i] = 999;
//		dWeight[i] = 0;
//	}
//
//
//	for (i = 0; i < nNumVert; i++)
//	{
//		tmpH = Mat(cvSize(3, 3), CV_64FC1, cvScalar(0));
//		tmpCen = arrMesh[i].central;
//
//		//각 Layer별 최소거리 구하기.
//		for (j = 0; j < nNumLayer; j++)
//		{
//			for (int k = 0; k < nNumSetPoint[j]; k++)
//			{
//				tmpPoint.x = SetPointLayerR[j].at(k).x;
//				tmpPoint.y = SetPointLayerR[j].at(k).y;
//
//				tmp = GetDistance(tmpCen.x, tmpCen.y, tmpPoint.x, tmpPoint.y);
//
//				if (tmp < dNearDis[j])
//				{
//					dNearDis[j] = tmp;
//				}
//			}
//
//			//Get Weight
//			tmp = -1 * dNearDis[j] * dNearDis[j] / (sigma * sigma);
//			dWeight[j] = exp(tmp);
//			weightSum += dWeight[j];
//		}
//
//		//Blending 
//		for (j = 0; j < nNumLayer; j++)
//		{
//			dWeight[j] /= weightSum;
//			for (int a = 0; a < 3; a++)
//				for (int b = 0; b < 3; b++)
//				{
//					tmpH.at<double>(a, b) += (dWeight[j] * setHomoLayer[j].at<double>(a, b));
//				}
//		}
//
//		for (y = arrMesh[i].vertex[0].y; y <= arrMesh[i].vertex[3].y; y++)
//			for (x = arrMesh[i].vertex[0].x; x <= arrMesh[i].vertex[3].x; x++)
//			{
//				homoAll[y][x] = tmpH;
//			}
//
//		for (j = 0; j < nNumLayer; j++)
//		{
//			dNearDis[j] = 999;
//		}
//
//		weightSum = 0;
//	}
//
//	delete[] dNearDis;
//	delete[] dWeight;
//}
//
//void CImageStitchLayer::DrawLayer()
//{
//	int x, y, i, j;
//
//	//-----------------------------------------
//	//-------------<Layer 확인>---------------
//	int ** arrColor;
//	int nNumPoint;
//	int nSize;
//
//	nSize = SetPointLayerL.size();
//	arrColor = imatrix(nSize, 3);
//
//	GetRandColorArray(arrColor, nSize);
//
//	for (i = 0; i < nSize; i++)
//	{
//		nNumPoint = SetPointLayerR[i].size();
//
//		for (j = 0; j < nNumPoint; j++)
//		{
//			y = SetPointLayerR[i].at(j).y;
//			x = SetPointLayerR[i].at(j).x;
//			DrawCross(imgMark.imgRed, imgR.nW, imgR.nH, CPoint(x, y), arrColor[i][0]);
//			DrawCross(imgMark.imgGreen, imgR.nW, imgR.nH, CPoint(x, y), arrColor[i][1]);
//			DrawCross(imgMark.imgBlue, imgR.nW, imgR.nH, CPoint(x, y), arrColor[i][2]);
//		}
//	}
//
//	DisplayCimage2DColor(imgMark.imgRed, imgMark.imgGreen, imgMark.imgBlue, imgR.nW, imgR.nH, 0, 0);
//
//	free_imatrix(arrColor, nSize, 3);
//}
//
//CImageStitchKMeans::CImageStitchKMeans()
//{
//	arrHomo = new Mat[2];
//	weightMap = dmatrix(imgR.nH, imgR.nW);
//}
//
//CImageStitchKMeans::~CImageStitchKMeans()
//{
//	delete[] arrHomo;
//	free_dmatrix(weightMap, imgR.nH, imgR.nW);
//}
//
//void CImageStitchKMeans::RemoveOutlier(vector<Point2f>& pointR, vector<Point2f>& pointL, double thresh)
//{
//	int i;
//	int nTmp=0, nNum;
//	vector<Point2f> tmpPointR, tmpPointL;
//	double distance;
//	Point2f inputPoint, outputPoint;
//	Mat homo;
//
//	nNum = pointR.size();
//
//	homo = findHomography(pointR, pointL, CV_RANSAC);
//
//	for (i = 0; i < nNum; i++)
//	{
//		inputPoint.x = pointR[i].x;
//		inputPoint.y = pointR[i].y;
//
//		GetPerspectivePoint(inputPoint, outputPoint, homo);
//
//		distance = GetDistance(pointL[i].x, pointL[i].y, outputPoint.x, outputPoint.y);
//
//		if (distance < thresh)
//		{
//			tmpPointR.push_back(pointR[i]);
//			tmpPointL.push_back(pointL[i]);
//			nTmp++;
//		}
//	}
//
//	pointR.clear();
//	pointL.clear();
//
//	for (i = 0; i < nTmp; i++)
//	{
//		pointR.push_back(tmpPointR[i]);
//		pointL.push_back(tmpPointL[i]);
//	}
//}
//
//double CImageStitchKMeans::GetInlierRate(vector<Point2f>& pointR, vector<Point2f>& pointL, Mat& homo, double thresh)
//{
//	int i;
//	int nNum = 0;
//	int nNumInlier = 0;
//	double distance;
//	double dInlierRate = 0;
//	Point2f inputPoint, outputPoint;
//
//	nNum = pointR.size();
//
//	homo = findHomography(pointR, pointL, CV_RANSAC);
//
//	for (i = 0; i < nNum; i++)
//	{
//		inputPoint.x = pointR[i].x;
//		inputPoint.y = pointR[i].y;
//
//		GetPerspectivePoint(inputPoint, outputPoint, homo);
//
//		distance = GetDistance(pointL[i].x, pointL[i].y, outputPoint.x, outputPoint.y);
//
//		if (distance < thresh)
//		{
//			nNumInlier++;
//		}
//	}
//
//	dInlierRate = double(nNumInlier) / double(nNum);
//
//	return dInlierRate;
//}
//
//void CImageStitchKMeans::DrawFeature()
//{
//	int x, y, i, j;
//
//	//-----------------------------------------
//	//-------------<Layer 확인>---------------
//	int ** arrColor;
//	int nNum;
//
//	arrColor = imatrix(2, 3);
//
//	GetRandColorArray(arrColor, 2);
//
//	for (i = 0; i < 2; i++)
//	{
//		nNum = arrClusterR[i].size();
//
//		for (j = 0; j < nNum; j++)
//		{
//			y = arrClusterR[i][j].y;
//			x = arrClusterR[i][j].x;
//			DrawCross(imgMark.imgRed, imgR.nW, imgR.nH, CPoint(x, y), arrColor[i][0]);
//			DrawCross(imgMark.imgGreen, imgR.nW, imgR.nH, CPoint(x, y), arrColor[i][1]);
//			DrawCross(imgMark.imgBlue, imgR.nW, imgR.nH, CPoint(x, y), arrColor[i][2]);
//		}
//	}
//
//	DisplayCimage2DColor(imgMark.imgRed, imgMark.imgGreen, imgMark.imgBlue, imgR.nW, imgR.nH, 0, 0);
//
//	free_imatrix(arrColor, 2, 3);
//}
//
//void CImageStitchKMeans::RemoveOutlier1st(double dThresh)
//{
//	RemoveOutlier(GodPointR, GodPointL, dThresh);
//}
//
//void CImageStitchKMeans::KMeansCluster()
//{
//	int i, j;
//	double dTempDis, dTmp;
//	int* arrNumData;
//	double dTempX, dTempY;
//	int nLabel;
//	int nNum;
//	bool bChange = true;
//
//	nNum = GodPointR.size();
//
//	// 메모리 할당
//	arrNumData = new int[2];
//	labelCluster = new int[nNum];
//
//	//Kmean초기값 설정.
//	RNG rng;
//	int nRanSed;
//
//	for (int i = 0; i < 2; i++)
//	{
//		nRanSed = rng.uniform(0, nNum - 1);
//		arrInitKmean[i] = GodPointR[nRanSed];
//	}
//
//	//k-means clustering
//	for (i = 0; i < 2; i++)
//	{
//		arrCenter[i] = arrInitKmean[i];
//	}
//
//	while (bChange)
//	{
//		bChange = false;
//
//		// 중심과 cluster의 거리
//		for (i = 0; i < nNum; i++)
//		{
//			dTempX = GodPointR[i].x;
//			dTempY = GodPointR[i].y;
//
//			for (j = 0; j < 2; j++)
//			{
//				dTmp = GetDistance(dTempX, dTempY, arrCenter[j].x, arrCenter[j].y);
//
//				if (j == 0)
//				{
//					dTempDis = dTmp;
//					nLabel = j;
//				}
//				else
//				{
//					if (dTempDis > dTmp)
//					{
//						nLabel = j;
//						dTempDis = dTmp;
//					}
//				}
//			}
//
//			//변화가 있는지 확인한다.
//			if (labelCluster[i] != nLabel)
//			{
//				bChange = true;
//			}
//
//			labelCluster[i] = nLabel;
//		}
//
//		// 다시 중심값을 구한다
//		// Array 초기화
//		for (i = 0; i < 2; i++)
//		{
//			arrCenter[i].y = 0;
//			arrCenter[i].x = 0;
//			arrNumData[i] = 0;
//		}
//
//		for (i = 0; i < nNum; i++)
//		{
//			arrCenter[labelCluster[i]].y += GodPointR[i].y;
//			arrCenter[labelCluster[i]].x += GodPointR[i].x;
//			arrNumData[labelCluster[i]] ++;
//		}
//
//		for (i = 0; i < 2; i++)
//		{
//			arrCenter[i].y /= arrNumData[i];
//			arrCenter[i].x /= arrNumData[i];
//		}
//	}
//
//	//Make arrClusterR
//	for (i = 0; i < nNum; i++)
//	{
//		arrClusterR[labelCluster[i]].push_back(GodPointR[i]);
//		arrClusterL[labelCluster[i]].push_back(GodPointL[i]);
//	}
//
//	delete[] arrNumData;
//}
//
//void CImageStitchKMeans::RemoveOutlier2nd(double dThresh)
//{
//	int i;
//	bool bBreak = true;
//	double dInlierRate;
//	double* arrThresh;
//	Mat tmpHomo;
//
//	arrThresh = new double[2];
//
//	for (i = 0; i < 2; i++)
//	{
//		arrThresh[i] = dThresh;
//	}
//
//	for (i = 0; i < 2; i++)
//	{
//		while (bBreak)
//		{
//			bBreak = false;
//
//			dInlierRate = GetInlierRate(arrClusterR[i], arrClusterL[i], tmpHomo, dThresh);
//
//			if (dInlierRate < 0.2)
//			{
//				arrThresh[i] += 5;
//				bBreak = true;
//			}
//			else
//			{
//				arrHomo[i] = tmpHomo;
//			}
//		}
//
//		RemoveOutlier(arrClusterR[i], arrClusterL[i], arrThresh[i]);
//		bBreak = true;
//	}
//
//	delete[] arrThresh;
//
//	DrawFeature();
//}
//
//void CImageStitchKMeans::BlendingHomo()
//{
//	int y, x, i, j;
//	int tmp;
//	double TopDistance, BtmDistance;
//
//	for (y = 0; y < imgR.nH; y++)
//		for (x = 0; x < imgR.nW; x++)
//		{
//			// Top Set와 제일 가까운 값 구하기
//			for (i = 0; i < arrClusterR[0].size(); i++) {
//				if (i == 0) { TopDistance = GetDistance(double(x), double(y), arrClusterR[0][i].x, arrClusterR[0][i].y); }
//				else{
//					tmp = GetDistance(double(x), double(y), arrClusterR[0][i].x, arrClusterR[0][i].y);
//					if (tmp < TopDistance) TopDistance = tmp;
//				}
//			}
//
//			// Botttom Set와 제일 가까운 값 구하기
//			for (i = 0; i < arrClusterR[1].size(); i++) {
//				if (i == 0) { BtmDistance = GetDistance(double(x), double(y), arrClusterR[1][i].x, arrClusterR[1][i].y); }
//				else{
//					tmp = GetDistance(double(x), double(y), arrClusterR[1][i].x, arrClusterR[1][i].y);
//					if (tmp < BtmDistance) BtmDistance = tmp;
//				}
//			}
//
//			weightMap[y][x] = (TopDistance) / (TopDistance + BtmDistance);
//		}
//
//	// homography blending
//	for (int y = 0; y < imgR.nH; y++)
//		for (int x = 0; x < imgR.nW; x++)
//		{
//			homoAll[y][x] = Mat(cvSize(3, 3), CV_64FC1, cvScalar(0));
//			for (int i = 0; i < 3; i++)
//				for (int j = 0; j < 3; j++)
//				{
//					homoAll[y][x].at<double>(i, j) = (weightMap[y][x] * arrHomo[1].at<double>(i, j)) + ((1 - weightMap[y][x]) * arrHomo[0].at<double>(i, j));
//				}
//		}
//}
////===============================================================================================================================================================================================================================================
////===============================================================================================================================================================================================================================================
//
//CMultiHomoStitcing_jy::CMultiHomoStitcing_jy(int k)
//{
//	// 영상 정보 읽기
//	if (!GetCurrentImageInfo(&imgL.nW, &imgL.nH, &imgL.nPosX, &imgL.nPosY)) return;
//	if (!GetCurrentImageInfo(&imgR.nW, &imgR.nH, &imgR.nPosX, &imgR.nPosY, 1)) return;
//
//	imgL.imgGray = cmatrix(imgL.nH, imgL.nW);
//	imgR.imgGray= cmatrix(imgR.nH, imgR.nW);
//
//	imgL.imgRed = cmatrix(imgL.nH, imgL.nW);
//	imgL.imgGreen = cmatrix(imgL.nH, imgL.nW);
//	imgL.imgBlue = cmatrix(imgL.nH, imgL.nW);
//
//	imgR.imgRed = cmatrix(imgR.nH, imgR.nW);
//	imgR.imgGreen = cmatrix(imgR.nH, imgR.nW);
//	imgR.imgBlue = cmatrix(imgR.nH, imgR.nW);
//
//	imgMark.imgRed = cmatrix(imgR.nH, imgR.nW);
//	imgMark.imgGreen = cmatrix(imgR.nH, imgR.nW);
//	imgMark.imgBlue = cmatrix(imgR.nH, imgR.nW);
//
//	LabelMeanShift = imatrix(imgR.nH, imgR.nW);
//
//	weightMap = dmatrix(imgR.nH, imgR.nW);
//	labelWeight = imatrix(imgR.nH, imgR.nW);
//	homography_all = new Mat*[imgR.nH];
//	for (int i = 0; i < imgR.nH; i++) 	homography_all[i] = new Mat[imgR.nW];
//
//	segmentRed = cmatrix(imgR.nH, imgR.nW);
//	segmentBlue = cmatrix(imgR.nH, imgR.nW);
//	segmentGreen = cmatrix(imgR.nH, imgR.nW);
//
//	// 회색조 영상 읽기
//	GetCurrentImageGray(imgL.imgGray);
//	GetCurrentImageGray(imgR.imgGray, 1);
//
//	// 컬러 영상 읽기
//	GetCurrentImageColor(imgL.imgRed, imgL.imgGreen, imgL.imgBlue);
//	GetCurrentImageColor(imgR.imgRed, imgR.imgGreen, imgR.imgBlue, 1);
//	GetCurrentImageColor(imgMark.imgRed, imgMark.imgGreen, imgMark.imgBlue, 1);
//
//	nNumCluster = k;
//
//	arrInitKmean = new Point2f[nNumCluster];
//	arrClusterR = new vector<Point2f>[nNumCluster];
//	arrClusterL = new vector<Point2f>[nNumCluster];
//	arrCenter = new Point2f[nNumCluster];
//	arrHomo = new Mat[nNumCluster];
//	arrCost = new double[nNumCluster];
//
//	arrCovMat = new double**[nNumCluster];
//	for (int i = 0; i < nNumCluster; i++)
//	{
//		arrCovMat[i] = dmatrix(2, 2);
//	}
//
//	arrPCAvec = dmatrix(nNumCluster, 2);
//	arrAngle = new double[nNumCluster];
//}
//
//CMultiHomoStitcing_jy::~CMultiHomoStitcing_jy()
//{
//	//free_cmatrix(imgL.imgGray, imgL.nH, imgL.nW);
//	//free_cmatrix(imgR.imgGray, imgR.nH, imgR.nW);
//	//free_cmatrix(imgL.imgRed, imgL.nH, imgL.nW);
//	//free_cmatrix(imgL.imgGreen, imgL.nH, imgL.nW);
//	//free_cmatrix(imgL.imgBlue, imgL.nH, imgL.nW);
//	//free_cmatrix(imgR.imgRed, imgR.nH, imgR.nW);
//	//free_cmatrix(imgR.imgGreen, imgR.nH, imgR.nW); 
//	//free_cmatrix(imgR.imgBlue, imgR.nH, imgR.nW);
//	//free_cmatrix(imgMark.imgRed, imgR.nH, imgR.nW);
//	//free_cmatrix(imgMark.imgGreen, imgR.nH, imgR.nW);
//	//free_cmatrix(imgMark.imgBlue, imgR.nH, imgR.nW);
//	//free_cmatrix(segmentRed, imgR.nH, imgR.nW);
//	//free_cmatrix(segmentBlue, imgR.nH, imgR.nW);
//	//free_cmatrix(segmentGreen, imgR.nH, imgR.nW);
//	//free_imatrix(LabelMeanShift, imgR.nH, imgR.nW);
//	//free_dmatrix(weightMap, imgR.nH, imgR.nW);
//	//free_dmatrix(arrPCAvec, nNumCluster, 2);
//	//free_imatrix(labelWeight, imgR.nH, imgR.nW);
//
//	//delete[] labelCluster;
//	//delete[] arrInitKmean;
//	//delete[] arrClusterR;
//	//delete[] arrClusterL;
//	//delete[] arrCenter;
//	//delete[] arrHomo;
//	//delete[] arrAngle;
//
//	//for (int i = 0; i < imgR.nH; i++)
//	//	delete[] homography_all[i];
//	//delete[] homography_all;
//
//	//for (int i = 0; i < nNumCluster; i++)
//	//	free_dmatrix(arrCovMat[i], 2, 2);
//	//delete[] arrCovMat;
//}
//
////input : ImageL, ImageR, imgR.nH, imgR.nW, imgL.nH, imgL.nW
////output : GodPointL, GodPointR
//void CMultiHomoStitcing_jy::GetMatchingPoint()
//{
//	Mat M_ImageL(cvSize(imgL.nW, imgL.nH), CV_8UC1, cvScalar(0));
//	Mat M_ImageR(cvSize(imgR.nW, imgR.nH), CV_8UC1, cvScalar(0));
//
//	// Byte -> Mat
//
//	ByteToMatGray(imgL.imgGray, M_ImageL.data, imgL.nH, imgL.nW);
//	ByteToMatGray(imgR.imgGray, M_ImageR.data, imgR.nH, imgR.nW);
//
//	//Detect the keypoints using SURF Detector
//
//	SiftFeatureDetector detector;
//	std::vector< KeyPoint > keypoints_L, keypoints_R;
//	detector.detect(M_ImageL, keypoints_L);
//	detector.detect(M_ImageR, keypoints_R);
//
//	//-- Step 2: Calculate descriptors (feature vectors)
//
//	SiftDescriptorExtractor extractor;
//	Mat descriptors_L, descriptors_R;
//	extractor.compute(M_ImageL, keypoints_L, descriptors_L);
//	extractor.compute(M_ImageR, keypoints_R, descriptors_R);
//
//	//-- Step 3: Matching descriptor vectors using FLANN matcher
//
//	FlannBasedMatcher matcher;
//	std::vector< DMatch > matches;
//	matcher.match(descriptors_L, descriptors_R, matches);
//	double max_dist = 0; double min_dist = 100;
//
//
//	//-- Quick calculation of max and min distances between keypoints
//	for (int i = 0; i < descriptors_L.rows; i++)
//	{
//		double dist = matches[i].distance;
//		if (dist < min_dist) min_dist = dist;
//		if (dist > max_dist) max_dist = dist;
//	}
//
//	//-- Use only "good" matches (i.e. whose distance is less than 3*min_dist )
//
//	std::vector< DMatch > good_matches;
//	for (int i = 0; i < descriptors_L.rows; i++)
//		if (matches[i].distance <  4*min_dist)
//			good_matches.push_back(matches[i]);
//
//	//--특징점 갯수 확인
//	if (good_matches.size()<10)
//	{
//		AfxMessageBox(_T("특징점의 갯수가 부족합니다."));
//	}
//
//
//	for (unsigned int i = 0; i < good_matches.size(); i++)
//	{
//		//-- Get the keypoints from the good matches
//		GodPointL.push_back(keypoints_L[good_matches[i].queryIdx].pt);
//		GodPointR.push_back(keypoints_R[good_matches[i].trainIdx].pt);
//	}
//
//	nNumPoint = GodPointR.size();
//	labelCluster = new int[nNumPoint];
//}
//
//void CMultiHomoStitcing_jy::DrawMatchingPoint()
//{
//	BYTE ** ImageMatchRed;
//	BYTE ** ImageMatchGreen;
//	BYTE ** ImageMatchBlue;
//	int nHM, nWM;
//
//	nHM = imgR.nH;
//	nWM = imgR.nW + imgL.nW;
//
//	ImageMatchRed = cmatrix(nHM, nWM);
//	ImageMatchGreen = cmatrix(nHM, nWM);
//	ImageMatchBlue = cmatrix(nHM, nWM);
//
//	for (int i = 0; i < imgL.nH; i++)
//		for (int j = 0; j < imgL.nW; j++)
//		{
//			ImageMatchRed[i][j] = imgL.imgRed[i][j];
//			ImageMatchGreen[i][j] = imgL.imgGreen[i][j];
//			ImageMatchBlue[i][j] = imgL.imgBlue[i][j];
//		}
//
//	for (int i = 0; i < imgR.nH; i++)
//		for (int j = 0; j < imgR.nW; j++)
//		{
//			ImageMatchRed[i][j + imgL.nW] = imgR.imgRed[i][j];
//			ImageMatchGreen[i][j + imgL.nW] = imgR.imgGreen[i][j];
//			ImageMatchBlue[i][j + imgL.nW] = imgR.imgBlue[i][j];
//		}
//
//	for (int i = 0; i < GodPointR.size(); i++)
//	{
//		DrawLine(ImageMatchRed, imgR.nW + imgL.nW, imgR.nH, GodPointL[i].x, GodPointL[i].y, GodPointR[i].x + imgL.nW, GodPointR[i].y, 0);
//		DrawLine(ImageMatchGreen, imgR.nW + imgL.nW, imgR.nH, GodPointL[i].x, GodPointL[i].y, GodPointR[i].x + imgL.nW, GodPointR[i].y, 255);
//		DrawLine(ImageMatchBlue, imgR.nW + imgL.nW, imgR.nH, GodPointL[i].x, GodPointL[i].y, GodPointR[i].x + imgL.nW, GodPointR[i].y, 0);
//	}
//
//	DisplayCimage2DColor(ImageMatchRed, ImageMatchGreen, ImageMatchBlue, imgR.nW + imgL.nW, imgL.nH, 0, 0);
//}
//
////input : nNmCluster
////output : arrInitKmean
//void CMultiHomoStitcing_jy::SetInitCenPoint(Point2f* arrInput)
//{
//	delete[] arrInitKmean;
//
//	arrInitKmean = new Point2f[nNumCluster];
//
//	//--------------------------------------------
//
//	for (int i = 0; i < nNumCluster; i++)
//	{
//		arrInitKmean[i] = arrInput[i];
//	}
//}
//
////input : nNumCluster, imgR.nH
////output : arrInitKmean
////위쪽 클러스터에서 차례로 저장된다.
////bForgy Forgy 기법을 이용한다.; 
//void CMultiHomoStitcing_jy::GetInitCenPointForKmean(bool bForgy)
//{
//	delete[] arrInitKmean;
//
//	arrInitKmean = new Point2f[nNumCluster];
//	
//	//-------------------------------------------
//	double dTemp;
//	dTemp = double(imgR.nH) / double(nNumCluster * 2);
//
//	if (bForgy == true)
//	{
//		RNG rng;
//		int nRanSed;
//
//		for (int i = 0; i < nNumCluster; i++)
//		{
//			nRanSed = rng.uniform(0, nNumPoint-1);
//			arrInitKmean[i] = GodPointR[nRanSed];
//		}
//
//
//	}
//
//	if (bForgy == false)
//	{
//		for (int i = 0; i < nNumCluster; i++)
//		{
//			if (i == 0)
//			{
//				arrInitKmean[i].y = int(dTemp);
//			}
//
//			if (i > 0)
//			{
//				arrInitKmean[i].y = int(dTemp * (2 * i + 1));
//			}
//
//			arrInitKmean[i].x = double(imgR.nW) / double(2);
//		}
//	}
//}
//
////input : GodPointR, nNumCluster, nNumPoint, arrInitKmean, imgR.nH
////output : labelCluster, arrClusterL, arrClusterR, arrCenter
//void CMultiHomoStitcing_jy::KmeanForMultiHomo()
//{
//	double dTempDis;
//	int* arrNumData;
//	int nTempX, nTempY;
//	int nLabel;
//	bool bChange=true;
//
//	delete[] arrClusterR;
//	delete[] arrClusterL;
//	delete[] arrCenter;
//
//	arrClusterR = new vector<Point2f>[nNumCluster];
//	arrClusterL = new vector<Point2f>[nNumCluster];
//	arrCenter = new Point2f[nNumCluster];
//	arrNumData = new int[nNumCluster];
//	//------------<설정끝>---------------------------------------------------------
//	                                                     
//	for (int i = 0; i < nNumCluster; i++)
//	{
//		arrCenter[i].y = arrInitKmean[i].y;
//		arrCenter[i].x = arrInitKmean[i].x;
//	}
//
//
//	while (bChange)
//	{
//		bChange = false;
//
//		// 중심과 cluster의 거리
//		for (int i = 0; i < nNumPoint; i++)
//		{
//			nTempX = GodPointR[i].x;
//			nTempY = GodPointR[i].y;
//
//
//			for (int j = 0; j < nNumCluster; j++)
//			{
//				if (j == 0)
//				{
//					//dTempDis = GetDistance(nTempX, nTempY, arrCenter[j].x, arrCenter[j].y);
//					dTempDis = abs(nTempY - arrCenter[j].y);
//					nLabel = j;
//				} 
//				else
//				{
//					if (dTempDis > abs(nTempY - arrCenter[j].y))
//					{
//						nLabel = j;
//						dTempDis = abs(nTempY - arrCenter[j].y);
//					}
//				}
//			}
//
//			//변화가 있는지 확인한다.
//			if (labelCluster[i] != nLabel)
//			{
//				bChange = true;
//			}
//
//			labelCluster[i] = nLabel;
//		}
//
//		// 다시 중심값을 구한다
//		// Array 초기화
//		for (int i = 0; i < nNumCluster; i++)
//		{
//			arrCenter[i].y = 0;
//			arrCenter[i].x = 0;
//			arrNumData[i] = 0;
//		}
//
//		for (int i = 0; i < nNumPoint; i++)
//		{
//			arrCenter[labelCluster[i]].y += GodPointR[i].y;
//			arrCenter[labelCluster[i]].x += GodPointR[i].x;
//			arrNumData[labelCluster[i]] ++;
//		}
//
//		for (int i = 0; i < nNumCluster; i++)
//		{
//			arrCenter[i].y /= arrNumData[i];
//			arrCenter[i].x /= arrNumData[i];
//		}
//	}
//
//	//Make arrClusterR
//	for (int i = 0; i < nNumPoint; i++)
//	{
//		arrClusterR[labelCluster[i]].push_back(GodPointR[i]);
//		arrClusterL[labelCluster[i]].push_back(GodPointL[i]);
//	}
//
//	delete[] arrNumData;
//}
////input : arrClusterR, nNumPoint, Image_Mark, imgR.nH, imgR.nW, imgR.nPosX, imgL.nPosX
//void CMultiHomoStitcing_jy::DrawCluster()
//{
//	int nRand;
//	int** arrColor;
//
//	arrColor = imatrix(nNumCluster, 3);
//
//	// 중심점 표시하기
//	for (int i = 0; i < nNumCluster; i++)
//	{
//		DrawCross(imgMark.imgRed, imgR.nW, imgR.nH, CPoint(arrCenter[i].x, arrCenter[i].y), 0);
//		DrawCross(imgMark.imgGreen, imgR.nW, imgR.nH, CPoint(arrCenter[i].x, arrCenter[i].y), 0);
//		DrawCross(imgMark.imgBlue, imgR.nW, imgR.nH, CPoint(arrCenter[i].x, arrCenter[i].y), 0);
//	}
//
////-----------------------------------------------------------------------------------------
//	for (int i = 0; i < nNumCluster; i++)
//	{
//		nRand = rand() % 255;
//		arrColor[i][0] = nRand;
//		nRand = rand() % 255;
//		arrColor[i][1] = nRand;
//		nRand = rand() % 255;
//		arrColor[i][2] = nRand;
//	}
//
//	for (int i = 0; i < nNumCluster; i++)
//	{
//		DrawCross(imgMark.imgRed, imgR.nW, imgR.nH, CPoint(30 * i +10,10), arrColor[i][0]);
//		DrawCross(imgMark.imgGreen, imgR.nW, imgR.nH, CPoint(30 * i + 10, 10), arrColor[i][1]);
//		DrawCross(imgMark.imgBlue, imgR.nW, imgR.nH, CPoint(30 * i + 10, 10), arrColor[i][2]);
//	}
//
//	for (int i = 0; i < nNumPoint; i++)
//	{
//		DrawCross(imgMark.imgRed, imgR.nW, imgR.nH, CPoint(GodPointR[i].x, GodPointR[i].y), arrColor[labelCluster[i]][0]);
//		DrawCross(imgMark.imgGreen, imgR.nW, imgR.nH, CPoint(GodPointR[i].x, GodPointR[i].y), arrColor[labelCluster[i]][1]);
//		DrawCross(imgMark.imgBlue, imgR.nW, imgR.nH, CPoint(GodPointR[i].x, GodPointR[i].y), arrColor[labelCluster[i]][2]);
//	}
//
//	DisplayCimage2DColor(imgMark.imgRed, imgMark.imgGreen, imgMark.imgBlue, imgR.nW, imgR.nH, imgR.nPosX, imgR.nPosY);
//	free_imatrix(arrColor, nNumCluster, 3);
//}
////input : arrClusterR, nNumCluster, arrClusterL
////output : arrHomo
//void CMultiHomoStitcing_jy::GetHomography()
//{
//	delete[] arrHomo;
//
//	arrHomo = new Mat[nNumCluster];
//	//-------------------------------------------------------
//	for (int i = 0; i < nNumCluster; i++)
//	{
//		arrHomo[i] = findHomography(arrClusterR[i], arrClusterL[i], CV_RANSAC);
//	}
//}
//
////input : nMode, nNumCluster, ImageL, ImageR, arrHomo, imgR.nH, imgR.nW, imgL.nH, imgL.nW
////nMode : 0 Single Homography
////nMode : 1 Multi Homography
//
////Seam을 이용한 방법을 추가 160316
////방식은 먼저 웨이트 맵을 구하고 웨이트 맵을 트랜스폼을 한다.
////그리고 편의상 먼저 output에 오른쪽 영상을 워핑하고 
////나중에 트랜스된 웨이트맵을 서로 비교하여 
////output에 왼쪽 영상을 워핑하여 넣은다. 
//
//void CMultiHomoStitcing_jy::WarpHomography(int nMode, int nIdx, bool bOnContour)
//{
//	int x, y, i, j;
//	BYTE** outputImageRed, ** outputImageGreen, ** outputImageBlue;
//	Point2f inputPoint, transPoint;
//	double** weightSeamR, **weightSeamL;
//
//	//Set weightmap for Seam
//	weightSeamR = dmatrix(imgR.nH, imgR.nW);
//	weightSeamL = dmatrix(imgL.nH, imgL.nW);
//
//	MakeWeightSeam(weightSeamR, imgR.nH, imgR.nW);
//	MakeWeightSeam(weightSeamL, imgL.nH, imgL.nW);
//	
//	//
//	int offsetX = 0, offsetY = 0;
//	int nH_transR, nW_transR;
//	int maxX_totalR = 0, maxY_totalR = 0, minX_totalR = imgR.nW, minY_totalR = imgR.nH;
//	double maxX_transR = 0, maxY_transR = 0, minX_transR = imgR.nW, minY_transR = imgR.nH;
//	int nH_total, nW_total;
//
//	
//	for (int y = 0; y < imgR.nH; y++)
//		for (int x = 0; x < imgR.nW; x++)
//		{
//			inputPoint = cvPoint(x, y);
//			if (nMode == 0)
//				GetPerspectivePoint(inputPoint, transPoint, arrHomo[nIdx]);
//			if (nMode == 1)
//				GetPerspectivePoint(inputPoint, transPoint, homography_all[y][x]);
//
//			if (transPoint.y > maxY_transR) maxY_transR = transPoint.y;
//			if (transPoint.x > maxX_transR) maxX_transR = transPoint.x;
//			if (transPoint.y < minY_transR) minY_transR = transPoint.y;
//			if (transPoint.x < minX_transR) minX_transR = transPoint.x;
//		}
//
//	if (minY_transR < 0) minY_totalR = minY_transR; else minY_totalR = 0;
//	if (maxY_transR > imgR.nH - 1) maxY_totalR = maxY_transR; else maxY_totalR = imgR.nH - 1;
//
//	if (minX_transR < 0) minX_totalR = minX_transR; else minX_totalR = 0;
//	if (maxX_transR > imgR.nW - 1) maxX_totalR = maxX_transR; else maxX_totalR = imgR.nW - 1;
//
//	nH_total = maxY_totalR - minY_totalR + 1;
//	nW_total = maxX_totalR - minX_totalR + 1;
//
//	if (minY_transR < 0) offsetY = abs(minY_transR);
//	if (minX_transR < 0) offsetX = abs(minX_transR);
//
//	outputImageRed = cmatrix(nH_total, nW_total); outputImageGreen = cmatrix(nH_total, nW_total); outputImageBlue = cmatrix(nH_total, nW_total);
//	ImageLtrans_R = cmatrix(nH_total, nW_total); ImageLtrans_G = cmatrix(nH_total, nW_total); ImageLtrans_B = cmatrix(nH_total, nW_total);
//	ImageRtrans_R = cmatrix(nH_total, nW_total); ImageRtrans_G = cmatrix(nH_total, nW_total); ImageRtrans_B = cmatrix(nH_total, nW_total);
//	weightRtrans = dmatrix(nH_total, nW_total); weightLtrans = dmatrix(nH_total, nW_total);
//
//	//WarpSeamWeight
//	for (y = 0; y < nH_total; y++)
//		for (int x = 0; x < nW_total; x++)
//		{
//			weightRtrans[y][x] = 0;
//			weightLtrans[y][x] = 0;
//
//			ImageLtrans_R[y][x] = 0; ImageLtrans_G[y][x] = 0; ImageLtrans_B[y][x] = 0;
//			ImageRtrans_R[y][x] = 0; ImageRtrans_G[y][x] = 0; ImageRtrans_B[y][x] = 0;
//		}
//
//	for (int y = 0; y < imgL.nH; y++)
//		for (int x = 0; x < imgL.nW; x++)
//		{
//			weightLtrans[y + offsetY][x + offsetX] = weightSeamL[y][x];
//			ImageLtrans_R[y + offsetY][x + offsetX] = imgL.imgRed[y][x];
//			ImageLtrans_G[y + offsetY][x + offsetX] = imgL.imgGreen[y][x];
//			ImageLtrans_B[y + offsetY][x + offsetX] = imgL.imgBlue[y][x];
//		}
//
//	////Warping Image
//
//	bool bContour;
//
//	if (nMode == 0)
//	{
//		CString strIdx;
//		Mat inverH = arrHomo[nIdx].inv();
//
//		for (int y = minY_transR; y < maxY_transR; y++)
//			for (int x = minX_transR; x < maxX_transR; x++) {
//				bContour = false;
//
//				inputPoint = cvPoint(x, y);
//				GetPerspectivePoint(inputPoint, transPoint, inverH);
//				if (transPoint.x >= 0 && transPoint.x < imgR.nW && transPoint.y >= 0 && transPoint.y < imgR.nH)
//				{
//					if (bOnContour == true && (transPoint.x == 0 || transPoint.y == 0 || transPoint.y == imgR.nH - 1 || transPoint.x == imgR.nW - 1)) { bContour = true; }
//					
//					if (bContour == false)
//					{
//						ImageRtrans_R[y + offsetY][x + offsetX] = imgR.imgRed[int(transPoint.y)][int(transPoint.x)];
//						ImageRtrans_G[y + offsetY][x + offsetX] = imgR.imgGreen[int(transPoint.y)][int(transPoint.x)];
//						ImageRtrans_B[y + offsetY][x + offsetX] = imgR.imgBlue[int(transPoint.y)][int(transPoint.x)];
//						weightRtrans[y + offsetY][x + offsetX] = weightSeamR[int(transPoint.y)][int(transPoint.x)];
//					}
//					if (bContour == true)
//					{
//						outputImageRed[y + offsetY][x + offsetX] = 255;
//						outputImageGreen[y + offsetY][x + offsetX] = 255;
//						outputImageBlue[y + offsetY][x + offsetX] = 255;
//					}
//				}
//			}
//
//		//Seam에 따라 그림 넣기
//
//		for (y = 0; y < nH_total; y++)
//			for (x = 0; x < nW_total; x++)
//			{
//				if (weightRtrans[y][x] > weightLtrans[y][x])
//				{
//					weightRtrans[y][x] = 1;
//					weightLtrans[y][x] = 0;
//				}
//					
//
//				if (weightLtrans[y][x] > weightRtrans[y][x])
//				{
//					weightRtrans[y][x] = 0;
//					weightLtrans[y][x] = 1;
//				}
//					
//			}
//
//		this->MultiBandBlending(nH_total, nW_total, outputImageRed, outputImageGreen, outputImageBlue);
//
//	//	strIdx.Format(_T("%d"), nIdx);
//	//	DrawTextOnImag2DGray(outputImageRed, nW_total, nH_total, 0, 0, strIdx, DT_LEFT, 255, 1);
//	//	DrawTextOnImag2DGray(outputImageGreen, nW_total, nH_total, 0, 0, strIdx, DT_LEFT, 255, 1);
//	//	DrawTextOnImag2DGray(outputImageBlue, nW_total, nH_total, 0, 0, strIdx, DT_LEFT, 255, 1);
//	}
//
//	if (nMode == 1)
//	{
//		for (int y = 0; y < imgL.nH; y++)
//			for (int x = 0; x < imgL.nW; x++)
//			{
//				outputImageRed[y + offsetY][x + offsetX] = imgL.imgRed[y][x];
//				outputImageGreen[y + offsetY][x + offsetX] = imgL.imgGreen[y][x];
//				outputImageBlue[y + offsetY][x + offsetX] = imgL.imgBlue[y][x];
//			}
//
//		for (int y = 0; y < imgR.nH; y++)
//			for (int x = 0; x < imgR.nW; x++)
//			{
//				inputPoint = Point2f(x, y);
//				GetPerspectivePoint(inputPoint, transPoint, homography_all[y][x]);
//			}
//
//		for (int y = 0; y < imgR.nH; y++)
//			for (int x = 0; x < imgR.nW; x++) {
//				bContour = false;
//				if (bOnContour == true && (x == 0 || y == 0 || y == imgR.nH - 1 || x == imgR.nW - 1)) { bContour = true; }
//
//				inputPoint = Point2f(x, y);
//				GetPerspectivePoint(inputPoint, transPoint, homography_all[y][x]);
//
//				if (bContour == false)
//				{
//					outputImageRed[int(transPoint.y) + offsetY][int(transPoint.x) + offsetX] = imgR.imgRed[int(inputPoint.y)][int(inputPoint.x)];
//					outputImageGreen[int(transPoint.y) + offsetY][int(transPoint.x) + offsetX] = imgR.imgGreen[int(inputPoint.y)][int(inputPoint.x)];
//					outputImageBlue[int(transPoint.y) + offsetY][int(transPoint.x) + offsetX] = imgR.imgBlue[int(inputPoint.y)][int(inputPoint.x)];
//				}
//				if (bContour == true)
//				{
//					outputImageRed[int(transPoint.y) + offsetY][int(transPoint.x) + offsetX] = 255;
//					outputImageGreen[int(transPoint.y) + offsetY][int(transPoint.x) + offsetX] = 255;
//					outputImageBlue[int(transPoint.y) + offsetY][int(transPoint.x) + offsetX] = 255;
//				}
//			}
//	}
//
//	DisplayCimage2DColor(ImageLtrans_R, ImageLtrans_G, ImageLtrans_B, nW_total, nH_total, 0, 0);
//	DisplayCimage2DColor(ImageRtrans_R, ImageRtrans_G, ImageRtrans_B, nW_total, nH_total, 0, 0);
//	DisplayCimage2DColor(outputImageRed, outputImageGreen, outputImageBlue, nW_total, nH_total, 0, 0);
//	
//
//	free_cmatrix(outputImageRed, nH_total, nW_total);
//	free_cmatrix(outputImageGreen, nH_total, nW_total);
//	free_cmatrix(outputImageBlue, nH_total, nW_total);
//}
//
//void CMultiHomoStitcing_jy::MeshWarpHomography(bool bOnGrid)
//{
//	int i, j, x, y;
//	BYTE** outputImageRed, ** outputImageGreen, ** outputImageBlue;
//
//	// value for meshed warp
//	int nNumVert, nNumVertH, nNumVertW;
//	int nRestVertH, nRestVertW;
//	
//	CMesh* arrMesh;
//
//	// value for get Image size
//	int offsetX = 0, offsetY = 0;
//	int maxX_totalR = 0, maxY_totalR = 0, minX_totalR = imgR.nW, minY_totalR = imgR.nH;
//	double maxX_transR = 0, maxY_transR = 0, minX_transR = imgR.nW, minY_transR = imgR.nH;
//	int nH_total, nW_total;
//	Point2f inputPoint, transPoint;
//
//	//Set weightmap for Seam
//	double** weightSeamR, ** weightSeamL;
//
//	weightSeamR = dmatrix(imgR.nH, imgR.nW);
//	weightSeamL = dmatrix(imgL.nH, imgL.nW);
//
//	MakeWeightSeam(weightSeamR, imgR.nH, imgR.nW);
//	MakeWeightSeam(weightSeamL, imgL.nH, imgL.nW);
//
//	// get image's size
//	for (int y = 0; y < imgR.nH; y++)
//		for (int x = 0; x < imgR.nW; x++)
//		{
//			inputPoint = cvPoint(x, y);
//			GetPerspectivePoint(inputPoint, transPoint, homography_all[y][x]);
//
//			if (transPoint.y > maxY_transR) maxY_transR = transPoint.y;
//			if (transPoint.x > maxX_transR) maxX_transR = transPoint.x;
//			if (transPoint.y < minY_transR) minY_transR = transPoint.y;
//			if (transPoint.x < minX_transR) minX_transR = transPoint.x;
//		}
//
//	//Set transformed Right Image
//	if (minY_transR < 0) minY_totalR = minY_transR; else minY_totalR = 0;
//	if (maxY_transR > imgR.nH-1) maxY_totalR = maxY_transR; else maxY_totalR = imgR.nH-1;
//
//	if (minX_transR < 0) minX_totalR = minX_transR; else minX_totalR = 0;
//	if (maxX_transR > imgR.nW - 1) maxX_totalR = maxX_transR; else maxX_totalR = imgR.nW - 1;
//
//	nH_total = maxY_totalR - minY_totalR + 1;
//	nW_total = maxX_totalR - minX_totalR + 1;
//
//	if (minY_transR < 0) offsetY = abs(minY_transR);
//	if (minX_transR < 0) offsetX = abs(minX_transR);
//
//	//Set total Image's nH, nW
//
//	outputImageRed = cmatrix(nH_total, nW_total); outputImageGreen = cmatrix(nH_total, nW_total); outputImageBlue = cmatrix(nH_total, nW_total);
//	ImageLtrans_R = cmatrix(nH_total, nW_total); ImageLtrans_G = cmatrix(nH_total, nW_total); ImageLtrans_B = cmatrix(nH_total, nW_total);
//	ImageRtrans_R = cmatrix(nH_total, nW_total); ImageRtrans_G = cmatrix(nH_total, nW_total); ImageRtrans_B = cmatrix(nH_total, nW_total);
//	weightRtrans = dmatrix(nH_total, nW_total); weightLtrans = dmatrix(nH_total, nW_total);
//
//	//WarpSeamWeight
//	
//	for (int y = 0; y < nH_total; y++)
//		for (int x = 0; x < nW_total; x++)
//		{
//			ImageLtrans_R[y][x] = 0; ImageLtrans_G[y][x] = 0; ImageLtrans_B[y][x] = 0;
//			ImageRtrans_R[y][x] = 0; ImageRtrans_G[y][x] = 0; ImageRtrans_B[y][x] = 0;
//			weightRtrans[y][x] = 0; weightLtrans[y][x] = 0;
//		}
//	
//	for (int y = 0; y < imgL.nH; y++)
//		for (int x = 0; x < imgL.nW; x++)
//		{
//			weightLtrans[y + offsetY][x + offsetX] = weightSeamL[y][x];
//			ImageLtrans_R[y + offsetY][x + offsetX] = imgL.imgRed[y][x];
//			ImageLtrans_G[y + offsetY][x + offsetX] = imgL.imgGreen[y][x];
//			ImageLtrans_B[y + offsetY][x + offsetX] = imgL.imgBlue[y][x];
//		}
//	
//	//--------------------------------------------------------------
//	// mesh warp
//
//	nRestVertH = (imgR.nH-1) % 10;
//	nRestVertW = (imgR.nW-1) % 10;
//
//	if (nRestVertH == 0)
//		nNumVertH = (imgR.nH - 1) / 10;
//	else
//		nNumVertH = (imgR.nH - 1) / 10 + 1;
//
//	if (nRestVertW == 0)
//		nNumVertW = (imgR.nW - 1) / 10;
//	else
//		nNumVertW = (imgR.nW - 1) / 10 + 1;
//	
//	nNumVert = nNumVertH * nNumVertW;
//
//	arrMesh = new CMesh[nNumVert];
//	
//	// Set vertice
//
//	for (i = 0; i < nNumVertH - 1; i++)
//		for (j = 0; j < nNumVertW - 1; j++)
//		{
//			arrMesh[i * nNumVertW + j] = CMesh(10*i, 10*j, 10);
//		}
//	
//	for (i = 0; i < nNumVertH - 1; i++)
//	{
//		if (nRestVertW == 0)
//		{
//			nRestVertW = 10;
//		}
//
//		arrMesh[i * nNumVertW + nNumVertW - 1] = CMesh(10 * i, 10 * (nNumVertW - 1), 10, nRestVertW);
//	}
//	
//	for (i = 0; i < nNumVertW - 1; i++)
//	{
//		if (nRestVertH == 0)
//		{
//			nRestVertH = 10;
//		}
//
//		arrMesh[(nNumVertH - 1)* nNumVertW + i] = CMesh(10 * (nNumVertH - 1), 10 * i, nRestVertH, 10);
//	}
//
//	arrMesh[(nNumVertH - 1)* nNumVertW + nNumVertW - 1] = CMesh(10 * (nNumVertH - 1), 10 * (nNumVertW - 1), nRestVertH, nRestVertW);
//	
//	//Blending homography.
//	
//
//	// Set deformed vertice
//	int nTmpX, nTmpY;
//
//	for (i = 0; i < nNumVert; i++)
//	{
//		arrMesh[i].DeformMesh(false, homography_all);
//	}
//	
//	//inverse mesh warp
//	for (i = 0; i < nNumVert; i++)
//	{
//		// inverse warping
//		for (y = arrMesh[i].nMinY; y <= arrMesh[i].nMaxY; y++)
//			for (x = arrMesh[i].nMinX; x <= arrMesh[i].nMaxX; x++) {
//
//				inputPoint = cvPoint(x, y); 
//
//				Mat inverH = arrMesh[i].Homo.inv();
//				GetPerspectivePoint(inputPoint, transPoint, inverH);
//
//				if (   transPoint.x >= arrMesh[i].vertex[0].x 
//					&& transPoint.x <= arrMesh[i].vertex[3].x
//					&& transPoint.y >= arrMesh[i].vertex[0].y 
//					&& transPoint.y <= arrMesh[i].vertex[3].y)
//				{
//					ImageRtrans_R[y + offsetY][x + offsetX] = imgR.imgRed[int(transPoint.y)][int(transPoint.x)];
//					ImageRtrans_G[y + offsetY][x + offsetX] = imgR.imgGreen[int(transPoint.y)][int(transPoint.x)];
//					ImageRtrans_B[y + offsetY][x + offsetX] = imgR.imgBlue[int(transPoint.y)][int(transPoint.x)];
//					weightRtrans[y + offsetY][x + offsetX] = weightSeamR[int(transPoint.y)][int(transPoint.x)];
//				}
//			}
//	}
//
//	for (y = 0; y < nH_total; y++)
//		for (x = 0; x < nW_total; x++)
//		{
//			if (weightRtrans[y][x] > weightLtrans[y][x])
//			{
//				weightRtrans[y][x] = 1;
//				weightLtrans[y][x] = 0;
//			}
//
//			if (weightLtrans[y][x] > weightRtrans[y][x])
//			{
//				weightLtrans[y][x] = 1;
//				weightRtrans[y][x] = 0;
//			}
//		}
//
//	this->MultiBandBlending(nH_total, nW_total, outputImageRed, outputImageGreen, outputImageBlue);
//
//	//Check Display Grid
//	if (bOnGrid == true)
//	{
//		
//		/*for (i = 0; i < nNumVertH; i++)
//			for (j = 0; j < nNumVertW - 1; j++)
//			{
//				DrawLine(outputImageRed, nW_total, nH_total, arrVertDeform[i][j].x, arrVertDeform[i][j].y + offsetY, arrVertDeform[i][j + 1].x, arrVertDeform[i][j + 1].y + offsetY, 0);
//				DrawLine(outputImageGreen, nW_total, nH_total, arrVertDeform[i][j].x, arrVertDeform[i][j].y + offsetY, arrVertDeform[i][j + 1].x, arrVertDeform[i][j + 1].y + offsetY, 255);
//				DrawLine(outputImageBlue, nW_total, nH_total, arrVertDeform[i][j].x, arrVertDeform[i][j].y + offsetY, arrVertDeform[i][j + 1].x, arrVertDeform[i][j + 1].y + offsetY, 0);
//			}
//
//		for (i = 0; i < nNumVertH - 1; i++)
//			for (j = 0; j < nNumVertW; j++)
//			{
//				DrawLine(outputImageRed, nW_total, nH_total, arrVertDeform[i][j].x, arrVertDeform[i][j].y + offsetY, arrVertDeform[i + 1][j].x, arrVertDeform[i + 1][j].y + offsetY, 0);
//				DrawLine(outputImageGreen, nW_total, nH_total, arrVertDeform[i][j].x, arrVertDeform[i][j].y + offsetY, arrVertDeform[i + 1][j].x, arrVertDeform[i + 1][j].y + offsetY, 255);
//				DrawLine(outputImageBlue, nW_total, nH_total, arrVertDeform[i][j].x, arrVertDeform[i][j].y + offsetY, arrVertDeform[i + 1][j].x, arrVertDeform[i + 1][j].y + offsetY, 0);
//			}*/
//			
//	}
//
//	DisplayCimage2DColor(ImageLtrans_R, ImageLtrans_G, ImageLtrans_B, nW_total, nH_total, 0, 0);
//	DisplayCimage2DColor(ImageRtrans_R, ImageRtrans_G, ImageRtrans_B, nW_total, nH_total, 0, 0);
//	DisplayCimage2DColor(outputImageRed, outputImageGreen, outputImageBlue, nW_total, nH_total, 0, 0);
//
//	free_cmatrix(outputImageRed, nH_total, nW_total);
//	free_cmatrix(outputImageGreen, nH_total, nW_total);
//	free_cmatrix(outputImageBlue, nH_total, nW_total);
//}
//
////input : arrClusterR, arrCovMat, nNumCluster
////output : arrCovMat, arrPCAvec
//void CMultiHomoStitcing_jy::PCA()
//{
//	if (nNumCluster > 2)
//	{
//		free_dmatrix(arrPCAvec, nNumCluster - 1, 2);
//
//		for (int i = 0; i < nNumCluster - 1; i++)
//		{
//			free_dmatrix(arrCovMat[i], 2, 2);
//		}
//		delete[] arrCovMat;
//
//		arrPCAvec = dmatrix(nNumCluster, 2);
//		arrCovMat = new double**[nNumCluster];
//		for (int i = 0; i < nNumCluster; i++)
//		{
//			arrCovMat[i] = dmatrix(2, 2);
//		}
//	}
//	//--------------------------------------------
//
//	Mat tmpCovM(cvSize(2, 2), CV_64FC1, cvScalar(0, 0));
//	Mat eivec(cvSize(2, 2), CV_64FC1, cvScalar(0, 0));
//	Mat eival(cvSize(1, 2), CV_64FC1, cvScalar(0, 0));
//
//	for (int i = 0; i < nNumCluster; i++)
//	{
//		GetCovMatrix2D(arrCovMat[i], arrClusterR[i]);
//	}
//
//	for (int i = 0; i < nNumCluster; i++)
//	{
//		DoubleToMat(arrCovMat[i], tmpCovM, 2, 2);
//		eigen(tmpCovM, eival, eivec);
//
//		if (eival.at<double>(0, 0) > eival.at<double>(1, 0))
//		{
//			arrPCAvec[i][0] = eivec.at<double>(0, 0);
//			arrPCAvec[i][1] = eivec.at<double>(0, 1);
//		}
//		if (eival.at<double>(0, 0) < eival.at<double>(1, 0))
//		{
//			arrPCAvec[i][0] = eivec.at<double>(1, 0);
//			arrPCAvec[i][1] = eivec.at<double>(1, 1);
//		}
//	}
//}
////input : arrPCAvec, nNumCluster
////output : arrAngle
//void CMultiHomoStitcing_jy::GetVectAngle2D()
//{
//	delete [] arrAngle;
//
//	arrAngle = new double[nNumCluster - 1];
//	//-------------------------------------------------------
//
//	double dInner;
//	double dMagVec1, dMagVec2;
//	double dAngle;
//
//	for (int i = 0; i < nNumCluster - 1; i++)
//	{
//		dInner = (arrPCAvec[i][0] * arrPCAvec[i+1][0]) + (arrPCAvec[i][1] * arrPCAvec[i+1][1]);
//
//		dAngle = acos(dInner); 
//		if (dAngle > (3.141592 / 2)) dAngle = 3.141592 - dAngle;
//		arrAngle[i] = dAngle;
//	}
//}
////input : nNumCluster, arrCenter, imgR.nH, imgR.nW
////output : weightMap
//void CMultiHomoStitcing_jy::MakeWeightMap()
//{
//	double dTmpH = imgR.nH / (nNumCluster*2);
//	double dTopDistMahal, dBtmDistMahal;
//	int nTmp;
//
//	// Label Image for homography belending
//	for (int y = 0; y < imgR.nH; y++)
//		for (int x = 0; x < imgR.nW; x++)
//		{
//			for (int i = 0; i < nNumCluster; i++)
//			{
//				if (y < arrCenter[i].y)
//				{
//					labelWeight[y][x] = i;
//					break;
//				}
//
//				if (y >= arrCenter[nNumCluster - 1].y)
//				{
//					labelWeight[y][x] = nNumCluster;
//				}
//			}
//		}
//
//	// Get weght map for homography blending
//	double dDistance;
//	for (int y = 0; y < imgR.nH; y++)
//		for (int x = 0; x < imgR.nW; x++)
//		{
//			nTmp = labelWeight[y][x];
//
//			if (nTmp == 0)
//			{
//				dTopDistMahal = GetDistance_Mahalanobis(x, y, arrCenter[0].x, arrCenter[0].y, arrCovMat[0]);
//				dBtmDistMahal = GetDistance_Mahalanobis(x, y, arrCenter[1].x, arrCenter[1].y, arrCovMat[1]);
//				
//				dDistance = arrCenter[0].y - y;
//				dBtmDistMahal += (dDistance * 0.02);
//			}
//			
//			if (nTmp == nNumCluster)
//			{
//				dTopDistMahal = GetDistance_Mahalanobis(x, y, arrCenter[nNumCluster - 2].x, arrCenter[nNumCluster - 2].y, arrCovMat[nNumCluster - 2]);
//				dBtmDistMahal = GetDistance_Mahalanobis(x, y, arrCenter[nNumCluster - 1].x, arrCenter[nNumCluster - 1].y, arrCovMat[nNumCluster - 1]);
//
//				dDistance = y - arrCenter[nNumCluster-1].y;
//				dTopDistMahal += (dDistance * 0.02);
//			}
//
//			if ((nTmp > 0) && (nTmp < nNumCluster))
//			{
//				dTopDistMahal = GetDistance_Mahalanobis(x, y, arrCenter[nTmp - 1].x, arrCenter[nTmp - 1].y, arrCovMat[nTmp - 1]);
//				dBtmDistMahal = GetDistance_Mahalanobis(x, y, arrCenter[nTmp].x, arrCenter[nTmp].y, arrCovMat[nTmp]);
//			}
//
//			weightMap[y][x] = (dTopDistMahal) / ((dTopDistMahal)+(dBtmDistMahal));
//		}
//}
//
////input : nNumCluster, arrCenter, imgR.nH, imgR.nW
////output : weightMap
////Top, Bottom 으로 Cluster가 나누어 질때만 사용 가능.
//void CMultiHomoStitcing_jy::MakeWeightMap_TopBtm()
//{
//	double ** covTopMat, ** covBtmMat;
//	double dDistTop, dDistBtm;
//	double dDistCen;
//
//	covTopMat = dmatrix(2, 2);
//	covBtmMat = dmatrix(2, 2);
//
//	GetCovMatrix2D(covTopMat, arrClusterR[0]);
//	GetCovMatrix2D(covBtmMat, arrClusterR[1]);
//
//	nOperator = (4 * arrClusterR[0].size()) + 2;
//	nOperator += (4 * arrClusterL[1].size()) + 1;
//
//	// Make WeightMap
//	for (int y = 0; y < imgR.nH; y++)
//		for (int x = 0; x < imgR.nW; x++)
//		{
//
//			dDistTop = GetDistance_Mahalanobis(x, y, arrCenter[0].x, arrCenter[0].y, covTopMat);
//			dDistBtm = GetDistance_Mahalanobis(x, y, arrCenter[1].x, arrCenter[1].y, covBtmMat);
//
//			nOperator += 42;
//
//			if (dDistTop == 0) weightMap[y][x] = 0;
//			if (dDistBtm == 0) weightMap[y][x] = 1;
//			
//			if (y < arrCenter[0].y)
//			{
//				dDistCen = arrCenter[0].y - y;
//				dDistBtm += (dDistCen * 0.03);
//
//				nOperator += 3;
//			}
//
//			if (y > arrCenter[1].y)
//			{
//				dDistCen = y - arrCenter[1].y;
//				dDistTop += (dDistCen * 0.03);
//
//				nOperator += 3;
//			}
//			
//			weightMap[y][x] = (dDistTop) / ((dDistTop)+(dDistBtm));
//		}
//}
//
////input : nNumCluster, imgR.nH, imgR.nW
////output : weightMap
//void CMultiHomoStitcing_jy::MakeWeightMap_OriginTopBtm()
//{
//	int tmp;
//	double TopDistance, BtmDistance;
//	for (int y = 0; y < imgR.nH; y++)
//		for (int x = 0; x < imgR.nW; x++)
//		{
//			// Top Set와 제일 가까운 값 구하기
//			for (int i = 0; i < arrClusterR[0].size(); i++) {
//				if (i == 0) { TopDistance = GetDistance(double(x), double(y), arrClusterR[0][i].x, arrClusterR[0][i].y); }
//				else{
//					tmp = GetDistance(double(x), double(y), arrClusterR[0][i].x, arrClusterR[0][i].y);
//					if (tmp < TopDistance) TopDistance = tmp;
//				}
//			}
//
//			// Botttom Set와 제일 가까운 값 구하기
//			for (int i = 0; i < arrClusterR[1].size(); i++) {
//				if (i == 0) { BtmDistance = GetDistance(double(x), double(y), arrClusterR[1][i].x, arrClusterR[1][i].y); }
//				else{
//					tmp = GetDistance(double(x), double(y), arrClusterR[1][i].x, arrClusterR[1][i].y);
//					if (tmp < BtmDistance) BtmDistance = tmp;
//				}
//			}
//
//			if (TopDistance == 0) weightMap[y][x] = 0;
//			if (BtmDistance == 0) weightMap[y][x] = 1;
//			if (TopDistance > 0 && BtmDistance > 0)	weightMap[y][x] = (TopDistance) / (TopDistance + BtmDistance);
//		
//		}
//
//	DisplayDimage2D(weightMap, imgR.nW, imgR.nH, 0, 0);
//}
//
////bOnAngle == 0 : 각도 그림 포함
////bOnAngle == 1 : 각도 그림 미포함 
////input : nMode, arrWeight, arrAngle, arrPCAvec, arrCenter, imgR.nH, imgR.nW
//void CMultiHomoStitcing_jy::DrawWeightMap(bool bOnAngle)
//{
//	BYTE** outputImage;
//	int nY1, nY2;
//
//	outputImage = cmatrix(imgR.nH, imgR.nW);
//
//	for (int i = 0; i < imgR.nH; i++)
//		for (int j = 0; j < imgR.nW; j++)
//		{
//			outputImage[i][j] = weightMap[i][j] * 255;
//		}
//
//	//Draw PCA Line in ouputWeightImage
//	if (bOnAngle == true)
//	{
//		for (int i = 0; i < nNumCluster; i++)
//		{
//			nY1 = arrPCAvec[i][1] / arrPCAvec[i][0] * double(-80) + arrCenter[i].y;
//			nY2 = arrPCAvec[i][1] / arrPCAvec[i][0] * double(80) + arrCenter[i].y;
//
//			DrawLine(outputImage, imgR.nW, imgR.nH, arrCenter[i].x - 80, nY1, arrCenter[i].x + 80, nY2, 100);
//		}
//	}
//
//	DisplayCimage2D(outputImage, imgR.nW, imgR.nH, 0, 0);
//
//	//----------------------------------------------------------------
//	// option : draw degree between two PCA vector
//	if (bOnAngle == true)
//	{
//		BYTE** outputAngle;
//		CString strAngle;
//		double dDegree;
//		int nAngH, nAngW;
//
//		nAngH = 100;
//		nAngW = 100 * (nNumCluster-1);
//
//		outputAngle = cmatrix(nAngH, nAngW);
//
//		for (int j = 0; j < nAngH; j++)
//			for (int i = 0; i < nAngW; i++)
//			{
//				outputAngle[j][i] = 100;
//			}
//
//		for (int i = 0; i < nNumCluster-1; i++)
//		{
//			dDegree = (arrAngle[i] * 180 / 3.141592);
//			strAngle.Format(_T("%f"), dDegree);
//			DrawTextOnImag2DGray(outputAngle, nAngW, nAngH, 100 * i, 0, strAngle, DT_LEFT, 255, 1);
//
//			DrawLine(outputAngle, nAngW, nAngH, (100*i) + 10, 50, (100*i) + 10 + arrPCAvec[i][0] * 40, 50 + arrPCAvec[i][1] * 40, 0);
//			DrawLine(outputAngle, nAngW, nAngH, (100*i) + 10, 50, (100*i) + 10 + arrPCAvec[i + 1][0] * 40, 50 + arrPCAvec[i + 1][1] * 40, 0);
//		}
//
//		DisplayCimage2D(outputAngle, nAngW, nAngH, 0, 0);
//	}
//}
//
////input : weightMap, imgR.nH, imgR.nW
////output : homography_all
////nNumCluster에 따라 상황이 나눠져있다.
////nNumCluster > 2 : 위 아래 호모그래피 혼합 (맨 위, 아래는 혼합을 이용하지 않음.)
////nNumCluster == 2 : 단순히 두개만 혼합
////nNumCluster == 1 : 단일 호모그래피만 이용
//void CMultiHomoStitcing_jy::BlendHomo()
//{
//	int nIdx;
//
//	if (nNumCluster > 2)
//	{
//		for (int y = 0; y < imgR.nH; y++)
//			for (int x = 0; x < imgR.nW; x++)
//			{
//				nIdx = labelWeight[y][x];
//				homography_all[y][x] = Mat(cvSize(3, 3), CV_64FC1, cvScalar(0));
//
//				if (nIdx == 0)
//				{
//					homography_all[y][x] = arrHomo[0];
//				}
//
//				if (nIdx == nNumCluster)
//				{
//					homography_all[y][x] = arrHomo[nIdx - 1];
//				}
//
//				if ((nIdx > 0) && (nIdx < nNumCluster))
//				{
//					for (int i = 0; i < 3; i++)
//						for (int j = 0; j < 3; j++)
//						{
//							homography_all[y][x].at<double>(i, j) = (weightMap[y][x] * arrHomo[nIdx].at<double>(i, j)) + ((1 - weightMap[y][x]) * arrHomo[nIdx - 1].at<double>(i, j));
//						}
//				}
//			}
//	}
//
//	if (nNumCluster == 2)
//	{
//		for (int y = 0; y < imgR.nH; y++)
//			for (int x = 0; x < imgR.nW; x++)
//			{
//				homography_all[y][x] = Mat(cvSize(3, 3), CV_64FC1, cvScalar(0));
//				for (int i = 0; i < 3; i++)
//					for (int j = 0; j < 3; j++)
//					{
//						homography_all[y][x].at<double>(i, j) = (weightMap[y][x] * arrHomo[1].at<double>(i, j)) + ((1 - weightMap[y][x]) * arrHomo[0].at<double>(i, j));
//					}
//			}
//	}
//
//	if (nNumCluster == 1)
//	{
//		for (int y = 0; y < imgR.nH; y++)
//			for (int x = 0; x < imgR.nW; x++)
//			{
//				homography_all[y][x] = Mat(cvSize(3, 3), CV_64FC1, cvScalar(0));
//				homography_all[y][x] = arrHomo[0];
//			}
//	}
//}
//
//void CMultiHomoStitcing_jy::MakeWeightSeam(double** weightSeam, int nH, int nW)
//{
//	int i, j;
//
//	double dCenH, dCenW;
//	double dTmp;
//
//	dCenH = double(nH) / 2.;
//	dCenW = double(nW) / 2.;
//
//	for (i = 0; i < nH; i++)
//		for (j = 0; j < nW; j++)
//		{
//			dTmp = 1. - (1. / dCenH)*(abs(dCenH - double(i)));
//			dTmp = dTmp - (dTmp / dCenW)*(abs(dCenW - double(j)));
//
//			if (dTmp > 0)
//			{
//				dTmp = dTmp;
//			}
//
//			weightSeam[i][j] = dTmp;
//		}
//}
///****************************************************
//<MultiBandBlending>
//
//두영상의 이미지를 주파수에 따라 범위를 설정하여 블레딩을 시행해 준다.
//주파수가 높은 것은 좁은 영역, 주파수가 낮은 것은 넓은 영역으로 블렌딩을 시행.
//
//-----------------------------------------------------------------------
//변수명 설명 <*L_Ch1_R>
//
//G : 가우시안, B : 주파수 대역, W : 가우시안된 웨이트
//
//L : 왼쪽, 오른쪽
//Ch1 : 영상의 주파수 Ch1(low), Ch2(high)
//R : 색상 (R,G,B)
//
//----------------------------------------------------------------------
//Pre : imgR.nW, imgR.nH, ImageLtrans_R, ImageRtrans_G, ImageRtrans_B,
//	ImageLtrans_R, ImageLtrans_G, ImageLtrans_B, weightLtrans, weightRtrans
//Post : 
//
//*******************************************************/
//void CMultiHomoStitcing_jy::MultiBandBlending(int nH, int nW, BYTE** Result_R, BYTE** Result_G, BYTE** Result_B)
//{
//	int i, j, x, y;
//
//	int nGSize = 11; // Gaussian Filter Size
//	double dSigma =20; // Gaussian's sigma
//	double dSigma1, dSigma2;
//	double** Gfilter = dmatrix(nGSize, nGSize);
//
//
//	// 가우시안 필터링한 이미지
//	double** GL_Ch1_R, **GL_Ch1_G, **GL_Ch1_B; //왼쪽 영상
//	double** GR_Ch1_R, **GR_Ch1_G, **GR_Ch1_B; //오른쪽 영상
//
//	double** GL_Ch2_R, **GL_Ch2_G, **GL_Ch2_B;
//	double** GR_Ch2_R, **GR_Ch2_G, **GR_Ch2_B;
//
//	double** GL_Ch3_R, **GL_Ch3_G, **GL_Ch3_B;
//	double** GR_Ch3_R, **GR_Ch3_G, **GR_Ch3_B;
//
//	// 원본이미지와 가우시안한 이미지의 차이(즉 이미지의 주파수 대역)
//	double** BL_Ch1_R, **BL_Ch1_G, **BL_Ch1_B; //왼쪽 영상
//	double** BR_Ch1_R, **BR_Ch1_G, **BR_Ch1_B; //오른쪽 영상
//
//	double** BL_Ch2_R, **BL_Ch2_G, **BL_Ch2_B;
//	double** BR_Ch2_R, **BR_Ch2_G, **BR_Ch2_B;
//
//	double** BL_Ch3_R, **BL_Ch3_G, **BL_Ch3_B;
//	double** BR_Ch3_R, **BR_Ch3_G, **BR_Ch3_B;
//
//	// 가우시안 필터링한 웨이트 값
//	double** WL_Ch1, **WL_Ch2, **WL_Ch3; //왼쪽 영상
//	double** WR_Ch1, **WR_Ch2, **WR_Ch3; //오른쪽 영상
//
//
//	double** Out_Ch1_R, **Out_Ch1_G, **Out_Ch1_B;
//	double** Out_Ch2_R, **Out_Ch2_G, **Out_Ch2_B;
//	double** Out_Ch3_R, **Out_Ch3_G, **Out_Ch3_B;
//
//
//	//메모리 할당
//
//	WL_Ch1 = dmatrix(nH, nW); WL_Ch2 = dmatrix(nH, nW); WL_Ch3 = dmatrix(nH, nW);
//	WR_Ch1 = dmatrix(nH, nW); WR_Ch2 = dmatrix(nH, nW); WR_Ch3 = dmatrix(nH, nW);
//
//	GL_Ch1_R = dmatrix(nH, nW);	GL_Ch1_G = dmatrix(nH, nW);	GL_Ch1_B = dmatrix(nH, nW);
//	GR_Ch1_R = dmatrix(nH, nW);	GR_Ch1_G = dmatrix(nH, nW);	GR_Ch1_B = dmatrix(nH, nW);
//
//	GL_Ch2_R = dmatrix(nH, nW);	GL_Ch2_G = dmatrix(nH, nW);	GL_Ch2_B = dmatrix(nH, nW);
//	GR_Ch2_R = dmatrix(nH, nW);	GR_Ch2_G = dmatrix(nH, nW);	GR_Ch2_B = dmatrix(nH, nW);
//
//	GL_Ch3_R = dmatrix(nH, nW);	GL_Ch3_G = dmatrix(nH, nW);	GL_Ch3_B = dmatrix(nH, nW);
//	GR_Ch3_R = dmatrix(nH, nW);	GR_Ch3_G = dmatrix(nH, nW);	GR_Ch3_B = dmatrix(nH, nW);
//	
//	BL_Ch1_R = dmatrix(nH, nW); BL_Ch1_G = dmatrix(nH, nW);	BL_Ch1_B = dmatrix(nH, nW);
//	BR_Ch1_R = dmatrix(nH, nW);	BR_Ch1_G = dmatrix(nH, nW);	BR_Ch1_B = dmatrix(nH, nW);
//
//	BL_Ch2_R = dmatrix(nH, nW);	BL_Ch2_G = dmatrix(nH, nW);	BL_Ch2_B = dmatrix(nH, nW);
//	BR_Ch2_R = dmatrix(nH, nW);	BR_Ch2_G = dmatrix(nH, nW);	BR_Ch2_B = dmatrix(nH, nW);
//
//	BL_Ch3_R = dmatrix(nH, nW);	BL_Ch3_G = dmatrix(nH, nW);	BL_Ch3_B = dmatrix(nH, nW);
//	BR_Ch3_R = dmatrix(nH, nW);	BR_Ch3_G = dmatrix(nH, nW);	BR_Ch3_B = dmatrix(nH, nW);
//
//	Out_Ch1_R = dmatrix(nH, nW); Out_Ch1_G = dmatrix(nH, nW); Out_Ch1_B = dmatrix(nH, nW);
//	Out_Ch2_R = dmatrix(nH, nW); Out_Ch2_G = dmatrix(nH, nW); Out_Ch2_B = dmatrix(nH, nW);
//	Out_Ch3_R = dmatrix(nH, nW); Out_Ch3_G = dmatrix(nH, nW); Out_Ch3_B = dmatrix(nH, nW);
//
//	//-----------------------------------------------------------------------------------------
//
//	//Ch1(high frequency) 이미지 필터링
//	GaussianFilter(Gfilter, dSigma, nGSize);
//
//	//이미지 가우시안 필터링
//	MirrorPadding(GL_Ch1_R, Gfilter, ImageLtrans_R, nGSize, nW, nH); MirrorPadding(GL_Ch1_G, Gfilter, ImageLtrans_G, nGSize, nW, nH); MirrorPadding(GL_Ch1_B, Gfilter, ImageLtrans_B, nGSize, nW, nH);
//	MirrorPadding(GR_Ch1_R, Gfilter, ImageRtrans_R, nGSize, nW, nH); MirrorPadding(GR_Ch1_G, Gfilter, ImageRtrans_G, nGSize, nW, nH); MirrorPadding(GR_Ch1_B, Gfilter, ImageRtrans_B, nGSize, nW, nH);
//
//	//웨이트 가우시안 필터링
//	MirrorPadding(WL_Ch1, Gfilter, weightLtrans, nGSize, nW, nH); MirrorPadding(WR_Ch1, Gfilter, weightRtrans, nGSize, nW, nH);
//
//	double val;
//	for (int y = 0; y<nH; y++)
//		for (int x = 0; x<nW; x++)
//		{
//			BL_Ch1_R[y][x] =  ImageLtrans_R[y][x] - GL_Ch1_R[y][x];
//			BL_Ch1_G[y][x] = ImageLtrans_G[y][x] - GL_Ch1_G[y][x];
//			BL_Ch1_B[y][x] = ImageLtrans_B[y][x] - GL_Ch1_B[y][x];
//
//			BR_Ch1_R[y][x] =  ImageRtrans_R[y][x] - GR_Ch1_R[y][x];
//			BR_Ch1_G[y][x] =  ImageRtrans_G[y][x] - GR_Ch1_G[y][x];
//			BR_Ch1_B[y][x] =  ImageRtrans_B[y][x] - GR_Ch1_B[y][x];
//
//			val = (BL_Ch1_R[y][x] * WL_Ch1[y][x] + BR_Ch1_R[y][x] * WR_Ch1[y][x]) / (WL_Ch1[y][x] + WR_Ch1[y][x]);
//			Out_Ch1_R[y][x] = val;
//
//			val = (BL_Ch1_G[y][x] * WL_Ch1[y][x] + BR_Ch1_G[y][x] * WR_Ch1[y][x]) / (WL_Ch1[y][x] + WR_Ch1[y][x]);
//			Out_Ch1_G[y][x] = val;
//
//			val = (BL_Ch1_B[y][x] * WL_Ch1[y][x] + BR_Ch1_B[y][x] * WR_Ch1[y][x]) / (WL_Ch1[y][x] + WR_Ch1[y][x]);
//			Out_Ch1_B[y][x] = val;
//		}
//
//	//Ch2(middle frequency) 이미지 필터링
//	dSigma1 = sqrt(3.)*dSigma;
//	GaussianFilter(Gfilter, dSigma1, nGSize);
//
//	MirrorPadding(GL_Ch2_R, Gfilter, GL_Ch1_R, nGSize, nW, nH); MirrorPadding(GL_Ch2_G, Gfilter, GL_Ch1_R, nGSize, nW, nH); MirrorPadding(GL_Ch2_B, Gfilter, GL_Ch1_R, nGSize, nW, nH);
//	MirrorPadding(GR_Ch2_R, Gfilter, GR_Ch1_R, nGSize, nW, nH); MirrorPadding(GR_Ch2_G, Gfilter, GR_Ch1_G, nGSize, nW, nH); MirrorPadding(GR_Ch2_B, Gfilter, GR_Ch1_B, nGSize, nW, nH);
//
//	MirrorPadding(WL_Ch2, Gfilter, WL_Ch1, nGSize, nW, nH); MirrorPadding(WR_Ch2, Gfilter, WR_Ch1, nGSize, nW, nH);
//
//	for (y = 0; y<nH; y++)
//		for (x = 0; x<nW; x++)
//		{
//			BL_Ch2_R[y][x] =  GL_Ch1_R[y][x] - GL_Ch2_R[y][x];
//			BL_Ch2_G[y][x] = GL_Ch1_G[y][x] - GL_Ch2_G[y][x];
//			BL_Ch2_B[y][x] =  GL_Ch1_B[y][x] - GL_Ch2_B[y][x];
//
//			BR_Ch2_R[y][x] =  GR_Ch1_R[y][x] - GR_Ch2_R[y][x];
//			BR_Ch2_G[y][x] =  GR_Ch1_G[y][x] - GR_Ch2_G[y][x];
//			BR_Ch2_B[y][x] =  GR_Ch1_B[y][x] - GR_Ch2_B[y][x];
//
//			val = (BL_Ch2_R[y][x] * WL_Ch2[y][x] + BR_Ch2_R[y][x] * WR_Ch2[y][x]) / (WL_Ch2[y][x] + WR_Ch2[y][x]);
//			Out_Ch2_R[y][x] = val;
//
//			val = (BL_Ch2_G[y][x] * WL_Ch2[y][x] + BR_Ch2_G[y][x] * WR_Ch2[y][x]) / (WL_Ch2[y][x] + WR_Ch2[y][x]);
//			Out_Ch2_G[y][x] = val;
//
//			val = (BL_Ch2_B[y][x] * WL_Ch2[y][x] + BR_Ch2_B[y][x] * WR_Ch2[y][x]) / (WL_Ch2[y][x] + WR_Ch2[y][x]);
//			Out_Ch2_B[y][x] = val;
//		}
//	
//	//Ch3(low frequency) 이미지 필터링
//	dSigma2 = sqrt(5.)*dSigma;
//	GaussianFilter(Gfilter, dSigma2, nGSize);
//
//	MirrorPadding(GL_Ch3_R, Gfilter, GL_Ch2_R, nGSize, nW, nH); MirrorPadding(GL_Ch3_G, Gfilter, GL_Ch2_R, nGSize, nW, nH); MirrorPadding(GL_Ch3_B, Gfilter, GL_Ch2_R, nGSize, nW, nH);
//	MirrorPadding(GR_Ch3_R, Gfilter, GR_Ch2_R, nGSize, nW, nH); MirrorPadding(GR_Ch3_G, Gfilter, GR_Ch2_G, nGSize, nW, nH); MirrorPadding(GR_Ch3_B, Gfilter, GR_Ch2_B, nGSize, nW, nH);
//
//	MirrorPadding(WL_Ch3, Gfilter, WL_Ch2, nGSize, nW, nH); MirrorPadding(WR_Ch3, Gfilter, WR_Ch2, nGSize, nW, nH);
//
//	for (y = 0; y<nH; y++)
//		for (x = 0; x<nW; x++)
//		{
//			BL_Ch3_R[y][x] = GL_Ch2_R[y][x];// -GL_Ch3_R[y][x];
//			BL_Ch3_G[y][x] = GL_Ch2_G[y][x];// -GL_Ch3_G[y][x];
//			BL_Ch3_B[y][x] = GL_Ch2_B[y][x];// -GL_Ch3_B[y][x];
//
//			BR_Ch3_R[y][x] = GR_Ch2_R[y][x];// -GR_Ch3_R[y][x];
//			BR_Ch3_G[y][x] = GR_Ch2_G[y][x];// -GR_Ch3_G[y][x];
//			BR_Ch3_B[y][x] = GR_Ch2_B[y][x];// -GR_Ch3_B[y][x];
//
//			val = (BL_Ch3_R[y][x] * WL_Ch3[y][x] + BR_Ch3_R[y][x] * WR_Ch3[y][x]) / (WL_Ch3[y][x] + WR_Ch3[y][x]);
//			Out_Ch3_R[y][x] = val;
//			
//			val = (BL_Ch3_G[y][x] * WL_Ch3[y][x] + BR_Ch3_G[y][x] * WR_Ch3[y][x]) / (WL_Ch3[y][x] + WR_Ch3[y][x]);
//			Out_Ch3_G[y][x] = val;
//			
//			val = (BL_Ch3_B[y][x] * WL_Ch3[y][x] + BR_Ch3_B[y][x] * WR_Ch3[y][x]) / (WL_Ch3[y][x] + WR_Ch3[y][x]);
//			Out_Ch3_B[y][x] = val;
//		}
//
//	for (y = 0; y < nH; y++)
//		for (x = 0; x < nW; x++)
//		{
//			if ( WR_Ch3[y][x] == 0)
//			{
//				WR_Ch3[y][x] = 1;
//			}
//			else
//				WR_Ch3[y][x] = 0;
//		}
//
//	// 블렌딩 합치기
//	for (int y = 0; y<nH; y++)
//		for (int x = 0; x<nW; x++)
//		{
//			val = Out_Ch1_R[y][x] + Out_Ch2_R[y][x] + Out_Ch3_R[y][x];
//			val = val < 0 ? 0 : val;
//			val = val > 255 ? 255 : val;
//			Result_R[y][x] = val;
//			
//			val = Out_Ch1_G[y][x] + Out_Ch2_G[y][x] + Out_Ch3_G[y][x];
//			val = val < 0 ? 0 : val;
//			val = val > 255 ? 255 : val;
//			Result_G[y][x] = val;
//
//			val = Out_Ch1_B[y][x] + Out_Ch2_B[y][x] + Out_Ch3_B[y][x];
//			val = val < 0 ? 0 : val;
//			val = val > 255 ? 255 : val;
//			Result_B[y][x] = val;
//		}
//
//
//	//메모리 해제
//	free_dmatrix(Gfilter, nGSize, nGSize);
//
//	free_dmatrix(GL_Ch1_R, nH, nW); free_dmatrix(GL_Ch1_G, nH, nW); free_dmatrix(GL_Ch1_B, nH, nW);
//	free_dmatrix(GR_Ch1_R, nH, nW); free_dmatrix(GR_Ch1_G, nH, nW); free_dmatrix(GR_Ch1_B, nH, nW);
//
//	free_dmatrix(GL_Ch2_R, nH, nW); free_dmatrix(GL_Ch2_G, nH, nW); free_dmatrix(GL_Ch2_B, nH, nW);
//	free_dmatrix(GR_Ch2_R, nH, nW); free_dmatrix(GR_Ch2_G, nH, nW); free_dmatrix(GR_Ch2_B, nH, nW);
//
//	free_dmatrix(GL_Ch3_R, nH, nW); free_dmatrix(GL_Ch3_G, nH, nW); free_dmatrix(GL_Ch3_B, nH, nW);
//	free_dmatrix(GR_Ch3_R, nH, nW); free_dmatrix(GR_Ch3_G, nH, nW); free_dmatrix(GR_Ch3_B, nH, nW);
//
//	free_dmatrix(BL_Ch1_R, nH, nW); free_dmatrix(BL_Ch1_G, nH, nW); free_dmatrix(BL_Ch1_B, nH, nW);
//	free_dmatrix(BR_Ch1_R, nH, nW); free_dmatrix(BR_Ch1_G, nH, nW); free_dmatrix(BR_Ch1_B, nH, nW);
//
//	free_dmatrix(BL_Ch2_R, nH, nW); free_dmatrix(BL_Ch2_G, nH, nW); free_dmatrix(BL_Ch2_B, nH, nW);
//	free_dmatrix(BR_Ch2_R, nH, nW); free_dmatrix(BR_Ch2_G, nH, nW); free_dmatrix(BR_Ch2_B, nH, nW);
//
//	free_dmatrix(BL_Ch3_R, nH, nW); free_dmatrix(BL_Ch3_G, nH, nW); free_dmatrix(BL_Ch3_B, nH, nW);
//	free_dmatrix(BR_Ch3_R, nH, nW); free_dmatrix(BR_Ch3_G, nH, nW); free_dmatrix(BR_Ch3_B, nH, nW);
//
//	free_dmatrix(Out_Ch1_R, nH, nW); free_dmatrix(Out_Ch1_G, nH, nW); free_dmatrix(Out_Ch1_B, nH, nW);
//	free_dmatrix(Out_Ch2_R, nH, nW); free_dmatrix(Out_Ch2_G, nH, nW); free_dmatrix(Out_Ch2_B, nH, nW);
//	free_dmatrix(Out_Ch3_R, nH, nW); free_dmatrix(Out_Ch3_G, nH, nW); free_dmatrix(Out_Ch3_B, nH, nW);
//}
///****************************************************
//<SegmentMeanshift>
//
//Byte Image를 IplImage로 바꾸고 MeanShift segmentation를 해준다.
//
//minFeature 이상의 특징점을 가지고 있는 Region Number와
//minArea 이상의 영역을 가지고 있는 Region Number을 골라준다.
//그리고 그 영역에 대한 Index 배열을 가진다.
//
//------------------------------------
//Pre : imgR.nW, imgR.nH, imgR.imgRed, imgR.imgBlue, imgR.imgGreen, minrRegion
//Post : nNumSeg, LabelMeanShift
//
//*******************************************************/
//void CMultiHomoStitcing_jy::SegmentMeanshift(int minRegion, int minFeature, int minArea)
//{
//	int i, j, x, y;
//
//	IplImage* inputIplImg;
//	IplImage* resultIplImg;
//	int nIdx;
//	int tmpX, tmpY;
//	int tmp=0;
//
//	inputIplImg = cvCreateImage(cvSize(imgR.nW, imgR.nH), IPL_DEPTH_8U, 3);
//	resultIplImg = cvCreateImage(cvSize(imgR.nW, imgR.nH), IPL_DEPTH_8U, 3);
//
//	for (y = 0; y < imgR.nH; y++)
//		for (x = 0; x < imgR.nW; x++)
//		{
//			nIdx = y * inputIplImg->widthStep + x * inputIplImg->nChannels;
//			inputIplImg->imageData[nIdx] = imgR.imgBlue[y][x];
//			inputIplImg->imageData[nIdx + 1] = imgR.imgGreen[y][x];
//			inputIplImg->imageData[nIdx + 2] = imgR.imgRed[y][x];
//		}
//	
//	nNumSeg = MeanShift(inputIplImg, LabelMeanShift, resultIplImg, minRegion);
//
//	//
//	arrRegionFeaturepoint = new int[nNumSeg];
//	arrRegionArea = new int[nNumSeg];
//
//	for (i = 0; i < nNumSeg; i++)
//	{
//		arrRegionFeaturepoint[i] = 0;
//		arrRegionArea[i] = 0;
//	}
//
//	// Set Feature point's number by Region
//	for (i = 0; i < nNumPoint; i++)
//	{
//		tmpX = GodPointR[i].x;
//		tmpY = GodPointR[i].y;
//		arrRegionFeaturepoint[LabelMeanShift[tmpY][tmpX]]++;
//	}
//
//	// Set area's number by Region
//	for (y = 0; y < imgR.nH; y++)
//		for (x = 0; x < imgR.nW; x++)
//		{
//			arrRegionArea[LabelMeanShift[y][x]]++;
//		}
//
//	// Set Region's number for getting homography
//	nNumHomoRegion = 0;
//
//	for (i = 0; i < nNumSeg; i++)
//	{
//		if (arrRegionFeaturepoint[i] >= minFeature && arrRegionArea[i] >= minArea)
//		{
//			nNumHomoRegion++;
//		}
//	}
//
//	// Assign data's memory for getting homography
//	homo_seg = new Mat[nNumHomoRegion];
//	segClusterExcptR = new vector< Point2f >[nNumHomoRegion];
//	segClusterExcptL = new vector< Point2f >[nNumHomoRegion];
//	segClusterR = new vector< Point2f >[nNumHomoRegion];
//	segClusterL = new vector< Point2f >[nNumHomoRegion];
//	regionIdx = new int[nNumHomoRegion];
//
//	// Set Region's index for homography
//	for (i = 0; i < nNumSeg; i++)
//	{
//		if (arrRegionFeaturepoint[i] >= minFeature && arrRegionArea[i] >= minArea)
//		{
//			regionIdx[tmp] = i;
//			tmp++;
//		}
//	}
//
//	// Set Clusters
//	for (i = 0; i < nNumHomoRegion; i++)
//	{
//		for (j = 0; j < nNumPoint; j++)
//		{
//			if (LabelMeanShift[int(GodPointR[j].y)][int(GodPointR[j].x)] != regionIdx[i])
//			{
//				segClusterExcptR[i].push_back(GodPointR[j]);
//				segClusterExcptL[i].push_back(GodPointL[j]);
//			}
//			else
//			{
//				segClusterR[i].push_back(GodPointR[j]);
//				segClusterL[i].push_back(GodPointL[j]);
//			}
//		}
//	}
//
//
//}
//
//void CMultiHomoStitcing_jy::DetectMultiObject()
//{
//	int i, j;
//
//	double inlierRate, dissmillar = 0;
//	int* index;
//	double thresh = 0.7;
//	int newNum = nNumHomoRegion;
//	int tmp = 0;
//	bool bDone = true;
//
//	// 모든 object 후보군을 제외한 특징점의 inlier 비율 구하기
//	inlierRate = GetExceptInlierRate(regionIdx, nNumHomoRegion, 0, backHomo);
//
//	if (inlierRate > thresh)
//	{
//		for (i = 0; i < nNumHomoRegion; i++)
//		{
//			homo_seg[i] = findHomography(segClusterR[i], segClusterL[i], CV_RANSAC);
//			dissmillar = GetDissmiliarity(imgR.nH, imgR.nW, backHomo, homo_seg[i]);
//
//			if (dissmillar > thresh)
//			{
//				regionIdx[i] = -1;
//				newNum--;
//			}
//		}
//
//		if (newNum > 0)
//		{
//			index = new int[newNum];
//
//			for (i = 0; i < nNumHomoRegion; i++)
//			{
//				if (regionIdx[i] != -1)
//				{
//					index[tmp] = regionIdx[i];
//					tmp++;
//				}
//			}
//
//			delete[] regionIdx;
//			regionIdx = new int[newNum];
//
//			for (i = 0; i < newNum; i++)
//			{
//				regionIdx[i] = index[i];
//			}
//
//			delete[] index;
//		}
//		else
//		{
//			bExeDualHomo = true;
//		}
//	}
//	else
//	{
//		bExeDualHomo = true;	// 싱글 호모그래피를 사용한다.
//	}
//}
//
//
///****************************************************
//<GetHomographyForSeg>
//
//minFeature 이상의 특징점을 가지고 있는 Region을 고른다.
//그리고 각 Region마다 해당 특징점을 제외한 Homography 행렬을 구한다.
//또한 전체 호모그래피를 구한다. 
//
//------------------------------------
//Pre : nNumSeg, GodPointR, LabelMeanShift, minFeature
//Post : arrRegionFeaturepoint, segMarkR, nNumHomoRegion, 
//		homo_seg, segClusterExcptR, segClusterExcptL, regionIdx
//
//*******************************************************/
//
//void CMultiHomoStitcing_jy::GetHomographyForSeg()
//{
//	int i, j, x, y;
//
//	segMarkR = new Point2f[nNumHomoRegion];
//
//	// Mark segmentation region's position
//	for (i = 0; i < nNumHomoRegion; i++)
//	{
//		for (y = 0; y < imgR.nH; y++)
//			for (x = 0; x < imgR.nW; x++)
//			{
//				if (LabelMeanShift[y][x] == regionIdx[i])
//				{
//					segMarkR[i].y = y;
//					segMarkR[i].x = x;
//					goto outsegmark;
//				}
//			}
//
//	outsegmark:
//		int a;
//	}
//
//	// Set Homography except region
//	for (i = 0; i < nNumHomoRegion; i++)
//	{
//		homo_seg[i]=findHomography(segClusterExcptR[i], segClusterExcptL[i], CV_RANSAC);
//	}
//
//	// Set Global Homography
//	gloHomo = findHomography(GodPointR, GodPointL, CV_RANSAC);
//}
//
///****************************************************
//<GetObjectHomography>
//
//homo_seg를 이용하여 Region를 제외한 곳에 inlier 갯수를 구한다. 
//최고 inlier의 비율이 높은 것을 찾고 그 Rigion이 Object라고 판별한다.
//해당 object의 Homography를 구한다.
//
//+ object H를 쓸지 single H를 쓸지 판단한다.
//1. similarity bivariance gaussian를 얻는다.
//2. similarity가 높을 때 object homography를 구한다.  
//
//------------------------------------
//Pre : nNumHomoRegion, segClusterExcptR, segClusterExcptL, numSegCluster, maxInlierDis 
//Post : numInlier, arrInlierRate, nMaxInlierRegion, 
//       objHomo, objClusterR, objClusterL,
//	   backHomo, backClusterR, backClusterL
//
//*******************************************************/
////  <test>
//void CMultiHomoStitcing_jy::GetObjectHomography(float maxInlierDis)
//{
//	int i, j, x, y;
//	Point2f tmpInput, tmpOutput;
//	int numInlier;
//	int numSegPoint;
//	float fMaxInlierRate = 0;
//	float fTmp;
//
//	arrInlierRate = new float[nNumHomoRegion];
//
//	for (i = 0; i < nNumHomoRegion; i++)
//	{
//		numInlier = 0;
//		numSegPoint = segClusterExcptR[i].size();
//
//		for (j = 0; j < numSegPoint; j++)
//
//		{
//			tmpInput.x = segClusterExcptR[i][j].x;
//			tmpInput.y = segClusterExcptR[i][j].y;
//
//			GetPerspectivePoint(tmpInput, tmpOutput, homo_seg[i]);
//
//			fTmp = GetDistance(tmpOutput.x, tmpOutput.y, segClusterExcptL[i][j].x, segClusterExcptL[i][j].y);
//
//			if (fTmp < maxInlierDis)
//			{
//				numInlier++;
//			}
//		}
//
//		arrInlierRate[i] = float(numInlier) / float(numSegPoint);
//
//		if (fMaxInlierRate < arrInlierRate[i])
//		{
//			fMaxInlierRate = arrInlierRate[i];
//			nMaxInlierRegion = regionIdx[i];
//		}
//	}
//
//	// object cluster 형성
//	for (i = 0; i < nNumPoint; i++)
//	{
//		if (LabelMeanShift[int(GodPointR[i].y)][int(GodPointR[i].x)] == nMaxInlierRegion)
//		{
//			objClusterR.push_back(GodPointR[i]);
//			objClusterL.push_back(GodPointL[i]);
//		}
//		else
//		{
//			backClusterR.push_back(GodPointR[i]);
//			backClusterL.push_back(GodPointL[i]);
//		}
//	}
//
//	objHomo = findHomography(objClusterR, objClusterL, CV_RANSAC);
//	backHomo = findHomography(backClusterR, backClusterL, CV_RANSAC);
//
//	// object Homography 형성.
//	double** covMatR, **covMatF;
//	double* meanR, *meanF;
//	vector<Point2f> regionPoint;
//	double similarity;
//
//	covMatR = dmatrix(2, 2);
//	covMatF = dmatrix(2, 2);
//	meanR = new double[2];
//	meanF = new double[2];
//
//	// Get Region Mean, covMat
//	for (y = 0; y < imgR.nH; y++)
//		for (x = 0; x < imgR.nW; x++)
//		{
//			if (LabelMeanShift[y][x] == nMaxInlierRegion)
//			{
//				regionPoint.push_back(Point2f(x, y));
//			}
//		}
//
//	GetCovAndMean2D(covMatR, meanR, regionPoint);
//
//	// Get Feature in Region Mean, covMat
//	GetCovAndMean2D(covMatF, meanF, objClusterR);
//
//	// KL-divergence
//	similarity = KLdivergenceForBivariateGaussiian(meanF, meanR, covMatF, covMatR);
//	similarity += KLdivergenceForBivariateGaussiian(meanR, meanF, covMatR, covMatF);
//
//	similarity /= 2.;
//
//	// 분포의 차이 클 때 싱글 호모그래피 이용
//	if (similarity > 5)
//	{
//		bExeDualHomo = false;
//	}
//	else
//	{
//		bExeDualHomo = true;
//	}
//}
//
///****************************************************
//<SetWeightMap>
//
//WeightMap을 형성한다. 
//
//opening -> Labeling(지역이 작은 것 제거) -> dilation(Region 확장)
//
//------------------------------------
//
//Pre : imgR.nH, imgR.nW, LabelMeanShift, nMaxInlierRegion
//Post : weightMap
//
//*******************************************************/
//void CMultiHomoStitcing_jy::SetWeightMap(int dilation)
//{
//	int x, y, i, j;
//
//	BYTE ** outWeight;
//	int** tmpLabel;
//	int windowSize;
//
//	windowSize = imgR.nW / 10;
//
//	outWeight = cmatrix(imgR.nH, imgR.nW);
//	tmpLabel = imatrix(imgR.nH, imgR.nW);
//	weightObj = cmatrix(imgR.nH, imgR.nW);
//
//	for (i = 0; i < imgR.nH; i++)
//		for (j = 0; j < imgR.nW; j++)
//		{
//			if (LabelMeanShift[i][j] == nMaxInlierRegion)
//				weightObj[i][j] = 255;
//			else
//				weightObj[i][j] = 0;
//		}
//
//	// open
//	ErosionBinary(weightObj, outWeight, imgR.nW, imgR.nH, 0, 10);
//	DilationBinary(outWeight, weightObj, imgR.nW, imgR.nH, 0, 10);
//
//	// Labeling
//	Labeling(weightObj, tmpLabel, imgR.nW, imgR.nH, 1000);
//
//	for (y = 0; y < imgR.nH; y++)
//		for (x = 0; x < imgR.nW; x++)
//		{
//			if (tmpLabel[y][x] == 0)
//			{
//				weightMap[y][x] = 255;
//			}
//			else
//				weightMap[y][x] = 0;
//		}
//
//
//	//Fill Algorithm
//	Fill(weightObj, imgR.nW, imgR.nH);
//
//
//	// dilation
//	DilationBinary(weightObj, outWeight, imgR.nW, imgR.nH, 0, dilation);
//
//	for (y = 0; y < imgR.nH; y++)
//		for (x = 0; x < imgR.nW; x++)
//		{
//			weightMap[y][x] = outWeight[y][x];
//		}
//
//	DisplayCimage2D(weightObj, imgR.nW, imgR.nH, 0, 0);
//}
//
///****************************************************
//<DrawSegmentImage>
//
//Region's Label, Feature's number in Region, Inlier's Rate 를 표시 해준다.
//
//------------------------------------
//Pre : nNumSeg, LabelMeanSift, arrRegionFeaturepoint, regionIdx, segMarkR, nNumPoint, GodPointR, nMaxInlierRegion
//Post :
//
//*******************************************************/
//
//void CMultiHomoStitcing_jy::DrawSegmentImage(bool bOnOnlyRegion)
//{
//	int i, j, x, y;
//
//	CString strNum;
//	CString strInlier;
//	CString strNumPoint;
//
//	BYTE** outputDissimular;
//	int** arrColor;
//	int tmp;
//	int markColor[3];
//	bool bBigMark;
//	int charColor;
//	int nHView;
//
//	nHView = nNumHomoRegion * 15;
//
//	outputDissimular = cmatrix(nHView, 200);
//	arrColor = imatrix(nNumSeg, 3);
//
//	GetRandColorArray(arrColor, nNumSeg);
//	//----------------------------------------------
//
//	for (y = 0; y < nHView; y++)
//		for (x = 0; x < 200; x++)
//		{
//			outputDissimular[y][x] = 0;
//		}
//
//	for (y = 0; y < imgR.nH; y++)
//		for (x = 0; x < imgR.nW; x++)
//		{
//			tmp = LabelMeanShift[y][x];
//			segmentRed[y][x] = arrColor[tmp][2];
//			segmentGreen[y][x] = arrColor[tmp][1];
//			segmentBlue[y][x] = arrColor[tmp][0];
//		}
//
//	// Label, Feature's number in region, Inlier Rate 계산
//
//	for (i = 0; i < nNumHomoRegion; i++)
//	{
//		strNum.Format(_T("%d"), regionIdx[i]);
//		strNumPoint.Format(_T("%d"), arrRegionFeaturepoint[regionIdx[i]]);
//		strInlier.Format(_T("%f"), arrInlierRate[i]);
//
//		if (regionIdx[i] == nMaxInlierRegion)
//			charColor = 255;
//		else
//			charColor = 150;
//
//		DrawTextOnImag2DGray(outputDissimular, 200, nHView, 0, i * 15, strNum, DT_LEFT, charColor, false);
//		DrawTextOnImag2DGray(outputDissimular, 200, nHView, 40, i * 15, strNumPoint, DT_LEFT, charColor, false);
//		DrawTextOnImag2DGray(outputDissimular, 200, nHView, 100, i * 15, strInlier, DT_LEFT, charColor, false);
//	}
//
//	if (bOnOnlyRegion == true)
//	{
//		for (y = 0; y < imgR.nH; y++)
//			for (x = 0; x < imgR.nW; x++)
//			{	
//				tmp = LabelMeanShift[y][x];
//
//				if (tmp == nMaxInlierRegion)
//				{
//					segmentRed[y][x] = arrColor[tmp][0];
//					segmentGreen[y][x] = arrColor[tmp][1];
//					segmentBlue[y][x] = arrColor[tmp][2];
//				}
//				else
//				{
//					segmentRed[y][x] = 255;
//					segmentGreen[y][x] = 255;
//					segmentBlue[y][x] = 255;
//				}
//			}
//	}
//
//	if (bOnOnlyRegion == false)
//	{
//		for (y = 0; y < imgR.nH; y++)
//			for (x = 0; x < imgR.nW; x++)
//			{
//				tmp = LabelMeanShift[y][x];
//
//				segmentRed[y][x] = arrColor[tmp][2];
//				segmentGreen[y][x] = arrColor[tmp][1];
//				segmentBlue[y][x] = arrColor[tmp][0];
//			}
//	}
//
//	tmp = 0;
//	
//	// Region's Mark 표시
//	for (i = 0; i < nNumHomoRegion; i++)
//	{
//		strNum.Format(_T("%d"), regionIdx[i]);
//	
//		// 크기 설정
//		if (regionIdx[i] == nMaxInlierRegion)
//			bBigMark = true;
//		else
//			bBigMark = false;
//
//		// 글자 체크
//		DrawTextOnImag2DGray(segmentRed, imgR.nW, imgR.nH, segMarkR[i].x, segMarkR[i].y, strNum, DT_LEFT, 255, bBigMark);
//		DrawTextOnImag2DGray(segmentGreen, imgR.nW, imgR.nH, segMarkR[i].x, segMarkR[i].y, strNum, DT_LEFT, 0, bBigMark);
//		DrawTextOnImag2DGray(segmentBlue, imgR.nW, imgR.nH, segMarkR[i].x, segMarkR[i].y, strNum, DT_LEFT, 0, bBigMark);
//	}
//
//	// Feature point
//	for (int i = 0; i < nNumPoint; i++)
//	{
//		DrawCross(segmentRed, imgR.nW, imgR.nH, CPoint(GodPointR[i].x, GodPointR[i].y), 0);
//		DrawCross(segmentGreen, imgR.nW, imgR.nH, CPoint(GodPointR[i].x, GodPointR[i].y), 0);
//		DrawCross(segmentBlue, imgR.nW, imgR.nH, CPoint(GodPointR[i].x, GodPointR[i].y), 0);
//	}
//
//	DisplayCimage2D(outputDissimular, 200, nHView, imgR.nW, 0);
//	DisplayCimage2DColor(segmentRed, segmentGreen, segmentBlue, imgR.nW, imgR.nH, 0, 0);
//}
///****************************************************
//<MeshWarpForSeg>
//
//object Homography와 background Homography를 사용하여 이미지를 합성을 해준다.
//그 결과를 출력한다.
//
//------------------------------------
//Pre : imgR.nH, imgR.nW, imgL.nH, imgL.nW, LabelMeanShift, objHomo, backHomo
//Post : weightSeamR, weightSeamL, ImageLtrans_R, ImageRtrans_R, weightRtrans
//								 
//*******************************************************/
//void CMultiHomoStitcing_jy::MeshWarpForSeg()
//{
//	int i, j, x, y;
//	BYTE** outputImageRed, ** outputImageGreen, ** outputImageBlue;
//
//	// value for meshed warp
//	int nNumVert;	// 전체 제어점의 갯수
//	int nNumVertH, nNumVertW;	// 가로, 세로의 제어점 갯수
//	int nRestVertH, nRestVertW;	// 남은 제어점의 갯수
//
//	CMesh* arrMesh;	// 각 메쉬 class array
//
//	// value for get Image size
//	int offsetX = 0, offsetY = 0;
//	int maxX_totalR = 0, maxY_totalR = 0, minX_totalR = imgR.nW, minY_totalR = imgR.nH;
//	double maxX_transR = 0, maxY_transR = 0, minX_transR = imgR.nW, minY_transR = imgR.nH;
//	int nH_total, nW_total;
//	Point2f inputPoint, transPoint;
//
//	//Set weightmap for Seam
//	double** weightSeamR, ** weightSeamL;
//
//	weightSeamR = dmatrix(imgR.nH, imgR.nW);
//	weightSeamL = dmatrix(imgL.nH, imgL.nW);
//
//	MakeWeightSeam(weightSeamR, imgR.nH, imgR.nW);
//	MakeWeightSeam(weightSeamL, imgL.nH, imgL.nW);
//
//	// get image's size
//	for (int y = 0; y < imgR.nH; y++)
//		for (int x = 0; x < imgR.nW; x++)
//		{
//			inputPoint = cvPoint(x, y);
//
//			GetPerspectivePoint(inputPoint, transPoint, backHomo);
//
//			if (transPoint.y > maxY_transR) maxY_transR = transPoint.y;
//			if (transPoint.x > maxX_transR) maxX_transR = transPoint.x;
//			if (transPoint.y < minY_transR) minY_transR = transPoint.y;
//			if (transPoint.x < minX_transR) minX_transR = transPoint.x;
//		}
//
//	if (minY_transR < 0) minY_totalR = minY_transR; else minY_totalR = 0;
//	if (maxY_transR > imgR.nH - 1) maxY_totalR = maxY_transR; else maxY_totalR = imgR.nH - 1;
//
//	if (minX_transR < 0) minX_totalR = minX_transR; else minX_totalR = 0;
//	if (maxX_transR > imgR.nW - 1) maxX_totalR = maxX_transR; else maxX_totalR = imgR.nW - 1;
//
//	nH_total = maxY_totalR - minY_totalR + 1;
//	nW_total = maxX_totalR - minX_totalR + 1;
//
//	if (minY_transR < 0) offsetY = abs(minY_transR);
//	offsetX = minX_transR;
//
//	//Set total Image's nH, nW
//
//	outputImageRed = cmatrix(nH_total, nW_total); outputImageGreen = cmatrix(nH_total, nW_total); outputImageBlue = cmatrix(nH_total, nW_total);
//	ImageLtrans_R = cmatrix(nH_total, nW_total); ImageLtrans_G = cmatrix(nH_total, nW_total); ImageLtrans_B = cmatrix(nH_total, nW_total);
//	ImageRtrans_R = cmatrix(nH_total, nW_total); ImageRtrans_G = cmatrix(nH_total, nW_total); ImageRtrans_B = cmatrix(nH_total, nW_total);
//	weightRtrans = dmatrix(nH_total, nW_total); weightLtrans = dmatrix(nH_total, nW_total);
//
//	//WarpSeamWeight
//
//	for (int y = 0; y < nH_total; y++)
//		for (int x = 0; x < nW_total; x++)
//		{
//			outputImageRed[y][x] = 0;; outputImageGreen[y][x] = 0; outputImageBlue[y][x] = 0;
//			ImageLtrans_R[y][x] = 0; ImageLtrans_G[y][x] = 0; ImageLtrans_B[y][x] = 0;
//			ImageRtrans_R[y][x] = 0; ImageRtrans_G[y][x] = 0; ImageRtrans_B[y][x] = 0;
//			weightRtrans[y][x] = 0; weightLtrans[y][x] = 0;
//		}
//
//	for (int y = 0; y < imgL.nH; y++)
//		for (int x = 0; x < imgL.nW; x++)
//		{
//			
//			/*weightLtrans[y + offsetY][x] = weightSeamL[y][x];
//			ImageLtrans_R[y + offsetY][x] = imgL.imgRed[y][x];
//			ImageLtrans_G[y + offsetY][x] = imgL.imgGreen[y][x];
//			ImageLtrans_B[y + offsetY][x] = imgL.imgBlue[y][x];*/
//			
//			weightLtrans[y + offsetY][x] = weightSeamL[y][x];
//			outputImageRed[y + offsetY][x] = imgL.imgRed[y][x];
//			outputImageGreen[y + offsetY][x] = imgL.imgGreen[y][x];
//			outputImageBlue[y + offsetY][x] = imgL.imgBlue[y][x];
//			
//		}
//
//	//--------------------------------------------------------------
//	// mesh warp
//
//	nRestVertH = (imgR.nH-1) % 10;
//	nRestVertW = (imgR.nW-1) % 10;
//
//	if (nRestVertH == 0)
//		nNumVertH = (imgR.nH-1) / 10;
//	else
//		nNumVertH = (imgR.nH-1) / 10 + 1;
//
//	if (nRestVertW == 0)
//		nNumVertW = (imgR.nW - 1) / 10;
//	else
//		nNumVertW = (imgR.nW - 1) / 10 + 1;
//
//	nNumVert = nNumVertH * nNumVertW;
//
//	arrMesh = new CMesh[nNumVert];
//
//	// Set vertice
//	for (i = 0; i < nNumVertH - 1; i++)
//		for (j = 0; j < nNumVertW - 1; j++)
//		{
//			arrMesh[i * nNumVertW + j] = CMesh(10 * i, 10 * j, 10);
//		}
//
//	for (i = 0; i < nNumVertH - 1; i++)
//	{
//		if (nRestVertW == 0)
//		{
//			nRestVertW = 10;
//		}
//
//		arrMesh[i * nNumVertW + nNumVertW - 1] = CMesh(10 * i, 10 * (nNumVertW - 1), 10, nRestVertW);
//	}
//
//	for (i = 0; i < nNumVertW - 1; i++)
//	{
//		if (nRestVertH == 0)
//		{
//			nRestVertH = 10;
//		}
//
//		arrMesh[(nNumVertH - 1)* nNumVertW + i] = CMesh(10 * (nNumVertH - 1), 10 * i, nRestVertH, 10);
//	}
//
//	arrMesh[(nNumVertH - 1)* nNumVertW + nNumVertW - 1] = CMesh(10 * (nNumVertH - 1), 10 * (nNumVertW - 1), nRestVertH, nRestVertW);
//
//	// Set deformed vertice
//	int nTmpX, nTmpY;
//
//	for (i = 0; i < nNumVert; i++)
//	{	
//		arrMesh[i].DeformMesh(false, homography_all);
//	}
//
//	//inverse mesh warp
//	for (i = 0; i < nNumVert; i++)
//	{
//		// inverse warping
//		for (y = arrMesh[i].nMinY; y <= arrMesh[i].nMaxY; y++)
//			for (x = arrMesh[i].nMinX; x <= arrMesh[i].nMaxX; x++) {
//
//				inputPoint = cvPoint(x, y);
//
//				Mat inverH = arrMesh[i].Homo.inv();
//				GetPerspectivePoint(inputPoint, transPoint, inverH);
//
//				if (transPoint.x >= arrMesh[i].vertex[0].x
//					&& transPoint.x <= arrMesh[i].vertex[3].x
//					&& transPoint.y >= arrMesh[i].vertex[0].y
//					&& transPoint.y <= arrMesh[i].vertex[3].y)
//				{
//					
//					/*ImageRtrans_R[y + offsetY][x] = imgR.imgRed[int(transPoint.y)][int(transPoint.x)];
//					ImageRtrans_G[y + offsetY][x] = imgR.imgGreen[int(transPoint.y)][int(transPoint.x)];
//					ImageRtrans_B[y + offsetY][x] = imgR.imgBlue[int(transPoint.y)][int(transPoint.x)];
//					weightRtrans[y + offsetY][x] = weightSeamR[int(transPoint.y)][int(transPoint.x)];*/
//					weightRtrans[y + offsetY][x] = weightSeamR[int(transPoint.y)][int(transPoint.x)];
//					outputImageRed[y + offsetY][x] = BilinearInterpolation(transPoint, imgR.imgRed, imgR.nH, imgR.nW);
//					outputImageGreen[y + offsetY][x] = BilinearInterpolation(transPoint, imgR.imgGreen, imgR.nH, imgR.nW);
//					outputImageBlue[y + offsetY][x] = BilinearInterpolation(transPoint, imgR.imgBlue, imgR.nH, imgR.nW);
//				}
//			}
//	}
//
//	for (y = 0; y < nH_total; y++)
//		for (x = 0; x < nW_total; x++)
//		{
//			if (weightRtrans[y][x] > weightLtrans[y][x])
//			{
//				weightRtrans[y][x] = 1;
//				weightLtrans[y][x] = 0;
//			}
//
//			if (weightLtrans[y][x] > weightRtrans[y][x])
//			{
//				weightLtrans[y][x] = 1;
//				weightRtrans[y][x] = 0;
//			}
//		}
//
//	DisplayCimage2DColor(outputImageRed, outputImageGreen, outputImageBlue, nW_total, nH_total, 0, 0);
//	//DisplayCimage2DColor(ImageRtrans_R, ImageRtrans_G, ImageRtrans_B, nW_total, nH_total, 0, 0);
//	//DisplayCimage2DColor(ImageLtrans_R, ImageLtrans_G, ImageLtrans_B, nW_total, nH_total, 0, 0);
//
//	free_cmatrix(outputImageRed, nH_total, nW_total);
//	free_cmatrix(outputImageGreen, nH_total, nW_total);
//	free_cmatrix(outputImageBlue, nH_total, nW_total);
//}
//
///****************************************************
//<WarpTest>
//
//여러종류의 Warping을 수행한다.
//
//적용결과 
//
//-물체와 배경에 각각의 호모그래피를 적용한 단일 이미지.
//-물체에는 배경에 각각 호모그래피를 적용하여 하나의 이미지에 표현
//------------------------------------
//Pre : 
//Post : 
//
//*******************************************************/
//void CMultiHomoStitcing_jy::WarpTest()
//{
//	int x, y;
//	BYTE** outputImageRed, ** outputImageGreen, ** outputImageBlue;
//	BYTE** backImageRed, ** backImageGreen, ** backImageBlue;
//
//	
//	// value for get Image size
//	int offsetX = 0, offsetY = 0;
//	int maxX_totalR = 0, maxY_totalR = 0, minX_totalR = imgR.nW, minY_totalR = imgR.nH;
//	double maxX_transR = 0, maxY_transR = 0, minX_transR = imgR.nW, minY_transR = imgR.nH;
//	int nH_total, nW_total;
//	Point2f inputPoint, transPoint;
//
//	//Set weightmap for Seam
//	double** weightSeamBackR, ** weightSeamBackL;
//
//	weightSeamBackR = dmatrix(imgR.nH, imgR.nW);
//	weightSeamBackL = dmatrix(imgL.nH, imgL.nW);
//
//	MakeWeightSeam(weightSeamBackR, imgR.nH, imgR.nW);
//	MakeWeightSeam(weightSeamBackL, imgL.nH, imgL.nW);
//
//	// get image's size
//	for (int y = 0; y < imgR.nH; y++)
//		for (int x = 0; x < imgR.nW; x++)
//		{
//			inputPoint = cvPoint(x, y);
//
//			GetPerspectivePoint(inputPoint, transPoint, backHomo);
//
//			if (transPoint.y > maxY_transR) maxY_transR = transPoint.y;
//			if (transPoint.x > maxX_transR) maxX_transR = transPoint.x;
//			if (transPoint.y < minY_transR) minY_transR = transPoint.y;
//			if (transPoint.x < minX_transR) minX_transR = transPoint.x;
//		}
//
//	if (minY_transR < 0) minY_totalR = minY_transR; else minY_totalR = 0;
//	if (maxY_transR > imgR.nH - 1) maxY_totalR = maxY_transR; else maxY_totalR = imgR.nH - 1;
//
//	if (minX_transR < 0) minX_totalR = minX_transR; else minX_totalR = 0;
//	if (maxX_transR > imgR.nW - 1) maxX_totalR = maxX_transR; else maxX_totalR = imgR.nW - 1;
//
//	nH_total = maxY_totalR - minY_totalR + 1;
//	nW_total = maxX_totalR - minX_totalR + 1;
//
//	if (minY_transR < 0) offsetY = abs(minY_transR);
//	if (minX_totalR < 0) offsetX = abs(minX_totalR);
//
//	imgTotal.nH = nH_total;
//	imgTotal.nW = nW_total;
//
//	//Set total Image's nH, nW
//	outputImageRed = cmatrix(nH_total, nW_total); outputImageGreen = cmatrix(nH_total, nW_total); outputImageBlue = cmatrix(nH_total, nW_total);
//	backImageRed = cmatrix(nH_total, nW_total); backImageGreen = cmatrix(nH_total, nW_total); backImageBlue = cmatrix(nH_total, nW_total);
//	ImageLtrans_R = cmatrix(nH_total, nW_total); ImageLtrans_G = cmatrix(nH_total, nW_total); ImageLtrans_B = cmatrix(nH_total, nW_total);
//	ImageRtrans_R = cmatrix(nH_total, nW_total); ImageRtrans_G = cmatrix(nH_total, nW_total); ImageRtrans_B = cmatrix(nH_total, nW_total);
//	weightLtrans = dmatrix(nH_total, nW_total); weightRtrans = dmatrix(nH_total, nW_total);
//
//	for (int y = 0; y < nH_total; y++)
//		for (int x = 0; x < nW_total; x++)
//		{
//			outputImageRed[y][x] = 0; outputImageGreen[y][x] = 0; outputImageBlue[y][x] = 0;
//			backImageRed[y][x] = 0; backImageGreen[y][x] = 0; backImageBlue[y][x] = 0;
//			
//			ImageLtrans_R[y][x] = 0; ImageLtrans_G[y][x] = 0; ImageLtrans_B[y][x] = 0;
//			ImageRtrans_R[y][x] = 0; ImageRtrans_G[y][x] = 0; ImageRtrans_B[y][x] = 0;
//
//			weightLtrans[y][x] = 0; weightRtrans[y][x] = 0;
//		}
//
//	// 배경이미지 합성.
//	for (int y = 0; y < imgL.nH; y++)
//		for (int x = 0; x < imgL.nW; x++)
//		{
//			ImageLtrans_R[y + offsetY][x + offsetX] = imgL.imgRed[y][x];
//			ImageLtrans_G[y + offsetY][x + offsetX] = imgL.imgGreen[y][x];
//			ImageLtrans_B[y + offsetY][x + offsetX] = imgL.imgBlue[y][x];
//
//			weightLtrans[y + offsetY][x + offsetX] = weightSeamBackL[y][x];
//		}
//	
//	Mat inverBackH = backHomo.inv();
//
//	for (int y = minY_transR; y < maxY_transR; y++)
//		for (int x = minX_transR; x < maxX_transR; x++) {
//
//			inputPoint = cvPoint(x, y);
//			GetPerspectivePoint(inputPoint, transPoint, inverBackH);
//
//			if (transPoint.x >= 0 && transPoint.x < imgR.nW && transPoint.y >= 0 && transPoint.y < imgR.nH)
//			{
//				ImageRtrans_R[y + offsetY][x + offsetX] = BilinearInterpolation(transPoint, imgR.imgRed, imgR.nH, imgR.nW);
//				ImageRtrans_G[y + offsetY][x + offsetX] = BilinearInterpolation(transPoint, imgR.imgGreen, imgR.nH, imgR.nW);
//				ImageRtrans_B[y + offsetY][x + offsetX] = BilinearInterpolation(transPoint, imgR.imgBlue, imgR.nH, imgR.nW);
//
//				if (weightMap[int(transPoint.y)][int(transPoint.x)] == 0){
//				weightRtrans[y + offsetY][x + offsetX] = weightSeamBackR[int(transPoint.y)][int(transPoint.x)];
//				}
//			}
//		}
//
//	for (y = 0; y < nH_total; y++)
//		for (x = 0; x < nW_total; x++)
//		{
//			if (weightRtrans[y][x] > weightLtrans[y][x])
//			{
//				weightRtrans[y][x] = 1;
//				weightLtrans[y][x] = 0;
//			}
//
//			if (weightLtrans[y][x] > weightRtrans[y][x])
//			{
//				weightRtrans[y][x] = 0;
//				weightLtrans[y][x] = 1;
//			}
//		}
//
//	this->MultiBandBlending(nH_total, nW_total, backImageRed, backImageGreen, backImageBlue);
//	DisplayCimage2DColor(backImageRed, backImageGreen, backImageBlue, nW_total, nH_total, 0, 0);
//	
//	//------------------------------------------------------------------------
//	// 물체이미지 합성.
//	// transL : 배경이미지, transR : 물체이미지
//
//	Mat inverObjH = objHomo.inv();
//
//	for (int y = 0; y < imgL.nH; y++)
//		for (int x = 0; x < imgL.nW; x++)
//		{
//			weightLtrans[y + offsetY][x + offsetX] = 1;
//		}
//
//	for (int y = minY_transR; y < maxY_transR; y++)
//		for (int x = minX_transR; x < maxX_transR; x++) {
//
//			inputPoint = cvPoint(x, y);
//			GetPerspectivePoint(inputPoint, transPoint, inverBackH);
//
//			if (transPoint.x >= 0 && transPoint.x < imgR.nW && transPoint.y >= 0 && transPoint.y < imgR.nH)
//			{
//				if (weightMap[int(transPoint.y)][int(transPoint.x)] == 0)
//				{
//					weightLtrans[y + offsetY][x + offsetX] = 1;
//				}
//			}
//		}
//
//	for (int y = 0; y < nH_total; y++)
//		for (int x = 0; x < nW_total; x++)
//		{
//			ImageLtrans_R[y][x] = backImageRed[y][x];
//			ImageLtrans_G[y][x] = backImageGreen[y][x];
//			ImageLtrans_B[y][x] = backImageBlue[y][x];
//			ImageRtrans_R[y][x] = 0;
//			ImageRtrans_G[y][x] = 0;
//			ImageRtrans_B[y][x] = 0;
//
//			weightRtrans[y][x] = 0;
//		}
//
//	// 물체 이미지 합성
//
//	for (int y = minY_totalR; y < maxY_totalR; y++)
//		for (int x = minX_totalR; x < maxX_totalR; x++) {
//
//			inputPoint = cvPoint(x, y);
//			GetPerspectivePoint(inputPoint, transPoint, inverObjH);
//
//			if (transPoint.x >= 0 && transPoint.x < imgR.nW && transPoint.y >= 0 && transPoint.y < imgR.nH)
//			{
//				
//				ImageRtrans_R[y + offsetY][x + offsetX] = BilinearInterpolation(transPoint, imgR.imgRed, imgR.nH, imgR.nW);
//				ImageRtrans_G[y + offsetY][x + offsetX] = BilinearInterpolation(transPoint, imgR.imgGreen, imgR.nH, imgR.nW);
//				ImageRtrans_B[y + offsetY][x + offsetX] = BilinearInterpolation(transPoint, imgR.imgBlue, imgR.nH, imgR.nW);
//
//				if (weightMap[int(transPoint.y)][int(transPoint.x)] == 255)
//				{
//					weightRtrans[y + offsetY][x + offsetX] = 1;
//					weightLtrans[y + offsetY][x + offsetX] = 0;
//				}
//			}
//		}
//
//	this->MultiBandBlending(nH_total, nW_total, outputImageRed, outputImageGreen, outputImageBlue);
//
//	DisplayCimage2DColor(outputImageRed, outputImageGreen, outputImageBlue, nW_total, nH_total, 0, 0);
//
//	free_cmatrix(outputImageRed, nH_total, nW_total);
//	free_cmatrix(outputImageGreen, nH_total, nW_total);
//	free_cmatrix(outputImageBlue, nH_total, nW_total);
//	
//}
//
//void CMultiHomoStitcing_jy::WarpSingleHomo()
//{
//	int i, j, x, y;
//
//	BYTE** outputImageRed, ** outputImageGreen, ** outputImageBlue;
//	BYTE** backImageRed, ** backImageGreen, ** backImageBlue;
//
//	// value for get Image size
//	int offsetX = 0, offsetY = 0;
//	int maxX_totalR = 0, maxY_totalR = 0, minX_totalR = imgR.nW, minY_totalR = imgR.nH;
//	double maxX_transR = 0, maxY_transR = 0, minX_transR = imgR.nW, minY_transR = imgR.nH;
//	int nH_total, nW_total;
//	Point2f inputPoint, transPoint;
//
//	//Set weightmap for Seam
//	double** weightSeamR, ** weightSeamL;
//
//	weightSeamR = dmatrix(imgR.nH, imgR.nW);
//	weightSeamL = dmatrix(imgL.nH, imgL.nW);
//
//	MakeWeightSeam(weightSeamR, imgR.nH, imgR.nW);
//	MakeWeightSeam(weightSeamL, imgL.nH, imgL.nW);
//
//	// get image's size
//	for (int y = 0; y < imgR.nH; y++)
//		for (int x = 0; x < imgR.nW; x++)
//		{
//			inputPoint = cvPoint(x, y);
//
//			GetPerspectivePoint(inputPoint, transPoint, backHomo);
//
//			if (transPoint.y > maxY_transR) maxY_transR = transPoint.y;
//			if (transPoint.x > maxX_transR) maxX_transR = transPoint.x;
//			if (transPoint.y < minY_transR) minY_transR = transPoint.y;
//			if (transPoint.x < minX_transR) minX_transR = transPoint.x;
//		}
//
//	if (minY_transR < 0) minY_totalR = minY_transR; else minY_totalR = 0;
//	if (maxY_transR > imgR.nH - 1) maxY_totalR = maxY_transR; else maxY_totalR = imgR.nH - 1;
//
//	if (minX_transR < 0) minX_totalR = minX_transR; else minX_totalR = 0;
//	if (maxX_transR > imgR.nW - 1) maxX_totalR = maxX_transR; else maxX_totalR = imgR.nW - 1;
//
//	nH_total = maxY_totalR - minY_totalR + 1;
//	nW_total = maxX_totalR - minX_totalR + 1;
//
//	if (minY_transR < 0) offsetY = abs(minY_transR);
//	if (minX_totalR < 0) offsetX = abs(minX_totalR);
//
//	outputImageRed = cmatrix(nH_total, nW_total); outputImageGreen = cmatrix(nH_total, nW_total); outputImageBlue = cmatrix(nH_total, nW_total);
//	ImageLtrans_R = cmatrix(nH_total, nW_total); ImageLtrans_G = cmatrix(nH_total, nW_total); ImageLtrans_B = cmatrix(nH_total, nW_total);
//	ImageRtrans_R = cmatrix(nH_total, nW_total); ImageRtrans_G = cmatrix(nH_total, nW_total); ImageRtrans_B = cmatrix(nH_total, nW_total);
//	weightRtrans = dmatrix(nH_total, nW_total); weightLtrans = dmatrix(nH_total, nW_total);
//
//	//WarpSeamWeight
//	for (y = 0; y < nH_total; y++)
//		for (int x = 0; x < nW_total; x++)
//		{
//			weightRtrans[y][x] = 0;
//			weightLtrans[y][x] = 0;
//
//			ImageLtrans_R[y][x] = 0; ImageLtrans_G[y][x] = 0; ImageLtrans_B[y][x] = 0;
//			ImageRtrans_R[y][x] = 0; ImageRtrans_G[y][x] = 0; ImageRtrans_B[y][x] = 0;
//		}
//
//	for (int y = 0; y < imgL.nH; y++)
//		for (int x = 0; x < imgL.nW; x++)
//		{
//			weightLtrans[y + offsetY][x + offsetX] = weightSeamL[y][x];
//			ImageLtrans_R[y + offsetY][x + offsetX] = imgL.imgRed[y][x];
//			ImageLtrans_G[y + offsetY][x + offsetX] = imgL.imgGreen[y][x];
//			ImageLtrans_B[y + offsetY][x + offsetX] = imgL.imgBlue[y][x];
//		}
//
//	Mat inverH = gloHomo.inv();
//
//	for (int y = minY_transR; y < maxY_transR; y++)
//		for (int x = minX_transR; x < maxX_transR; x++) {
//			inputPoint = cvPoint(x, y);
//			GetPerspectivePoint(inputPoint, transPoint, inverH);
//			if (transPoint.x >= 0 && transPoint.x < imgR.nW && transPoint.y >= 0 && transPoint.y < imgR.nH)
//			{
//				ImageRtrans_R[y + offsetY][x + offsetX] = BilinearInterpolation(transPoint, imgR.imgRed, imgR.nH, imgR.nW);
//				ImageRtrans_G[y + offsetY][x + offsetX] = BilinearInterpolation(transPoint, imgR.imgGreen, imgR.nH, imgR.nW);
//				ImageRtrans_B[y + offsetY][x + offsetX] = BilinearInterpolation(transPoint, imgR.imgBlue, imgR.nH, imgR.nW);
//				weightRtrans[y + offsetY][x + offsetX] = weightSeamR[int(transPoint.y)][int(transPoint.x)];	
//			}
//		}
//
//	//Seam에 따라 그림 넣기
//
//	for (y = 0; y < nH_total; y++)
//		for (x = 0; x < nW_total; x++)
//		{
//			if (weightRtrans[y][x] > weightLtrans[y][x])
//			{
//				weightRtrans[y][x] = 1;
//				weightLtrans[y][x] = 0;
//			}
//
//			if (weightLtrans[y][x] > weightRtrans[y][x])
//			{
//				weightRtrans[y][x] = 0;
//				weightLtrans[y][x] = 1;
//			}
//
//		}
//
//	this->MultiBandBlending(nH_total, nW_total, outputImageRed, outputImageGreen, outputImageBlue);
//
//	DisplayCimage2DColor(outputImageRed, outputImageGreen, outputImageBlue, nW_total, nH_total, 0, 0);
//}
//
///****************************************************
//<WarpLayer>
//
//먼저 RANSAC을 이용하여 같은 plane과 같은 depth를 가지는 특정점 그룹을 형성한다.
//영상의 그리드를 나눈 뒤 각 그리드 마다 호모그래피를 생성한다. 
//호모그래피는 그리드 중심과 가장 가까이에 있는 레이어의 포인트의 거리를 이용하여 구한다. 
//그리고 각 그리드의 호모그래피를 이용하여 warping을 수행한다.
//이때 계산을 가속화 시키기 위해 pixel mapping table을 제작한다. 
//
//
//------------------------------------
//Pre : GodPointL, GodPointRR, nNumPoint
//Post : setPointLayer, setHomoLayer
//
//*******************************************************/
//void CMultiHomoStitcing_jy::WarpLayer(int nThresh)
//{
//	int i, j, x, y;
//	vector < Point2f > tmpVecL, tmpVecR;
//	vector <Point2f> inlierL, inlierR;
//	vector <Point2f> outlierL, outlierR;
//	vector <Point2f> emptyVec;
//	Mat H;
//	Point2f input, output, nullPoint(0,0);
//	double dTmp;
//	int nSize;
//
//	for (i = 0; i < nNumPoint; i++) {
//		tmpVecR.push_back(GodPointR[i]);
//		tmpVecL.push_back(GodPointL[i]);
//	}
//
//	nSize = nNumPoint;
//
//	while (nSize > nThresh)
//	{
//		H = findHomography(tmpVecR, tmpVecL, CV_RANSAC);
//
//		for (i = 0; i < nSize; i++)
//		{
//			input = tmpVecR[i];
//			GetPerspectivePoint(input, output, H);
//			dTmp = GetDistance(tmpVecL[i].x, tmpVecL[i].y, output.x, output.y);
//
//			if (dTmp < 3)
//			{
//				inlierL.push_back(tmpVecL[i]);
//				inlierR.push_back(tmpVecR[i]);
//			}
//			else
//			{
//				outlierL.push_back(tmpVecL[i]);
//				outlierR.push_back(tmpVecR[i]);
//			}
//		}
//
//		if (inlierL.size() > nThresh)
//		{
//			SetPointLayerL.push_back(inlierL);
//			SetPointLayerR.push_back(inlierR);
//			setHomoLayer.push_back(H);
//		}
//
//		tmpVecR.clear();
//		tmpVecL.clear();
//
//		nSize = outlierL.size();
//
//		for (i = 0; i < nSize; i++){
//			tmpVecL.push_back(outlierL[i]);
//			tmpVecR.push_back(outlierR[i]);
//		}
//
//		inlierR.clear();
//		inlierL.clear();
//		outlierR.clear();
//		outlierL.clear();
//
//		nSize = tmpVecR.size();
//	}
//
//	//-----------------------------------------
//	//-------------<Layer 확인>---------------
//	int ** arrColor;
//	int nNumPoint;
//	nSize = SetPointLayerL.size();
//	arrColor = imatrix(nSize,3);
//
//	GetRandColorArray(arrColor, nSize);
//
//	for (i = 0; i < nSize; i++)
//	{
//		nNumPoint = SetPointLayerR[i].size();
//		for (j = 0; j < nNumPoint; j++)
//		{
//			y = SetPointLayerR[i].at(j).y;
//			x = SetPointLayerR[i].at(j).x;
//			DrawCross(imgMark.imgRed, imgR.nW, imgR.nH, CPoint(x,y), arrColor[i][0]);
//			DrawCross(imgMark.imgGreen, imgR.nW, imgR.nH, CPoint(x, y), arrColor[i][1]);
//			DrawCross(imgMark.imgBlue, imgR.nW, imgR.nH, CPoint(x, y), arrColor[i][2]);
//		}
//	}
//
//	DisplayCimage2DColor(imgMark.imgRed, imgMark.imgGreen, imgMark.imgBlue, imgR.nW, imgR.nH, 0, 0);
//
//	//-----------<Homography Blending>----------------------------
//	//10x10 Mesh
//	MeshWarpHomographyForLayer();
//}
//
//
//void CMultiHomoStitcing_jy::MeshWarpHomographyForLayer(bool bOnGrid)
//{
//	int i, j, x, y;
//	BYTE** outputImageRed, ** outputImageGreen, ** outputImageBlue;
//
//	// value for meshed warp
//	int nNumVert, nNumVertH, nNumVertW;
//	int nRestVertH, nRestVertW;
//
//	CMesh* arrMesh;
//
//	// value for get Image size
//	int offsetX = 0, offsetY = 0;
//	int maxX_totalR = 0, maxY_totalR = 0, minX_totalR = imgR.nW, minY_totalR = imgR.nH;
//	double maxX_transR = 0, maxY_transR = 0, minX_transR = imgR.nW, minY_transR = imgR.nH;
//	int nH_total, nW_total;
//	Point2f inputPoint, transPoint;
//
//	//Set weightmap for Seam
//	double** weightSeamR, ** weightSeamL;
//
//	weightSeamR = dmatrix(imgR.nH, imgR.nW);
//	weightSeamL = dmatrix(imgL.nH, imgL.nW);
//
//	MakeWeightSeam(weightSeamR, imgR.nH, imgR.nW);
//	MakeWeightSeam(weightSeamL, imgL.nH, imgL.nW);
//
//	//--------------------------------------------------------------
//	// mesh warp
//
//	nRestVertH = (imgR.nH - 1) % 10;
//	nRestVertW = (imgR.nW - 1) % 10;
//
//	if (nRestVertH == 0)
//		nNumVertH = (imgR.nH - 1) / 10;
//	else
//		nNumVertH = (imgR.nH - 1) / 10 + 1;
//
//	if (nRestVertW == 0)
//		nNumVertW = (imgR.nW - 1) / 10;
//	else
//		nNumVertW = (imgR.nW - 1) / 10 + 1;
//
//	nNumVert = nNumVertH * nNumVertW;
//
//	arrMesh = new CMesh[nNumVert];
//
//	// Set vertice
//
//	for (i = 0; i < nNumVertH - 1; i++)
//		for (j = 0; j < nNumVertW - 1; j++)
//		{
//			arrMesh[i * nNumVertW + j] = CMesh(10 * i, 10 * j, 10);
//		}
//
//	for (i = 0; i < nNumVertH - 1; i++)
//	{
//		if (nRestVertW == 0)
//		{
//			nRestVertW = 10;
//		}
//
//		arrMesh[i * nNumVertW + nNumVertW - 1] = CMesh(10 * i, 10 * (nNumVertW - 1), 10, nRestVertW);
//	}
//
//	for (i = 0; i < nNumVertW - 1; i++)
//	{
//		if (nRestVertH == 0)
//		{
//			nRestVertH = 10;
//		}
//
//		arrMesh[(nNumVertH - 1)* nNumVertW + i] = CMesh(10 * (nNumVertH - 1), 10 * i, nRestVertH, 10);
//	}
//
//	arrMesh[(nNumVertH - 1)* nNumVertW + nNumVertW - 1] = CMesh(10 * (nNumVertH - 1), 10 * (nNumVertW - 1), nRestVertH, nRestVertW);
//
//	//Blending homography.
//	int nNumLayer = SetPointLayerR.size();
//	int* nNumSetPoint = new int[nNumLayer];
//	double* dNearDis = new double[nNumLayer];
//	double* dWeight = new double[nNumLayer];
//	double tmp, alpa, weightSum = 0;
//	Point2f tmpPoint, tmpCen;
//	double sigma = 20;
//	Mat tmpH;
//
//	for (i = 0; i < nNumLayer; i++)
//	{
//		nNumSetPoint[i] = SetPointLayerR[i].size();
//		dNearDis[i] = 999;
//		dWeight[i] = 0;
//	}
//
//
//	for (i = 0; i < nNumVert; i++)
//	{
//		tmpH = Mat(cvSize(3, 3), CV_64FC1, cvScalar(0));
//
//		tmpCen = arrMesh[i].central;
//
//		//각 Layer별 최소거리 구하기.
//		for (j = 0; j < nNumLayer; j++)
//		{
//			for (int k = 0; k < nNumSetPoint[j]; k++)
//			{
//				tmpPoint.x = SetPointLayerR[j].at(k).x;
//				tmpPoint.y = SetPointLayerR[j].at(k).y;
//
//				tmp = GetDistance(tmpCen.x, tmpCen.y, tmpPoint.x, tmpPoint.y);
//
//				if (tmp < dNearDis[j])
//				{
//					dNearDis[j] = tmp;
//				}
//			}
//
//			//Get Weight
//			tmp = -1 * dNearDis[j] * dNearDis[j] / (sigma * sigma);
//			dWeight[j] = exp(tmp);
//			weightSum += dWeight[j];
//		}
//
//		//Blending 
//		for (j = 0; j < nNumLayer; j++)
//		{
//			dWeight[j] /= weightSum;
//			for (int a = 0; a < 3; a++)
//				for (int b = 0; b < 3; b++)
//				{
//					tmpH.at<double>(a, b) += (dWeight[j] * setHomoLayer[j].at<double>(a, b));
//				}
//		}
//
//		for (y = arrMesh[i].vertex[0].y; y <= arrMesh[i].vertex[3].y; y++)
//			for (x = arrMesh[i].vertex[0].x; x <= arrMesh[i].vertex[3].x; x++)
//			{
//				homography_all[y][x] = tmpH;
//			}
//
//		for (j = 0; j < nNumLayer; j++)
//		{
//			dNearDis[j] = 999;
//		}
//		weightSum = 0;
//	}
//
//	// get image's size
//	for (int y = 0; y < imgR.nH; y++)
//		for (int x = 0; x < imgR.nW; x++)
//		{
//			inputPoint = Point2f(x, y);
//			GetPerspectivePoint(inputPoint, transPoint, homography_all[y][x]);
//
//			if (transPoint.y > maxY_transR) maxY_transR = transPoint.y;
//			if (transPoint.x > maxX_transR) maxX_transR = transPoint.x;
//			if (transPoint.y < minY_transR) minY_transR = transPoint.y;
//			if (transPoint.x < minX_transR) minX_transR = transPoint.x;
//		}
//
//	//Set transformed Right Image
//	if (minY_transR < 0) minY_totalR = minY_transR; else minY_totalR = 0;
//	if (maxY_transR > imgR.nH - 1) maxY_totalR = maxY_transR; else maxY_totalR = imgR.nH - 1;
//
//	if (minX_transR < 0) minX_totalR = minX_transR; else minX_totalR = 0;
//	if (maxX_transR > imgR.nW - 1) maxX_totalR = maxX_transR; else maxX_totalR = imgR.nW - 1;
//
//	nH_total = maxY_totalR - minY_totalR + 1;
//	nW_total = maxX_totalR - minX_totalR + 1;
//
//	if (minY_transR < 0) offsetY = abs(minY_transR);
//	if (minX_transR < 0) offsetX = abs(minX_transR);
//
//	//Set total Image's nH, nW
//
//	outputImageRed = cmatrix(nH_total, nW_total); outputImageGreen = cmatrix(nH_total, nW_total); outputImageBlue = cmatrix(nH_total, nW_total);
//	ImageLtrans_R = cmatrix(nH_total, nW_total); ImageLtrans_G = cmatrix(nH_total, nW_total); ImageLtrans_B = cmatrix(nH_total, nW_total);
//	ImageRtrans_R = cmatrix(nH_total, nW_total); ImageRtrans_G = cmatrix(nH_total, nW_total); ImageRtrans_B = cmatrix(nH_total, nW_total);
//	weightRtrans = dmatrix(nH_total, nW_total); weightLtrans = dmatrix(nH_total, nW_total);
//
//	//WarpSeamWeight
//
//	for (int y = 0; y < nH_total; y++)
//		for (int x = 0; x < nW_total; x++)
//		{
//			ImageLtrans_R[y][x] = 0; ImageLtrans_G[y][x] = 0; ImageLtrans_B[y][x] = 0;
//			ImageRtrans_R[y][x] = 0; ImageRtrans_G[y][x] = 0; ImageRtrans_B[y][x] = 0;
//			weightRtrans[y][x] = 0; weightLtrans[y][x] = 0;
//		}
//
//	for (int y = 0; y < imgL.nH; y++)
//		for (int x = 0; x < imgL.nW; x++)
//		{
//			weightLtrans[y + offsetY][x + offsetX] = weightSeamL[y][x];
//			ImageLtrans_R[y + offsetY][x + offsetX] = imgL.imgRed[y][x];
//			ImageLtrans_G[y + offsetY][x + offsetX] = imgL.imgGreen[y][x];
//			ImageLtrans_B[y + offsetY][x + offsetX] = imgL.imgBlue[y][x];
//		}
//
//	//----------------------------------------------------------------------------------------
//	// Set deformed vertice
//	int nTmpX, nTmpY;
//
//	for (i = 0; i < nNumVert; i++)
//	{
//		arrMesh[i].DeformMesh(false, homography_all);
//	}
//
//	//inverse mesh warp
//	for (i = 0; i < nNumVert; i++)
//	{
//		// inverse warping
//		for (y = arrMesh[i].nMinY; y <= arrMesh[i].nMaxY; y++)
//			for (x = arrMesh[i].nMinX; x <= arrMesh[i].nMaxX; x++) {
//
//				inputPoint = Point2f(x, y);
//
//				Mat inverH = arrMesh[i].Homo.inv();
//				GetPerspectivePoint(inputPoint, transPoint, inverH);
//
//				if (transPoint.x >= arrMesh[i].vertex[0].x
//					&& transPoint.x <= arrMesh[i].vertex[3].x
//					&& transPoint.y >= arrMesh[i].vertex[0].y
//					&& transPoint.y <= arrMesh[i].vertex[3].y)
//				{
//					ImageRtrans_R[y + offsetY][x + offsetX] = imgR.imgRed[int(transPoint.y)][int(transPoint.x)];
//					ImageRtrans_G[y + offsetY][x + offsetX] = imgR.imgGreen[int(transPoint.y)][int(transPoint.x)];
//					ImageRtrans_B[y + offsetY][x + offsetX] = imgR.imgBlue[int(transPoint.y)][int(transPoint.x)];
//					weightRtrans[y + offsetY][x + offsetX] = weightSeamR[int(transPoint.y)][int(transPoint.x)];
//				}
//			}
//	}
//	
//	for (y = 0; y < nH_total; y++)
//		for (x = 0; x < nW_total; x++)
//		{
//			if (weightRtrans[y][x] > weightLtrans[y][x])
//			{
//				weightRtrans[y][x] = 1;
//				weightLtrans[y][x] = 0;
//			}
//
//			if (weightLtrans[y][x] > weightRtrans[y][x])
//			{
//				weightLtrans[y][x] = 1;
//				weightRtrans[y][x] = 0;
//			}
//		}
//
//	this->MultiBandBlending(nH_total, nW_total, outputImageRed, outputImageGreen, outputImageBlue);
//
//	DisplayCimage2DColor(outputImageRed, outputImageGreen, outputImageBlue, nW_total, nH_total, 0, 0);
//
//	free_cmatrix(outputImageRed, nH_total, nW_total);
//	free_cmatrix(outputImageGreen, nH_total, nW_total);
//	free_cmatrix(outputImageBlue, nH_total, nW_total);
//}
//
//void CMultiHomoStitcing_jy::ParallaxHandling(int maxInlierDis)
//{
//	int i, j;
//
//	double dMinDis, dTmpDis;
//	int* arrNumData;
//	double dTempX, dTempY;
//	int nLabel;
//	int nRand;
//	bool bChange;
//	bool bRepeat = true;
//
//	delete[] arrCenter;
//	delete[] arrCost;
//
//	arrCenter = new Point2f[nNumCluster];
//	arrNumData = new int[nNumCluster];
//	arrCost = new double[nNumCluster];
//	//------------<설정끝>---------------------------------------------------------
//
//	while (bRepeat)
//	{
//		srand((unsigned int)time(NULL));
//		bRepeat = false;
//		bChange = true;
//		delete[] arrClusterR;
//		delete[] arrClusterL;
//		delete[] arrHomo;
//
//		arrClusterR = new vector<Point2f>[nNumCluster];
//		arrClusterL = new vector<Point2f>[nNumCluster];
//		arrHomo = new Mat[nNumCluster];
//
//		for (i = 0; i < 2; i++)
//		{
//			nRand = rand() % nNumPoint;
//			
//			arrCenter[i] = GodPointR[nRand];
//		}
//
//		//K-Means Cluatering
//		while (bChange)
//		{
//			bChange = false;
//			// 중심과 cluster의 거리
//			for (int i = 0; i < nNumPoint; i++)
//			{
//				dTempX = GodPointR[i].x;
//				dTempY = GodPointR[i].y;
//
//				for (int j = 0; j < nNumCluster; j++)
//				{
//					dTmpDis = GetDistance(dTempX, dTempY, arrCenter[j].x, arrCenter[j].y);
//
//					if (j == 0)
//					{
//						dMinDis = dTmpDis;
//						nLabel = j;
//					}
//					else
//					{
//
//						if (dMinDis > dTmpDis)
//						{
//							nLabel = j;
//							dMinDis = dTmpDis;
//						}
//					}
//				}
//
//				//변화가 있는지 확인한다.
//				if (labelCluster[i] != nLabel)
//				{
//					bChange = true;
//				}
//
//				labelCluster[i] = nLabel;
//			}
//
//			// 다시 중심값을 구한다
//			// Array 초기화
//			for (i = 0; i < nNumCluster; i++)
//			{
//				arrCenter[i].y = 0;
//				arrCenter[i].x = 0;
//				arrNumData[i] = 0;
//			}
//
//			for (i = 0; i < nNumPoint; i++)
//			{
//				arrCenter[labelCluster[i]].y += GodPointR[i].y;
//				arrCenter[labelCluster[i]].x += GodPointR[i].x;
//				arrNumData[labelCluster[i]] ++;
//			}
//
//			for (i = 0; i < nNumCluster; i++)
//			{
//				arrCenter[i].y /= arrNumData[i];
//				arrCenter[i].x /= arrNumData[i];
//			}
//		}
//
//		//Make arrClusterR
//		for (i = 0; i < nNumPoint; i++)
//		{
//			arrClusterR[labelCluster[i]].push_back(GodPointR[i]);
//			arrClusterL[labelCluster[i]].push_back(GodPointL[i]);
//		}
//
//		//Get Homography
//		for (i = 0; i < nNumCluster; i++)
//		{
//			arrHomo[i] = findHomography(arrClusterR[i], arrClusterL[i],CV_RANSAC);
//		}
//
//		//Get number of inlier
//		int numInlier;
//		int numClusterData;
//		double dTmp;
//		Point2f tmpInput, tmpOutput;
//
//		for (i = 0; i < nNumCluster; i++)
//		{
//			numInlier = 0;
//			numClusterData = arrNumData[i];
//
//			for (j = 0; j < numClusterData; j++)
//			{
//				tmpInput.x = arrClusterR[i][j].x;
//				tmpInput.y = arrClusterR[i][j].y;
//
//				GetPerspectivePoint(tmpInput, tmpOutput, arrHomo[i]);
//
//				dTmp = GetDistance(tmpOutput.x, tmpOutput.y, arrClusterL[i][j].x, arrClusterL[i][j].y);
//
//				if (dTmp < maxInlierDis)
//				{
//					numInlier++;
//				}
//			}
//
//			arrCost[i] = double(numInlier) / double(arrNumData[i]);
//
//			if (arrCost[i] < 0.5)
//			{
//				bRepeat = true;
//			}
//		}
//	}
//}
//
//void CMultiHomoStitcing_jy::CheckDImageColor(double ** R, double ** G, double ** B, int nH, int nW)
//{
//	int i, j, x, y;
//
//	BYTE** tmpR, **tmpG, **tmpB;
//
//	tmpR = cmatrix(nH, nW);
//	tmpG = cmatrix(nH, nW);
//	tmpB = cmatrix(nH, nW);
//
//	for (y = 0; y < nH; y++)
//		for (x = 0; x < nW; x++)
//		{
//			tmpR[y][x] = (BYTE)R[y][x];
//			tmpG[y][x] = (BYTE)G[y][x];
//			tmpB[y][x] = (BYTE)B[y][x];
//		}
//
//	DisplayCimage2DColor(tmpR, tmpG, tmpB, nW, nH, 0, 0);
//
//	return;
//}
//
//
///****************************************************
//<GetDissmiliarity>
//
//기존 사각형 포인트와 호모그래피를 적용한 사각형 포인트를 서로 비교하여
//호모그래피가 적절한지 아닌지 에 대한 dissmiliarity를 구한다.
//
//<좌표 포인트 위치>
//
//[0]-------[1]
//|			|
//|			|
//[2]-------[3]
//
//------------------------------------
//Pre : nH, nW, Homo
//Post : dissimilar
//
//*******************************************************/
//float CMultiHomoStitcing_jy::GetDissmiliarity(int nH, int nW, Mat& homo0, Mat& homo1)
//{
//	int i, j;
//
//	Point2f corner[4];
//	Point2f cornerTrans0[4];
//	Point2f cornerTrans1[4];
//
//	float dist0Trans0, dist1Trans0;				// homo0로 변형된 영상의 대각선 길이 (항상 dist0 > dist1)
//	float dist0Trans1, dist1Trans1;				// homo1로 변형된 영상의 대각선 길이 (항상 dist0 > dist1)
//
//	float tmp;
//	float dissimilar;
//
//	corner[0].x = 0; corner[0].y = 0;
//	corner[1].x = nW - 1; corner[1].y = 0;
//	corner[2].x = 0; corner[2].y = nH - 1;
//	corner[3].x = nW - 1; corner[3].y = nH - 1;
//
//	//------------------------------------------
//	for (i = 0; i < 4; i++)
//	{
//		GetPerspectivePoint(corner[i], cornerTrans0[i], homo0);
//		GetPerspectivePoint(corner[i], cornerTrans1[i], homo1);
//	}
//
//	// homo0로 변형된 영상의 대각선 길이 구하기
//	dist0Trans0 = (cornerTrans0[0].y - cornerTrans0[3].y)*(cornerTrans0[0].y - cornerTrans0[3].y)
//		+ (cornerTrans0[0].x - cornerTrans0[3].x)*(cornerTrans0[0].x - cornerTrans0[3].x);
//	dist1Trans0 = (cornerTrans0[1].y - cornerTrans0[2].y)*(cornerTrans0[1].y - cornerTrans0[2].y)
//		+ (cornerTrans0[1].x - cornerTrans0[2].x)*(cornerTrans0[1].x - cornerTrans0[2].x);
//
//	dist0Trans0 = sqrt(dist0Trans0);
//	dist1Trans0 = sqrt(dist1Trans0);
//
//	if (dist0Trans0 < dist1Trans0)
//	{
//		tmp = dist1Trans0;
//		dist1Trans0 = dist0Trans0;
//		dist0Trans0 = tmp;
//	}
//
//	// homo1로 변형된 영상의 대각선 길이 구하기
//	dist0Trans1 = (cornerTrans1[0].y - cornerTrans1[3].y)*(cornerTrans1[0].y - cornerTrans1[3].y)
//		+ (cornerTrans1[0].x - cornerTrans1[3].x)*(cornerTrans1[0].x - cornerTrans1[3].x);
//	dist1Trans1 = (cornerTrans1[1].y - cornerTrans1[2].y)*(cornerTrans1[1].y - cornerTrans1[2].y)
//		+ (cornerTrans1[1].x - cornerTrans1[2].x)*(cornerTrans1[1].x - cornerTrans1[2].x);
//
//	dist0Trans1 = sqrt(dist0Trans1);
//	dist1Trans1 = sqrt(dist1Trans1);
//
//	if (dist0Trans1 < dist1Trans1)
//	{
//		tmp = dist1Trans1;
//		dist1Trans1 = dist0Trans1;
//		dist0Trans1 = tmp;
//	}
//
//	// dissimilarity 구하기
//
//	dissimilar = (2 * dist0Trans1 * dist0Trans0 / ((dist0Trans1 * dist0Trans1) + (dist0Trans0 * dist0Trans0)))
//		* (2 * dist1Trans1 * dist1Trans0 / ((dist1Trans1 * dist1Trans1) + (dist1Trans0 * dist1Trans0)));
//
//
//	//그림 나타내기
//	BYTE** drawSquare;
//	int x, y;
//	int offsetX, offsetY;
//	int nH_img, nW_img;
//	double minX = 20, minY = 20;
//	double maxX = 0, maxY = 0;
//
//	for (i = 0; i < 4; i++)
//	{
//		corner[i].y = corner[i].y / 2;
//		corner[i].x = corner[i].x / 2;
//
//		cornerTrans0[i].y = cornerTrans0[i].y / 2;
//		cornerTrans0[i].x = cornerTrans0[i].x / 2;
//
//		cornerTrans1[i].y = cornerTrans1[i].y / 2;
//		cornerTrans1[i].x = cornerTrans1[i].x / 2;
//	}
//
//	for (i = 0; i < 4; i++)
//	{
//		if (minX > corner[i].x) minX = corner[i].x;
//		if (minY > corner[i].y) minY = corner[i].y;
//		if (maxX < corner[i].x) maxX = corner[i].x;
//		if (maxY < corner[i].y) maxY = corner[i].y;
//
//		if (minX > cornerTrans0[i].x) minX = cornerTrans0[i].x;
//		if (minY > cornerTrans0[i].y) minY = cornerTrans0[i].y;
//		if (maxX < cornerTrans0[i].x) maxX = cornerTrans0[i].x;
//		if (maxY < cornerTrans0[i].y) maxY = cornerTrans0[i].y;
//
//		if (minX > cornerTrans1[i].x) minX = cornerTrans1[i].x;
//		if (minY > cornerTrans1[i].y) minY = cornerTrans1[i].y;
//		if (maxX < cornerTrans1[i].x) maxX = cornerTrans1[i].x;
//		if (maxY < cornerTrans1[i].y) maxY = cornerTrans1[i].y;
//	}
//
//	if (minX < 0) offsetX = -1 * minX; else offsetX = 0;
//	if (minY < 0) offsetY = -1 * minY; else offsetY = 0;
//
//	nH_img = maxY - minY + 1;
//	nW_img = maxX - minX + 1;
//
//	drawSquare = cmatrix(nH_img, nW_img);
//
//	for (y = 0; y < nH_img; y++)
//		for (x = 0; x < nW_img; x++)
//		{
//			drawSquare[y][x] = 0;
//		}
//
//	DrawLine(drawSquare, nW_img, nH_img, corner[0].x + offsetX, corner[0].y + offsetY, corner[1].x + offsetX, corner[1].y + offsetY, 50);
//	DrawLine(drawSquare, nW_img, nH_img, corner[1].x + offsetX, corner[1].y + offsetY, corner[3].x + offsetX, corner[3].y + offsetY, 50);
//	DrawLine(drawSquare, nW_img, nH_img, corner[3].x + offsetX, corner[3].y + offsetY, corner[2].x + offsetX, corner[2].y + offsetY, 50);
//	DrawLine(drawSquare, nW_img, nH_img, corner[2].x + offsetX, corner[2].y + offsetY, corner[0].x + offsetX, corner[0].y + offsetY, 50);
//
//	DrawLine(drawSquare, nW_img, nH_img, cornerTrans0[0].x + offsetX, cornerTrans0[0].y + offsetY, cornerTrans0[1].x + offsetX, cornerTrans0[1].y + offsetY, 255);
//	DrawLine(drawSquare, nW_img, nH_img, cornerTrans0[1].x + offsetX, cornerTrans0[1].y + offsetY, cornerTrans0[3].x + offsetX, cornerTrans0[3].y + offsetY, 255);
//	DrawLine(drawSquare, nW_img, nH_img, cornerTrans0[2].x + offsetX, cornerTrans0[2].y + offsetY, cornerTrans0[3].x + offsetX, cornerTrans0[3].y + offsetY, 255);
//	DrawLine(drawSquare, nW_img, nH_img, cornerTrans0[2].x + offsetX, cornerTrans0[2].y + offsetY, cornerTrans0[0].x + offsetX, cornerTrans0[0].y + offsetY, 255);
//
//	DrawLine(drawSquare, nW_img, nH_img, cornerTrans1[0].x + offsetX, cornerTrans1[0].y + offsetY, cornerTrans1[1].x + offsetX, cornerTrans1[1].y + offsetY, 200);
//	DrawLine(drawSquare, nW_img, nH_img, cornerTrans1[1].x + offsetX, cornerTrans1[1].y + offsetY, cornerTrans1[3].x + offsetX, cornerTrans1[3].y + offsetY, 200);
//	DrawLine(drawSquare, nW_img, nH_img, cornerTrans1[2].x + offsetX, cornerTrans1[2].y + offsetY, cornerTrans1[3].x + offsetX, cornerTrans1[3].y + offsetY, 200);
//	DrawLine(drawSquare, nW_img, nH_img, cornerTrans1[2].x + offsetX, cornerTrans1[2].y + offsetY, cornerTrans1[0].x + offsetX, cornerTrans1[0].y + offsetY, 200);
//
//	DisplayCimage2D(drawSquare, nW_img, nH_img, 0, 0);
//
//	free_cmatrix(drawSquare, nH_img, nW_img);
//
//	return dissimilar;
//}
//
//
///****************************************************
//<GetExcepInlierRate>
//
//해당 index의 영역을 제외한 부분의 Inlier Rate를 구해준다.
//
//Inlier의 판별은 거리 값으로 한다. (default :3)
//Inlier Rate는 (Inlier 갯수) / (제외된 특징점의 갯수)
//
//*Mode
//0 : 제외한 부분의 특징점에서 구한 Homography를 활용한다.
//1 : 정해진 Homography를 활용한다.
//
//------------------------------------
//Pre : 
//Post : 
//
//*******************************************************/
//double CMultiHomoStitcing_jy::GetExceptInlierRate(int* arrIdx, int numIdx, int mode, Mat& homo)
//{
//	int i, j;
//	vector<Point2f> tmpPointR, tmpPointL;
//	Mat tmpHomo;
//	bool bCheck = false;
//	
//	// 영역 이외의 특징점 분포를 구한다.
//	// 영역 이외의 호모그래피를 구한다.
//	for (int i = 0; i < nNumPoint; i++)
//	{
//		for (int j = 0; j < numIdx; j++)
//		{
//			if (LabelMeanShift[int(GodPointR[i].y)][int(GodPointR[i].x)] == arrIdx[j])
//			{
//				bCheck = true;
//			}
//		}
//
//		if (bCheck)
//		{
//			bCheck = false;
//		}
//		else
//		{
//			tmpPointR.push_back(GodPointR[i]);
//			tmpPointL.push_back(GodPointL[i]);
//		}
//	}
//
//	if (mode == 0)
//		homo = findHomography(tmpPointR, tmpPointL, CV_RANSAC);
//	
//	// Inlier 비율을 구한다.
//	int numInlier;
//	int numSegPoint;
//	double dTmp;
//	double inlierRate;
//	Point2f tmpInput, tmpOutput;
//
//	
//	numInlier = 0;
//	numSegPoint = tmpPointR.size();
//
//	for (i = 0; i < numSegPoint; i++)
//	{
//		tmpInput.x = tmpPointR[i].x;
//		tmpInput.y = tmpPointR[i].y;
//
//		GetPerspectivePoint(tmpInput, tmpOutput, homo);
//
//		dTmp = GetDistance(tmpOutput.x, tmpOutput.y, tmpPointL[i].x, tmpPointL[i].y);
//
//		if (dTmp < 3)
//		{
//			numInlier++;
//		}
//	}
//
//	inlierRate = float(numInlier) / float(numSegPoint);
//	
//	return inlierRate;
//}
//
///****************************************************
//<CompareDistri>
//
//특징점이 골고루 퍼져 있는 지 확인한다.
//Region안 mean, sigma, Region의 Feature의 mean, sigma를 구한다.
//
//< 비교 방법 >
//
//1. Region의 mean과 특징점의 mean를 서로 비교한다.
//2. mean이 비슷하게 형성되면 sigma를 통해 분산이 일정 이상이 되게 한다.
//
//------------------------------------
//Pre : searchR(mean 탐색범위), thresh(분산 임계값)
//	LabelMeanShift, nMaxInlierRegion, objClusterR
//Post : bExeDualHomo, meanF, meanR, sigmaF
//
//*******************************************************/
//bool CMultiHomoStitcing_jy::CompareDistri(int searchR, int thresh)
//{
//	int x, y, i, j;
//	CvPoint meanR, sigmaR;
//	CvPoint meanF, sigmaF;
//	double tmpSumX=0, tmpSumY=0;
//	int num=0; 
//	bool bExeDualHomo=false;
//
//	// 전체 mean를 구한다.
//	for (y = 0; y < imgR.nH; y++)
//		for (x = 0; x < imgR.nW; x++)
//		{
//			if (LabelMeanShift[y][x] == nMaxInlierRegion)
//			{
//				tmpSumX += x;
//				tmpSumY += y;
//				num++;
//			}
//		}
//
//	meanR.x = tmpSumX / num;
//	meanR.y = tmpSumY / num;
//
//	// feature의 mean를 구한다.
//	tmpSumX = 0; tmpSumY = 0; num = 0;
//
//	int objCluSize = objClusterR.size();
//
//	for (i = 0; i < objCluSize; i++)
//	{
//		tmpSumX += objClusterR[i].x;
//		tmpSumY += objClusterR[i].y;
//		num++;
//	}
//
//	meanF.x = tmpSumX / num;
//	meanF.y = tmpSumY / num;
//
//	// feature의 sigma를 구한다.
//	tmpSumX = 0; tmpSumY = 0;
//
//	for (i = 0; i < objCluSize; i++)
//	{
//		tmpSumX += ((objClusterR[i].x - meanF.x)*(objClusterR[i].x - meanF.x));
//		tmpSumY += ((objClusterR[i].y - meanF.y)*(objClusterR[i].y - meanF.y));
//	}
//
//	sigmaF.x = tmpSumX / num;
//	sigmaF.y = tmpSumY / num;
//
//	sigmaF.x = sqrt(sigmaF.x);
//	sigmaF.y = sqrt(sigmaF.y);
//
//	// 비교한다.
//	//1. mean이 일정 원안에 있는 지 확인.
//	double tmp;
//
//	tmp = (meanF.x - meanR.x)*(meanF.x - meanR.x) + (meanF.y - meanR.y)*(meanF.y - meanR.y);
//
//	if (tmp < searchR*searchR)
//	{
//		if (sigmaF.x > thresh && sigmaF.y > thresh)
//		{
//			bExeDualHomo = true;
//		}
//	}
//
//	return bExeDualHomo;
//}
//
///****************************************************
//<BilinearInterpolation>
//
//Backward Warping시에 bilinear로 픽셀의 명암 값을 가져온다.
//먼저 x축으로 명암값을 가져오고 
//그다음 y축으로 명암값을 가져온다.
//
//IA ----I'1------ IB
//|	dX1	|	dX2	|
//|		|		| dY1
//|		|		|
//I'--------------I'
//|		|		| dY2
//|		|		|
//IC ----I'2------ ID
//
//<< 공식 >>
//|
//|			|
//|-----------|------------|
//I1   d1    Ix    d2     I2
//
//Ix = (d1*I2 + d2*I1) / (d1 + d2)
//
//------------------------------------
//Pre : point, image, nH , nW 
//Post : result
//
//*******************************************************/
//int CMultiHomoStitcing_jy::BilinearInterpolation(Point2f point, BYTE** image, int nH, int nW)
//{
//	int result;
//	double dX1, dX2;
//	double dY1, dY2;
//	double IA, IB, IC, ID;
//	double Itmp1, Itmp2;
//	CvPoint A;
//	
//	A.x = int(point.x);
//	A.y = int(point.y);
//
//	if (A.x >= nW - 1 || A.y >= nH - 1)
//	{
//		result = image[int(point.y)][int(point.x)];
//		return result;
//	}
//
//
//	IA = image[A.y][A.x];		IB = image[A.y][A.x+1];
//	IC = image[A.y+1][A.x];	ID = image[A.y + 1][A.x + 1];
//	
//	dX1 = point.x - A.x;		dX2 = A.x + 1 - point.x; 
//	dY1 = point.y - A.y;		dY2 = A.y + 1 - point.y;
//
//	// I'1,2 값 구하기
//	Itmp1 = ((dX1 * IB) + (dX2 * IA));
//	Itmp2 = ((dX1 * ID) + (dX2 * IC));
//
//	// result 값 구하기
//	result = int(((dY1 * Itmp2) + (dY2 * Itmp1)));
//	
//	return result;
//}
//
///****************************************************
//<KLdivergenceForBivariateGaussian>
//
//두 joint 가우시안의 similarity를 구한다. 
//
//------------------------------------
//Pre : mean1, mean2, covMat1, covMat2
//Post : 
//
//*******************************************************/
//double CMultiHomoStitcing_jy::KLdivergenceForBivariateGaussiian(double* mean0, double* mean1, double** covMat0, double** covMat1)
//{
//	double tmp;
//	double** invCov1;
//	double** tmpMat;
//	double* tmpVec;
//	double* difMean;
//	double detCov0, detCov1;
//
//	invCov1 = dmatrix(2, 2);
//	tmpMat = dmatrix(2, 2);
//	tmpVec = new double[2];
//	difMean = new double[2];
//
//	invCov1 = InverseMatrix2D(covMat1);
//
//	//tr(cov1^-1*cov0)
//	tmpMat = MultipleMatrix2D(invCov1, covMat0);
//	
//	tmp = tmpMat[0][0] + tmpMat[1][1];
//
//	//(u1-u0)^T * cov1^-1 * (u1-u0)
//	difMean[0] = mean1[0] - mean0[0];
//	difMean[1] = mean1[1] - mean0[1];
//
//	tmpVec[0] = (difMean[0] * invCov1[0][0]) + (difMean[1] * invCov1[1][0]);
//	tmpVec[1] = (difMean[0] * invCov1[0][1]) + (difMean[1] * invCov1[1][1]);
//
//	tmp += ((tmpVec[0] * difMean[0]) + (tmpVec[1] * difMean[1]));
//
//	//ln(det(cov1)/det(cov0))
//	detCov0 = DetermineMat2D(covMat0);
//	detCov1 = DetermineMat2D(covMat1);
//
//	tmp += log(detCov1 / detCov0);
//
//	tmp -= 2;
//	tmp /= 2;
//
//	free_dmatrix(invCov1, 2, 2);
//	free_dmatrix(tmpMat, 2, 2);
//	delete[]tmpVec;
//	delete[]difMean;
//
//	return tmp;
//}
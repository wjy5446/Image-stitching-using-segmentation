#pragma once
#include "stdafx.h"
#include "opencv2\core\core.hpp"
#include "opencv2\imgproc\imgproc.hpp"
#include "opencv2\features2d\features2d.hpp"
#include "opencv2\nonfree\nonfree.hpp"
#include "opencv2\nonfree\features2d.hpp"
#include "opencv2\calib3d\calib3d.hpp"
#include<vector>
#include<list>
#include "meanshift\MeanShift.h"

#include"Mesh.h"

using namespace cv;
using namespace std;

typedef struct Image2D{
	int nH, nW, nPosX, nPosY;
	BYTE** imgRed, **imgBlue, **imgGreen;
	BYTE** imgGray;
} Image2D;

class CImageStitchBasic
{
	/*****************************************************************
	본 함수를 이용하기 위해서는 다음과 같은 절차가 꼭 필요하다.
	1. GetMatchingPoint();
	2. GetHomography();
	3. WarpHomography();
	*******************************************************************/
protected:
	// value for Image
	Image2D imgL, imgR;
	Image2D imgMark;
	Image2D imgTotal;

	// value for match
	vector< Point2f > GodPointL;
	vector< Point2f > GodPointR;
	int nNumPoint;

	// homography
	Mat homo;

	// value for multibandblend
	double** weightSeamR, ** weightSeamL;
	double **weightLtrans, **weightRtrans;
	Image2D imgRtrans, imgLtrans;

	void DrawFeature();
	void DrawMatch();
	void DrawLR();

	void MultiBandBlending(int nH, int nW, BYTE** Result_R, BYTE** Result_G, BYTE** Result_B); // multiBandBlending
	void MakeWeightSeam(double** weightSeam, int nH, int nW);
	int BilinearInterpolation(Point2f point, BYTE** image, int nH, int nW);

public:
	// value Error
	bool bErr;

	CImageStitchBasic();
	CImageStitchBasic(BYTE* img1D_L, BYTE* img1D_R, int nHL, int nWL, int nHR, int nWR);
	~CImageStitchBasic();

	void GetMatchingPoint(int nPara = 800, bool bOnFeature = false, bool bOnMatch = false);
	void GetHomography();
	void WarpHomography(bool bOnLR = false, bool bMultiBandBlend= false);
	
	void GetAffine();
	void GetAffineLSM(vector<Point2f>& inlier_src, vector<Point2f>& inlier_dst);
	void GetAffinePoint(Point2f& input, Point2f& output, Mat& affineMat);
	void WarpAffine();
	void GetInvAffinePoint(Point2f& input, Point2f& output, Mat& affineMat);

	// function for video stitching
	void WarpHomographyForVideo();
};
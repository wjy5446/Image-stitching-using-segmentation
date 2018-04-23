#pragma once
#include "StitchMultiHomo.h"

class CImageStitchKMeans : public CImageStitchMultiHomo
{
private:
	Mat* arrHomo;
	double** weightMap;

	void RemoveOutlier(vector<Point2f>& pointR, vector<Point2f>& pointL, double thresh);
	double GetInlierRate(vector<Point2f>& pointR, vector<Point2f>& pointL, Mat& homo, double thresh);
	void DrawFeature();
public:
	CImageStitchKMeans();
	~CImageStitchKMeans();

	void RemoveOutlier1st(double dThresh = 7);
	void KMeansCluster();
	void RemoveOutlier2nd(double dThresh = 3);
	void BlendingHomo();
};
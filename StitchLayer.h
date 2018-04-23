#pragma once
#include "StitchMultiHomo.h"

class CImageStitchLayer : public CImageStitchMultiHomo
{
private:
	vector< vector<Point2f> > SetPointLayerL, SetPointLayerR;
	vector< Mat> setHomoLayer;
	int nNumLayer;
	int* nNumSetPoint;

public:
	CImageStitchLayer();
	~CImageStitchLayer();

	void FindLayer(int nThresh);
	void BlendingHomo(double sigma = 20);

	void DrawLayer();
};
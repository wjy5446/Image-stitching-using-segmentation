#pragma once
#include "StitchBasic.h"

class CImageStitchMultiHomo : public CImageStitchBasic
{
protected:
	Point2f* arrInitKmean;
	Point2f* arrCenter;
	int* labelCluster;

	// Mesh
	CMesh* arrMesh;
	int nNumVert;

	Mat** homoAll;
	vector <Point2f> * arrClusterR, *arrClusterL;

public:
	CImageStitchMultiHomo();
	~CImageStitchMultiHomo();

	void KMeansCluster(bool bForgy = false);
	void BlendingHomo();

	void MakeMesh();
	void WarpMeshHomography();
};
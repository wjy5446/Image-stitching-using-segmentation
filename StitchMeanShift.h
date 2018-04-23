#pragma once
#include"StitchBasic.h"

class CImageStitchMeanShift : public CImageStitchBasic
{
	/*****************************************************************
	본 함수를 이용하기 위해서는 다음과 같은 절차가 꼭 필요하다.
	1. GetMatchingPoint();
	2. SegmentMeanshift();
	3. FindSingleObject();
	4. DetermineActive();
	5. MakeWeightMap();
	6. WarpHomograpy();
	*******************************************************************/
	private :
		Image2D imgSegment;
	
		int nNumSeg;
		int** LabelMeanShift; // Labeling map
	
		int nNumObjCandi;
		int* arrObjIdx;
		int* arrErr;
		double* arrInfo;
	
		int* arrFeatureNum;
		int* arrRegion;
		Mat* arrHomo;
	
		int nIdxObj;
		Mat homoBack, homoObj;
		Mat homoGlo;
	
		BYTE ** weightObj;
	
		double GetExceptInlierRate(int* arrIdx, int numIdx, int mode = 0, Mat& homo = Mat());
		double GetInlierRate(int Idx, Mat& homo);
		void GetArrFeatureNum();
		void GetArrRegion();
		void GetArrHomo();
		void GetHomography(int Idx, Mat& homo);
		void DrawSegment();
		double KLdivergenceForBivariateGaussiian(double* mean0, double* mean1, double** covMat0, double** covMat1);
		double DissimilarHomo(int nH, int nW, Mat& homo0, Mat& homo1, bool bOnDraw = false);
		
		//후보군 판별
		void ActiveFeatureNum(int minFeature);
		void ActiveRegion(int minArea);
		bool ActiveFeatureRate(double minFeatureRate);
		void ActiveMisMatch();
		void ActiveKLdivergence(double minKLdivergence);
		void DrawActiveErr();
		
	public :
		CString strFileName;
		bool bExeDualHomo;
		
		CImageStitchMeanShift();
		CImageStitchMeanShift(BYTE* img1D_L, BYTE* img1D_R, int nHL, int nWL, int nHR, int nWR, CString fileName);
		~CImageStitchMeanShift();
		
		void SegmentMeanshift(int minRegion = 100);
		void DetermineActive(int minFeature = 4, int minArea = 200, double minFeatureRate = 0.5, int minKLdiver = 50, bool bDrawErrInfo = false);
		void FindSingleObject(float fDiffInlierRate = 0.1, bool bDrawErrInfo = false);
		void MakeWeightMap(int dilation, bool bOnDraw = false);
		void WarpHomography(bool bWrite = false);
		
		//------------------------------
		void FindMultiObject();
		void FindObject();
		
		void Run();
};
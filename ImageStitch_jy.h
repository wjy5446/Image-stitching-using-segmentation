#pragma once
#include "afxwin.h"

#include "Image.h"
#include "opencv2\core\core.hpp"
#include "opencv2\imgproc\imgproc.hpp"
#include "opencv2\features2d\features2d.hpp"
#include "opencv2\nonfree\nonfree.hpp"
#include "opencv2\nonfree\features2d.hpp"
#include "opencv2\calib3d\calib3d.hpp"

//#include <allheaders.h>
//#include <baseapi.h>

#include "meanshift\MeanShift.h"
//#include "MultiHomoStitcing_jy.h"

#include "StitchBasic.h"
#include "StitchKMeans.h"
#include "StitchLayer.h"
#include "StitchMeanShift.h"
#include "StitchMultiHomo.h"

using namespace cv;
//using namespace tesseract;

//#pragma comment(lib,"liblept168")
//#pragma comment(lib,"libtesseract302")

// ImageStitch_jy dialog

class ImageStitch_jy : public CDialog
{
	DECLARE_DYNAMIC(ImageStitch_jy)

public:
	ImageStitch_jy(CWnd* pParent = NULL);   // standard constructor
	virtual ~ImageStitch_jy();

	BOOL m_bCreated;
// Dialog Data
	enum { IDD = IDD_IMAGESTITCH_JY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:

	int GetChainCode(int y, int x, int preCC);
	void ShiftChainCode(int& y, int& x, int CC);

	afx_msg void OnBnClickedDetectedge();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedGetdualhomography();
	afx_msg void OnBnClickedGetsinglehomography();
	afx_msg void OnBnClickedMeanshift();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedParallaxrobust();
	afx_msg void OnBnClickedParallaxhandling();
	afx_msg void OnClickedOcr();
	afx_msg void OnBnClickedMeanAuto();
	afx_msg void OnBnClickedDraw();

	int m_EditMinRegion;
	int m_EditMinFeature;
	float m_EditFeatureRate;
	float m_EditKLDiver;
	int m_EditSIFT;
	int m_EditSegRegion;
	float m_EditDiff;

	CButton m_CheckOnlyRegion;
	CButton m_checkFeature;
	CButton m_checkMatch;
	CButton m_checkLR;
	
	afx_msg void OnVideostitch();
	afx_msg void OnBnClickedGetsingleaffine();
};

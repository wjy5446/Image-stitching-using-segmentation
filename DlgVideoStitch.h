#pragma once
#include <Vfw.h>

// CVideoStitch dialog

class CVideoStitch : public CDialog
{
	DECLARE_DYNAMIC(CVideoStitch)

public:
	//----value----
	bool m_bCreated;

	// Left Video
	CString m_AviFileName_L;
	PAVIFILE m_pavi_L;
	AVIFILEINFO m_fi_L;
	PAVISTREAM m_pstm_L;
	AVISTREAMINFO m_si_L;
	PGETFRAME m_pfrm_L;
	int m_nAviLength_L;

	int m_nStart_L;
	int m_nEnd_L;
	int m_nHL, m_nWL;

	// Right Video
	CString m_AviFileName_R;
	PAVIFILE m_pavi_R;
	AVIFILEINFO m_fi_R;
	PAVISTREAM m_pstm_R;
	AVISTREAMINFO m_si_R;
	PGETFRAME m_pfrm_R;
	int m_nAviLength_R;

	int m_nStart_R;
	int m_nEnd_R;
	int m_nHR, m_nWR;

	//
	int m_nCurrentProcessingNum;
	//----function----------
	CVideoStitch(CWnd* pParent = NULL);   // standard constructor
	virtual ~CVideoStitch();

	void GetImg2DSize(PGETFRAME pfrm, int currentFrmNum, int* nH, int* nW);
	void GetImg2DColor(BYTE** img2D_R, BYTE** img2D_G, BYTE** img2D_B, PGETFRAME pfrm, int currentFrmNum);
	BYTE* GetImg1D(PGETFRAME pfrm, int currentFrmNum);

// Dialog Data
	enum { IDD = IDD_VIDEOSTITCH };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedSelSourceL();
	afx_msg void OnBnClickedSelSourceR();
	afx_msg void OnBnClickedCancel();
	
	afx_msg void OnBnClickedVideostitch();
};

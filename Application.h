#if !defined(AFX_APPLICATION_H__4B42E990_DD4F_46B0_972D_876A7A80A55A__INCLUDED_)
#define AFX_APPLICATION_H__4B42E990_DD4F_46B0_972D_876A7A80A55A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Application.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CApplication dialog
#include "Bpn.h"
#include <vfw.h>
#define VEHICLE			1
#define AVI_VEHICLE		2
#define MOTION			3
#define BPN_EX			4

class CApplication : public CDialog
{
// Construction
public:
	CApplication(CWnd* pParent = NULL);   // standard constructor

	void VehicleProc();
	void AviVehicleProc();
	void MotionProc();
	void BpnProc();

	void Detection();

	BOOL m_bCreated;

	BOOL m_bVehicleRun;
	BOOL m_bAviVehicleRun;
	BOOL m_bMotionRun;
	BOOL m_bBpnRun;

	CString m_Folder;
	UINT m_nFrameNumber;

	int m_nW, m_nH;
	BYTE **m_CurrentImageGray, **m_BackgroundImageGray;
	BYTE **m_ProcessingImageGray;
	int **m_LabelImageInt;

	PAVIFILE m_pavi;
	AVIFILEINFO m_fi;
	PAVISTREAM m_pstm;
	AVISTREAMINFO m_si;
	PGETFRAME m_pfrm; 

	CString m_AviFileName;

	BYTE **m_CurrentImageRed, **m_CurrentImageGreen, **m_CurrentImageBlue;

	HWND m_hWndCap;			
	BITMAPINFO	m_BmInfo;

	BYTE **m_PreviousImageGray;


	CBpn *m_pBpn;
	BYTE **ErrorImage, **TestImage;
	int m_nErrorW, m_nErrorH, m_nTestW, m_nTestH;
	double m_PreviousError;
	int nTrainCnt;

// Dialog Data
	//{{AFX_DATA(CApplication)
	enum { IDD = IDD_APPLICATION };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CApplication)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CApplication)
	virtual BOOL OnInitDialog();
	afx_msg void OnVehicleRun();
	afx_msg void OnAviRun();
	afx_msg void OnMotionRun();
	afx_msg void OnCancel();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();
	afx_msg void OnFolderSel();
	afx_msg void OnFileSel();
	afx_msg void OnBpnExRun();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_APPLICATION_H__4B42E990_DD4F_46B0_972D_876A7A80A55A__INCLUDED_)

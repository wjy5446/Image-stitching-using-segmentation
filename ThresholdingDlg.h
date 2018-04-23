#if !defined(AFX_THRESHOLDINGDLG_H__B7E6665C_2406_4CFA_BF71_DC817EBE05A8__INCLUDED_)
#define AFX_THRESHOLDINGDLG_H__B7E6665C_2406_4CFA_BF71_DC817EBE05A8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ThresholdingDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CThresholdingDlg dialog

class CThresholdingDlg : public CDialog
{
// Construction
public:
	CThresholdingDlg(CWnd* pParent = NULL);   // standard constructor

	void Set(BYTE **Input, BYTE **Output, int nW, int nH, int nPosX, int nPosY);

	int m_nW, m_nH;
	int m_nPosX, m_nPosY;
	BYTE **m_InputGray, **m_OutputGray;
	
	BYTE **m_HisImage, **m_HisImageThre;
	int m_nHisW, m_nHisH;


// Dialog Data
	//{{AFX_DATA(CThresholdingDlg)
	enum { IDD = IDD_THRESHOLDING_DLG };
	CSliderCtrl	m_ThresholdCtrl;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CThresholdingDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CThresholdingDlg)
	afx_msg void OnCustomdrawThreshold(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCancel();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_THRESHOLDINGDLG_H__B7E6665C_2406_4CFA_BF71_DC817EBE05A8__INCLUDED_)

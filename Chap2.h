#if !defined(AFX_CHAP2_H__E604F964_F544_445F_B70C_CA4362FC8BC2__INCLUDED_)
#define AFX_CHAP2_H__E604F964_F544_445F_B70C_CA4362FC8BC2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Chap2.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CChap2 dialog

class CChap2 : public CDialog
{
// Construction
public:
	CChap2(CWnd* pParent = NULL);   // standard constructor

	BOOL m_bCreated;

// Dialog Data
	//{{AFX_DATA(CChap2)
	enum { IDD = IDD_CHAP2 };
	CComboBox	m_OperationType;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChap2)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CChap2)
	afx_msg void OnHistogram();
	afx_msg void OnCancel();
	afx_msg void OnReverse();
	afx_msg void OnThresholding();
	afx_msg void OnThresholdingDlg();
	virtual BOOL OnInitDialog();
	afx_msg void OnOperation();
	afx_msg void OnMove();
	afx_msg void OnScaleX2();
	afx_msg void OnRotate();
	afx_msg void OnDithering();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHAP2_H__E604F964_F544_445F_B70C_CA4362FC8BC2__INCLUDED_)

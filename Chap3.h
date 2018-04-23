#if !defined(AFX_CHAP3_H__7A4349D5_6A88_4062_BEF0_48DCEE9F39F5__INCLUDED_)
#define AFX_CHAP3_H__7A4349D5_6A88_4062_BEF0_48DCEE9F39F5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Chap3.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CChap3 dialog

class CChap3 : public CDialog
{
// Construction
public:
	CChap3(CWnd* pParent = NULL);   // standard constructor

	BOOL m_bCreated;

// Dialog Data
	//{{AFX_DATA(CChap3)
	enum { IDD = IDD_CHAP3 };
	CComboBox	m_FilterType2Ctrl;
	CComboBox	m_FilterTypeCtrl;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChap3)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CChap3)
	afx_msg void OnCancel();
	afx_msg void OnHistogramEq();
	afx_msg void OnMean();
	virtual BOOL OnInitDialog();
	afx_msg void OnMedian();
	afx_msg void OnSobel();
	afx_msg void OnLaplacian();
	afx_msg void OnLaplacianSharp();
	afx_msg void OnDft();
	afx_msg void OnFft();
	afx_msg void OnDftRowCol();
	afx_msg void OnFiltering();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHAP3_H__7A4349D5_6A88_4062_BEF0_48DCEE9F39F5__INCLUDED_)

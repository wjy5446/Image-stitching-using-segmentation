#if !defined(AFX_CHAP4_H__3C26D027_698F_4E22_B636_C34D6EB7C729__INCLUDED_)
#define AFX_CHAP4_H__3C26D027_698F_4E22_B636_C34D6EB7C729__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Chap4.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CChap4 dialog

class CChap4 : public CDialog
{
// Construction
public:
	CChap4(CWnd* pParent = NULL);   // standard constructor

	BOOL m_bCreated;

// Dialog Data
	//{{AFX_DATA(CChap4)
	enum { IDD = IDD_CHAP4 };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChap4)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CChap4)
	afx_msg void OnCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnDct();
	afx_msg void OnCompression();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedReverse();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHAP4_H__3C26D027_698F_4E22_B636_C34D6EB7C729__INCLUDED_)

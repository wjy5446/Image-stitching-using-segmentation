#if !defined(AFX_YUVOPTIONDLG_H__8CC3E024_2D71_42A5_8643_C78CED8459CC__INCLUDED_)
#define AFX_YUVOPTIONDLG_H__8CC3E024_2D71_42A5_8643_C78CED8459CC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// YuvOptionDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CYuvOptionDlg dialog

class CYuvOptionDlg : public CDialog
{
// Construction
public:
	CYuvOptionDlg(CWnd* pParent = NULL);   // standard constructor

	void ImageSizeFormat();

// Dialog Data
	//{{AFX_DATA(CYuvOptionDlg)
	enum { IDD = IDD_YUV_OPT };
	CEdit	m_CtrlH;
	CEdit	m_CtrlW;
	int		m_nYuvType;
	int		m_nW;
	int		m_nH;
	int		m_nScaleFormat;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CYuvOptionDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CYuvOptionDlg)
	afx_msg void OnScaleFormat0();
	afx_msg void OnScaleFormat1();
	afx_msg void OnScaleFormat2();
	afx_msg void OnScaleFormat3();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_YUVOPTIONDLG_H__8CC3E024_2D71_42A5_8643_C78CED8459CC__INCLUDED_)

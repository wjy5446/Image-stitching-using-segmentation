#if !defined(AFX_SAVESCREENOPT_H__97E78E78_EEFB_4424_96A7_699D8C5BFC06__INCLUDED_)
#define AFX_SAVESCREENOPT_H__97E78E78_EEFB_4424_96A7_699D8C5BFC06__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SaveScreenOpt.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSaveScreenOpt dialog

class CSaveScreenOpt : public CDialog
{
// Construction
public:
	CSaveScreenOpt(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSaveScreenOpt)
	enum { IDD = IDD_SAVE_SCREEN_OPT };
	int		m_nH;
	int		m_nW;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSaveScreenOpt)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSaveScreenOpt)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SAVESCREENOPT_H__97E78E78_EEFB_4424_96A7_699D8C5BFC06__INCLUDED_)

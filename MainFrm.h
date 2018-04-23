// MainFrm.h : 
//face of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__E18E3098_19D7_4881_B522_CC1C50720806__INCLUDED_)
#define AFX_MAINFRM_H__E18E3098_19D7_4881_B522_CC1C50720806__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Chap2.h"		// 추가
#include "Chap3.h"		// 추가
#include "Chap4.h"		// 추가
#include "Chap5.h"		// 추가
#include "Application.h"	// 추가
#include "ImageStitch_jy.h" // 추가
#include "DlgVideoStitch.h" // 추가
#include "CoolDialogbar.h"
#include "MainProcessDlg.h"

class CMainFrame : public CMDIFrameWnd
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();

// Attributes
public:

// Operations
public:
	CChap2 *m_pChap2;
	CChap3 *m_pChap3;
	CChap4 *m_pChap4;
	CChap5 *m_pChap5;
	CApplication *m_pApplication;
	ImageStitch_jy *m_pImageStitching;
	CVideoStitch *m_pVideoStitch;

	CCoolDialogBar	m_MainProcessDlgBar;
	CMainProcessDlg m_MainProcessDlg;

	void CMainFrame::DlgPrintf(LPCTSTR ptr, ...);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnViewMainProcess();
	afx_msg void OnUpdateViewMainProcess(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__E18E3098_19D7_4881_B522_CC1C50720806__INCLUDED_)

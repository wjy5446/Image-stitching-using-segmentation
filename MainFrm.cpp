// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "ImageProcessing.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_COMMAND(IDD_VIEW_MAIN_PROCESS, OnViewMainProcess)
	ON_UPDATE_COMMAND_UI(IDD_VIEW_MAIN_PROCESS, OnUpdateViewMainProcess)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	m_pChap2 = new CChap2;
	m_pChap3 = new CChap3;
	m_pChap4 = new CChap4;
	m_pChap5 = new CChap5;
	m_pApplication = new CApplication;
	m_pImageStitching = new ImageStitch_jy;
	m_pVideoStitch = new CVideoStitch;
}

CMainFrame::~CMainFrame()
{
	delete m_pChap2;
	delete m_pChap3;
	delete m_pChap4;
	delete m_pChap5;
	delete m_pApplication;
	delete m_pImageStitching;
	delete m_pVideoStitch;
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	if (!m_MainProcessDlgBar.Create(this, &m_MainProcessDlg, CString("MainProcess"), 
		IDD_MAIN_PROCESS, WS_CHILD | WS_VISIBLE | CBRS_LEFT))
	{
		TRACE0("Failed to create dialogbar\n");
		return 0;      // fail to create
	}

    m_MainProcessDlgBar.SetBarStyle(m_MainProcessDlgBar.GetBarStyle()|CBRS_TOOLTIPS|CBRS_FLYBY|CBRS_SIZE_DYNAMIC);
	m_MainProcessDlgBar.EnableDocking(CBRS_ALIGN_LEFT);
	DockControlBar(&m_MainProcessDlgBar);

	ShowControlBar(&m_MainProcessDlgBar, TRUE, FALSE);
	//ShowControlBar(&m_MainProcessDlgBar, FALSE, FALSE);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	cs.style = WS_OVERLAPPED | WS_CAPTION | FWS_ADDTOTITLE
		| WS_THICKFRAME | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_MAXIMIZE;

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers


void CMainFrame::OnViewMainProcess() 
{
	if(!m_MainProcessDlgBar.IsWindowVisible())	
	{
		ShowControlBar(&m_MainProcessDlgBar, TRUE, FALSE);
	}
	else
	{
		ShowControlBar(&m_MainProcessDlgBar, FALSE, FALSE);
	}	
}

void CMainFrame::OnUpdateViewMainProcess(CCmdUI* pCmdUI) 
{
	if(!m_MainProcessDlgBar.IsWindowVisible())	
	{
		pCmdUI->SetCheck(FALSE);
	}
	else
	{
		pCmdUI->SetCheck(TRUE);
	}	
}

void CMainFrame::DlgPrintf(LPCTSTR ptr, ...)
{
	CString Buf;
	int Num;

	TCHAR ach[512];
	va_list args; 
	va_start(args, ptr); 
	_vstprintf(ach, ptr, args);
	Buf.Format(ach);
	m_MainProcessDlg.m_ListCtrl.AddString(Buf);
	Num = m_MainProcessDlg.m_ListCtrl.GetCount();
	m_MainProcessDlg.m_ListCtrl.SetCurSel(Num-1);
}
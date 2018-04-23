// SaveScreenOpt.cpp : implementation file
//

#include "stdafx.h"
#include "ImageProcessing.h"
#include "SaveScreenOpt.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSaveScreenOpt dialog


CSaveScreenOpt::CSaveScreenOpt(CWnd* pParent /*=NULL*/)
	: CDialog(CSaveScreenOpt::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSaveScreenOpt)
	m_nW = 640;
	m_nH = 480;
	//}}AFX_DATA_INIT
}


void CSaveScreenOpt::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSaveScreenOpt)
	DDX_Text(pDX, IDC_H, m_nH);
	DDX_Text(pDX, IDC_W, m_nW);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSaveScreenOpt, CDialog)
	//{{AFX_MSG_MAP(CSaveScreenOpt)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSaveScreenOpt message handlers

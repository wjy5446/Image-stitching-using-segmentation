// YuvOptionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ImageProcessing.h"
#include "YuvOptionDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CYuvOptionDlg dialog


CYuvOptionDlg::CYuvOptionDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CYuvOptionDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CYuvOptionDlg)
	m_nYuvType = 0;
	m_nW = 352;
	m_nH = 288;
	m_nScaleFormat = 1;
	//}}AFX_DATA_INIT
}


void CYuvOptionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CYuvOptionDlg)
	DDX_Control(pDX, IDC_H, m_CtrlH);
	DDX_Control(pDX, IDC_W, m_CtrlW);
	DDX_Radio(pDX, IDC_YUV_TYPE0, m_nYuvType);
	DDX_Text(pDX, IDC_W, m_nW);
	DDX_Text(pDX, IDC_H, m_nH);
	DDX_Radio(pDX, IDC_SCALE_FORMAT0, m_nScaleFormat);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CYuvOptionDlg, CDialog)
	//{{AFX_MSG_MAP(CYuvOptionDlg)
	ON_BN_CLICKED(IDC_SCALE_FORMAT0, OnScaleFormat0)
	ON_BN_CLICKED(IDC_SCALE_FORMAT1, OnScaleFormat1)
	ON_BN_CLICKED(IDC_SCALE_FORMAT2, OnScaleFormat2)
	ON_BN_CLICKED(IDC_SCALE_FORMAT3, OnScaleFormat3)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CYuvOptionDlg message handlers

void CYuvOptionDlg::OnScaleFormat0() 
{
	UpdateData(TRUE);
	
	ImageSizeFormat();
}

void CYuvOptionDlg::OnScaleFormat1() 
{
	UpdateData(TRUE);
	
	ImageSizeFormat();
}

void CYuvOptionDlg::OnScaleFormat2() 
{
	UpdateData(TRUE);
	
	ImageSizeFormat();
}

void CYuvOptionDlg::OnScaleFormat3() 
{
	UpdateData(TRUE);
	
	ImageSizeFormat();
}

void CYuvOptionDlg::ImageSizeFormat()
{
	m_CtrlW.EnableWindow(FALSE);
	m_CtrlH.EnableWindow(FALSE);

	if(m_nScaleFormat == 0) {
		m_nW = 176;
		m_nH = 144;
	}
	else if(m_nScaleFormat == 1) {
		m_nW = 352;
		m_nH = 288;
	}
	else if(m_nScaleFormat == 2) {
		m_nW = 528;
		m_nH = 384;
	}
	else
	{
		m_CtrlW.EnableWindow(TRUE);
		m_CtrlH.EnableWindow(TRUE);
	}

	UpdateData(FALSE);
}

BOOL CYuvOptionDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_CtrlW.EnableWindow(FALSE);
	m_CtrlH.EnableWindow(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

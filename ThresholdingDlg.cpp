// ThresholdingDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ImageProcessing.h"
#include "ThresholdingDlg.h"

#include "Image.h"		// 추가

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CThresholdingDlg dialog


CThresholdingDlg::CThresholdingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CThresholdingDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CThresholdingDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CThresholdingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CThresholdingDlg)
	DDX_Control(pDX, IDC_THRESHOLD, m_ThresholdCtrl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CThresholdingDlg, CDialog)
	//{{AFX_MSG_MAP(CThresholdingDlg)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_THRESHOLD, OnCustomdrawThreshold)
	ON_BN_CLICKED(IDC_CANCEL, OnCancel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CThresholdingDlg message handlers

BOOL CThresholdingDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// 임계치 범위 및 초기값 지정
	m_ThresholdCtrl.SetRange(0, 255);
	m_ThresholdCtrl.SetPos(128);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CThresholdingDlg::Set(BYTE **Input, BYTE **Output, int nW, int nH, int nPosX, int nPosY)
{
	m_InputGray = Input;
	m_OutputGray = Output;

	m_nW = nW;
	m_nH = nH;

	m_nPosX = nPosX;
	m_nPosY = nPosY;

	int x, y, i, MaxHis = -1;
	int Histogram[GRAY_CNT] = {0};

	for(y = 0 ; y < nH ; y++)
		for(x = 0 ; x < nW ; x++)
			Histogram[Input[y][x]]++;

	// 최대 도수값 추출
	for(i = 0 ; i < GRAY_CNT ; i++)
		if(Histogram[i] > MaxHis) MaxHis = Histogram[i];

	m_nHisW = 256;
	m_nHisH = 100;

	// 히스토그램 영상 메모리 할당
	m_HisImage = cmatrix(m_nHisH, m_nHisW);
	m_HisImageThre = cmatrix(m_nHisH, m_nHisW);

	// 히스토그램 영상 생성
	for(y = 0 ; y < m_nHisH ; y++)
		for(x = 0 ; x < m_nHisW ; x++)
		{
			m_HisImage[y][x] = (Histogram[x]*(m_nHisH-1)/MaxHis > m_nHisH-1-y) ? 0 : GRAY_CNT-1;
		}
}

void CThresholdingDlg::OnCancel() 
{
	// 히스토그램 영상 메모리 해제
	free_cmatrix(m_HisImage, m_nHisH, m_nHisW);
	free_cmatrix(m_HisImageThre, m_nHisH, m_nHisW);

	EndDialog(IDOK);
}

void CThresholdingDlg::OnCustomdrawThreshold(NMHDR* pNMHDR, LRESULT* pResult) 
{
	int nThre = m_ThresholdCtrl.GetPos();
	int x, y;

	// 임계치를 표시
	SetDlgItemInt(IDC_THRESHOLD_VALUE, nThre);

	// 이진화
	for(y = 0 ; y < m_nH ; y++)
		for(x = 0 ; x < m_nW ; x++)
			m_OutputGray[y][x] = (m_InputGray[y][x]>nThre) ? GRAY_CNT-1 : 0;

	DisplayCimage2D(m_OutputGray, m_nW, m_nH, m_nPosX+m_nW, m_nPosY, false, true);

	for(y = 0 ; y < m_nHisH ; y++)
		for(x = 0 ; x < m_nHisW ; x++)
			m_HisImageThre[y][x] = m_HisImage[y][x];

	for(y = 0 ; y < m_nHisH ; y++)
		m_HisImageThre[y][nThre] = 0;	
		
	DisplayCimage2DColor(m_HisImage, m_HisImageThre, m_HisImageThre, m_nHisW, m_nHisH, m_nPosX, m_nPosY+m_nH, false, true);
	
	*pResult = 0;
}

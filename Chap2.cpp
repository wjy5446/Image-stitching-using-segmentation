// Chap2.cpp : implementation file
//

#include "stdafx.h"
#include "ImageProcessing.h"
#include "Chap2.h"

#include "Image.h"		// 추가

#include "ThresholdingDlg.h"	// 추가

#include <cmath>			// 추가

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChap2 dialog


CChap2::CChap2(CWnd* pParent /*=NULL*/)
	: CDialog(CChap2::IDD, pParent)
{
	//{{AFX_DATA_INIT(CChap2)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_bCreated = FALSE;
}


void CChap2::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CChap2)
	DDX_Control(pDX, IDC_OPERATION_TYPE, m_OperationType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CChap2, CDialog)
	//{{AFX_MSG_MAP(CChap2)
	ON_BN_CLICKED(IDC_HISTOGRAM, OnHistogram)
	ON_BN_CLICKED(IDC_REVERSE, OnReverse)
	ON_BN_CLICKED(IDC_THRESHOLDING, OnThresholding)
	ON_BN_CLICKED(IDC_THRESHOLDING_DLG, OnThresholdingDlg)
	ON_BN_CLICKED(IDC_OPERATION, OnOperation)
	ON_BN_CLICKED(IDC_MOVE, OnMove)
	ON_BN_CLICKED(IDC_SCALE_X2, OnScaleX2)
	ON_BN_CLICKED(IDC_ROTATE, OnRotate)
	ON_BN_CLICKED(IDC_CANCEL, OnCancel)
	ON_BN_CLICKED(IDC_DITHERING, OnDithering)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChap2 message handlers

void CChap2::OnCancel() 
{
	DestroyWindow();	
	m_bCreated = FALSE;
}

BOOL CChap2::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	SetDlgItemInt(IDC_THRESHOLD_VALUE, 128);	

	m_OperationType.InsertString(0, _T("합"));
	m_OperationType.InsertString(1, _T("차"));
	m_OperationType.InsertString(2, _T("AND"));
	m_OperationType.InsertString(3, _T("OR"));

	m_OperationType.SetCurSel(0);

	SetDlgItemInt(IDC_X0, 50);
	SetDlgItemInt(IDC_Y0, 50);

	SetDlgItemInt(IDC_DEG, 30);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CChap2::OnHistogram() 
{
	int nW, nH, nPosX, nPosY;
	BYTE **ImageGray;
	int Histogram[GRAY_CNT] = {0};
	int x, y, i, MaxHis = -1;

	// 영상 정보 읽기
	if(!GetCurrentImageInfo(&nW, &nH, &nPosX, &nPosY)) return;

	ImageGray = cmatrix(nH, nW);

	// 회색조 영상 읽기
	GetCurrentImageGray(ImageGray);

	// 히스토그램 계산
	for(y = 0 ; y < nH ; y++)
		for(x = 0 ; x < nW ; x++)
			Histogram[ImageGray[y][x]]++;

	// 최대 도수값 추출
	for(i = 0 ; i < GRAY_CNT ; i++)
		if(Histogram[i] > MaxHis) MaxHis = Histogram[i];

	int nHisW = GRAY_CNT, nHisH = 100;
	BYTE **HisImage;

	// 히스토그램 영상 초기화
	HisImage = cmatrix(nHisH, nHisW);

	// 히스토그램 영상 생성
	for(y = 0 ; y < nHisH ; y++)
		for(x = 0 ; x < nHisW ; x++)
		{
			HisImage[y][x] = (Histogram[x]*(nHisH-1)/MaxHis > nHisH-1-y) ? 0 : GRAY_CNT-1;
		}

	// 히스토그램 영상 출력
	DisplayCimage2D(HisImage, nHisW, nHisH, nPosX, nPosY+nH);
	
	free_cmatrix(HisImage, nHisH, nHisW);
	free_cmatrix(ImageGray, nH, nW);	
}



void CChap2::OnReverse() 
{
	int nW, nH, nPosX, nPosY;
	BYTE **ImageGray;
	int x, y;

	// 영상 정보 읽기
	if(!GetCurrentImageInfo(&nW, &nH, &nPosX, &nPosY)) return;

	ImageGray = cmatrix(nH, nW);

	// 회색조 영상 읽기
	GetCurrentImageGray(ImageGray);

	// 역영상 계산
	for(y = 0 ; y < nH ; y++)
		for(x = 0 ; x < nW ; x++)
			ImageGray[y][x] = ~ImageGray[y][x];


	DisplayCimage2D(ImageGray, nW, nH, nPosX+nW, nPosY);
	
	free_cmatrix(ImageGray, nH, nW);	
}

void CChap2::OnThresholding() 
{
	int nW, nH, nPosX, nPosY;
	BYTE **ImageGray;
	int x, y;
	int Thre;

	// 영상 정보 읽기
	if(!GetCurrentImageInfo(&nW, &nH, &nPosX, &nPosY)) return;

	ImageGray = cmatrix(nH, nW);

	// 회색조 영상 읽기
	GetCurrentImageGray(ImageGray);

	Thre = GetDlgItemInt(IDC_THRESHOLD_VALUE);
	// 이진화
	for(y = 0 ; y < nH ; y++)
		for(x = 0 ; x < nW ; x++)
			ImageGray[y][x] = (ImageGray[y][x]>Thre) ? GRAY_CNT-1 : 0;

	DisplayCimage2D(ImageGray, nW, nH, nPosX+nW, nPosY);
	
	free_cmatrix(ImageGray, nH, nW);	
}

void CChap2::OnThresholdingDlg() 
{
	int nW, nH, nPosX, nPosY;
	BYTE **ImageGray, **OutputGray;

	// 영상 정보 읽기
	if(!GetCurrentImageInfo(&nW, &nH, &nPosX, &nPosY)) return;

	ImageGray = cmatrix(nH, nW);
	OutputGray = cmatrix(nH, nW);

	// 회색조 영상 읽기
	GetCurrentImageGray(ImageGray);

	CThresholdingDlg dlg;
	
	dlg.Set(ImageGray, OutputGray, nW, nH, nPosX, nPosY);
	dlg.DoModal();	

	free_cmatrix(ImageGray, nH, nW);	
	free_cmatrix(OutputGray, nH, nW);	
}

void CChap2::OnOperation() 
{
	ImageOperation(m_OperationType.GetCurSel());
}

void CChap2::OnMove() 
{
	int nW, nH, nPosX, nPosY;
	BYTE **ImageGray, **OutputGray;

	// 영상 정보 읽기
	if(!GetCurrentImageInfo(&nW, &nH, &nPosX, &nPosY)) return;

	ImageGray = cmatrix(nH, nW);
	OutputGray = cmatrix(nH, nW);

	// 회색조 영상 읽기
	GetCurrentImageGray(ImageGray);

	int x0 = GetDlgItemInt(IDC_X0);
	int y0 = GetDlgItemInt(IDC_Y0);

	Move(ImageGray, OutputGray, nW, nH, x0, y0);

	DisplayCimage2D(OutputGray, nW, nH, nPosX+nW, nPosY);

	free_cmatrix(ImageGray, nH, nW);	
	free_cmatrix(OutputGray, nH, nW);	
}

void CChap2::OnScaleX2() 
{
	int nW, nH, nPosX, nPosY;
	BYTE **ImageGray, **OutputGray;

	// 영상 정보 읽기
	if(!GetCurrentImageInfo(&nW, &nH, &nPosX, &nPosY)) return;

	ImageGray = cmatrix(nH, nW);
	OutputGray = cmatrix(nH*2, nW*2);

	// 회색조 영상 읽기
	GetCurrentImageGray(ImageGray);

	ScaleX2(ImageGray, OutputGray, nW, nH);

	DisplayCimage2D(OutputGray, nW*2, nH*2, nPosX+nW, nPosY);

	free_cmatrix(ImageGray, nH, nW);	
	free_cmatrix(OutputGray, nH*2, nW*2);	
}

void CChap2::OnRotate() 
{
	int nW, nH, nPosX, nPosY;
	BYTE **ImageGray, **OutputGray;

	// 영상 정보 읽기
	if(!GetCurrentImageInfo(&nW, &nH, &nPosX, &nPosY)) return;

	double dDeg = GetDlgItemInt(IDC_DEG);
	double dAng = dDeg * acos(-1.) / 180.;	// 각도를 라디안으로 수정

	int nOutW = (int)(nW*fabs(cos(dAng)) + nH*fabs(sin(dAng)));
	int nOutH = (int)(nW*fabs(sin(dAng)) + nH*fabs(cos(dAng)));

	ImageGray = cmatrix(nH, nW);
	OutputGray = cmatrix(nOutH, nOutW);

	// 회색조 영상 읽기
	GetCurrentImageGray(ImageGray);

	Rotate(ImageGray, OutputGray, nW, nH, nOutW, nOutH, dDeg);

	DisplayCimage2D(OutputGray, nOutW, nOutH, nPosX+nW, nPosY);

	free_cmatrix(ImageGray, nH, nW);	
	free_cmatrix(OutputGray, nOutH, nOutW);
}

void CChap2::OnDithering() 
{
	int nW, nH, nPosX, nPosY;
	BYTE **ImageGray, **OutputGray;

	// 영상 정보 읽기
	if(!GetCurrentImageInfo(&nW, &nH, &nPosX, &nPosY)) return;

	ImageGray = cmatrix(nH, nW);
	OutputGray = cmatrix(nH, nW);

	// 회색조 영상 읽기
	GetCurrentImageGray(ImageGray);

	int x, y;

	BYTE Dither2[2][2] = {{0, 128}, {192, 64}};

	// 2*2 디더링 패턴에 의한 디더링
	for(y = 0 ; y < nH ; y++)
		for(x = 0 ; x < nW ; x++)
		{
			if(ImageGray[y][x] > Dither2[y%2][x%2]) OutputGray[y][x] = 255;
			else OutputGray[y][x] = 0;
		}

	DisplayCimage2D(OutputGray, nW, nH, nPosX+nW, nPosY);

	BYTE Dither4[4][4] = {{0, 128, 32, 160}, {192, 64, 224, 96}, {48, 176, 16, 144}, {240, 112, 208, 80}};

	// 4*4 디더링 패턴에 의한 디더링
	for(y = 0 ; y < nH ; y++)
		for(x = 0 ; x < nW ; x++)
		{
			if(ImageGray[y][x] > Dither4[y%4][x%4]) OutputGray[y][x] = 255;
			else OutputGray[y][x] = 0;
		}

	DisplayCimage2D(OutputGray, nW, nH, nPosX+nW*2, nPosY);

	free_cmatrix(ImageGray, nH, nW);	
	free_cmatrix(OutputGray, nH, nW);	
}

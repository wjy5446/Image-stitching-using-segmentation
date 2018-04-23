// Chap3.cpp : implementation file
//

#include "stdafx.h"
#include "ImageProcessing.h"
#include "Chap3.h"

#include "Image.h"		// 추가

#include <cmath>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChap3 dialog


CChap3::CChap3(CWnd* pParent /*=NULL*/)
	: CDialog(CChap3::IDD, pParent)
{
	//{{AFX_DATA_INIT(CChap3)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_bCreated = FALSE;
}


void CChap3::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CChap3)
	DDX_Control(pDX, IDC_FILTER_TYPE2, m_FilterType2Ctrl);
	DDX_Control(pDX, IDC_FILTER_TYPE, m_FilterTypeCtrl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CChap3, CDialog)
	//{{AFX_MSG_MAP(CChap3)
	ON_BN_CLICKED(IDC_HISTOGRAM_EQ, OnHistogramEq)
	ON_BN_CLICKED(IDC_MEAN, OnMean)
	ON_BN_CLICKED(IDC_MEDIAN, OnMedian)
	ON_BN_CLICKED(IDC_SOBEL, OnSobel)
	ON_BN_CLICKED(IDC_LAPLACIAN, OnLaplacian)
	ON_BN_CLICKED(IDC_LAPLACIAN_SHARP, OnLaplacianSharp)
	ON_BN_CLICKED(IDC_DFT, OnDft)
	ON_BN_CLICKED(IDC_FFT, OnFft)
	ON_BN_CLICKED(IDC_DFT_ROW_COL, OnDftRowCol)
	ON_BN_CLICKED(IDC_FILTERING, OnFiltering)
	ON_BN_CLICKED(IDC_CANCEL, OnCancel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChap3 message handlers
BOOL CChap3::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	SetDlgItemInt(IDC_WINDOW_SIZE, 3);
	
	m_FilterTypeCtrl.InsertString(0, _T("Ideal"));
	m_FilterTypeCtrl.InsertString(1, _T("Butterworth"));

	m_FilterTypeCtrl.SetCurSel(0);

	m_FilterType2Ctrl.InsertString(0, _T("LPF"));
	m_FilterType2Ctrl.InsertString(1, _T("HPF"));

	m_FilterType2Ctrl.SetCurSel(0);

	SetDlgItemInt(IDC_N, 2);
	SetDlgItemInt(IDC_DC, 100);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CChap3::OnCancel() 
{
	DestroyWindow();	
	m_bCreated = FALSE;
}

void CChap3::OnHistogramEq() 
{
	int nW, nH, nPosX, nPosY;
	BYTE **ImageGray, **OutputGray;

	// 영상 정보 읽기
	if(!GetCurrentImageInfo(&nW, &nH, &nPosX, &nPosY)) return;

	ImageGray = cmatrix(nH, nW);
	OutputGray = cmatrix(nH, nW);

	// 회색조 영상 읽기
	GetCurrentImageGray(ImageGray);

	HistogramEqualization(ImageGray, OutputGray, nW, nH);

	DisplayCimage2D(OutputGray, nW, nH, nPosX+nW, nPosY);

	free_cmatrix(ImageGray, nH, nW);	
	free_cmatrix(OutputGray, nH, nW);
}

void CChap3::OnMean() 
{
	int nW, nH, nPosX, nPosY;
	BYTE **ImageGray, **OutputGray;

	// 영상 정보 읽기
	if(!GetCurrentImageInfo(&nW, &nH, &nPosX, &nPosY)) return;

	ImageGray = cmatrix(nH, nW);
	OutputGray = cmatrix(nH, nW);

	// 회색조 영상 읽기
	GetCurrentImageGray(ImageGray);

	int nWinSize = GetDlgItemInt(IDC_WINDOW_SIZE);

	MeanFilter(ImageGray, OutputGray, nW, nH, nWinSize);

	DisplayCimage2D(OutputGray, nW, nH, nPosX+nW, nPosY);

	free_cmatrix(ImageGray, nH, nW);	
	free_cmatrix(OutputGray, nH, nW);
}


void CChap3::OnMedian() 
{
	int nW, nH, nPosX, nPosY;
	BYTE **ImageGray, **OutputGray;

	// 영상 정보 읽기
	if(!GetCurrentImageInfo(&nW, &nH, &nPosX, &nPosY)) return;

	ImageGray = cmatrix(nH, nW);
	OutputGray = cmatrix(nH, nW);

	// 회색조 영상 읽기
	GetCurrentImageGray(ImageGray);

	int nWinSize = GetDlgItemInt(IDC_WINDOW_SIZE);

	MedianFilter(ImageGray, OutputGray, nW, nH, nWinSize);

	DisplayCimage2D(OutputGray, nW, nH, nPosX+nW, nPosY);

	free_cmatrix(ImageGray, nH, nW);	
	free_cmatrix(OutputGray, nH, nW);	
}

void CChap3::OnSobel() 
{
	int nW, nH, nPosX, nPosY;
	BYTE **ImageGray, **OutputGray;

	// 영상 정보 읽기
	if(!GetCurrentImageInfo(&nW, &nH, &nPosX, &nPosY)) return;

	ImageGray = cmatrix(nH, nW);
	OutputGray = cmatrix(nH, nW);

	// 회색조 영상 읽기
	GetCurrentImageGray(ImageGray);

	Sobel(ImageGray, OutputGray, nW, nH);

	DisplayCimage2D(OutputGray, nW, nH, nPosX+nW, nPosY);

	free_cmatrix(ImageGray, nH, nW);	
	free_cmatrix(OutputGray, nH, nW);	
}

void CChap3::OnLaplacian() 
{
	int nW, nH, nPosX, nPosY;
	BYTE **ImageGray, **OutputGray;

	// 영상 정보 읽기
	if(!GetCurrentImageInfo(&nW, &nH, &nPosX, &nPosY)) return;

	ImageGray = cmatrix(nH, nW);
	OutputGray = cmatrix(nH, nW);

	// 회색조 영상 읽기
	GetCurrentImageGray(ImageGray);

	Laplacian(ImageGray, OutputGray, nW, nH);

	DisplayCimage2D(OutputGray, nW, nH, nPosX+nW, nPosY);

	free_cmatrix(ImageGray, nH, nW);	
	free_cmatrix(OutputGray, nH, nW);	
}

void CChap3::OnLaplacianSharp() 
{
	int nW, nH, nPosX, nPosY;
	BYTE **ImageGray, **OutputGray;

	// 영상 정보 읽기
	if(!GetCurrentImageInfo(&nW, &nH, &nPosX, &nPosY)) return;

	ImageGray = cmatrix(nH, nW);
	OutputGray = cmatrix(nH, nW);

	// 회색조 영상 읽기
	GetCurrentImageGray(ImageGray);

	LaplacianSharp(ImageGray, OutputGray, nW, nH);

	DisplayCimage2D(OutputGray, nW, nH, nPosX+nW, nPosY);

	free_cmatrix(ImageGray, nH, nW);	
	free_cmatrix(OutputGray, nH, nW);
}

void CChap3::OnDft() 
{
	int nW, nH, nPosX, nPosY;
	BYTE **ImageGray;
	double **OutputDouble;
	double **OutputReal, **OutputImag;

	// 영상 정보 읽기
	if(!GetCurrentImageInfo(&nW, &nH, &nPosX, &nPosY)) return;

	ImageGray = cmatrix(nH, nW);
	OutputDouble = dmatrix(nH, nW);
	OutputReal = dmatrix(nH, nW);
	OutputImag = dmatrix(nH, nW);

	// 회색조 영상 읽기
	GetCurrentImageGray(ImageGray);

	double time = DFT2D(ImageGray, OutputReal, OutputImag, nW, nH);
	CString ViewMsg;
	ViewMsg.Format(_T("Processing time: %7.3lfsec"), time);
	SetViewMsg(ViewMsg);

	int u, v;
	for(v = 0 ; v < nH ; v++)
		for(u = 0 ; u < nW ; u++)
		{
			OutputDouble[(v+nH/2)%nH][(u+nW/2)%nW] 
				= log(sqrt(OutputReal[v][u]*OutputReal[v][u] + OutputImag[v][u]*OutputImag[v][u])+1.);
		}

	DisplayDimage2D(OutputDouble, nW, nH, nPosX+nW, nPosY);

	free_cmatrix(ImageGray, nH, nW);	
	free_dmatrix(OutputDouble, nH, nW);
	free_dmatrix(OutputReal, nH, nW);
	free_dmatrix(OutputImag, nH, nW);
}

void CChap3::OnDftRowCol() 
{
	int nW, nH, nPosX, nPosY;
	BYTE **ImageGray;
	double **OutputDouble;
	double **OutputReal, **OutputImag;

	// 영상 정보 읽기
	if(!GetCurrentImageInfo(&nW, &nH, &nPosX, &nPosY)) return;

	ImageGray = cmatrix(nH, nW);
	OutputDouble = dmatrix(nH, nW);
	OutputReal = dmatrix(nH, nW);
	OutputImag = dmatrix(nH, nW);

	// 회색조 영상 읽기
	GetCurrentImageGray(ImageGray);

	double time = RowColDFT2D(ImageGray, OutputReal, OutputImag, nW, nH);
	CString ViewMsg;
	ViewMsg.Format(_T("Processing time: %7.3lfsec"), time);
	SetViewMsg(ViewMsg);

	int u, v;
	for(v = 0 ; v < nH ; v++)
		for(u = 0 ; u < nW ; u++)
		{
			OutputDouble[(v+nH/2)%nH][(u+nW/2)%nW] 
				= log(sqrt(OutputReal[v][u]*OutputReal[v][u] + OutputImag[v][u]*OutputImag[v][u])+1.);
		}

	DisplayDimage2D(OutputDouble, nW, nH, nPosX+nW, nPosY);

	free_cmatrix(ImageGray, nH, nW);	
	free_dmatrix(OutputDouble, nH, nW);
	free_dmatrix(OutputReal, nH, nW);
	free_dmatrix(OutputImag, nH, nW);
}

void CChap3::OnFft() 
{
	int nW, nH, nPosX, nPosY;
	BYTE **ImageGray;
	double **OutputDouble;
	double **OutputReal, **OutputImag;

	// 영상 정보 읽기
	if(!GetCurrentImageInfo(&nW, &nH, &nPosX, &nPosY)) return;

	int Radix2N;

	Radix2N = 1;
	while(true)	{
		Radix2N <<= 1;
		if(Radix2N >= nW) break;
	}
	
	if(Radix2N != nW) return;

	Radix2N = 1;
	while(true)	{
		Radix2N <<= 1;
		if(Radix2N >= nH) break;
	}
	
	if(Radix2N != nH) return;

	ImageGray = cmatrix(nH, nW);
	OutputDouble = dmatrix(nH, nW);
	OutputReal = dmatrix(nH, nW);
	OutputImag = dmatrix(nH, nW);

	// 회색조 영상 읽기
	GetCurrentImageGray(ImageGray);

	double time = FFT2D(ImageGray, OutputReal, OutputImag, nW, nH);
	CString ViewMsg;
	ViewMsg.Format(_T("Processing time: %7.3lfsec"), time);
	SetViewMsg(ViewMsg);

	int u, v;
	for(v = 0 ; v < nH ; v++)
		for(u = 0 ; u < nW ; u++)
		{
			OutputDouble[(v+nH/2)%nH][(u+nW/2)%nW] 
				= log(sqrt(OutputReal[v][u]*OutputReal[v][u] + OutputImag[v][u]*OutputImag[v][u])+1.);
		}

	DisplayDimage2D(OutputDouble, nW, nH, nPosX+nW, nPosY);

	free_cmatrix(ImageGray, nH, nW);	
	free_dmatrix(OutputDouble, nH, nW);
	free_dmatrix(OutputReal, nH, nW);
	free_dmatrix(OutputImag, nH, nW);
}

void CChap3::OnFiltering() 
{
	int nFilterType1 = m_FilterTypeCtrl.GetCurSel();
	int nFilterType2 = m_FilterType2Ctrl.GetCurSel();

	int nDC = GetDlgItemInt(IDC_DC);
	int nN = GetDlgItemInt(IDC_N);

	int nW, nH, nPosX, nPosY;
	BYTE **ImageGray;
	double **OutputDouble;
	
	// 영상 정보 읽기
	if(!GetCurrentImageInfo(&nW, &nH, &nPosX, &nPosY)) return;

	ImageGray = cmatrix(nH, nW);
	OutputDouble = dmatrix(nH, nW);

	// 회색조 영상 읽기
	GetCurrentImageGray(ImageGray);

	Filtering(ImageGray, OutputDouble, nW, nH, nPosX, nPosY, nFilterType1, nFilterType2, nDC, nN); 

	DisplayDimage2D(OutputDouble, nW, nH, nPosX+nW, nPosY);

	free_cmatrix(ImageGray, nH, nW);	
	free_dmatrix(OutputDouble, nH, nW);
}

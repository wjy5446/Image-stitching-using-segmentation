// Chap4.cpp : implementation file
//

#include "stdafx.h"
#include "ImageProcessing.h"
#include "Chap4.h"

#include "Image.h"		// 추가
#include <cmath>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChap4 dialog


CChap4::CChap4(CWnd* pParent /*=NULL*/)
	: CDialog(CChap4::IDD, pParent)
{
	//{{AFX_DATA_INIT(CChap4)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_bCreated = FALSE;
}


void CChap4::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CChap4)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CChap4, CDialog)
	//{{AFX_MSG_MAP(CChap4)
	ON_BN_CLICKED(IDC_DCT, OnDct)
	ON_BN_CLICKED(IDC_CANCEL, OnCancel)
	ON_BN_CLICKED(IDC_COMPRESSION, OnCompression)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_REVERSE, &CChap4::OnBnClickedReverse)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChap4 message handlers

BOOL CChap4::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	SetDlgItemInt(IDC_BLOCK_SIZE, 8);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CChap4::OnCancel() 
{
	DestroyWindow();	
	m_bCreated = FALSE;
}


void CChap4::OnDct() 
{
	int nW, nH, nPosX, nPosY;
	BYTE **ImageGray;
	double **OutputDouble;

	int nBlockSize = GetDlgItemInt(IDC_BLOCK_SIZE);

	// 영상 정보 읽기
	if(!GetCurrentImageInfo(&nW, &nH, &nPosX, &nPosY)) return;

	ImageGray = cmatrix(nH, nW);
	OutputDouble = dmatrix(nH, nW);

	// 회색조 영상 읽기
	GetCurrentImageGray(ImageGray);

	DCT2D(ImageGray, OutputDouble, nW, nH, nBlockSize);

	int u, v;
	for(v = 0 ; v < nH ; v++)
		for(u = 0 ; u < nW ; u++)
		{
			OutputDouble[v][u]
				= log(fabs(OutputDouble[v][u])+1.);
		}
	
	DisplayDimage2D(OutputDouble, nW, nH, nPosX+nW, nPosY);

	free_cmatrix(ImageGray, nH, nW);	
	free_dmatrix(OutputDouble, nH, nW);	
}

void CChap4::OnCompression() 
{
	int nW, nH, nPosX, nPosY;
	BYTE **ImageGray;
	BYTE **OutputGray;

	int nBlockSize = GetDlgItemInt(IDC_BLOCK_SIZE);

	// 영상 정보 읽기
	if(!GetCurrentImageInfo(&nW, &nH, &nPosX, &nPosY)) return;

	ImageGray = cmatrix(nH, nW);
	OutputGray = cmatrix(nH, nW);

	// 회색조 영상 읽기
	GetCurrentImageGray(ImageGray);

	Compression(ImageGray, OutputGray, nW, nH);
	
	DisplayCimage2D(OutputGray, nW, nH, nPosX+nW, nPosY);

	free_cmatrix(ImageGray, nH, nW);	
	free_cmatrix(OutputGray, nH, nW);	
	
}


void CChap4::OnBnClickedReverse() //좌우반전
{
	int nW, nH, nPosX, nPosY;
	BYTE **ImageR,**ImageG,**ImageB;
	BYTE **OutImageR,**OutImageG,**OutImageB;

	// 영상 정보 읽기
	if(!GetCurrentImageInfo(&nW, &nH, &nPosX, &nPosY)) return;


	ImageR = cmatrix(nH, nW);
	ImageG = cmatrix(nH, nW);
	ImageB = cmatrix(nH, nW);
	OutImageR = cmatrix(nH, nW);
	OutImageG = cmatrix(nH, nW);
	OutImageB = cmatrix(nH, nW);

	
	//컬러영상 읽기
	GetCurrentImageColor(ImageR, ImageG, ImageB);


	//반전 수행
	for(int y = 0;y<nH;y++)
		for(int x =0;x<nW;x++){
			OutImageR[y][nW-1-x] = ImageR[y][x];
			OutImageG[y][nW-1-x] = ImageG[y][x];
			OutImageB[y][nW-1-x] = ImageB[y][x];

		}



	

	DisplayCimage2DColor(OutImageR, OutImageG,OutImageB,  nW,  nH, 
	nPosX+nW, nPosY ); //2D 컬러영상 출력

	free_cmatrix(ImageR, nH, nW);	
	free_cmatrix(ImageG, nH, nW);
	free_cmatrix(ImageB, nH, nW);
	free_cmatrix(OutImageR, nH, nW);
	free_cmatrix(OutImageG, nH, nW);
	free_cmatrix(OutImageB, nH, nW);
	// TODO: Add your control notification handler code here
}

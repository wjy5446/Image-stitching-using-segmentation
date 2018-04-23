// VideoStitch.cpp : implementation file
//


#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")


#include "stdafx.h"
#include "ImageProcessing.h"
#include "DlgVideoStitch.h"
#include "afxdialogex.h"
#include "Vfw.h"
#include "Image.h"
#include "StitchBasic.h"
#include "time.h"
#include <fstream>

// CVideoStitch dialog

IMPLEMENT_DYNAMIC(CVideoStitch, CDialog)

CVideoStitch::CVideoStitch(CWnd* pParent /*=NULL*/)
	: CDialog(CVideoStitch::IDD, pParent)
{
	m_bCreated = false;
	m_nCurrentProcessingNum = 0;
}

CVideoStitch::~CVideoStitch()
{
	m_bCreated = false;

	AVIStreamGetFrameClose(m_pfrm_L);
	AVIStreamRelease(m_pstm_L);
	AVIFileRelease(m_pavi_L);

	AVIStreamGetFrameClose(m_pfrm_R);
	AVIStreamRelease(m_pstm_R);
	AVIFileRelease(m_pavi_R);
}

void CVideoStitch::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CVideoStitch, CDialog)
	ON_BN_CLICKED(IDC_SEL_SOURCE_L, &CVideoStitch::OnBnClickedSelSourceL)
	ON_BN_CLICKED(IDC_CANCEL, &CVideoStitch::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_SEL_SOURCE_R, &CVideoStitch::OnBnClickedSelSourceR)
	ON_BN_CLICKED(IDC_VIDEOSTITCH, &CVideoStitch::OnBnClickedVideostitch)
END_MESSAGE_MAP()

void CVideoStitch::GetImg2DSize(PGETFRAME pfrm, int currentFrmNum, int* nH, int* nW)
{
	LPBITMAPINFOHEADER pbmih;
	pbmih = (LPBITMAPINFOHEADER)AVIStreamGetFrame(pfrm, currentFrmNum);

	*nH = pbmih->biHeight;
	*nW = pbmih->biWidth;
}

void CVideoStitch::GetImg2DColor(BYTE** img2D_R, BYTE** img2D_G, BYTE** img2D_B, PGETFRAME pfrm, int currentFrmNum)
{
	LPBITMAPINFOHEADER pbmih;
	pbmih = (LPBITMAPINFOHEADER)AVIStreamGetFrame(pfrm, currentFrmNum);
	
	BYTE *image = (unsigned char *)((LPSTR)pbmih + (WORD)pbmih->biSize);

	//			int x, y, offset;
	BYTE *Image1D = NULL;
	
	if (pbmih->biBitCount == 24 || pbmih->biBitCount == 32 || pbmih->biBitCount == 16)
	{
		Image1D = new BYTE[GetBmp24Size(pbmih->biWidth, pbmih->biHeight)];

		if (pbmih->biBitCount == 24)
		{
			memcpy(Image1D, image, GetBmp24Size(pbmih->biWidth, pbmih->biHeight));
		}
		else if (pbmih->biBitCount == 32)
		{
			int x, y, offset1, offset2;
			for (y = 0; y < pbmih->biHeight; y++)
				for (x = 0; x < pbmih->biWidth; x++)
				{
					offset1 = (y*pbmih->biWidth + x)*pbmih->biBitCount / 8;
					offset2 = (y*pbmih->biWidth + x) * 24 / 8;
					memcpy(Image1D + offset2, image + offset1, 3);
				}
		}
		else if (pbmih->biBitCount == 16)
		{
			int x, y, offset1, offset2;
			for (y = 0; y < pbmih->biHeight; y++)
				for (x = 0; x < pbmih->biWidth; x++)
				{
					offset1 = (y*pbmih->biWidth * 2 + 3) / 4 * 4 + x * 2;

					offset2 = (y*pbmih->biWidth + x) * 24 / 8;

					WORD img = *(WORD *)(image + offset1);

					if (pbmih->biCompression == BI_BITFIELDS)
					{
						Image1D[offset2++] = (BYTE)(((img >> 0) & 0x001F) * 8);
						Image1D[offset2++] = (BYTE)(((img >> 5) & 0x003F) * 4);
						Image1D[offset2] = (BYTE)(((img >> 11) & 0x001F) * 8);
					}
					else
					{
						Image1D[offset2++] = (BYTE)(((img >> 0) & 0x001F) * 8);
						Image1D[offset2++] = (BYTE)(((img >> 5) & 0x001F) * 8);
						Image1D[offset2] = (BYTE)(((img >> 10) & 0x001F) * 8);
					}
				}
		}
	}

	DisplayCimage1D(Image1D, pbmih->biWidth, pbmih->biHeight, 0, 0, false, true);

	MakeImage1DToImage2DColor(Image1D, 0, pbmih->biWidth, pbmih->biHeight, img2D_R, img2D_G, img2D_B);

	delete[] Image1D;
}

BYTE* CVideoStitch::GetImg1D(PGETFRAME pfrm, int currentFrmNum)
{
	LPBITMAPINFOHEADER pbmih;
	pbmih = (LPBITMAPINFOHEADER)AVIStreamGetFrame(pfrm, currentFrmNum);

	BYTE *image = (unsigned char *)((LPSTR)pbmih + (WORD)pbmih->biSize);

	//			int x, y, offset;
	BYTE* Image1D = NULL;

	if (pbmih->biBitCount == 24 || pbmih->biBitCount == 32 || pbmih->biBitCount == 16)
	{
		Image1D = new BYTE[GetBmp24Size(pbmih->biWidth, pbmih->biHeight)];

		if (pbmih->biBitCount == 24)
		{
			memcpy(Image1D, image, GetBmp24Size(pbmih->biWidth, pbmih->biHeight));
		}
		else if (pbmih->biBitCount == 32)
		{
			int x, y, offset1, offset2;
			for (y = 0; y < pbmih->biHeight; y++)
				for (x = 0; x < pbmih->biWidth; x++)
				{
					offset1 = (y*pbmih->biWidth + x)*pbmih->biBitCount / 8;
					offset2 = (y*pbmih->biWidth + x) * 24 / 8;
					memcpy(Image1D + offset2, image + offset1, 3);
				}
		}
		else if (pbmih->biBitCount == 16)
		{
			int x, y, offset1, offset2;
			for (y = 0; y < pbmih->biHeight; y++)
				for (x = 0; x < pbmih->biWidth; x++)
				{
					offset1 = (y*pbmih->biWidth * 2 + 3) / 4 * 4 + x * 2;

					offset2 = (y*pbmih->biWidth + x) * 24 / 8;

					WORD img = *(WORD *)(image + offset1);

					if (pbmih->biCompression == BI_BITFIELDS)
					{
						Image1D[offset2++] = (BYTE)(((img >> 0) & 0x001F) * 8);
						Image1D[offset2++] = (BYTE)(((img >> 5) & 0x003F) * 4);
						Image1D[offset2] = (BYTE)(((img >> 11) & 0x001F) * 8);
					}
					else
					{
						Image1D[offset2++] = (BYTE)(((img >> 0) & 0x001F) * 8);
						Image1D[offset2++] = (BYTE)(((img >> 5) & 0x001F) * 8);
						Image1D[offset2] = (BYTE)(((img >> 10) & 0x001F) * 8);
					}
				}
		}
	}

	return Image1D;
}

// CVideoStitch message handlers


void CVideoStitch::OnBnClickedSelSourceL()
{
	CFileDialog dlg(TRUE, _T("*.avi"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("AVI 파일(*.avi)|*.avi||"));

	if (dlg.DoModal() == IDOK)
	{
		CString FileName = dlg.GetPathName();
		SetDlgItemText(IDC_DISP_SOURCEL, FileName);

		GetDlgItemText(IDC_DISP_SOURCEL, m_AviFileName_L);

		if (m_AviFileName_L.IsEmpty())
		{
			return;
		}

		if (AVIFileOpen(&m_pavi_L, m_AviFileName_L, OF_READ | OF_SHARE_DENY_NONE, NULL) != 0)
		{
			return;
		}

		AVIFileInfo(m_pavi_L, &m_fi_L, sizeof(AVIFILEINFO));

		UINT stream;

		for (stream = 0; stream < m_fi_L.dwStreams; stream++)
		{
			AVIFileGetStream(m_pavi_L, &m_pstm_L, 0, stream);
			AVIStreamInfo(m_pstm_L, &m_si_L, sizeof(AVISTREAMINFO));

			if (m_si_L.fccType == streamtypeVIDEO)
			{
				m_pfrm_L = AVIStreamGetFrameOpen(m_pstm_L, (LPBITMAPINFOHEADER)AVIGETFRAMEF_BESTDISPLAYFMT);
				m_nAviLength_L = AVIStreamLength(m_pstm_L);
				break;
			}
		}

		if (stream == m_fi_L.dwStreams || m_pfrm_L == NULL)
		{
			AVIStreamRelease(m_pstm_L);
			AVIFileRelease(m_pavi_L);

			return;
		}

		m_nStart_L = 0;
		m_nEnd_L = m_nAviLength_L - 1;

		UpdateData(FALSE);

		CString DispFrameNum;
		DispFrameNum.Format(_T("(0,%d)"), m_nAviLength_L - 1);
		SetDlgItemText(IDC_TOTAL_FRAME, DispFrameNum);

		///////////////////
	}
}

void CVideoStitch::OnBnClickedSelSourceR()
{
	CFileDialog dlg(TRUE, _T("*.avi"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("AVI 파일(*.avi)|*.avi||"));

	if (dlg.DoModal() == IDOK)
	{
		CString FileName = dlg.GetPathName();
		SetDlgItemText(IDC_DISP_SOURCER, FileName);

		GetDlgItemText(IDC_DISP_SOURCER, m_AviFileName_R);

		if (m_AviFileName_R.IsEmpty())
		{
			return;
		}

		if (AVIFileOpen(&m_pavi_R, m_AviFileName_R, OF_READ | OF_SHARE_DENY_NONE, NULL) != 0)
		{
			return;
		}
		AVIFileInfo(m_pavi_R, &m_fi_R, sizeof(AVIFILEINFO));

		UINT stream;

		for (stream = 0; stream < m_fi_R.dwStreams; stream++)
		{
			AVIFileGetStream(m_pavi_R, &m_pstm_R, 0, stream);
			AVIStreamInfo(m_pstm_R, &m_si_R, sizeof(AVISTREAMINFO));

			if (m_si_R.fccType == streamtypeVIDEO)
			{
				m_pfrm_R = AVIStreamGetFrameOpen(m_pstm_R, (LPBITMAPINFOHEADER)AVIGETFRAMEF_BESTDISPLAYFMT);
				m_nAviLength_R = AVIStreamLength(m_pstm_R);
				break;
			}
		}

		if (stream == m_fi_R.dwStreams || m_pfrm_R == NULL)
		{
			AVIStreamRelease(m_pstm_R);
			AVIFileRelease(m_pavi_R);

			return;
		}

		m_nStart_R = 0;
		m_nEnd_R = m_nAviLength_R - 1;

		UpdateData(FALSE);

		CString DispFrameNum;
		DispFrameNum.Format(_T("(0,%d)"), m_nAviLength_R - 1);
		SetDlgItemText(IDC_TOTAL_FRAME, DispFrameNum);

		///////////////////
	}
}

void CVideoStitch::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	DestroyWindow();
	m_bCreated = false;
}

void CVideoStitch::OnBnClickedVideostitch()
{
	clock_t Tstart, Tend;
	double result;

	BYTE* img1D_L, *img1D_R;
	int nFin;

	if (m_nEnd_R < m_nEnd_L) nFin = m_nEnd_R;
	if (m_nEnd_L < m_nEnd_R) nFin = m_nEnd_L;

	GetImg2DSize(m_pfrm_L, 0, &m_nHL, &m_nWL);
	GetImg2DSize(m_pfrm_R, 0, &m_nHR, &m_nWR);

	for (m_nCurrentProcessingNum = 0; m_nCurrentProcessingNum < nFin; m_nCurrentProcessingNum++)
	{
		img1D_L = GetImg1D(m_pfrm_L, m_nCurrentProcessingNum);
		img1D_R = GetImg1D(m_pfrm_R, m_nCurrentProcessingNum);

		CImageStitchBasic imgStitchBasic(img1D_L, img1D_R, m_nHL, m_nWL, m_nHR, m_nWR);

		cout << endl;

		Tstart = clock();
		imgStitchBasic.GetMatchingPoint(400);
		if (imgStitchBasic.bErr)
			continue;
		Tend = clock();
		result = double(Tend - Tstart) / CLOCKS_PER_SEC;
		cout << "GetMAtchingPoint" << m_nCurrentProcessingNum << " : "<< result << endl;

		Tstart = clock();
		imgStitchBasic.GetHomography();
		Tend = clock();
		result = double(Tend - Tstart) / CLOCKS_PER_SEC;
		cout << "GetHography" << m_nCurrentProcessingNum << " : " << result << endl;

		Tstart = clock();
		imgStitchBasic.WarpHomographyForVideo();
		if (imgStitchBasic.bErr)
			continue;
		Tend = clock();
		result = double(Tend - Tstart) / CLOCKS_PER_SEC;
		cout << "WarpHomography" << m_nCurrentProcessingNum << " : " << result << endl;

		delete[] img1D_L;
		delete[] img1D_R;
	}
}

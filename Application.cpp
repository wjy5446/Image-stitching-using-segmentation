// Application.cpp : implementation file
//

#include "stdafx.h"
#include "ImageProcessing.h"
#include "Application.h"

#include "Image.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CApplication dialog


CApplication::CApplication(CWnd* pParent /*=NULL*/)
	: CDialog(CApplication::IDD, pParent)
{
	//{{AFX_DATA_INIT(CApplication)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_bCreated = FALSE;
		
	m_nW = 320;
	m_nH = 240;

	m_nErrorW = 500;
	m_nErrorH = 200;

	m_nTestW = 256;
	m_nTestH = 256;
}


void CApplication::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CApplication)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CApplication, CDialog)
	//{{AFX_MSG_MAP(CApplication)
	ON_BN_CLICKED(IDC_VEHICLE_RUN, OnVehicleRun)
	ON_BN_CLICKED(IDC_AVI_RUN, OnAviRun)
	ON_BN_CLICKED(IDC_MOTION_RUN, OnMotionRun)
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_FOLDER_SEL, OnFolderSel)
	ON_BN_CLICKED(IDC_FILE_SEL, OnFileSel)
	ON_BN_CLICKED(IDC_CANCEL, OnCancel)
	ON_BN_CLICKED(IDC_BPN_EX_RUN, OnBpnExRun)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CApplication message handlers

BOOL CApplication::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_CurrentImageGray = cmatrix(m_nH, m_nW);
	m_BackgroundImageGray = cmatrix(m_nH, m_nW);
	m_ProcessingImageGray = cmatrix(m_nH, m_nW);
	m_LabelImageInt = imatrix(m_nH, m_nW);

	m_CurrentImageRed = cmatrix(m_nH, m_nW);
	m_CurrentImageGreen = cmatrix(m_nH, m_nW);
	m_CurrentImageBlue = cmatrix(m_nH, m_nW);

	m_PreviousImageGray = cmatrix(m_nH, m_nW);

	SetDlgItemText(IDC_FOLDER_NAME, _T("ex"));
	SetDlgItemText(IDC_FILE_NAME, _T("data.avi"));

	m_bVehicleRun = FALSE;
	m_bAviVehicleRun = FALSE;
	m_bMotionRun = FALSE;
	m_bBpnRun = FALSE;

	m_pBpn = NULL;

	ErrorImage = cmatrix(m_nErrorH, m_nErrorW);
	TestImage = cmatrix(m_nTestH, m_nTestW);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CApplication::OnCancel() 
{
	DestroyWindow();	
	m_bCreated = FALSE;
}

void CApplication::OnVehicleRun() 
{
	if(!m_bVehicleRun && !m_bAviVehicleRun && !m_bMotionRun && !m_bBpnRun)
	{
		GetDlgItemText(IDC_FOLDER_NAME, m_Folder);

		if(m_Folder.IsEmpty()) return;

		m_nFrameNumber = 0;
		SetTimer(VEHICLE, 30, NULL);

		SetDlgItemText(IDC_VEHICLE_RUN, _T("중지"));

		m_bVehicleRun = TRUE;
	}
	else if(m_bVehicleRun)
	{
		KillTimer(VEHICLE);

		SetDlgItemText(IDC_VEHICLE_RUN, _T("실행"));

		m_bVehicleRun = FALSE;
	}
}

void CApplication::OnAviRun() 
{
	if(!m_bVehicleRun && !m_bAviVehicleRun && !m_bMotionRun && !m_bBpnRun)
	{
		GetDlgItemText(IDC_FILE_NAME, m_AviFileName);

		if(m_AviFileName.IsEmpty()) return;

		AVIFileInit();   
		if(AVIFileOpen(&m_pavi, m_AviFileName, OF_READ | OF_SHARE_DENY_NONE,NULL) != 0) return;
		AVIFileInfo(m_pavi,&m_fi,sizeof(AVIFILEINFO));

		UINT stream;

		for(stream = 0 ; stream < m_fi.dwStreams ; stream++)
		{
			AVIFileGetStream(m_pavi,&m_pstm,0,stream);
			AVIStreamInfo(m_pstm, &m_si, sizeof(m_si)); 
 
			if (m_si.fccType == streamtypeVIDEO) 
			{
				m_pfrm = AVIStreamGetFrameOpen(m_pstm, (LPBITMAPINFOHEADER)AVIGETFRAMEF_BESTDISPLAYFMT);
				break;
			}
		}

		if(stream == m_fi.dwStreams)
			return;

		m_nFrameNumber = 0;
		SetTimer(AVI_VEHICLE, 30, NULL);

		SetDlgItemText(IDC_LPR_RUN, _T("중지"));

		m_bAviVehicleRun = TRUE;
	}
	else if(m_bAviVehicleRun)
	{
		AVIStreamGetFrameClose(m_pfrm);
		AVIStreamRelease(m_pstm);    
		AVIFileRelease(m_pavi);    
		AVIFileExit();

		KillTimer(AVI_VEHICLE);

		SetDlgItemText(IDC_LPR_RUN, _T("실행"));

		m_bAviVehicleRun = FALSE;
	}
	
}

// 프레임 영상을 얻어올 때 호출되는 함수 - 사용자 지정 콜백 함수
static LRESULT	CALLBACK capCallbackOnFrame(HWND hWnd, LPVIDEOHDR lpVHdr)
{
	UINT i;
	int x, y;

	CApplication *pDlg = (CApplication *)CApplication::FromHandle(::GetParent(hWnd));

	for(i = 0 ; i < lpVHdr->dwBufferLength ; i+=3)
	{
		y = (lpVHdr->dwBufferLength-i-1)/(pDlg->m_nW*3);
		x = i%(pDlg->m_nW*3)/3;

		pDlg->m_CurrentImageRed[y][x] = *(lpVHdr->lpData+i+2);
		pDlg->m_CurrentImageGreen[y][x] = *(lpVHdr->lpData+i+1);
		pDlg->m_CurrentImageBlue[y][x] = *(lpVHdr->lpData+i);
	}

	return (LRESULT) true;
}

void CApplication::OnMotionRun() 
{
	if(!m_bVehicleRun && !m_bAviVehicleRun && !m_bMotionRun && !m_bBpnRun)
	{
		m_hWndCap = capCreateCaptureWindow(_T("Motion Detection Window"), WS_CHILD,
			0, 0, m_nW, m_nH, m_hWnd, NULL);
		
		// 프레임 영상을 얻어올 때 호출되는 콜백 함수의 지정
		capSetCallbackOnFrame(m_hWndCap, capCallbackOnFrame);

		int Result;

		// 드라이버 연결
		if(Result = capDriverConnect(m_hWndCap, 0))
		{
			// 현재 설정되어 있는 비디오 정보 얻어오기
			capGetVideoFormat(m_hWndCap, &m_BmInfo, sizeof(BITMAPINFO));

			// 320*240의 24비트 bitmap으로 지정
		
			m_BmInfo.bmiHeader.biBitCount = 24;
			m_BmInfo.bmiHeader.biCompression = 0;
			m_BmInfo.bmiHeader.biWidth = 320;
			m_BmInfo.bmiHeader.biHeight = 240;
			m_BmInfo.bmiHeader.biSizeImage = m_BmInfo.bmiHeader.biWidth * m_BmInfo.bmiHeader.biHeight * 3;
		
			// 설정된 비디오 정보를 지정
			capSetVideoFormat(m_hWndCap, &m_BmInfo, sizeof(BITMAPINFO));

			capOverlay(m_hWndCap, false);
			capPreview(m_hWndCap, false);
		}

		SetTimer(MOTION, 30, NULL);
		SetDlgItemText(IDC_MOTION_RUN, _T("중지"));

		m_bMotionRun = TRUE;
	}
	else if(m_bMotionRun)
	{
		KillTimer(MOTION);

		capDriverDisconnect(m_hWndCap);

		SetDlgItemText(IDC_MOTION_RUN, _T("실행"));

		m_bMotionRun = FALSE;
	}
}

void CApplication::Detection() 
{
	int x, y;

	if(m_nFrameNumber == 0)
	{	
		for(y = 0 ; y < m_nH ; y++)
			memcpy(m_BackgroundImageGray[y], m_CurrentImageGray[y], m_nW);

		DisplayCimage2D(m_BackgroundImageGray, m_nW, m_nH, 0, m_nH, false, true);
	}

	DisplayCimage2D(m_CurrentImageGray, m_nW, m_nH, 0, 0, false, true);
	
	for(y = 0 ; y < m_nH ; y++)
		for(x = 0 ; x < m_nW ; x++)
			m_ProcessingImageGray[y][x] = abs(m_CurrentImageGray[y][x] - m_BackgroundImageGray[y][x]);

	DisplayCimage2D(m_ProcessingImageGray, m_nW, m_nH, m_nW, 0, false, true);

	for(y = 0 ; y < m_nH ; y++)
		for(x = 0 ; x < m_nW ; x++)
			m_ProcessingImageGray[y][x] = m_ProcessingImageGray[y][x] > 40 ? GRAY_CNT-1 : 0;

	DisplayCimage2D(m_ProcessingImageGray, m_nW, m_nH, m_nW*2, 0, false, true);
	
	int LabelCnt = Labeling(m_ProcessingImageGray, m_LabelImageInt, m_nW, m_nH, 50);

	DisplayIimage2D(m_LabelImageInt, m_nW, m_nH, m_nW, m_nH, false, true);

	if(LabelCnt > 0)
	{
		CRect *pBoundBox;
		int i;
		pBoundBox = new CRect[LabelCnt];

		for(i = 0 ; i < LabelCnt ; i++)
			pBoundBox[i] = CRect(m_nW, m_nH, 0, 0);

		for(y = 0 ; y < m_nH ; y++)
			memcpy(m_ProcessingImageGray[y], m_CurrentImageGray[y], m_nW);

		for(y = 0 ; y < m_nH ; y++)
			for(x = 0 ; x < m_nW ; x++)
			{
				if(m_LabelImageInt[y][x] < 0) continue;
				if(pBoundBox[m_LabelImageInt[y][x]].left > x) pBoundBox[m_LabelImageInt[y][x]].left = x;
				if(pBoundBox[m_LabelImageInt[y][x]].top > y) pBoundBox[m_LabelImageInt[y][x]].top = y;
				if(pBoundBox[m_LabelImageInt[y][x]].right < x) pBoundBox[m_LabelImageInt[y][x]].right = x;
				if(pBoundBox[m_LabelImageInt[y][x]].bottom < y) pBoundBox[m_LabelImageInt[y][x]].bottom = y;
			}

		for(i = 0 ; i < LabelCnt ; i++)
		{
			for(y = pBoundBox[i].top ; y <= pBoundBox[i].bottom ; y++)
			{
				m_ProcessingImageGray[y][pBoundBox[i].left] = GRAY_CNT-1;
				m_ProcessingImageGray[y][pBoundBox[i].right] = GRAY_CNT-1;
			}
			for(x = pBoundBox[i].left ; x <= pBoundBox[i].right ; x++)
			{
				m_ProcessingImageGray[pBoundBox[i].top][x] = GRAY_CNT-1;
				m_ProcessingImageGray[pBoundBox[i].bottom][x] = GRAY_CNT-1;
			}
		}

		DisplayCimage2DColor(m_ProcessingImageGray, m_CurrentImageGray, m_CurrentImageGray, m_nW, m_nH, m_nW*2, m_nH, false, true);

		delete [] pBoundBox;
	}
}

void CApplication::VehicleProc() 
{
	int y;
	CString FileName;
	
	FileName.Format(_T("%s\\aa%04d.320"), m_Folder, m_nFrameNumber);

	SetViewMsg(FileName, false);

	CFile cfile;
	BOOL bOpen;

	bOpen = cfile.Open(FileName, CFile::modeRead | CFile::typeBinary);

	if(!bOpen) 
	{
		KillTimer(VEHICLE);

		SetDlgItemText(IDC_VEHICLE_RUN, _T("실행"));

		m_bVehicleRun = FALSE;

		return;
	}

	for(y = 0 ; y < m_nH ; y++)
		cfile.Read(m_CurrentImageGray[y], m_nW);

	cfile.Close();

	Detection();

	m_nFrameNumber++;
}

void CApplication::AviVehicleProc() 
{
	SetViewMsg(m_AviFileName, false);

	LPBITMAPINFOHEADER pbmih;
	pbmih = (LPBITMAPINFOHEADER) AVIStreamGetFrame(m_pfrm, m_nFrameNumber);
	BYTE *image = (unsigned char *) ((LPSTR)pbmih + (WORD)pbmih->biSize); 

	int x, y, offset;
	bool bExit = false;

	if(pbmih->biBitCount == 24 || pbmih->biBitCount == 32) 
	{
		for (y = 0; y < m_nH; y++)
			for (x = 0; x < m_nW; x++)
			{
				offset = (y*m_fi.dwHeight/m_nH*m_fi.dwWidth+x*m_fi.dwWidth/m_nW)*pbmih->biBitCount/8;
				m_CurrentImageBlue[m_nH-y-1][x] 
					= image[offset];
				m_CurrentImageGreen[m_nH-y-1][x] 
					= image[offset+1];
				m_CurrentImageRed[m_nH-y-1][x] 
					= image[offset+2];
			}

		DisplayCimage2DColor(m_CurrentImageRed, m_CurrentImageGreen, m_CurrentImageBlue, m_nW, m_nH, 0, m_nH*2, false, true);

		for(y = 0 ; y < m_nH ; y++)
			for(x = 0 ; x < m_nW ; x++)
				m_CurrentImageGray[y][x] 
					= (m_CurrentImageRed[y][x] + m_CurrentImageGreen[y][x] + m_CurrentImageBlue[y][x])/3;

		Detection();

		m_nFrameNumber++;
	}
	else
		bExit = true;

	if(m_nFrameNumber >= m_si.dwLength || bExit) 
	{
		AVIStreamGetFrameClose(m_pfrm);
		AVIStreamRelease(m_pstm);    
		AVIFileRelease(m_pavi);    
		AVIFileExit(); 

		KillTimer(AVI_VEHICLE);

		SetDlgItemText(IDC_LPR_RUN, _T("실행"));

		m_bAviVehicleRun = FALSE;

		return;
	}
}

void CApplication::MotionProc()
{
	CString Msg = _T("Camera(Live)");
	SetViewMsg(Msg, false);

	int x, y;
	
	capGrabFrame(m_hWndCap);

	for(y = 0 ; y < m_nH ; y++)
		for(x = 0 ; x < m_nW ; x++)
			m_CurrentImageGray[y][x] 
				= (m_CurrentImageRed[y][x] + m_CurrentImageGreen[y][x] + m_CurrentImageBlue[y][x])/3;

	DisplayCimage2DColor(m_CurrentImageRed, m_CurrentImageGreen, m_CurrentImageBlue, m_nW, m_nH, 0, 0, false, true);

	DisplayCimage2D(m_CurrentImageGray, m_nW, m_nH, m_nW, 0, false, true);
	DisplayCimage2D(m_PreviousImageGray, m_nW, m_nH, m_nW*2, 0, false, true);

	for(y = 0 ; y < m_nH ; y++)
		for(x = 0 ; x < m_nW ; x++)
			m_ProcessingImageGray[y][x] = abs(m_CurrentImageGray[y][x] - m_PreviousImageGray[y][x]);

	DisplayCimage2D(m_ProcessingImageGray, m_nW, m_nH, 0, m_nH, false, true);

	for(y = 0 ; y < m_nH ; y++)
		for(x = 0 ; x < m_nW ; x++)
			m_ProcessingImageGray[y][x] = m_ProcessingImageGray[y][x] > 20 ? GRAY_CNT-1 : m_CurrentImageGray[y][x];

	DisplayCimage2DColor(m_ProcessingImageGray, m_CurrentImageGray, m_CurrentImageGray, m_nW, m_nH, m_nW, m_nH, false, true);

	for(y = 0 ; y < m_nH ; y++)
		memcpy(m_PreviousImageGray[y], m_CurrentImageGray[y], m_nW);
}

void CApplication::OnTimer(UINT_PTR nIDEvent) 
{
	if(nIDEvent == VEHICLE)
	{
		VehicleProc();
	}
	else if(nIDEvent == AVI_VEHICLE) 
	{
		AviVehicleProc();
	}
	else if(nIDEvent == MOTION)
	{
		MotionProc();
	}
	else if(nIDEvent == BPN_EX)
	{
		BpnProc();
	}
		
	CDialog::OnTimer(nIDEvent);
}

void CApplication::OnDestroy() 
{
	CDialog::OnDestroy();
	
	free_cmatrix(m_CurrentImageGray, m_nH, m_nW);
	free_cmatrix(m_BackgroundImageGray, m_nH, m_nW);
	free_cmatrix(m_ProcessingImageGray, m_nH, m_nW);
	
	free_imatrix(m_LabelImageInt, m_nH, m_nW);
	
	free_cmatrix(m_CurrentImageRed, m_nH, m_nW);
	free_cmatrix(m_CurrentImageGreen, m_nH, m_nW);
	free_cmatrix(m_CurrentImageBlue, m_nH, m_nW);

	free_cmatrix(m_PreviousImageGray, m_nH, m_nW);	

	if(m_pBpn)
		delete m_pBpn;

	free_cmatrix(ErrorImage, m_nErrorH, m_nErrorW);
	free_cmatrix(TestImage, m_nTestH, m_nTestW);
}

void CApplication::OnFolderSel() 
{
	ITEMIDLIST	*pidlBrowse;
	TCHAR FolderName[MAX_PATH];

	GetDlgItemText(IDC_FOLDER_NAME, FolderName, MAX_PATH);

	BROWSEINFO BrInfo;
	memset(&BrInfo, 0 , sizeof(BrInfo));
	BrInfo.hwndOwner = GetSafeHwnd();

	pidlBrowse = ::SHBrowseForFolder(&BrInfo);

	if(pidlBrowse != NULL)
	{
		SHGetPathFromIDList(pidlBrowse, FolderName);

		SetDlgItemText(IDC_FOLDER_NAME, FolderName);
	}		
}

void CApplication::OnFileSel() 
{
	CFileDialog dlg(TRUE, _T("*.avi"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("AVI 파일(*.avi)|*.avi||"));
	
	if(dlg.DoModal() == IDOK) 
	{
		CString FileName = dlg.GetPathName();
		SetDlgItemText(IDC_FILE_NAME, FileName);
	}
}

void CApplication::BpnProc() 
{
	int x, y;
	double Input[3], Output[2];

	int Cnt;
	double Error = 0;
	for(Cnt = 0 ; Cnt < 1000 ; Cnt++)
	{
		Input[0] = (double)rand()/RAND_MAX;
		Input[1] = (double)rand()/RAND_MAX;
		Input[2] = 1.;

		if(((Input[0] >= 0.5) && (Input[1] < 0.5)) ||
			((Input[0] < 0.5) && (Input[1] >= 0.5)))
		{
			Output[0] = 1.;
			Output[1] = 0.;
		}
		else
		{
			Output[0] = 0.;
			Output[1] = 1.;
		}

		int Out = m_pBpn->Train(Input, Output);
		if(Output[Out] != 1.)
			Error++;
	}

	Error /= Cnt;

	if(nTrainCnt == 0) 
	{
		for(y = 0 ; y < m_nErrorH ; y++)
			for(x = 0 ; x < m_nErrorW ; x++)
				ErrorImage[y][x] = GRAY_CNT/2;
	}
	else
		DrawLine(ErrorImage, m_nErrorW, m_nErrorH, 
			nTrainCnt-1, (int)(m_nErrorH - m_PreviousError*(m_nErrorH-1) -1), 
			nTrainCnt, (int)(m_nErrorH - Error*(m_nErrorH-1) -1), 0);

	DisplayCimage2D(ErrorImage, m_nErrorW, m_nErrorH, 0, 0, false, true);
	m_PreviousError = Error;
	nTrainCnt++;
	if(nTrainCnt == m_nErrorW) nTrainCnt = 0;

	for(y = 0 ; y < m_nTestH ; y++)
		for(x = 0 ; x < m_nTestW ; x++)
		{
			Input[0] = (double)x/m_nTestW;
			Input[1] = (double)y/m_nTestH;
			Input[2] = 1.;
			
			int Out = m_pBpn->ComputeNet(Input);
							
			if(Out == 0)
				TestImage[y][x] =  GRAY_CNT/2;
			else
				TestImage[y][x] = 0;
		}

	DisplayCimage2D(TestImage, m_nTestW, m_nTestH, 0, m_nErrorH, false, true);
}

void CApplication::OnBpnExRun() 
{
	if(!m_bVehicleRun && !m_bAviVehicleRun && !m_bMotionRun && !m_bBpnRun)
	{
		int NodeCnt[3] = {3, 7, 2};

		if(m_pBpn)
			delete m_pBpn;

		m_pBpn = new CBpn(3, NodeCnt, 0.2);

		m_pBpn->InitWeight();
		
		nTrainCnt = 0;
		SetTimer(BPN_EX, 5, NULL);

		SetDlgItemText(IDC_BPN_EX_RUN, _T("중지"));

		m_bBpnRun = TRUE;
	}
	else if(m_bBpnRun)
	{
		KillTimer(BPN_EX);

		delete m_pBpn;
		m_pBpn = NULL;

		SetDlgItemText(IDC_BPN_EX_RUN, _T("실행"));

		m_bBpnRun = FALSE;
	}
}

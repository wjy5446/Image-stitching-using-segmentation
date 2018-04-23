// ImageStitch_jy.cpp : implementation file
//

// ImageStitch_jy dialog

#include "stdafx.h"
#include "ImageProcessing.h"
#include "ImageStitch_jy.h"
#include "afxdialogex.h"

#include "Mmsystem.h"

IMPLEMENT_DYNAMIC(ImageStitch_jy, CDialog)

ImageStitch_jy::ImageStitch_jy(CWnd* pParent /*=NULL*/)
	: CDialog(ImageStitch_jy::IDD, pParent)
	, m_EditMinRegion(5000)
	, m_EditMinFeature(10)
	, m_EditFeatureRate(0.5)
	, m_EditKLDiver(20)
	, m_EditSIFT(800)
	, m_EditSegRegion(2000)
	, m_EditDiff(0.1)
{
	m_bCreated = false;
}

ImageStitch_jy::~ImageStitch_jy()
{
	m_bCreated = false;
}

void ImageStitch_jy::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_EDIT_SIFT, m_EditSIFT);
	DDV_MinMaxInt(pDX, m_EditSIFT, 0, 20000);
	DDX_Text(pDX, IDC_EDIT_SEGREGIN, m_EditSegRegion);
	DDV_MinMaxInt(pDX, m_EditSegRegion, 0, 10000);

	DDX_Text(pDX, IDC_EDITMINREGION, m_EditMinRegion);
	DDV_MinMaxInt(pDX, m_EditMinRegion, 0, 10000);
	DDX_Text(pDX, IDC_EDITMINFEATURE, m_EditMinFeature);
	DDV_MinMaxInt(pDX, m_EditMinFeature, 0, 20);
	DDX_Text(pDX, IDC_EDIT_FEATURERATE, m_EditFeatureRate);
	DDV_MinMaxFloat(pDX, m_EditFeatureRate, 0, 1);
	DDX_Text(pDX, IDC_EDIT_KL, m_EditKLDiver);
	DDV_MinMaxFloat(pDX, m_EditKLDiver, 0, 1000);

	DDX_Text(pDX, IDC_EDIT_DIFF, m_EditDiff);
	DDV_MinMaxFloat(pDX, m_EditDiff, 0, 2);

	DDX_Control(pDX, IDC_ONLYREGION, m_CheckOnlyRegion);
	DDX_Control(pDX, IDC_CHECKFEATURE, m_checkFeature);
	DDX_Control(pDX, IDC_CHECKMATCHING, m_checkMatch);
	DDX_Control(pDX, IDC_CHECKLR, m_checkLR);
}


BEGIN_MESSAGE_MAP(ImageStitch_jy, CDialog)
	ON_BN_CLICKED(IDC_DETECTEDGE, &ImageStitch_jy::OnBnClickedDetectedge)
	ON_BN_CLICKED(IDCANCEL, &ImageStitch_jy::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_GetDualHomography, &ImageStitch_jy::OnBnClickedGetdualhomography)
	ON_BN_CLICKED(IDC_GetSingleHomography, &ImageStitch_jy::OnBnClickedGetsinglehomography)
	ON_BN_CLICKED(IDC_MEANSHIFT, &ImageStitch_jy::OnBnClickedMeanshift)
	ON_BN_CLICKED(IDC_OCR, &ImageStitch_jy::OnClickedOcr)
	ON_BN_CLICKED(IDC_PARALLAXROBUST, &ImageStitch_jy::OnBnClickedParallaxrobust)
	ON_BN_CLICKED(IDC_PARALLAXHANDLING, &ImageStitch_jy::OnBnClickedParallaxhandling)
	ON_BN_CLICKED(IDC_MEAN_AUTO, &ImageStitch_jy::OnBnClickedMeanAuto)
	ON_BN_CLICKED(IDC_DRAW, &ImageStitch_jy::OnBnClickedDraw)
	ON_COMMAND(IDM_VIDEOSTITCH, &ImageStitch_jy::OnVideostitch)
	ON_BN_CLICKED(IDC_GetSingleAffine, &ImageStitch_jy::OnBnClickedGetsingleaffine)
END_MESSAGE_MAP()


void ImageStitch_jy::OnBnClickedDetectedge()
{
	int nH, nW, nPosX, nPosY;
	BYTE **ImageGray, **OutputGray;
	int **ImageCC;

	// 영상 정보 읽기
	if (!GetCurrentImageInfo(&nW, &nH, &nPosX, &nPosY)) return;

	ImageGray = cmatrix(nH, nW);
	OutputGray = cmatrix(nH, nW);
	ImageCC = imatrix(nH, nW);

	//ImageCC 초기화
	for (int y = 0; y < nH; y++)
		for (int x = 0; x < nW; x++)
		{
			ImageCC[y][x] = -1;
		}

	// 회색조 영상 읽기
	GetCurrentImageGray(ImageGray);

	int tmpSaveX, tmpSaveY; // 255검색 스캔 마지막까지 읽은 위치
	int tmpPreCC=0, tmpCC;
	int labelLine = 0;
	int shiftX, shiftY;
	bool bNo255 = TRUE;
	int dxPoint[3] = { -1, 0, 1 };
	int dyPoint[3] = { -1, 0, 1 };


	//**Chain Code 를 부여해 주기**--------------------------------------------------------

	//초기 255 위치 찾기
	for (tmpSaveY = 1; tmpSaveY < nH - 1; tmpSaveY++)
		for (tmpSaveX = 1; tmpSaveX < nW - 1; tmpSaveX++)
		{
			if (ImageGray[tmpSaveY][tmpSaveX] == 255)
			{
				labelLine++;
				shiftY = tmpSaveY, shiftX = tmpSaveX;

				//위치에서 하나의 연결된 Chain Code를 생성
				while (bNo255)
				{
					bNo255 = FALSE;
					//주변부 확인
					for (int i = 0; i < 3; i++)
						for (int j = 0; j < 3; j++)
						{
							if (ImageGray[shiftY + dyPoint[i]][shiftX + dxPoint[j]] == 255)
							{
								if (!(dyPoint[i] == 0 && dxPoint[j] == 0))
								{
									if (tmpSaveX == shiftX) tmpPreCC = -1;
									
									tmpCC = GetChainCode(dyPoint[i], dxPoint[j], tmpPreCC);

									ImageGray[shiftY][shiftX] = tmpCC * 20;
									ImageCC[shiftY][shiftX] = tmpCC;
									tmpPreCC = tmpCC;

									shiftY += dyPoint[i];
									shiftX += dxPoint[j];
									bNo255 = TRUE;
									break;
								}
							}
						}
				}
				ImageGray[shiftY][shiftX] = 0;
				bNo255 = TRUE;
			}
		}
	//--------------------------------------------------------------------------------------

	for (int y = 0; y < nH; y++)
		for (int x = 0; x < nW; x++)
		{
			if (ImageCC[y][x] < 0) OutputGray[y][x] = 0;
			 else OutputGray[y][x] = ImageCC[y][x] * 20;
		}

//	DisplayCimage2D(ImageGray, nW, nH, nPosX + nW, nPosY);
	DisplayCimage2D(OutputGray, nW, nH, nPosX + nW, nPosY);

	//**Edge 찾기---------------------------------------------------------------
	int diffCC;
	int tmpShiftY, tmpShiftX, tmpCC2;

	for (tmpSaveY = 1; tmpSaveY < nH - 1; tmpSaveY++)
		for (tmpSaveX = 1; tmpSaveX < nW - 1; tmpSaveX++)
		{
			if (ImageCC[tmpSaveY][tmpSaveX] >= 0)
			{
				shiftY = tmpSaveY, shiftX = tmpSaveX;
				//위치에서 하나의 연결된 Chain Code를 생성
				while (bNo255)
				{
					if (ImageCC[shiftY][shiftX]!=-1)
					{
						tmpCC = ImageCC[shiftY][shiftX];
						ImageCC[shiftY][shiftX] = -1;

						tmpShiftY = shiftY; tmpShiftX = shiftX;
						tmpCC2 = tmpCC;
						for (int i = 0; i < 3; i++)
						{
							ShiftChainCode(tmpShiftY, tmpShiftX, tmpCC2);
							tmpCC2 = ImageCC[tmpShiftY][tmpShiftX];
							if (tmpCC2 == -1) break;

							diffCC=abs(tmpCC - tmpCC2);
							if (diffCC >= 2)
							{
								ImageCC[shiftY][shiftX] = 999;
								break;
							}
						}
						ShiftChainCode(shiftY, shiftX, tmpCC);
					}
					else{
						bNo255 = FALSE;
					}
				}
				bNo255 = TRUE;
			}
		}

	for (int y = 0; y < nH; y++)
		for (int x = 0; x < nW; x++)
		{
			if (ImageCC[y][x] == 999) 
				DrawCross(ImageGray, nW, nH, CPoint(x, y), 255);
		}

	DisplayCimage2D(ImageGray, nW, nH, nPosX + nW + nW, nPosY);
	//--------------------------------------------------------------------------
}

int ImageStitch_jy::GetChainCode(int y, int x, int preCC)
{
	int tmpCC;

	if (y == -1)
	{
		if (x == -1) tmpCC = 3;
		if (x == 0) tmpCC = 2;
		if (x == 1) tmpCC = 1;
	}

	if (y == 0)
	{
		if (x == -1) tmpCC = 4;
		if (x == 1) tmpCC = 0;
	}

	if (y == 1)
	{
		if (x == -1) tmpCC = 5;
		if (x == 0) tmpCC = 6;
		if (x == 1) tmpCC = 7;
	}

	if (preCC == -1)
		return tmpCC;
	
	else
	{
		if (abs(tmpCC + 8 - preCC) > abs(tmpCC - preCC))
			return tmpCC;
		else
			return tmpCC + 8;
	}
}

void ImageStitch_jy::ShiftChainCode(int& y, int& x, int CC)
{
	int tmpCC = CC % 8;

	if (tmpCC == 0) { x += 1; }
	if (tmpCC == 1) { x += 1; y -= 1; }
	if (tmpCC == 2) { y -= 1; }
	if (tmpCC == 3) { x -= 1; y -= 1; } 
	if (tmpCC == 4) { x -= 1; }
	if (tmpCC == 5) { x -= 1; y += 1; }
	if (tmpCC == 6) { y += 1; }
	if (tmpCC == 7) { x += 1; y += 1; }
}


void ImageStitch_jy::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	DestroyWindow();
	m_bCreated = false;
}


void ImageStitch_jy::OnBnClickedGetdualhomography()
{
//	CImageStitchMultiHomo multiHomoObject(2);
//	
//	multiHomoObject.GetMatchingPoint();
//	//k - means clustering
//	//set initial point
//	
//	Point2f* arrInitPoint;
//	arrInitPoint = new Point2f[2];
//	
//	int nSum=0, nAvg;
//	for (int i = 0; i < multiHomoObject.nNumPoint; i++)
//	{
//		nSum += multiHomoObject.GodPointR[i].x;
//	}
//	
//	nAvg = nSum / multiHomoObject.nNumPoint;
//	
//	int nH, nW;
//	nH = multiHomoObject.imgR.nH;
//	nW = multiHomoObject.imgR.nW;
//
//	arrInitPoint[0].x = nAvg;
//	arrInitPoint[0].y = 0;
//	arrInitPoint[1].x = nAvg;
//	arrInitPoint[1].y = nH;
//
//	multiHomoObject.SetInitCenPoint(arrInitPoint);
//	multiHomoObject.KmeanForMultiHomo();
//	multiHomoObject.DrawCluster();
//	//////////////////////////////////////////////////////
//	
//	multiHomoObject.GetHomography();
//
////	for (int i = 0; i < multiHomoObject.nNumCluster; i++)
//	//	multiHomoObject.WarpHomography(0,i);
//	
//	/*
//	multiHomoObject.MakeWeightMap_TopBtm();
//	multiHomoObject.DrawWeightMap(false);
//	multiHomoObject.BlendHomo();
//	multiHomoObject.MeshWarpHomography();
//	*/
//
//	multiHomoObject.MakeWeightMap_TopBtm();
//	multiHomoObject.DrawWeightMap(false);
//	
//	//blending Homography
//	multiHomoObject.BlendHomo();
//
//	//Get offset
////	multiHomoObject.WarpHomography(1);
//	multiHomoObject.MeshWarpHomography();
}


void ImageStitch_jy::OnBnClickedGetsinglehomography()
{
	CImageStitchBasic SingleHomo;
	bool bOnCheckFeature, bOnCheckMatch, bOnCheckLR;

	bOnCheckFeature = m_checkFeature.GetCheck();
	bOnCheckMatch = m_checkMatch.GetCheck();
	bOnCheckLR = m_checkLR.GetCheck();

	SingleHomo.GetMatchingPoint(bOnCheckFeature, bOnCheckMatch);
	SingleHomo.GetHomography();
	SingleHomo.WarpHomography();
}

void ImageStitch_jy::OnBnClickedMeanshift()
{
	int64 TimeFreq, TimeStart, TimeEnd;

	QueryPerformanceFrequency((LARGE_INTEGER*)&TimeFreq);
	QueryPerformanceCounter((LARGE_INTEGER*)&TimeStart);
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	CImageStitchMeanShift imgStitch;

	imgStitch.GetMatchingPoint(m_EditSIFT);

	imgStitch.SegmentMeanshift(m_EditSegRegion);

	imgStitch.DetermineActive(m_EditMinFeature, m_EditMinRegion, m_EditFeatureRate, m_EditKLDiver, true);

	if (imgStitch.bExeDualHomo)
	{
		imgStitch.FindSingleObject(m_EditDiff, false);
		imgStitch.MakeWeightMap(10);
	}

	imgStitch.WarpHomography(false);

	QueryPerformanceCounter((LARGE_INTEGER*)&TimeEnd);
	double timeDiff = (double)(TimeEnd - TimeStart) / (double)TimeFreq;

	cout << "meanshift : " << timeDiff << endl;

}



void ImageStitch_jy::OnBnClickedParallaxrobust()
{
	//CMultiHomoStitcing_jy Parallax(2);
	//Parallax.GetMatchingPoint();
	//Parallax.WarpLayer(20);
	int64 TimeFreq, TimeStart, TimeEnd;

	QueryPerformanceFrequency((LARGE_INTEGER*)&TimeFreq);
	QueryPerformanceCounter((LARGE_INTEGER*)&TimeStart);
	CImageStitchLayer MultiLayerHomo;

	MultiLayerHomo.GetMatchingPoint();
	MultiLayerHomo.FindLayer(10);
	MultiLayerHomo.MakeMesh();
	MultiLayerHomo.BlendingHomo();
	MultiLayerHomo.WarpMeshHomography();
	QueryPerformanceCounter((LARGE_INTEGER*)&TimeEnd);
	double timeDiff = (double)(TimeEnd - TimeStart) / (double)TimeFreq;

	cout << "Parallax robust : " << timeDiff << endl;
	// TODO: Add your control notification handler code here
}


void ImageStitch_jy::OnBnClickedParallaxhandling()
{
	int64 TimeFreq, TimeStart, TimeEnd;

	QueryPerformanceFrequency((LARGE_INTEGER*)&TimeFreq);
	QueryPerformanceCounter((LARGE_INTEGER*)&TimeStart);
	CImageStitchKMeans KMeanStitch;

	KMeanStitch.GetMatchingPoint();
	KMeanStitch.RemoveOutlier1st(30);
	KMeanStitch.KMeansCluster();
	KMeanStitch.RemoveOutlier2nd();

	KMeanStitch.MakeMesh();
	KMeanStitch.BlendingHomo();
	KMeanStitch.WarpMeshHomography();
	QueryPerformanceCounter((LARGE_INTEGER*)&TimeEnd);

	double timeDiff = (double)(TimeEnd - TimeStart) / (double)TimeFreq;

	cout << "Parallax handling : " << timeDiff << endl;
}


void ImageStitch_jy::OnBnClickedMeanAuto()
{
	BYTE* img1D_L, *img1D_R;
	int nWL, nHL, nWR, nHR;
	CFileFind Finder;
	CString pathP = _T("C:\\Users\\vision_jy\\Desktop\\1. Image DB\\Image Stitching\\example\\*.*");
	CString fileName, filePath, fileTitle;
	CList<CString> listPath;

	UpdateData(TRUE);

	BOOL Continue = Finder.FindFile(pathP);

	img1D_L = NULL, img1D_R = NULL;

	while (Continue)
	{
		// 포인터를 파일에 접근 시킨다.
		Continue = Finder.FindNextFile();

		// 찾은 파일이 기록파일인지 확인
		if (Finder.IsDots())
		{
			continue;
		}

		fileName = Finder.GetFileName();
		fileName.MakeUpper();

		filePath = Finder.GetFilePath();
		filePath.MakeUpper();

		fileTitle = Finder.GetFileTitle();
		fileTitle.MakeUpper();
		fileTitle = fileTitle.Left(fileTitle.GetLength() - 1);


		if (fileName.Right(5) == _T("R.BMP"))
			img1D_R = ReadBmp(filePath, &nWR, &nHR);

		if (fileName.Right(5) == _T("L.BMP"))
			img1D_L = ReadBmp(filePath, &nWL, &nHL);

		if (fileName.Right(5) == _T("R.JPG"))
			img1D_R = ReadJpeg(filePath, &nWR, &nHR);

		if (fileName.Right(5) == _T("L.JPG"))
			img1D_L = ReadJpeg(filePath, &nWL, &nHL);

		if (img1D_L && img1D_R)
		{
			CImageStitchMeanShift imgStitch(img1D_L, img1D_R, nHL, nWL, nHR, nWR, fileTitle);
			
			imgStitch.GetMatchingPoint(m_EditSIFT);

			imgStitch.SegmentMeanshift(m_EditSegRegion);

			imgStitch.DetermineActive(m_EditMinFeature, m_EditMinRegion, m_EditFeatureRate, m_EditKLDiver);

			if (imgStitch.bExeDualHomo)
			{
				imgStitch.FindSingleObject(m_EditDiff);
				imgStitch.MakeWeightMap(10);
			}

			imgStitch.WarpHomography(true);

			img1D_L = NULL;
			img1D_R = NULL;
		}

	}
}


void ImageStitch_jy::OnBnClickedDraw()
{
	UpdateData(TRUE);

	CImageStitchMeanShift imgTest;

	imgTest.GetMatchingPoint(m_EditSIFT);

	imgTest.SegmentMeanshift(m_EditSegRegion);

	// TODO: Add your control notification handler code here
}

void ImageStitch_jy::OnClickedOcr()
{
	//// TODO: Add your control notification handler code here
	//BYTE** outputText;
	//char *outText;
	//CString text;

	//outputText = cmatrix(100, 500);

	//for (int i = 0; i < 100; i++)
	//	for (int j = 0; j < 500; j++)
	//	{
	//		outputText[i][j] = 255;
	//	}

	//TessBaseAPI *api = new TessBaseAPI();

	//if (api->Init(NULL, "eng")){
	//	fprintf(stderr, "Could not initialize tesseract.\n");
	//	exit(1);
	//}

	//Pix *image = pixRead("C:/3DP/a.png");
	//api->SetImage(image);

	//outText = api->GetUTF8Text();
	//text.Append(outText);
	//DrawTextOnImag2DGray(outputText, 500, 100, 0, 0, text, DT_LEFT, 0, false);
	//DisplayCimage2D(outputText, 500, 100, 0, 0);
}



void ImageStitch_jy::OnVideostitch()
{
	// TODO: Add your command handler code here
}


void ImageStitch_jy::OnBnClickedGetsingleaffine()
{
	CImageStitchBasic SingleHomo;
	bool bOnCheckFeature, bOnCheckMatch, bOnCheckLR;

	bOnCheckFeature = m_checkFeature.GetCheck();
	bOnCheckMatch = m_checkMatch.GetCheck();
	bOnCheckLR = m_checkLR.GetCheck();

	SingleHomo.GetMatchingPoint(bOnCheckFeature, bOnCheckMatch);

	for (int i = 0; i < 10; i++)
	{
		SingleHomo.GetAffine();
		SingleHomo.WarpAffine();
	}
}

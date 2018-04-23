// Image.cpp : implementation of the Image Processing Subroutines
//

#include "stdafx.h"
#include "ImageProcessing.h"

#include "MainFrm.h"		// 추가
#include "ChildFrm.h"		// 추가
#include "ImageProcessingDoc.h"		// 추가
#include "ImageProcessingView.h"	// 추가
#include "opencv2\core\core.hpp"
#include "opencv2\imgproc\imgproc.hpp"
#include "opencv2\features2d\features2d.hpp"
#include "opencv2\nonfree\nonfree.hpp"
#include "opencv2\nonfree\features2d.hpp"
#include "opencv2\calib3d\calib3d.hpp"

#include <vector>
#include <cmath>			// 추가

using namespace cv;

// 2차원 메모리 할당
BYTE **cmatrix(int nH, int nW) {
	BYTE **Temp;
	
	Temp = new BYTE *[nH];
	for(int y = 0 ; y < nH ; y++)
		Temp[y] = new BYTE[nW];

	return Temp;
}

// 2차원 메모리 해제
void free_cmatrix(BYTE **Image, int nH, int nW) {
	for(int y = 0 ; y < nH ; y++)
		delete [] Image[y];

	delete [] Image;
}

// 2차원 메모리 할당
int **imatrix(int nH, int nW) {
	int **Temp;
	
	Temp = new int *[nH];
	for(int y = 0 ; y < nH ; y++)
		Temp[y] = new int[nW];

	return Temp;
}

// 2차원 메모리 해제
void free_imatrix(int **Image, int nH, int nW) {
	for(int y = 0 ; y < nH ; y++)
		delete [] Image[y];

	delete [] Image;
}

// 2차원 메모리 할당
double **dmatrix(int nH, int nW) {
	double **Temp;
	
	Temp = new double *[nH];
	for(int y = 0 ; y < nH ; y++)
		Temp[y] = new double[nW];

	return Temp;
}

// 2차원 메모리 해제
void free_dmatrix(double **Image, int nH, int nW) {
	for(int y = 0 ; y < nH ; y++)
		delete [] Image[y];

	delete [] Image;
}

// 24비트 비트맵 영상의 크기 계산
int GetBmp24Size(int nW, int nH) {
	return (nW*3+3)/4*4 * nH;
}

// 24비트 비트맵 영상의 데이터 위치를 계산
int GetBmp24Pos(int nW, int nH, int x, int y) {
	return (nW*3+3)/4*4 * (nH-1-y) + x*3;
}

// 비트맵 파일 읽기
BYTE *ReadBmp(LPCTSTR FileName, int *pW, int *pH) {
	BITMAPFILEHEADER bmiFileHeader;
	BITMAPINFOHEADER bmiHeader;
	
	CFile cfile;
	BOOL bOpen;
	int nColors;
	
	// 파일 열기
	bOpen = cfile.Open(FileName, CFile::modeRead | CFile::typeBinary);
	// 파일 열기 실패
	if(!bOpen) return NULL;

	// 파일 헤더 정보 읽기
	cfile.Read(&bmiFileHeader, sizeof(BITMAPFILEHEADER));

	// 식별자 인식
	if(bmiFileHeader.bfType != 'M'*0x0100 + 'B') {
		cfile.Close();
		return NULL;
	}

	// 비트맵 정보 헤더 읽기
	cfile.Read(&bmiHeader, sizeof(BITMAPINFOHEADER));
	

	// 압축 여부 확인 - 압축되어 있다면 종료
	if(bmiHeader.biCompression != BI_RGB) {
	    cfile.Close();
	    return NULL;
	}

	// 만일 Colors이 0이면 화소당 비트수에 대한 최대 컬러 개수
	if(bmiHeader.biClrUsed == 0) 
		nColors = 1 << bmiHeader.biBitCount;
	else
		nColors = bmiHeader.biClrUsed;

	// 팔레트 
	RGBQUAD *Palette = NULL;

	// 팔레트 정보 읽기
	switch (bmiHeader.biBitCount) {
		case 24:
			break;

		case 1:
		case 4:
		case 8:
			Palette = new RGBQUAD[nColors];

			int i;
			for(i = 0 ; i < nColors ; i++) {
				BYTE r, g, b, temp;
	
				cfile.Read(&b, sizeof(BYTE));
				cfile.Read(&g, sizeof(BYTE));
				cfile.Read(&r, sizeof(BYTE));
				cfile.Read(&temp, sizeof(BYTE));

				Palette[i].rgbRed = r;
				Palette[i].rgbGreen = g;
				Palette[i].rgbBlue = b;
			}
			break;
	}

	// 비트맵 데이터 위치로 이동
	cfile.Seek(bmiFileHeader.bfOffBits, CFile::begin);

	*pW = bmiHeader.biWidth;
	*pH = bmiHeader.biHeight;
	
	BYTE *Image1D = NULL;

	// 1차원 메모리 할당
	Image1D = new BYTE[GetBmp24Size(*pW, *pH)];

	// 24비트 컬러
	if(bmiHeader.biBitCount == 24)
		cfile.Read(Image1D, *pH*((*pW*3+3)/4*4));

	long Row, Col;

	// 1, 4, 8 비트 컬러
	for(Row = 0 ; Row < bmiHeader.biHeight ; Row++)
	{
		if(bmiHeader.biBitCount != 24) 
		{
			int BitCount = 0;
			UINT mask = (1 << bmiHeader.biBitCount) - 1;

			BYTE ReadByte = 0;
			int ReadByteCnt = 0;

			for(Col = 0; Col < *pW ; Col++) 
			{				
				int PaletteIndex = 0;

				if (BitCount <= 0) {
					BitCount = 8;
					cfile.Read(&ReadByte, sizeof(BYTE));
					ReadByteCnt++;
				}

				BitCount -= bmiHeader.biBitCount;

				// 팔레트 인덱스 번호 계산
				PaletteIndex = (ReadByte >> BitCount) & mask;

				int Pos;

				// 24비트 컬러로 저장
				Pos = (((*pW*3+3)/4*4) * Row) + Col*3;	
				Image1D[Pos++] = Palette[PaletteIndex].rgbBlue;
				Image1D[Pos++] = Palette[PaletteIndex].rgbGreen;
				Image1D[Pos] = Palette[PaletteIndex].rgbRed;
			}

			// 윗줄의 위치 결정
			while (ReadByteCnt&3) 
			{
				char temp;
				cfile.Read(&temp, sizeof(char));
				ReadByteCnt++;
			}
		}
	}

	if(Palette) delete [] Palette;

	cfile.Close();

	return Image1D;
}

// raw 파일 읽기
BYTE *ReadRaw(LPCTSTR FileName, int nW, int nH) {
	BYTE *ReadBytes;
	int x, y, Pos;

	CFile cfile;
	BOOL bOpen;
	
	bOpen = cfile.Open(FileName, CFile::modeRead | CFile::typeBinary);
	// 파일 열기 실패
	if(!bOpen) return NULL;
	
	BYTE *Image1D;

	// 메모리 할당
	Image1D = new BYTE[GetBmp24Size(nW, nH)];
	ReadBytes = new BYTE[nW];

	// 1차원 비트맵 구조로 회색조 raw 영상 읽기
	for(y = 0 ; y < nH ; y++)
	{
		cfile.Read(ReadBytes, nW);

		for(x = 0 ; x < nW ; x++)
		{
			Pos = GetBmp24Pos(nW, nH, x, y);
			Image1D[Pos++] = ReadBytes[x];
			Image1D[Pos++] = ReadBytes[x];
			Image1D[Pos] = ReadBytes[x];
		}
	}

	delete [] ReadBytes;

	cfile.Close();

	return Image1D;
}

// 비트맵 파일 저장(24비트 컬러)
bool SaveBmp(LPCTSTR FileName, BYTE *Image1D, int nW, int nH) {
	unsigned long dwBitsSize;

	unsigned long size;
		
	size = GetBmp24Size(nW, nH);
	dwBitsSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + size;

	// 헤더 정보 저장
	BITMAPINFOHEADER bmiHeader;
	bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmiHeader.biWidth = nW;
	bmiHeader.biHeight = nH;
	bmiHeader.biPlanes = 1;
	bmiHeader.biBitCount = 24;
	bmiHeader.biCompression = BI_RGB;
	bmiHeader.biSizeImage = size;
	bmiHeader.biXPelsPerMeter = 2000;
	bmiHeader.biYPelsPerMeter = 2000;
	bmiHeader.biClrUsed = 0;
	bmiHeader.biClrImportant = 0;

	BITMAPFILEHEADER bmiFileHeader;
	bmiFileHeader.bfType = 'M'*0x0100 + 'B';
	bmiFileHeader.bfSize = dwBitsSize;
	bmiFileHeader.bfReserved1 = 0;
	bmiFileHeader.bfReserved2 = 0;

	bmiFileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

	CFile cfile;
	BOOL bOpen;

	// 파일 열기
	bOpen = cfile.Open(FileName, CFile::modeWrite | CFile::typeBinary | CFile::modeCreate);

	// 파일 열기 실패
	if(!bOpen) return false;

	// 헤더 쓰기
	cfile.Write(&bmiFileHeader, sizeof(BITMAPFILEHEADER));
	cfile.Write(&bmiHeader, sizeof(BITMAPINFOHEADER));

	// 비트맵 정보 쓰기
	cfile.Write(Image1D, sizeof(BYTE)*size);

	cfile.Close();

	return true;
}

// 마지막 영상의 폭, 높이, 위치 읽기
bool GetCurrentImageInfo(int *pW, int *pH, int *pPosX, int *pPosY, int nIndex)
{
	CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd();
	CChildFrame *pChild = (CChildFrame *)pMain->MDIGetActive();

	if(!pChild) return false;
	CImageProcessingView *pView = (CImageProcessingView *)pChild->GetActiveView();

	if(!pView) return false;

	return pView->GetCurrentImageInfo(pW, pH, pPosX, pPosY, nIndex);
}

// 마지막 영상을 2차원 회색조 정보로 읽기
bool GetCurrentImageGray(BYTE **ImageGray, int nIndex)
{
	CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd();
	CChildFrame *pChild = (CChildFrame *)pMain->MDIGetActive();

	if(!pChild) return false;
	CImageProcessingView *pView = (CImageProcessingView *)pChild->GetActiveView();

	if(!pView) return false;

	return pView->GetCurrentImageGray(ImageGray, nIndex);
}

// 2차원 회색조 영상을 출력
bool DisplayCimage2D(BYTE **ImageGray, int nW, int nH, int nPosX, int nPosY, bool bErase, bool bDelete, int Rate)
{
	CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd();
	CChildFrame *pChild = (CChildFrame *)pMain->MDIGetActive();

	if(!pChild) return false;
	CImageProcessingView *pView = (CImageProcessingView *)pChild->GetActiveView();

	if(!pView) return false;

	return pView->DisplayCimage2D(ImageGray, nW, nH, nPosX, nPosY, bErase, bDelete, Rate);
}

// 마지막 영상을 2차원 컬러 정보로 읽기
bool GetCurrentImageColor(BYTE **ImageRed, BYTE **ImageGreen, BYTE **ImageBlue, int nIndex)
{
	CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd();
	CChildFrame *pChild = (CChildFrame *)pMain->MDIGetActive();

	if(!pChild) return false;
	CImageProcessingView *pView = (CImageProcessingView *)pChild->GetActiveView();

	if(!pView) return false;

	return pView->GetCurrentImageColor(ImageRed, ImageGreen, ImageBlue, nIndex);
}

bool GetCurrentImage1DColor(BYTE *Image1D, int nIndex)
{
	CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd();
	CChildFrame *pChild = (CChildFrame *)pMain->MDIGetActive();

	if(!pChild) return false;
	CImageProcessingView *pView = (CImageProcessingView *)pChild->GetActiveView();

	if(!pView) return false;

	return pView->GetCurrentImage1DColor(Image1D, nIndex);
}

bool DisplayCimage1D(BYTE *Image1D, int nW, int nH, int nPosX, int nPosY, bool bErase, bool bDelete)
{
	CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd();
	CChildFrame *pChild = (CChildFrame *)pMain->MDIGetActive();

	if(!pChild) return false;
	CImageProcessingView *pView = (CImageProcessingView *)pChild->GetActiveView();

	if(!pView) return false;

	return pView->DisplayCimage1D(Image1D, nW, nH, 
		nPosX, nPosY, bErase, bDelete);
}

// 2차원 컬러 영상을 출력
bool DisplayCimage2DColor(BYTE **ImageRed, BYTE **ImageGreen, BYTE **ImageBlue, int nW, int nH, 
	int nPosX, int nPosY, bool bErase, bool bDelete, int Rate)
{
	CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd();
	CChildFrame *pChild = (CChildFrame *)pMain->MDIGetActive();

	if(!pChild) return false;
	CImageProcessingView *pView = (CImageProcessingView *)pChild->GetActiveView();

	if(!pView) return false;

	return pView->DisplayCimage2DColor(ImageRed, ImageGreen, ImageBlue, nW, nH, 
		nPosX, nPosY, bErase, bDelete, Rate);
}

bool DisplayIimage2D(int **ImageInt, int nW, int nH, int nPosX, int nPosY, bool bErase, bool bDelete)
{
	CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd();
	CChildFrame *pChild = (CChildFrame *)pMain->MDIGetActive();

	if(!pChild) return false;
	CImageProcessingView *pView = (CImageProcessingView *)pChild->GetActiveView();

	if(!pView) return false;

	return pView->DisplayIimage2D(ImageInt, nW, nH, nPosX, nPosY, bErase, bDelete);
}

bool DisplayDimage2D(double **ImageDouble, int nW, int nH, int nPosX, int nPosY, bool bErase, bool bDelete)
{
	CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd();
	CChildFrame *pChild = (CChildFrame *)pMain->MDIGetActive();

	if(!pChild) return false;
	CImageProcessingView *pView = (CImageProcessingView *)pChild->GetActiveView();

	if(!pView) return false;

	return pView->DisplayDimage2D(ImageDouble, nW, nH, nPosX, nPosY, bErase, bDelete);
}

void SetViewMsg(CString ViewMsg, bool bErase)
{
	CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd();
	CChildFrame *pChild = (CChildFrame *)pMain->MDIGetActive();

	if(!pChild) return;
	CImageProcessingView *pView = (CImageProcessingView *)pChild->GetActiveView();

	if(!pView) return;

	pView->m_ViewMsg = ViewMsg;

	pView->Invalidate(bErase);
}

void Move(BYTE **ImageGray, BYTE **OutputGray, int nW, int nH, int x0, int y0)
{
	int x, y;
	int xx, yy;
	for(y = 0 ; y < nH ; y++)
		for(x = 0 ; x < nW ; x++)
		{
			xx =  x - x0;
			yy = y - y0;

			if(xx < 0 || xx >= nW || yy < 0 || yy >= nH)
				OutputGray[y][x] = 0;
			else
				OutputGray[y][x] = ImageGray[yy][xx];
		}
}

void ScaleX2(BYTE **ImageGray, BYTE **OutputGray, int nW, int nH)
{
	int x, y;
	int nOutW = nW*2, nOutH = nH*2;	// nW, nH: 원영상의 폭과 높이
									// nOutW, nOutH: 변환된 영상의 폭과 높이

	for(y = 0 ; y < nOutH ; y++)
		for(x = 0 ; x < nOutW ; x++)
		{
				OutputGray[y][x] = ImageGray[y/2][x/2];
		}
}

void Rotate(BYTE **ImageGray, BYTE **OutputGray, int nW, int nH, int nOutW, int nOutH, double dDeg)
{
	int x,y;	
	int xx, yy;
	double dAng = dDeg * acos(-1.) / 180.;	// 각도를 라디안으로 수정

	for(y = 0 ; y < nOutH ; y++)
		for(x = 0 ; x < nOutW ; x++)
		{
			xx = (int)((x-nOutW/2)*cos(dAng)+(y-nOutH/2)*sin(dAng));
			yy = (int)(-(x-nOutW/2)*sin(dAng)+(y-nOutH/2)*cos(dAng));

			xx += nW/2;
			yy += nH/2;

			if(xx < 0 || xx >= nW || yy < 0 || yy >= nH)
				OutputGray[y][x] = 0;
			else
				OutputGray[y][x] = ImageGray[yy][xx];
		}
}

void HistogramEqualization(BYTE **ImageGray, BYTE **OutputGray, int nW, int nH)
{
  	BYTE Table[GRAY_CNT];
 	int CumHisto[GRAY_CNT], Histogram[GRAY_CNT] ={0};
	int x, y, i;
	BYTE gray;
		
	for(y = 0 ; y < nH ; y++)
		for(x = 0 ; x < nW ; x++)
			Histogram[ImageGray[y][x]]++;

	int nSum = 0;
	for (i=0; i< GRAY_CNT ; i++) {
		nSum += Histogram[i];
		CumHisto[i] = nSum;
	}

	for (i=0; i< GRAY_CNT ; i++) 
		Table[i] = (BYTE)((CumHisto[i] * (GRAY_CNT-1)) / (nW*nH));
 
	for(y = 0 ; y < nH ; y++)
		for(x = 0 ; x < nW ; x++)
		{
			gray = Table[ImageGray[y][x]];
			if (gray < 0) gray = 0;
			else if (gray >= GRAY_CNT) gray = GRAY_CNT-1;
			OutputGray[y][x] = gray;
		}
}

void MeanFilter(BYTE **ImageGray, BYTE **OutputGray, int nW, int nH, int nWinSize)
{
	int x, y;
	int dx, dy;
	int nSum;
	int xx, yy;

	if(nWinSize <= 0) return;

	double nRange = nWinSize/2.;

	for(y = 0 ; y < nH ; y++)
		for(x = 0 ; x < nW ; x++)
		{
			nSum = 0;
			for(dy = -(int)nRange ; dy < nRange ; dy++)
				for(dx = -(int)nRange ; dx < nRange ; dx++)
				{
					xx = x+dx;
					yy = y+dy;

					if(xx < 0) xx = 0;
					if(xx >= nW) xx = nW-1;
					if(yy < 0) yy = 0;
					if(yy >= nH) yy = nH-1;

					nSum += ImageGray[yy][xx];
				}
			
			OutputGray[y][x] = nSum/(nWinSize*nWinSize);
		}
}

void MedianFilter(BYTE **ImageGray, BYTE **OutputGray, int nW, int nH, int nWinSize)
{
	int x, y;
	int dx, dy;
	int xx, yy;

	if(nWinSize <= 0) return;

	double nRange = nWinSize/2.;

	BYTE *Sort, nInsert;
	int nIndex, i;

	Sort = new BYTE[nWinSize*nWinSize];

	for(y = 0 ; y < nH ; y++)
		for(x = 0 ; x < nW ; x++)
		{
			nIndex = 0;
			for(dy = -(int)nRange ; dy < nRange ; dy++)
				for(dx = -(int)nRange ; dx < nRange ; dx++)
				{
					xx = x+dx;
					yy = y+dy;

					if(xx < 0) xx = 0;
					if(xx >= nW) xx = nW-1;
					if(yy < 0) yy = 0;
					if(yy >= nH) yy = nH-1;

					nInsert = ImageGray[yy][xx];

					// 삽입 정렬
					if(nIndex == 0) Sort[nIndex] = nInsert;
					else
					{
						for(i = nIndex - 1 ; i >= 0 && Sort[i] > nInsert ; i--)
							Sort[i+1] = Sort[i];

						Sort[i+1] = nInsert;
					}
					nIndex++;
				}	
			OutputGray[y][x] = Sort[nWinSize*nWinSize/2];
		}

	delete [] Sort;
}

void Sobel(BYTE **ImageGray, BYTE **OutputGray, int nW, int nH)
{
	int x, y;
	int nGx, nGy;

	for(y = 0 ; y < nH ; y++)
		for(x = 0 ; x < nW ; x++)
		{
			if(x <= 0 || x >= nW-1 || y <= 0 || y >= nH-1)
				OutputGray[y][x] = 0;
			else
			{
				nGx = ImageGray[y-1][x+1] + 2*ImageGray[y][x+1] + ImageGray[y+1][x+1]
					- ImageGray[y-1][x-1] - 2*ImageGray[y][x-1] - ImageGray[y+1][x-1];

				nGy = ImageGray[y+1][x-1] + 2*ImageGray[y+1][x] + ImageGray[y+1][x+1]
					- ImageGray[y-1][x-1] - 2*ImageGray[y-1][x] - ImageGray[y-1][x+1];
				
				OutputGray[y][x] = (abs(nGx) + abs(nGy))/8;
			}
		}
}

void SobelXY(BYTE **ImageGray, double **OutputGrayX, double **OutputGrayY, int nW, int nH)
{
	int x, y;

	for(y = 0 ; y < nH ; y++)
		for(x = 0 ; x < nW ; x++)
		{
			if(x <= 0 || x >= nW-1 || y <= 0 || y >= nH-1)
			{
				OutputGrayX[y][x] = 0;
				OutputGrayY[y][x] = 0;
			}
			else
			{
				OutputGrayX[y][x] = ImageGray[y-1][x+1] + 2*ImageGray[y][x+1] + ImageGray[y+1][x+1]
					- ImageGray[y-1][x-1] - 2*ImageGray[y][x-1] - ImageGray[y+1][x-1];

				OutputGrayY[y][x] = ImageGray[y+1][x-1] + 2*ImageGray[y+1][x] + ImageGray[y+1][x+1]
					- ImageGray[y-1][x-1] - 2*ImageGray[y-1][x] - ImageGray[y-1][x+1];
			}
		}
}

void Laplacian(BYTE **ImageGray, BYTE **OutputGray, int nW, int nH)
{
	int x, y;
	int nSum;

	for(y = 0 ; y < nH ; y++)
		for(x = 0 ; x < nW ; x++)
		{
			if(x <= 0 || x >= nW-1 || y <= 0 || y >= nH-1)
				OutputGray[y][x] = 0;
			else
			{
				nSum =  4*ImageGray[y][x] - ImageGray[y-1][x] - ImageGray[y][x-1] 
					- ImageGray[y][x+1] - ImageGray[y+1][x];

				nSum = (nSum+GRAY_CNT)/2;

				if(nSum < 0) nSum = 0;
				else if(nSum >= GRAY_CNT) nSum = GRAY_CNT-1;
				
				OutputGray[y][x] = nSum;
			}
		}
}

void LaplacianSharp(BYTE **ImageGray, BYTE **OutputGray, int nW, int nH)
{
	int x, y;
	int nSum;

	for(y = 0 ; y < nH ; y++)
		for(x = 0 ; x < nW ; x++)
		{
			if(x <= 0 || x >= nW-1 || y <= 0 || y >= nH-1)
				OutputGray[y][x] = 0;
			else
			{
				nSum =  4*ImageGray[y][x] - ImageGray[y-1][x] - ImageGray[y][x-1] 
					- ImageGray[y][x+1] - ImageGray[y+1][x];

				nSum = ImageGray[y][x] + nSum;

				if(nSum < 0) nSum = 0;
				else if(nSum >= GRAY_CNT) nSum = GRAY_CNT-1;
				
				OutputGray[y][x] = nSum;
			}
		}
}

int NeighborMatch(BYTE  **ImageGray, int x, int y, int pat)
{
	switch (pat) {

	   case 0:
	      if ((ImageGray[y-1][x-1] != 0 || ImageGray[y-1][x] != 0 || ImageGray[y-1][x+1] != 0) &&
		  (ImageGray[y+1][x-1] != 0 || ImageGray[y+1][x] != 0 || ImageGray[y+1][x+1] != 0) &&
		  (ImageGray[y][x-1] == 0 && ImageGray[y][x+1] == 0) )
		  return (TRUE);
	      break;

	   case 1:
	      if ((ImageGray[y-1][x-1] != 0 || ImageGray[y][x-1] != 0 || ImageGray[y+1][x-1] != 0) &&
		  (ImageGray[y-1][x+1] != 0 || ImageGray[y][x+1] != 0 || ImageGray[y+1][x+1] != 0) &&
		  (ImageGray[y+1][x] == 0 && ImageGray[y-1][x] == 0) )
		  return (TRUE);
	      break;

	   case 2:
	      if ((ImageGray[y-1][x+1] != 0 || ImageGray[y-1][x] != 0 || ImageGray[y-1][x-1] != 0 ||
		   ImageGray[y][x-1] != 0 || ImageGray[y+1][x-1] != 0) &&
		   (ImageGray[y][x+1] == 0 && ImageGray[y+1][x] == 0 && ImageGray[y+1][x+1] == 2) )
                  return (TRUE);
	      break;

	   case 3:
	      if ((ImageGray[y-1][x-1] != 0 || ImageGray[y][x-1] != 0 || ImageGray[y+1][x-1] != 0 ||
		   ImageGray[y+1][x] != 0 || ImageGray[y+1][x+1] != 0) &&
		   (ImageGray[y][x+1] == 0 && ImageGray[y-1][x] == 0 && ImageGray[y-1][x+1] == 2) )
                  return (TRUE);
	      break;

	   case 4:
	      if ((ImageGray[y+1][x-1] != 0 || ImageGray[y+1][x] != 0 || ImageGray[y+1][x+1] != 0 ||
		   ImageGray[y][x+1] != 0 || ImageGray[y-1][x+1] != 0) &&
		   (ImageGray[y-1][x] == 0 && ImageGray[y][x-1] == 0 && ImageGray[y-1][x-1] == 2) )
                  return (TRUE);
	      break;

	   case 5:
	      if ((ImageGray[y+1][x+1] != 0 || ImageGray[y][x+1] != 0 || ImageGray[y-1][x+1] != 0 ||
		   ImageGray[y-1][x] != 0 || ImageGray[y-1][x-1] != 0) &&
		   (ImageGray[y][x-1] == 0 && ImageGray[y+1][x] == 0 && ImageGray[y+1][x-1] == 2) )
                  return (TRUE);
	      break;

	   default:
	      break;
	}

        return (FALSE);

}

void Thinning(BYTE **ImageGray, BYTE **OutputGray, int nW, int nH)
{
	int x, y;
	int FourNY[4] = {0, -1, 0, 1};
	int FourNX[4] = {1, 0, -1, 0};

	for(y = 0 ; y < nH ; y++) 
		for(x = 0 ; x < nW; x++) 
		{
			if(ImageGray[y][x] > GRAY_CNT/2) OutputGray[y][x] = 1;
			else OutputGray[y][x] = 0;
		}

	BOOL Remain = TRUE;
	int j, Pattern;
	BOOL bSkeleton;
	while(Remain) 
	{
		Remain = FALSE;
		for(j = 0 ; j < 4; j++) 
		{
			for(y = 1 ; y < nH-1 ; y++) 
				for(x = 1 ; x < nW-1 ; x++) 
				{
					if(OutputGray[y][x] == 1 && OutputGray[y+FourNY[j]][x+FourNX[j]] == 0) {
						bSkeleton = FALSE;
						for(Pattern = 0 ; Pattern < 6; Pattern++) {
							if (NeighborMatch(OutputGray, x, y, Pattern)) {
								bSkeleton = TRUE;
								break;
							}
						}

						if(bSkeleton) OutputGray[y][x] = 2;
						else {
							OutputGray[y][x] = 3;
							Remain = TRUE;
						}
					}
				}

			for(y = 1 ; y < nH-1 ; y++) 
				for(x = 1 ; x < nW-1 ; x++) 
				{
					if(OutputGray[y][x] == 3) OutputGray[y][x] = 0;
				}
		}
	}

	for(y = 0 ; y < nH ; y++) 
		for(x = 0 ; x < nW ; x++) 
		{
			if(OutputGray[y][x] == 2) OutputGray[y][x] = GRAY_CNT-1;
			else OutputGray[y][x] = 0;
        }
}
	
int Labeling(BYTE **ImageGray, int **Label, int nW, int nH, int nConnNumThre)
{
	int x, y, num, left, top, k; 
	int *r, *area;

	r = new int[nW*nH];
	area = new int[nW*nH];

	for(y = 0 ; y < nH ; y++)
		for(x = 0 ; x < nW ; x++)
			if(ImageGray[y][x] > 128) Label[y][x] = 0;
			else Label[y][x] = -1;

	for (x = 0 ; x < nW ; x++) {
		Label[0][x] = -1;
		Label[nH-1][x] = -1;
	}
	for (y = 0 ; y < nH ; y++) {
		Label[y][0] = -1;
		Label[y][nW-1] = -1;
	}

	num = -1;
	for (y = 0; y < nH; y++) {
		for (x = 0; x < nW; x++) {
			if (y > 0 && x > 0) {
				if (Label[y][x] >= 0) {
					left = Label[y][x-1];
					top = Label[y-1][x];
					if (left == -1 && top != -1) {
						Label[y][x] = r[top];
					}
					else if (left != -1 && top == -1) {
						Label[y][x] = r[left];
					}
					else if (left == -1 && top == -1) {
						num++;
						if (num >= nW*nH) {
							delete [] r;
							delete [] area;
							return 0;
						}
		       
						r[num] = num;
						Label[y][x] = r[num];
					}
					else if (left != -1 && top != -1) {
						if (r[left] == r[top]) {
							Label[y][x] = r[left];
						}
						else if (r[left] > r[top]) {
							Label[y][x] = r[top];
							r[left] = r[top];
						}
						else {
							Label[y][x] = r[left];
							r[top] = r[left];
						}
					}
				}
			}
		}
	}
	
	for (k = 0; k <= num; k++) {
		if (k != r[k]) r[k] = r[r[k]];
			area[k] = 0;
	}

	for(y = 0 ; y < nH; y++)
		for(x = 0 ; x < nW ; x++) {
			if(Label[y][x] > -1) {
				Label[y][x] = r[Label[y][x]];
				area[Label[y][x]]++;
			}
		}

	int cnt=0;
	for (k = 0; k <= num; k++) {
		if (area[k] > nConnNumThre) r[k] = cnt++;
		else r[k] = -1;
	}

	for(y = 0 ; y < nH; y++)
		for(x = 0 ; x < nW ; x++) {
			if(Label[y][x] >= 0) 
				Label[y][x] = r[Label[y][x]];
		}

	delete [] r;
	delete [] area;

	return cnt;
}

int Labeling(BYTE **ImageGray, int **Label, int nW, int nH, int nConnNumThre, int *Area)
{
	int x, y, num, left, top, k; 
	int *r, *rev;

	r = new int[nW*nH];
	rev = new int[nW*nH];

	for(y = 0 ; y < nH ; y++)
		for(x = 0 ; x < nW ; x++)
			if(ImageGray[y][x] > 128) Label[y][x] = 0;
			else Label[y][x] = -1;

	for (x = 0 ; x < nW ; x++) {
		Label[0][x] = -1;
		Label[nH-1][x] = -1;
	}
	for (y = 0 ; y < nH ; y++) {
		Label[y][0] = -1;
		Label[y][nW-1] = -1;
	}

	num = -1;
	for (y = 0; y < nH; y++) {
		for (x = 0; x < nW; x++) {
			if (y > 0 && x > 0) {
				if (Label[y][x] >= 0) {
					left = Label[y][x-1];
					top = Label[y-1][x];
					if (left == -1 && top != -1) {
						Label[y][x] = r[top];
					}
					else if (left != -1 && top == -1) {
						Label[y][x] = r[left];
					}
					else if (left == -1 && top == -1) {
						num++;
						if (num >= nW*nH) {
							delete [] r;
							return 0;
						}
		       
						r[num] = num;
						Label[y][x] = r[num];
					}
					else if (left != -1 && top != -1) {
						if (r[left] == r[top]) {
							Label[y][x] = r[left];
						}
						else if (r[left] > r[top]) {
							Label[y][x] = r[top];
							r[left] = r[top];
						}
						else {
							Label[y][x] = r[left];
							r[top] = r[left];
						}
					}
				}
			}
		}
	}
	
	for (k = 0; k <= num; k++) {
		if (k != r[k]) r[k] = r[r[k]];
			Area[k] = 0;
	}

	for(y = 0 ; y < nH; y++)
		for(x = 0 ; x < nW ; x++) {
			if(Label[y][x] > -1) {
				Label[y][x] = r[Label[y][x]];
				Area[Label[y][x]]++;
			}
		}

	int cnt=0;
	for (k = 0; k <= num; k++) {
		if (Area[k] > nConnNumThre) {
			rev[cnt] = k;
			r[k] = cnt++;
		}
		else r[k] = -1;
	}

	for(k = 0 ; k < cnt ; k++)
	{
		Area[k] = Area[rev[k]];
	}

	for(y = 0 ; y < nH; y++)
		for(x = 0 ; x < nW ; x++) {
			if(Label[y][x] >= 0) 
				Label[y][x] = r[Label[y][x]];
		}

	delete [] r;
	delete [] rev;

	return cnt;
}

int Labeling(BYTE **ImageGray, int **Label, int nW, int nH, int nConnNumThre, int nConnNumThre2)
{
	int x, y, num, left, top, k; 
	int *r, *area;

	r = new int[nW*nH];
	area = new int[nW*nH];

	for(y = 0 ; y < nH ; y++)
		for(x = 0 ; x < nW ; x++)
			if(ImageGray[y][x] > 128) Label[y][x] = 0;
			else Label[y][x] = -1;

	for (x = 0 ; x < nW ; x++) {
		Label[0][x] = -1;
		Label[nH-1][x] = -1;
	}
	for (y = 0 ; y < nH ; y++) {
		Label[y][0] = -1;
		Label[y][nW-1] = -1;
	}

	num = -1;
	for (y = 0; y < nH; y++) {
		for (x = 0; x < nW; x++) {
			if (y > 0 && x > 0) {
				if (Label[y][x] >= 0) {
					left = Label[y][x-1];
					top = Label[y-1][x];
					if (left == -1 && top != -1) {
						Label[y][x] = r[top];
					}
					else if (left != -1 && top == -1) {
						Label[y][x] = r[left];
					}
					else if (left == -1 && top == -1) {
						num++;
						if (num >= nW*nH) {
							delete [] r;
							delete [] area;
							return 0;
						}
		       
						r[num] = num;
						Label[y][x] = r[num];
					}
					else if (left != -1 && top != -1) {
						if (r[left] == r[top]) {
							Label[y][x] = r[left];
						}
						else if (r[left] > r[top]) {
							Label[y][x] = r[top];
							r[left] = r[top];
						}
						else {
							Label[y][x] = r[left];
							r[top] = r[left];
						}
					}
				}
			}
		}
	}
	
	for (k = 0; k <= num; k++) {
		if (k != r[k]) r[k] = r[r[k]];
			area[k] = 0;
	}

	for(y = 0 ; y < nH; y++)
		for(x = 0 ; x < nW ; x++) {
			if(Label[y][x] > -1) {
				Label[y][x] = r[Label[y][x]];
				area[Label[y][x]]++;
			}
		}

	int cnt=0;
	for (k = 0; k <= num; k++) {
		if (area[k] > nConnNumThre && area[k] < nConnNumThre2) r[k] = cnt++;
		else r[k] = -1;
	}

	for(y = 0 ; y < nH; y++)
		for(x = 0 ; x < nW ; x++) {
			if(Label[y][x] >= 0) 
				Label[y][x] = r[Label[y][x]];
		}

	delete [] r;
	delete [] area;

	return cnt;
}

double DFT2D(BYTE **ImageGray, double **OutputReal, double **OutputImag, int nW, int nH)
{
	int x, y;
	int u, v;
	
	const double Pi = acos(-1.);

	_int64 freq, start, end;

	QueryPerformanceFrequency((LARGE_INTEGER*)&freq);
	QueryPerformanceCounter((LARGE_INTEGER*)&start);

	for(v = 0 ; v < nH ; v++)
		for(u = 0 ; u < nW ; u++)
		{
			OutputReal[v][u] = OutputImag[v][u] = 0;
			for(y = 0 ; y < nH ; y++)
				for(x = 0 ; x < nW ; x++)
				{
					OutputReal[v][u] += ImageGray[y][x] * cos(-2.*Pi*((double)u*x/nW+(double)v*y/nH));
					OutputImag[v][u] += ImageGray[y][x] * sin(-2.*Pi*((double)u*x/nW+(double)v*y/nH));
				}
		}

	QueryPerformanceCounter((LARGE_INTEGER*)&end);
	// 처리 시간(초)
	double timeDiff = (double)(end - start)/(double)freq; 

	return timeDiff;
}

double RowColDFT2D(BYTE **ImageGray, double **OutputReal, double **OutputImag, int nW, int nH)
{
	int x, y;
	int u, v;
	double **Real, **Imag;

	const double Pi = acos(-1.);

	_int64 freq, start, end;

	QueryPerformanceFrequency((LARGE_INTEGER*)&freq);
	QueryPerformanceCounter((LARGE_INTEGER*)&start);

	Real = dmatrix(nH, nW);
	Imag = dmatrix(nH, nW);

	for(y = 0 ; y < nH ; y++)
		for(u = 0 ; u < nW ; u++)
		{
			Real[y][u] = Imag[y][u] = 0;
			for(x = 0 ; x < nW ; x++)
			{
				Real[y][u] += ImageGray[y][x] * cos(-2.*Pi*u*x/nW);
				Imag[y][u] += ImageGray[y][x] * sin(-2.*Pi*u*x/nW);
			}
		}

	for(u = 0 ; u < nW ; u++)
		for(v = 0 ; v < nH ; v++)		
		{
			OutputReal[v][u] = OutputImag[v][u] = 0;
			for(y = 0 ; y < nH ; y++)
				{
					OutputReal[v][u] += Real[y][u] * cos(-2.*Pi*v*y/nH) - Imag[y][u] * sin(-2.*Pi*v*y/nH);
					OutputImag[v][u] += Real[y][u] * sin(-2.*Pi*v*y/nH) + Imag[y][u] * cos(-2.*Pi*v*y/nH);
				}
		}

	free_dmatrix(Real, nH, nW);
	free_dmatrix(Imag, nH, nW);

	QueryPerformanceCounter((LARGE_INTEGER*)&end);
	// 처리 시간(초)
	double timeDiff = (double)(end - start)/(double)freq; 

	return timeDiff;
}

void FFT2Radix(double *Xr, double *Xi, double *Yr, double *Yi, int nN, bool bInverse)
{
	int i, j, k;

	double T, Wr, Wi;
	const double Pi = acos(-1.);

	if(nN <= 1) return;

	for(i = 0 ; i < nN ; i++)
	{
		Yr[i] = Xr[i];
		Yi[i] = Xi[i];
	}

    j = 0;
    for (i = 1 ; i < (nN-1) ; i++) {
        k = nN/2;
        while(k <= j) {
            j = j - k;
            k = k/2;
        }
        j = j + k;
        if (i < j) {
            T = Yr[j];
            Yr[j] = Yr[i];
			Yr[i] = T;

			T = Yi[j];
            Yi[j] = Yi[i];
			Yi[i] = T;
        }
    }

	double Tr, Ti;
	int iter, j2, pos;
	k = nN >> 1;
	iter = 1;
	while(k > 0)
	{
		j = 0;
		j2 = 0;
		for(i = 0 ; i < nN >> 1 ; i++)
		{
			Wr = cos(2.*Pi*(j2*k)/nN);
			if(bInverse == 0)
				Wi = -sin(2.*Pi*(j2*k)/nN);
			else
				Wi = sin(2.*Pi*(j2*k)/nN);

			pos = j+(1 << (iter-1));

			Tr =	Yr[pos] * Wr - Yi[pos] * Wi;
			Ti = Yr[pos] * Wi +	Yi[pos] * Wr;

			Yr[pos] = Yr[j] - Tr;
			Yi[pos] = Yi[j] - Ti;

			Yr[j] += Tr;			
			Yi[j] += Ti;

			j += 1 << iter;
			if(j >= nN) j = ++j2;
		}

		k >>= 1;
		iter++;
	}

	if(bInverse)
	{
		for(i = 0 ; i < nN ; i++)
		{
			Yr[i] /= nN;
			Yi[i] /= nN;
		}
	}
}

double FFT2D(BYTE **ImageGray, double **OutputReal, double **OutputImag, int nW, int nH)
{
	int x, y;
	double *dRealX, *dImagX;
	double *dRealY, *dImagY;

	const double Pi = acos(-1.);

	_int64 freq, start, end;

	QueryPerformanceFrequency((LARGE_INTEGER*)&freq);
	QueryPerformanceCounter((LARGE_INTEGER*)&start);

	dRealX = new double[nW];
	dImagX = new double[nW];
	dRealY = new double[nW];
	dImagY = new double[nW];

	for(y = 0 ; y < nH ; y++)
	{
		for(x = 0 ; x < nW ; x++)
		{
			dRealX[x] = ImageGray[y][x];
			dImagX[x] = 0.;
		}
		
		FFT2Radix(dRealX, dImagX, dRealY, dImagY, nW, false);

		for(x = 0 ; x < nW ; x++)
		{
			OutputReal[y][x] = dRealY[x];
			OutputImag[y][x] = dImagY[x];
		}
	}

	delete [] dRealX;
	delete [] dImagX;
	delete [] dRealY;
	delete [] dImagY;

	dRealX = new double[nH];
	dImagX = new double[nH];
	dRealY = new double[nH];
	dImagY = new double[nH];

	for(x = 0 ; x < nW ; x++)
	{
		for(y = 0 ; y < nH ; y++)
		{
			dRealX[y] = OutputReal[y][x];
			dImagX[y] = OutputImag[y][x];
		}
		
		FFT2Radix(dRealX, dImagX, dRealY, dImagY, nH, false);

		for(y = 0 ; y < nH ; y++)
		{
			OutputReal[y][x] = dRealY[y];
			OutputImag[y][x] = dImagY[y];
		}
	}

	delete [] dRealX;
	delete [] dImagX;
	delete [] dRealY;
	delete [] dImagY;

	QueryPerformanceCounter((LARGE_INTEGER*)&end);
	// 처리 시간(초)
	double timeDiff = (double)(end - start)/(double)freq; 

	return timeDiff;
}

void FFT2DInverse(double **InputReal, double **InputImag, double **OutputDouble, int nW, int nH)
{
	int x, y;
	double *dRealX, *dImagX;
	double *dRealY, *dImagY;
	double **OutputReal, **OutputImag;

	const double Pi = acos(-1.);

	OutputReal = dmatrix(nH, nW);
	OutputImag = dmatrix(nH, nW);

	dRealX = new double[nW];
	dImagX = new double[nW];
	dRealY = new double[nW];
	dImagY = new double[nW];

	for(y = 0 ; y < nH ; y++)
	{
		for(x = 0 ; x < nW ; x++)
		{
			dRealX[x] = InputReal[y][x];
			dImagX[x] = InputImag[y][x];
		}
		
		FFT2Radix(dRealX, dImagX, dRealY, dImagY, nW, true);

		for(x = 0 ; x < nW ; x++)
		{
			OutputReal[y][x] = dRealY[x];
			OutputImag[y][x] = dImagY[x];
		}
	}

	delete [] dRealX;
	delete [] dImagX;
	delete [] dRealY;
	delete [] dImagY;

	dRealX = new double[nH];
	dImagX = new double[nH];
	dRealY = new double[nH];
	dImagY = new double[nH];

	for(x = 0 ; x < nW ; x++)
	{
		for(y = 0 ; y < nH ; y++)
		{
			dRealX[y] = OutputReal[y][x];
			dImagX[y] = OutputImag[y][x];
		}
		
		FFT2Radix(dRealX, dImagX, dRealY, dImagY, nH, true);

		for(y = 0 ; y < nH ; y++)
		{
			OutputReal[y][x] = dRealY[y];
			OutputImag[y][x] = dImagY[y];
		}
	}

	delete [] dRealX;
	delete [] dImagX;
	delete [] dRealY;
	delete [] dImagY;

	for(x = 0 ; x < nW ; x++)
	{
		for(y = 0 ; y < nH ; y++)
		{
			OutputDouble[y][x] = OutputReal[y][x];
		}
	}

	free_dmatrix(OutputReal, nH, nW);
	free_dmatrix(OutputImag, nH, nW);
}

void Filtering(BYTE **ImageGray, double **OutputDouble, int nW, int nH, int nPosX, int nPosY, 
	int nFilterType1, int nFilterType2, int nDC, int nN)
{
	double **FFTReal, **FFTImag;
	double **Filter;
	int u, v;

	FFTReal = dmatrix(nH, nW);
	FFTImag = dmatrix(nH, nW);

	Filter = dmatrix(nH, nW);

	FFT2D(ImageGray, FFTReal, FFTImag, nW, nH);

	for(v = 0 ; v < nH ; v++)
		for(u = 0 ; u < nW ; u++)
		{
			double Distance = sqrt(pow((double)((v+nH/2)%nH-nH/2), 2) + pow((double)((u+nW/2)%nW-nW/2), 2));
			if(nFilterType1 == 0) // 이상적인 필터
			{
				if(nFilterType2 == 0) // LPF
				{
					Filter[(v+nH/2)%nH][(u+nW/2)%nW] = 1;
					if(Distance > nDC)
						Filter[(v+nH/2)%nH][(u+nW/2)%nW] = 0;
				}
				else // HPF
				{
					Filter[(v+nH/2)%nH][(u+nW/2)%nW] = 1;
					if(Distance < nDC)
						Filter[(v+nH/2)%nH][(u+nW/2)%nW] = 0;
				}
			}
			else // 버터워즈 필터
			{
				if(nFilterType2 == 0) // LPF
					Filter[(v+nH/2)%nH][(u+nW/2)%nW] = 1./(1+pow(Distance/nDC, nN));
				else // HPF
					Filter[(v+nH/2)%nH][(u+nW/2)%nW] = 1./(1+pow(nDC/Distance, nN));
			}
		}

	for(v = 0 ; v < nH ; v++)
		for(u = 0 ; u < nW ; u++)
		{				
			FFTReal[v][u] *= Filter[(v+nH/2)%nH][(u+nW/2)%nW];
			FFTImag[v][u] *= Filter[(v+nH/2)%nH][(u+nW/2)%nW];
		}

	FFT2DInverse(FFTReal, FFTImag, OutputDouble, nW, nH);

	DisplayDimage2D(Filter, nW, nH, nPosX, nPosY+nH);

	free_dmatrix(FFTReal, nH, nW);
	free_dmatrix(FFTImag, nH, nW);

	free_dmatrix(Filter, nH, nW);
}

void DCT2D(BYTE **ImageGray, double **OutputDouble, int nW, int nH, int nBlockSize)
{
	int x, y;
	int u, v;
	int BlockX, BlockY;

	const double Pi = acos(-1.);

	for(v = 0 ; v < nH ; v++)
		for(u = 0 ; u < nW ; u++)
			OutputDouble[v][u] = 0;

	for(BlockY = 0 ; BlockY < nH-nBlockSize+1 ; BlockY += nBlockSize)
		for(BlockX = 0 ; BlockX < nW-nBlockSize+1 ; BlockX += nBlockSize)
		{
			for(v = 0 ; v < nBlockSize ; v++)
				for(u = 0 ; u < nBlockSize ; u++)
				{
					OutputDouble[BlockY+v][BlockX+u] = 0;

					for(y = 0 ; y < nBlockSize ; y++)
						for(x = 0 ; x < nBlockSize ; x++)
						{
							OutputDouble[BlockY+v][BlockX+u] += 
								ImageGray[BlockY+y][BlockX+x] 
								* cos((2*x+1)*u*Pi/(2.*nBlockSize)) * cos((2*y+1)*v*Pi/(2.*nBlockSize));
						}

					if(u == 0)
						OutputDouble[BlockY+v][BlockX+u] *= sqrt(1./nBlockSize);
					else
						OutputDouble[BlockY+v][BlockX+u] *= sqrt(2./nBlockSize);

					if(v == 0)
						OutputDouble[BlockY+v][BlockX+u] *= sqrt(1./nBlockSize);
					else
						OutputDouble[BlockY+v][BlockX+u] *= sqrt(2./nBlockSize);
				}
		}
}

void Compression(BYTE **ImageGray, BYTE **OutputGray, int nW, int nH)
{
	int x, y;
	int u, v;
	int BlockX, BlockY;
	double **DCT, **InverseDCT;
	int nBlockSize = 8;
	int nZeroCount = 0;

	int Q[8][8] = {
		16, 11, 10, 16, 24, 40, 51, 61,
		12, 12, 14, 19, 26, 58, 60, 66,
		14, 13, 16, 24, 40, 57, 69, 57,
		14, 17, 22, 29, 51, 87, 80, 62,
		18, 22, 37, 56, 68, 109, 103, 77,
		24, 36, 55, 64, 81, 104, 113, 92,
		49, 64, 78, 87, 103, 121, 120, 101,
		72, 92, 95, 98, 112, 100, 103, 99
	};

	DCT = dmatrix(nH, nW);
	InverseDCT = dmatrix(nH, nW);

	const double Pi = acos(-1.);

	for(v = 0 ; v < nH ; v++)
		for(u = 0 ; u < nW ; u++)
			DCT[v][u] = 0;

	for(BlockY = 0 ; BlockY < nH-nBlockSize+1 ; BlockY += nBlockSize)
		for(BlockX = 0 ; BlockX < nW-nBlockSize+1 ; BlockX += nBlockSize)
		{
			for(v = 0 ; v < nBlockSize ; v++)
				for(u = 0 ; u < nBlockSize ; u++)
				{
					DCT[BlockY+v][BlockX+u] = 0;

					for(y = 0 ; y < nBlockSize ; y++)
						for(x = 0 ; x < nBlockSize ; x++)
						{
							DCT[BlockY+v][BlockX+u] += 
								ImageGray[BlockY+y][BlockX+x] 
								* cos((2*x+1)*u*Pi/(2.*nBlockSize)) * cos((2*y+1)*v*Pi/(2.*nBlockSize));
						}

					if(u == 0)
						DCT[BlockY+v][BlockX+u] *= sqrt(1./nBlockSize);
					else
						DCT[BlockY+v][BlockX+u] *= sqrt(2./nBlockSize);

					if(v == 0)
						DCT[BlockY+v][BlockX+u] *= sqrt(1./nBlockSize);
					else
						DCT[BlockY+v][BlockX+u] *= sqrt(2./nBlockSize);

					DCT[BlockY+v][BlockX+u] = (int)(DCT[BlockY+v][BlockX+u])/Q[v][u];

					if(DCT[BlockY+v][BlockX+u] == 0) nZeroCount++;
				}
		}

	double C1, C2;

	for(y = 0 ; y < nH ; y++)
		for(x = 0 ; x < nW ; x++)
			InverseDCT[y][x] = 0;

	for(BlockY = 0 ; BlockY < nH-nBlockSize+1 ; BlockY += nBlockSize)
		for(BlockX = 0 ; BlockX < nW-nBlockSize+1 ; BlockX += nBlockSize)
		{
			for(y = 0 ; y < nBlockSize ; y++)
				for(x = 0 ; x < nBlockSize ; x++)
				{
					InverseDCT[BlockY+y][BlockX+x] = 0;

					for(v = 0 ; v < nBlockSize ; v++)
						for(u = 0 ; u < nBlockSize ; u++)
						{
							if(u == 0)
								C1 = sqrt(1./nBlockSize);
							else
								C1 = sqrt(2./nBlockSize);

							if(v == 0)
								C2 = sqrt(1./nBlockSize);
							else
								C2 = sqrt(2./nBlockSize);

							InverseDCT[BlockY+y][BlockX+x] += 
								C1 * C2  * DCT[BlockY+v][BlockX+u] * Q[v][u]
								* cos((2*x+1)*u*Pi/(2.*nBlockSize)) * cos((2*y+1)*v*Pi/(2.*nBlockSize));
						}

					if(InverseDCT[BlockY+y][BlockX+x] < 0)
						OutputGray[BlockY+y][BlockX+x] = 0;
					else if(InverseDCT[BlockY+y][BlockX+x] > GRAY_CNT-1)
						OutputGray[BlockY+y][BlockX+x] = GRAY_CNT-1;
					else
						OutputGray[BlockY+y][BlockX+x] = (BYTE)InverseDCT[BlockY+y][BlockX+x];
				}
		}

	// RMSE 계산
	double Rmse = 0;
	for(y = 0 ; y < nH ; y++)
		for(x = 0 ; x < nW ; x++)
			Rmse += pow((double)(OutputGray[y][x] - ImageGray[y][x]), 2);
	Rmse /= nW*nH;
	Rmse = sqrt(Rmse);

	// 결과 출력
	CString ViewMsg;
	ViewMsg.Format(_T("0: %d, RMSE: %7.3lf"), nZeroCount, Rmse);
	SetViewMsg(ViewMsg);

	free_dmatrix(DCT, nH, nW);
	free_dmatrix(InverseDCT, nH, nW);
}

void pTileThresholding(BYTE **ImageGray, BYTE **OutputGray, int nW, int nH, double dRate, int nType)
{
	int x, y, i, Thre;
	int Sum;
	int Histogram[GRAY_CNT] ={0};

	for(y = 0 ; y < nH ; y++)
		for(x = 0 ; x < nW ; x++)
			Histogram[ImageGray[y][x]]++;

	Sum = 0;

	if(nType == 0) // 상위
	{
		for(i = GRAY_CNT-1 ; i >= 0 ; i--)
		{
			Sum += Histogram[i];
			if(Sum > nW*nH*dRate)
			{
				Thre = i;
				break;
			}
		}
	}
	else	// 하위
	{
		for(i = 0 ; i < GRAY_CNT ; i++)
		{
			Sum += Histogram[i];
			if(Sum > nW*nH*dRate)
			{
				Thre = i;
				break;
			}
		}
	}

	for(y = 0 ; y < nH ; y++)
		for(x = 0 ; x < nW ; x++)
			if(!nType && ImageGray[y][x] > Thre ||	// 상위
				nType && ImageGray[y][x] < Thre)	// 하위
				OutputGray[y][x] = GRAY_CNT-1;
			else
				OutputGray[y][x] = 0;
}

void ValleyThresholding(BYTE **ImageGray, BYTE **OutputGray, int nW, int nH)
{
	int x, y, i, Thre;
	int M1, M2;
	int Max;
	int Histogram[GRAY_CNT] ={0};

	for(y = 0 ; y < nH ; y++)
		for(x = 0 ; x < nW ; x++)
			Histogram[ImageGray[y][x]]++;

	Max = 0;
	for(i = 0 ; i < GRAY_CNT ; i++)
	{
		if(Histogram[i] > Max) 
		{
			Max = Histogram[i];
			M1 = i;
		}
	}

	Max = 0;
	for(i = 0 ; i < GRAY_CNT ; i++)
	{
		if(abs(i-M1)*Histogram[i] > Max) 
		{
			Max = abs(i-M1)*Histogram[i];
			M2 = i;
		}
	}

	Thre = (M1 + M2)/2;
	
	for(y = 0 ; y < nH ; y++)
		for(x = 0 ; x < nW ; x++)
			if(ImageGray[y][x] > Thre)
				OutputGray[y][x] = GRAY_CNT-1;
			else
				OutputGray[y][x] = 0;
}

void IterateThresholding(BYTE **ImageGray, BYTE **OutputGray, int nW, int nH)
{
	int x, y, i, Thre, PrevThre;
	int M1, M2;
	int nCnt1, nCnt2;
	int Histogram[GRAY_CNT] ={0};

	for(y = 0 ; y < nH ; y++)
		for(x = 0 ; x < nW ; x++)
			Histogram[ImageGray[y][x]]++;

	for(i = 0 ; i < GRAY_CNT ; i++)
	{
		if(Histogram[i] > 0)
		{
			M1 = i;
			break;
		}
	}

	for(i = GRAY_CNT-1 ; i >= 0 ; i--)
	{
		if(Histogram[i] > 0)
		{
			M2 = i;
			break;
		}
	}

	Thre = (M1 + M2)/2;
	PrevThre = -1;

	while(PrevThre != Thre)
	{
		M1 = M2 = 0;
		nCnt1 = nCnt2 = 0;
		for(i = 0 ; i < GRAY_CNT ; i++)
			if(i > Thre)
			{
				M2 += i*Histogram[i];
				nCnt2 += Histogram[i];
			}
			else
			{
				M1 += i*Histogram[i];
				nCnt1 += Histogram[i];
			}

		M1 /= nCnt1;
		M2 /= nCnt2;

		PrevThre = Thre;
		Thre = (M1 + M2)/2;
	}
	
	for(y = 0 ; y < nH ; y++)
		for(x = 0 ; x < nW ; x++)
			if(ImageGray[y][x] > Thre)
				OutputGray[y][x] = GRAY_CNT-1;
			else
				OutputGray[y][x] = 0;
}

void HoughTransform(BYTE **ImageGray, double **HoughSpace, int nW, int nH) 
{
	int nSW = 180;
	int nSH = (int)(sqrt((double)(nW*nW+nH*nH))/2)*2;
	int x, y;
	int r, theta;

	const double Pi = acos(-1.);

	for(r = 0 ; r < nSH ; r++)
		for(theta = 0 ; theta < nSW ; theta++)
			HoughSpace[r][theta] = 0;

	for(y = 0 ; y < nH ; y++)
		for(x = 0 ; x < nW ; x++)
		{
			if(ImageGray[y][x] > GRAY_CNT/2)
			{
				for(theta = 0 ; theta < nSW ; theta++)
				{
					r = (int)(y*sin(theta*Pi/180) + x*cos(theta*Pi/180));

					HoughSpace[nSH/2+r/2][theta]++;
				}
			}
		}
}

void Turtle(BYTE **ImageGray, int nW, int nH, CPoint *Boundary, int *pCnt)
{
	int x, y;
	int DirX, DirY;
	int TempDirX;

	for(y = 0 ; y < nH ; y++)
		for(x = 0 ; x < nW ; x++)
			if(x == 0 || x == nW-1 || y == 0 || y == nH-1)
				ImageGray[y][x] = 0;

	DirX = DirY = 0;
	for(y = 0 ; y < nH ; y++)
		for(x = 0 ; x < nW ; x++)
			if(ImageGray[y][x] > GRAY_CNT/2) 
			{
				Boundary[0].x = x;
				Boundary[0].y = y;
				DirX = 1;
				goto Exit;
			}
Exit:

	*pCnt = 0;
	if(DirX == 0 && DirY == 0) return;

	BOOL Loop;
	Loop = TRUE;

	*pCnt = 1;

	Boundary[*pCnt].x = Boundary[*pCnt-1].x + DirX;
	Boundary[*pCnt].y = Boundary[*pCnt-1].y + DirY;

	while(Boundary[*pCnt] != Boundary[0])
	{
		if(ImageGray[Boundary[*pCnt].y][Boundary[*pCnt].x] > GRAY_CNT/2)
		{
			if(Boundary[*pCnt] != Boundary[*pCnt-1])
				(*pCnt)++;

			TempDirX = DirY ? DirY : 0;
			DirY = DirX ? -DirX : 0;
			DirX = TempDirX;

			Boundary[*pCnt].x = Boundary[*pCnt-1].x + DirX;
			Boundary[*pCnt].y = Boundary[*pCnt-1].y + DirY;
		}
		else
		{
			TempDirX = DirY ? -DirY : 0;
			DirY = DirX ? DirX : 0;
			DirX = TempDirX;
				
			Boundary[*pCnt].x = Boundary[*pCnt].x + DirX;
			Boundary[*pCnt].y = Boundary[*pCnt].y + DirY;
		}
	}
}

void Turtle(int **Label, int nW, int nH, int SelLabel, CPoint *Boundary, int *pCnt)
{
	int x, y;
	int DirX, DirY;
	int TempDirX;

	DirX = DirY = 0;
	for(y = 0 ; y < nH ; y++)
		for(x = 0 ; x < nW ; x++)
			if(Label[y][x] == SelLabel) 
			{
				Boundary[0].x = x;
				Boundary[0].y = y;
				DirX = 1;
				goto Exit;
			}
Exit:

	*pCnt = 0;
	if(DirX == 0 && DirY == 0) return;

	BOOL Loop;
	Loop = TRUE;

	*pCnt = 1;

	Boundary[*pCnt].x = Boundary[*pCnt-1].x + DirX;
	Boundary[*pCnt].y = Boundary[*pCnt-1].y + DirY;

	while(Boundary[*pCnt] != Boundary[0])
	{
		if(Label[Boundary[*pCnt].y][Boundary[*pCnt].x] == SelLabel)
		{
			if(Boundary[*pCnt] != Boundary[*pCnt-1])
				(*pCnt)++;

			TempDirX = DirY ? DirY : 0;
			DirY = DirX ? -DirX : 0;
			DirX = TempDirX;

			Boundary[*pCnt].x = Boundary[*pCnt-1].x + DirX;
			Boundary[*pCnt].y = Boundary[*pCnt-1].y + DirY;
		}
		else
		{
			TempDirX = DirY ? -DirY : 0;
			DirY = DirX ? DirX : 0;
			DirX = TempDirX;
				
			Boundary[*pCnt].x = Boundary[*pCnt].x + DirX;
			Boundary[*pCnt].y = Boundary[*pCnt].y + DirY;
		}
	}
}

static double GaussFn(double x, double sigma)
{
	return exp(-1. * x * x /(2. * sigma * sigma));
}

static double MeanGaussFn(double x, double sigma)
{
	double z;
	const double PI = acos(-1.);

	z = (GaussFn(x, sigma) +GaussFn(x+0.5, sigma)+GaussFn(x-0.5, sigma))/3.;
	z /= sqrt(PI*2.*sigma*sigma);	

	return z;
}

void ComputekCosine(CPoint *Boundary, int nCnt, int k, double *kCosine)
{
	int i;
	double abs1, abs2, cosValue;

	const double Pi = acos(-1.);

	for(i = 0 ; i < nCnt ; i++)
	{
		abs1= sqrt((double)(Boundary[(i-k+nCnt)%nCnt].x - Boundary[i].x)*(double)(Boundary[(i-k+nCnt)%nCnt].x - Boundary[i].x) + 
			(double)(Boundary[(i-k+nCnt)%nCnt].y - Boundary[i].y)*(double)(Boundary[(i-k+nCnt)%nCnt].y - Boundary[i].y));
		abs2= sqrt((double)(Boundary[(i+k)%nCnt].x - Boundary[i].x)*(double)(Boundary[(i+k)%nCnt].x - Boundary[i].x) + 
			(double)(Boundary[(i+k)%nCnt].y - Boundary[i].y)*(double)(Boundary[(i+k)%nCnt].y - Boundary[i].y));
		cosValue = ((double)(Boundary[(i-k+nCnt)%nCnt].x - Boundary[i].x)*(double)(Boundary[(i+k)%nCnt].x - Boundary[i].x) + 
			(double)(Boundary[(i-k+nCnt)%nCnt].y - Boundary[i].y)*(double)(Boundary[(i+k)%nCnt].y - Boundary[i].y)) /
			(abs1*abs2);
		
		kCosine[i] = cosValue;
	}

	{
		const int MaxMaskSize = 20;
		double Gauss[MaxMaskSize];
		int MaskSize=MaxMaskSize-1;

		for(i = 0 ; i < MaxMaskSize ; i++)
		{
			Gauss[i] = MeanGaussFn((double)i, 4.0);

			if(Gauss[i] < 0.00005)
			{
				MaskSize = i;
				break;
			}
		}

		double *NewkCosine;

		NewkCosine = new double[nCnt];

		for(i = 0 ; i < nCnt ; i++)
		{
			int k;

			NewkCosine[i] = kCosine[i]*Gauss[0];
			for(k = 1 ; k < MaskSize ; k++)
				NewkCosine[i] += kCosine[(i-k+nCnt)%nCnt]*Gauss[k] + kCosine[(i+k)%nCnt]*Gauss[k];

			if(NewkCosine[i] > 1) NewkCosine[i] = 1;
			else if(NewkCosine[i] < -1) NewkCosine[i] = -1;
		}

		for(i = 0 ; i < nCnt ; i++)
			kCosine[i] = NewkCosine[i];


		delete [] NewkCosine;
	}
}

void ComputekCosine2(CPoint *Boundary, int nCnt, int k, double *kCosine, bool *bConves, int xC, int yC)
{
	int i;
	double abs1, abs2, cosValue;
	double dist1, dist2, dist0;

	const double Pi = acos(-1.);

	for(i = 0 ; i < nCnt ; i++)
	{
		abs1= sqrt((double)(Boundary[(i-k+nCnt*10)%nCnt].x - Boundary[i].x)*(double)(Boundary[(i-k+nCnt*10)%nCnt].x - Boundary[i].x) + 
			(double)(Boundary[(i-k+nCnt*10)%nCnt].y - Boundary[i].y)*(double)(Boundary[(i-k+nCnt*10)%nCnt].y - Boundary[i].y));
		abs2= sqrt((double)(Boundary[(i+k)%nCnt].x - Boundary[i].x)*(double)(Boundary[(i+k)%nCnt].x - Boundary[i].x) + 
			(double)(Boundary[(i+k)%nCnt].y - Boundary[i].y)*(double)(Boundary[(i+k)%nCnt].y - Boundary[i].y));
		cosValue = ((double)(Boundary[(i-k+nCnt*10)%nCnt].x - Boundary[i].x)*(double)(Boundary[(i+k)%nCnt].x - Boundary[i].x) + 
			(double)(Boundary[(i-k+nCnt*10)%nCnt].y - Boundary[i].y)*(double)(Boundary[(i+k)%nCnt].y - Boundary[i].y)) /
			(abs1*abs2);

		dist0 = Dist(xC, yC, Boundary[i].x, Boundary[i].y);
		dist1 = Dist(xC, yC, Boundary[(i-k+nCnt*10)%nCnt].x, Boundary[(i-k+nCnt*10)%nCnt].y);
		dist2 = Dist(xC, yC, Boundary[(i+k)%nCnt].x, Boundary[(i+k)%nCnt].y);

		if(dist0 > (dist1+dist2)/2) bConves[i] = true;
		else bConves[i] = false;
		
		kCosine[i] = cosValue;
	}

	double *NewkCosine;

	NewkCosine = new double[nCnt];

	for(i = 0 ; i < nCnt ; i++)
		NewkCosine[i] = kCosine[(i-1+nCnt)%nCnt]*0.25 + kCosine[i]*0.5 + kCosine[(i+1)%nCnt]*0.25;

	for(i = 0 ; i < nCnt ; i++)
		kCosine[i] = NewkCosine[i];


	delete [] NewkCosine;
}

void GaussianSmooth(CPoint *Boundary, int nCnt, double sigma)
{
	int i;
	CPoint *NewBoundary;
	NewBoundary = new CPoint[nCnt];

	const int MaxMaskSize = 20;
	double Gauss[MaxMaskSize];
	int MaskSize=MaxMaskSize-1;

	for(i = 0 ; i < MaxMaskSize ; i++)
	{
		Gauss[i] = MeanGaussFn((double)i, sigma /*4.0*/);

		if(Gauss[i] < 0.00005)
		{
			MaskSize = i;
			break;
		}
	}

	for(i = 0 ; i < nCnt ; i++)
	{
		int k;
		double SumX, SumY;

		SumX = Boundary[i].x * Gauss[0];
		SumY = Boundary[i].y * Gauss[0];

		for(k = 1 ; k < MaskSize ; k++)
		{
			SumX += Boundary[(i-k+nCnt)%nCnt].x * Gauss[k] + Boundary[(i+k)%nCnt].x * Gauss[k];
			SumY += Boundary[(i-k+nCnt)%nCnt].y * Gauss[k] + Boundary[(i+k)%nCnt].y * Gauss[k];
		}

		NewBoundary[i].x = (int)SumX;
		NewBoundary[i].y = (int)SumY;
	}

	for(i = 0 ; i < nCnt ; i++)
		Boundary[i] = NewBoundary[i];


	delete [] NewBoundary;
}

void GaussianSmooth(double *Data, int nCnt, double sigma)
{
	int i;
	double *NewData;
	NewData = new double[nCnt];

	const int MaxMaskSize = 20;
	double Gauss[MaxMaskSize];
	int MaskSize=MaxMaskSize-1;

	for(i = 0 ; i < MaxMaskSize ; i++)
	{
		Gauss[i] = MeanGaussFn((double)i, sigma /*4.0*/);

		if(Gauss[i] < 0.00005)
		{
			MaskSize = i;
			break;
		}
	}

	for(i = 0 ; i < nCnt ; i++)
	{
		int k;
		double Sum;

		Sum = Data[i] * Gauss[0];

		for(k = 1 ; k < MaskSize ; k++)
			Sum += Data[(i-k+nCnt)%nCnt] * Gauss[k] + Data[(i+k)%nCnt] * Gauss[k];

		NewData[i] = Sum;
	}

	for(i = 0 ; i < nCnt ; i++)
		Data[i] = NewData[i];


	delete [] NewData;
}

void DrawLine(BYTE **ImageGray, int nW, int nH, int x0, int y0, int x1, int y1, BYTE Color)
{
	int nDiffX = abs(x0-x1);
	int nDiffY = abs(y0-y1);

	int x, y;
	int nFrom, nTo;

	if(nDiffY == 0 && nDiffX == 0)
	{
		y = y0;
		x = x0;
		if(!(x < 0 || x >= nW || y < 0 || y >= nH))
			ImageGray[y][x] = Color;
	}
	else if(nDiffX == 0)
	{
		x = x0;
		
		nFrom = (y0 < y1 ? y0 : y1);
		if(nFrom < 0) nFrom = 0;
		nTo = (y0 < y1 ? y1 : y0);
		if(nTo >= nH) nTo = nH-1;

		for(y = nFrom ; y <= nTo ; y++)
		{
			if(x < 0 || x >= nW || y < 0 || y >= nH) continue;
				ImageGray[y][x] = Color;
		}
	}
	else if(nDiffY == 0)
	{
		y = y0;

		nFrom = (x0 < x1 ? x0 : x1);
		if(nFrom < 0) nFrom = 0;
		nTo = (x0 < x1 ? x1 : x0);
		if(nTo >= nW) nTo = nW-1;

		for(x = nFrom ; x <= nTo ; x++)
		{
			if(x < 0 || x >= nW || y < 0 || y >= nH) continue;
				ImageGray[y][x] = Color;
		}
	}
	else if(nDiffY > nDiffX)
	{
		nFrom = (y0 < y1 ? y0 : y1);
		if(nFrom < 0) nFrom = 0;
		nTo = (y0 < y1 ? y1 : y0);
		if(nTo >= nH) nTo = nH-1;

		for(y = nFrom ; y <= nTo ; y++)
		{
			x = (y-y0)*(x0-x1)/(y0-y1) + x0;
			if(x < 0 || x >= nW || y < 0 || y >= nH) continue;
			ImageGray[y][x] = Color;
		}
	}
	else
	{
		nFrom = (x0 < x1 ? x0 : x1);
		if(nFrom < 0) nFrom = 0;
		nTo = (x0 < x1 ? x1 : x0);
		if(nTo >= nW) nTo = nW-1;

		for(x = nFrom ; x <= nTo ; x++)
		{
			y = (x-x0)*(y0-y1)/(x0-x1) + y0;
			if(x < 0 || x >= nW || y < 0 || y >= nH) continue;
			ImageGray[y][x] = Color;
		}
	}
}

void DrawLine1D(BYTE *Image1D, int nW, int nH, int x0, int y0, int x1, int y1, BYTE R, BYTE G, BYTE B)
{
	int nDiffX = abs(x0-x1);
	int nDiffY = abs(y0-y1);

	int x, y;
	int Pos;

	if(nDiffY == 0 && nDiffX == 0)
	{
		y = y0;
		x = x0;
		if(!(x < 0 || x >= nW || y < 0 || y >= nH))
		{	
			Pos = GetBmp24Pos(nW, nH, x, y);
			Image1D[Pos++] = B;
			Image1D[Pos++] = G;
			Image1D[Pos] = R;
		}
	}
	else if(nDiffX == 0)
	{
		x = x0;
		for(y = (y0 < y1 ? y0 : y1) ; y <= (y0 < y1 ? y1 : y0) ; y++)
		{
			if(x < 0 || x >= nW || y < 0 || y >= nH) continue;
			{	
				Pos = GetBmp24Pos(nW, nH, x, y);
				Image1D[Pos++] = B;
				Image1D[Pos++] = G;
				Image1D[Pos] = R;
			}
		}
	}
	else if(nDiffY == 0)
	{
		y = y0;
		for(x = (x0 < x1 ? x0 : x1) ; x <= (x0 < x1 ? x1 : x0) ; x++)
		{
			if(x < 0 || x >= nW || y < 0 || y >= nH) continue;
			{	
				Pos = GetBmp24Pos(nW, nH, x, y);
				Image1D[Pos++] = B;
				Image1D[Pos++] = G;
				Image1D[Pos] = R;
			}
		}
	}
	else if(nDiffY > nDiffX)
	{
		for(y = (y0 < y1 ? y0 : y1) ; y <= (y0 < y1 ? y1 : y0) ; y++)
		{
			x = (y-y0)*(x0-x1)/(y0-y1) + x0;
			if(x < 0 || x >= nW || y < 0 || y >= nH) continue;
			{	
				Pos = GetBmp24Pos(nW, nH, x, y);
				Image1D[Pos++] = B;
				Image1D[Pos++] = G;
				Image1D[Pos] = R;
			}
		}
	}
	else
	{
		for(x = (x0 < x1 ? x0 : x1) ; x <= (x0 < x1 ? x1 : x0) ; x++)
		{
			y = (x-x0)*(y0-y1)/(x0-x1) + y0;
			if(x < 0 || x >= nW || y < 0 || y >= nH) continue;
			{	
				Pos = GetBmp24Pos(nW, nH, x, y);
				Image1D[Pos++] = B;
				Image1D[Pos++] = G;
				Image1D[Pos] = R;
			}
		}
	}
}

void Dot(BYTE **ImageGray, int nW, int nH, int x, int y)
{
	int i, j;

	for(j = -2 ; j <= 2 ; j++)
		for(i = -2 ; i <= 2 ; i++)
		{
			if(x+i >=0 && y+j >= 0 && x+i < nW && y+j < nH)
			{
				ImageGray[y+j][x+i] = GRAY_CNT-1;
			}
		}	
}

void Dot(BYTE **ImageGray, int nW, int nH, int x, int y, BYTE Color)
{
	int i, j;

	for(j = -2 ; j <= 2 ; j++)
		for(i = -2 ; i <= 2 ; i++)
		{
			if(x+i >=0 && y+j >= 0 && x+i < nW && y+j < nH)
			{
				ImageGray[y+j][x+i] = Color;
			}
		}	
}

void DilationBinary(BYTE **ImageGray, BYTE **OutputGray, int nW, int nH, int nShape, int nSize)
{
	//	nShape 
	// 0: 사각, 1: 수평, 2: 수직

	int x, y;
	int dx, dy;
	int xx, yy;
	BOOL bDone;

	if(nSize <= 0) return;

	double nRange = nSize/2.;

	for(y = 0 ; y < nH ; y++)
		for(x = 0 ; x < nW ; x++)
		{
			bDone = false;
			OutputGray[y][x] = 0;
			for(dy = -(int)nRange ; dy < nRange ; dy++)
			{
				if(nShape == 1 && dy != 0) continue;

				for(dx = -(int)nRange ; dx < nRange ; dx++)
				{
					if(nShape == 2 && dx != 0) continue;

					xx = x+dx;
					yy = y+dy;

					if(xx < 0) xx = 0;
					if(xx >= nW) xx = nW-1;
					if(yy < 0) yy = 0;
					if(yy >= nH) yy = nH-1;

					if(ImageGray[yy][xx] > GRAY_CNT/2) 
					{
						OutputGray[y][x] = GRAY_CNT-1;
						bDone = TRUE;
						break;
					}
				}
				if(bDone) break;
			}
		}
}

void DilationSquareBinaryFast(unsigned char **Image2DGray, int nW, int nH, int nSize, unsigned char **Image2DGrayOut)
{
	BYTE **ImageBinary2D;

	ImageBinary2D = cmatrix(nH, nW);

	int x, y, dx, dy;
	int Sum, LeftSum;
	int SideA, SideB;

	for(y = 0 ; y < nH ; y++)
		for(x = 0 ; x < nW ; x++)
			ImageBinary2D[y][x] = Image2DGray[y][x] > 128 ? 1 : 0;

	int *SumStrip = new int[nW];

	SideA = -1*nSize/2;
	SideB = (int)(nSize/2.+0.5) - 1;

	for(x = 0 ; x < nW ; x++)
	{
		SumStrip[x] = 0;

		for(dy = SideA ; dy <= SideB ; dy++)
			SumStrip[x] += ImageBinary2D[(0+dy+nH)%nH][x];
	}

	for(y = 0 ; y < nH ; y++)
		for(x = 0 ; x < nW ; x++)
		{
			if(x == 0)
			{
				Sum = 0;
				for(dx = SideA ; dx <= SideB ; dx++)
					Sum += SumStrip[(x+dx+nW)%nW];
				LeftSum = SumStrip[(x+SideA+nW)%nW];
			}
			else
			{
				Sum -= LeftSum;
				Sum += SumStrip[(x+SideB+nW)%nW];
				LeftSum = SumStrip[(x+SideA+nW)%nW];
			}

			if(x == nW-1)
			{
				for(dx = 0 ; dx < nW ; dx++)
					SumStrip[dx] += -ImageBinary2D[(y+SideA+nH)%nH][dx]+ImageBinary2D[(y+SideB+1+nH)%nH][dx];
			}

			if(Sum > 0)
				Image2DGrayOut[y][x] = 255;
			else
				Image2DGrayOut[y][x] = 0;
		}

	delete [] SumStrip;

	free_cmatrix(ImageBinary2D, nH, nW);
}

void DilationHorizonBinaryFast(unsigned char **Image2DGray, int nW, int nH, int nSize, unsigned char **Image2DGrayOut)
{
	BYTE **ImageBinary2D;

	ImageBinary2D = cmatrix(nH, nW);

	int x, y, dx;
	int Sum, Left;
	int SideA, SideB;

	for(y = 0 ; y < nH ; y++)
		for(x = 0 ; x < nW ; x++)
			ImageBinary2D[y][x] = Image2DGray[y][x] > 128 ? 1 : 0;

	SideA = -1*nSize/2;
	SideB = (int)(nSize/2.+0.5) - 1;

	for(y = 0 ; y < nH ; y++)
		for(x = 0 ; x < nW ; x++)
		{
			if(x == 0)
			{
				Sum = 0;
				for(dx = SideA ; dx <= SideB ; dx++)
					Sum += ImageBinary2D[y][(x+dx+nW)%nW];
				Left = ImageBinary2D[y][(x+SideA+nW)%nW];
			}
			else
			{
				Sum -= Left;
				Sum += ImageBinary2D[y][(x+SideB+nW)%nW];
				Left = ImageBinary2D[y][(x+SideA+nW)%nW];
			}

			
			if(Sum > 0)
				Image2DGrayOut[y][x] = 255;
			else
				Image2DGrayOut[y][x] = 0;
		}

	free_cmatrix(ImageBinary2D, nH, nW);
}

void DilationVertialBinaryFast(unsigned char **Image2DGray, int nW, int nH, int nSize, unsigned char **Image2DGrayOut)
{
	BYTE **ImageBinary2D;

	ImageBinary2D = cmatrix(nH, nW);

	int x, y, dy;
	int Sum, Top;
	int SideA, SideB;

	for(y = 0 ; y < nH ; y++)
		for(x = 0 ; x < nW ; x++)
			ImageBinary2D[y][x] = Image2DGray[y][x] > 128 ? 1 : 0;

	SideA = -1*nSize/2;
	SideB = (int)(nSize/2.+0.5) - 1;

	for(x = 0 ; x < nW ; x++)
		for(y = 0 ; y < nH ; y++)
		{
			if(y == 0)
			{
				Sum = 0;
				for(dy = SideA ; dy <= SideB ; dy++)
					Sum += ImageBinary2D[(y+dy+nH)%nH][x];
				Top = ImageBinary2D[(y+SideA+nH)%nH][x];
			}
			else
			{
				Sum -= Top;
				Sum += ImageBinary2D[(y+SideB+nH)%nH][x];
				Top = ImageBinary2D[(y+SideA+nH)%nH][x];
			}

			
			if(Sum > 0)
				Image2DGrayOut[y][x] = 255;
			else
				Image2DGrayOut[y][x] = 0;
		}

	free_cmatrix(ImageBinary2D, nH, nW);
}

void ErosionBinary(BYTE **ImageGray, BYTE **OutputGray, int nW, int nH, int nShape, int nSize)
{
	//	nShape 
	// 0: 사각, 1: 수평, 2: 수직

	int x, y;
	int dx, dy;
	int xx, yy;
	BOOL bDone;

	if(nSize <= 0) return;

	double nRange = nSize/2.;

	for(y = 0 ; y < nH ; y++)
		for(x = 0 ; x < nW ; x++)
		{
			bDone = false;
			OutputGray[y][x] = GRAY_CNT-1;
			for(dy = -(int)nRange ; dy < nRange ; dy++)
			{
				if(nShape == 1 && dy != 0) continue;

				for(dx = -(int)nRange ; dx < nRange ; dx++)
				{
					if(nShape == 2 && dx != 0) continue;

					xx = x+dx;
					yy = y+dy;

					if(xx < 0) xx = 0;
					if(xx >= nW) xx = nW-1;
					if(yy < 0) yy = 0;
					if(yy >= nH) yy = nH-1;

					if(ImageGray[yy][xx] < GRAY_CNT/2) 
					{
						OutputGray[y][x] = 0;
						bDone = TRUE;
						break;
					}
				}
				if(bDone) break;
			}
		}
}

void ErosionSquareBinaryFast(unsigned char **Image2DGray, int nW, int nH, int nSize, unsigned char **Image2DGrayOut)
{
	BYTE **ImageBinary2D;

	ImageBinary2D = cmatrix(nH, nW);

	int x, y, dx, dy;
	int Sum, LeftSum;
	int SideA, SideB;

	for(y = 0 ; y < nH ; y++)
		for(x = 0 ; x < nW ; x++)
			ImageBinary2D[y][x] = Image2DGray[y][x] > 128 ? 1 : 0;

	int *SumStrip = new int[nW];

	SideA = -1*nSize/2;
	SideB = (int)(nSize/2.+0.5) - 1;

	for(x = 0 ; x < nW ; x++)
	{
		SumStrip[x] = 0;

		for(dy = SideA ; dy <= SideB ; dy++)
			SumStrip[x] += ImageBinary2D[(0+dy+nH)%nH][x];
	}

	for(y = 0 ; y < nH ; y++)
		for(x = 0 ; x < nW ; x++)
		{
			if(x == 0)
			{
				Sum = 0;
				for(dx = SideA ; dx <= SideB ; dx++)
					Sum += SumStrip[(x+dx+nW)%nW];
				LeftSum = SumStrip[(x+SideA+nW)%nW];
			}
			else
			{
				Sum -= LeftSum;
				Sum += SumStrip[(x+SideB+nW)%nW];
				LeftSum = SumStrip[(x+SideA+nW)%nW];
			}

			if(x == nW-1)
			{
				for(dx = 0 ; dx < nW ; dx++)
					SumStrip[dx] += -ImageBinary2D[(y+SideA+nH)%nH][dx]+ImageBinary2D[(y+SideB+1+nH)%nH][dx];
			}

			if(Sum == nSize*nSize)
				Image2DGrayOut[y][x] = 255;
			else
				Image2DGrayOut[y][x] = 0;
		}

	delete [] SumStrip;

	free_cmatrix(ImageBinary2D, nH, nW);
}

void ErosionHorizonBinaryFast(unsigned char **Image2DGray, int nW, int nH, int nSize, unsigned char **Image2DGrayOut)
{
	BYTE **ImageBinary2D;

	ImageBinary2D = cmatrix(nH, nW);

	int x, y, dx;
	int Sum, Left;
	int SideA, SideB;

	for(y = 0 ; y < nH ; y++)
		for(x = 0 ; x < nW ; x++)
			ImageBinary2D[y][x] = Image2DGray[y][x] > 128 ? 1 : 0;

	SideA = -1*nSize/2;
	SideB = (int)(nSize/2.+0.5) - 1;

	for(y = 0 ; y < nH ; y++)
		for(x = 0 ; x < nW ; x++)
		{
			if(x == 0)
			{
				Sum = 0;
				for(dx = SideA ; dx <= SideB ; dx++)
					Sum += ImageBinary2D[y][(x+dx+nW)%nW];
				Left = ImageBinary2D[y][(x+SideA+nW)%nW];
			}
			else
			{
				Sum -= Left;
				Sum += ImageBinary2D[y][(x+SideB+nW)%nW];
				Left = ImageBinary2D[y][(x+SideA+nW)%nW];
			}

			
			if(Sum == nSize)
				Image2DGrayOut[y][x] = 255;
			else
				Image2DGrayOut[y][x] = 0;
		}

	free_cmatrix(ImageBinary2D, nH, nW);
}

void ErosionVertialBinaryFast(unsigned char **Image2DGray, int nW, int nH, int nSize, unsigned char **Image2DGrayOut)
{
	BYTE **ImageBinary2D;

	ImageBinary2D = cmatrix(nH, nW);

	int x, y, dy;
	int Sum, Top;
	int SideA, SideB;

	for(y = 0 ; y < nH ; y++)
		for(x = 0 ; x < nW ; x++)
			ImageBinary2D[y][x] = Image2DGray[y][x] > 128 ? 1 : 0;

	SideA = -1*nSize/2;
	SideB = (int)(nSize/2.+0.5) - 1;

	for(x = 0 ; x < nW ; x++)
		for(y = 0 ; y < nH ; y++)
		{
			if(y == 0)
			{
				Sum = 0;
				for(dy = SideA ; dy <= SideB ; dy++)
					Sum += ImageBinary2D[(y+dy+nH)%nH][x];
				Top = ImageBinary2D[(y+SideA+nH)%nH][x];
			}
			else
			{
				Sum -= Top;
				Sum += ImageBinary2D[(y+SideB+nH)%nH][x];
				Top = ImageBinary2D[(y+SideA+nH)%nH][x];
			}

			
			if(Sum == nSize)
				Image2DGrayOut[y][x] = 255;
			else
				Image2DGrayOut[y][x] = 0;
		}

	free_cmatrix(ImageBinary2D, nH, nW);
}


void DilationGray(BYTE **ImageGray, BYTE **OutputGray, int nW, int nH, int nShape, int nSize)
{
	//	nShape 
	// 0: 사각, 1: 수평, 2: 수직

	int x, y;
	int dx, dy;
	int xx, yy;
	BYTE Max;

	if(nSize <= 0) return;

	double nRange = nSize/2.;

	for(y = 0 ; y < nH ; y++)
		for(x = 0 ; x < nW ; x++)
		{
			Max = 0;
			for(dy = -(int)nRange ; dy < nRange ; dy++)
			{
				if(nShape == 1 && dy != 0) continue;

				for(dx = -(int)nRange ; dx < nRange ; dx++)
				{
					if(nShape == 2 && dx != 0) continue;

					xx = x+dx;
					yy = y+dy;

					if(xx < 0) xx = 0;
					if(xx >= nW) xx = nW-1;
					if(yy < 0) yy = 0;
					if(yy >= nH) yy = nH-1;

					if(ImageGray[yy][xx] > Max) 
						Max = ImageGray[yy][xx];
				}
			}
			OutputGray[y][x] = Max;
		}
}

void ErosionGray(BYTE **ImageGray, BYTE **OutputGray, int nW, int nH, int nShape, int nSize)
{
	//	nShape 
	// 0: 사각, 1: 수평, 2: 수직

	int x, y;
	int dx, dy;
	int xx, yy;
	BYTE Min;

	if(nSize <= 0) return;

	double nRange = nSize/2.;

	for(y = 0 ; y < nH ; y++)
		for(x = 0 ; x < nW ; x++)
		{
			Min = GRAY_CNT-1;
			for(dy = -(int)nRange ; dy < nRange ; dy++)
			{
				if(nShape == 1 && dy != 0) continue;

				for(dx = -(int)nRange ; dx < nRange ; dx++)
				{
					if(nShape == 2 && dx != 0) continue;

					xx = x+dx;
					yy = y+dy;

					if(xx < 0) xx = 0;
					if(xx >= nW) xx = nW-1;
					if(yy < 0) yy = 0;
					if(yy >= nH) yy = nH-1;

					if(ImageGray[yy][xx] < Min)
						Min = ImageGray[yy][xx];
				}
			}
			OutputGray[y][x] = Min;
		}
}

bool InverseMatrix(double  **x, double **Inverse, int nN, double *Determinant)
{
	int i, j, k, l, kk;
	double TempDeterminant, Pivot;
	bool bFinish;

	int nM = nN*2;

	double **TempMatrix;

	const double eps = 1.0e-10;

	TempMatrix = dmatrix(nN, nM);

	for(i = 0 ; i < nN ; i++)
	{
		for(j = 0 ; j < nN ; j++)
			TempMatrix[i][j] = x[i][j];

		for(j = nN ; j < nM ; j++)
			TempMatrix[i][j] = 0.;
	}

	for(i = 0 ; i < nN ; i++) 
		TempMatrix[i][nN+i] = 1.;

	TempDeterminant = 1.;
	for(i = 0 ; i < nN ; i++) 
	{
		if(fabs(TempMatrix[i][i]) < eps) 
		{
			if(i == (nN-1)) 
			{
                *Determinant = 0.;
				free_dmatrix(TempMatrix, nN, nM);
				return false;
			}
            
			bFinish = false;
			for(l = i+1; l < nN ; l++) 
			{
				if(fabs(x[l][i]) >= eps) 
				{
					for(kk = i ; kk < nM ; kk++) 
						TempMatrix[i][kk] += TempMatrix[l][kk];
					
					bFinish = true;
				}
			}
		
			if(!bFinish)
			{
				*Determinant = 0.;
				free_dmatrix(TempMatrix, nN, nM);
				return false;
			}
		}	
		Pivot = TempMatrix[i][i];
		TempDeterminant *= Pivot;
	   
		for(j = i+1 ; j < nM ; j++) 
			TempMatrix[i][j] /= Pivot;

		TempMatrix[i][i] = 1.;
	   
		for (j = 0 ; j < nN ; j++) 
		{
			if (i != j) 
			{
				for(k = i+1 ; k < nM ; k++) 
					TempMatrix[j][k] -= TempMatrix[j][i]*TempMatrix[i][k];
				
				TempMatrix[j][i] = 0.;
			}
		}
	}

	*Determinant = TempDeterminant;

	for(i = 0 ; i < nN ; i++)
	   for(j = 0 ; j < nN ; j++)
	      Inverse[i][j] = TempMatrix[i][j+nN];

	free_dmatrix(TempMatrix, nN, nM);

	return true;
}

void DrawLabelBoundBox(CRect *pBoundBox, int LabelCnt, BYTE **Image2DGray, BYTE Color)
{
	int i;
	int x, y;

	for(i = 0 ; i < LabelCnt ; i++)
	{
		for(y = pBoundBox[i].top ; y <= pBoundBox[i].bottom ; y++)
		{
			Image2DGray[y][pBoundBox[i].left] = Color;
			Image2DGray[y][pBoundBox[i].right] = Color;
//			Image2DGray[y][pBoundBox[i].left+1] = Color;
//			Image2DGray[y][pBoundBox[i].right-1] = Color;
		}
		for(x = pBoundBox[i].left ; x <= pBoundBox[i].right ; x++)
		{
			Image2DGray[pBoundBox[i].top][x] = Color;
			Image2DGray[pBoundBox[i].bottom][x] = Color;
//			Image2DGray[pBoundBox[i].top+1][x] = Color;
//			Image2DGray[pBoundBox[i].bottom-1][x] = Color;
		}
	}
}

void MakeImage2DGrayToImage1D(unsigned char **Image2DGray, int nW, int nH, unsigned char *Image1D)
{
	int x, y, Offset;
	
	for(y = 0 ; y < nH ; y++)
	{
		Offset = (nW*3+3)/4*4 * (nH-y-1);
		for(x = 0 ; x < nW ; x++)
		{
			Image1D[Offset+x*3+2] = Image2DGray[y][x];
			Image1D[Offset+x*3+1] = Image2DGray[y][x];
			Image1D[Offset+x*3] = Image2DGray[y][x];
		}
	}
}

void MakeImage1DToImage2DGray(unsigned char *Image1D, bool bBottomTop, int nW, int nH, unsigned char **Image2DGray, int nDesW, int nDesH)
{
	int x, y, Offset;

	if(nDesW <= 0) nDesW = nW;
	if(nDesH <= 0) nDesH = nH;
	
	if(nDesW == nW && nDesH == nH)
	{
		for(y = 0 ; y < nH ; y++)
		{
			if(bBottomTop)
				Offset = (nW*3+3)/4*4 * (nH-y-1);
			else
				Offset = (nW*3+3)/4*4 * y;
			for(x = 0 ; x < nW ; x++)
			{
				Image2DGray[y][x] = (Image1D[Offset+x*3+2] +
								Image1D[Offset+x*3+1] +
								Image1D[Offset+x*3])/3;
			}
		}
	}
	else
	{		
		for(y = 0 ; y < nDesH ; y++)
		{
			if(bBottomTop)
				Offset = (nW*3+3)/4*4 * (nH-y*nH/nDesH-1);
			else
				Offset = (nW*3+3)/4*4 * y*nH/nDesH;

			for(x = 0 ; x < nDesW ; x++)
			{
				Image2DGray[y][x] = (Image1D[Offset+x*nW/nDesW*3+2] +
								Image1D[Offset+x*nW/nDesW*3+1] +
								Image1D[Offset+x*nW/nDesW*3])/3;
			}
		}
	}
}

void MakeImage1DToImage2DColor(unsigned char *Image1D, bool bBottomTop, int nW, int nH, unsigned char **Image2DRed, unsigned char **Image2DGreen, unsigned char **Image2DBlue, int nDesW, int nDesH)
{
	int x, y, Offset;

	if(nDesW <= 0) nDesW = nW;
	if(nDesH <= 0) nDesH = nH;
	
	if(nDesW == nW && nDesH == nH)
	{
		for(y = 0 ; y < nH ; y++)
		{
			if(bBottomTop)
				Offset = (nW*3+3)/4*4 * (nH-y-1);
			else
				Offset = (nW*3+3)/4*4 * y;

			for(x = 0 ; x < nW ; x++)
			{
				Image2DRed[y][x] = Image1D[Offset+x*3+2];
				Image2DGreen[y][x] = Image1D[Offset+x*3+1];
				Image2DBlue[y][x] = Image1D[Offset+x*3];
			}
		}
	}
	else
	{		
		for(y = 0 ; y < nDesH ; y++)
		{
			if(bBottomTop)
				Offset = (nW*3+3)/4*4 * (nH-y*nH/nDesH-1);
			else
				Offset = (nW*3+3)/4*4 * y*nH/nDesH;

			for(x = 0 ; x < nDesW ; x++)
			{
				Image2DRed[y][x] = Image1D[Offset+x*nW/nDesW*3+2];
				Image2DGreen[y][x] = Image1D[Offset+x*nW/nDesW*3+1];
				Image2DBlue[y][x] = Image1D[Offset+x*nW/nDesW*3];
			}
		}
	}
}

void DrawTextOnImag2DGray(unsigned char **Image2DGray, int nW, int nH, int x, int y, CString Msg, UINT nFormat, unsigned char Color, bool bBig)
{
	unsigned char *imGLD;

	imGLD = new unsigned char[(nW*3+3)/4*4*nH];

	MakeImage2DGrayToImage1D(Image2DGray, nW, nH, imGLD);

	HDC hDC, hCompDC;

	hDC = GetDC(NULL);

	hCompDC = CreateCompatibleDC(hDC);

	HBITMAP hBitmap;

	hBitmap = CreateCompatibleBitmap(hDC, nW, nH);
	
	SelectObject(hCompDC, hBitmap);

	BITMAPINFOHEADER bmiHeader;
	bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmiHeader.biWidth = nW;
	bmiHeader.biHeight = nH;
	bmiHeader.biPlanes = 1;
	bmiHeader.biBitCount = 24;
	bmiHeader.biCompression = BI_RGB;
	bmiHeader.biSizeImage = (nW*3+3)/4*4  * nH;
	bmiHeader.biXPelsPerMeter = 2000;
	bmiHeader.biYPelsPerMeter = 2000;
	bmiHeader.biClrUsed = 0;
	bmiHeader.biClrImportant = 0;

	SetStretchBltMode(hCompDC, HALFTONE);

	StretchDIBits(hCompDC,
		0, 0,
		bmiHeader.biWidth,
		bmiHeader.biHeight,
		0,0,
		bmiHeader.biWidth,
		bmiHeader.biHeight,
		imGLD,
		(LPBITMAPINFO)&bmiHeader,
		DIB_RGB_COLORS,
		SRCCOPY);

	CFont font;
	font.CreatePointFont(150, _T("Arial"), NULL);
	
	if(bBig) SelectObject(hCompDC, font.GetSafeHandle());

	SetTextColor(hCompDC, RGB(Color, Color, Color));
	SetBkMode(hCompDC, TRANSPARENT);

	RECT Rt;
	Rt.left = x;
	Rt.top = y;
	DrawText(hCompDC, Msg, Msg.GetLength(), &Rt, DT_CALCRECT | nFormat);
	DrawText(hCompDC, Msg, Msg.GetLength(), &Rt, nFormat);

	BITMAPINFO bi;

	bi.bmiHeader.biSize = sizeof(bi.bmiHeader);
	bi.bmiHeader.biWidth = nW;
	bi.bmiHeader.biHeight = nH;
	bi.bmiHeader.biPlanes = 1;
	bi.bmiHeader.biBitCount = 24;
	bi.bmiHeader.biCompression = BI_RGB;
	bi.bmiHeader.biSizeImage = (nW*3+3)/4*4  * nH;
	bi.bmiHeader.biClrUsed = 0;
	bi.bmiHeader.biClrImportant = 0;

	GetDIBits(hCompDC, hBitmap, 0, nH, imGLD, &bi,
                   DIB_RGB_COLORS);

	DeleteObject(hBitmap);
	
	ReleaseDC(NULL, hCompDC);
	ReleaseDC(NULL, hDC);
	DeleteDC(hCompDC);

	MakeImage1DToImage2DGray(imGLD, true, nW, nH, Image2DGray);

	delete [] imGLD;
}

void DrawTextOnImaGLD(unsigned char *Image1D, int nW, int nH, int x, int y, CString Msg, UINT nFormat, COLORREF Color, bool bBig)
{

	HDC hDC, hCompDC;

	hDC = GetDC(NULL);

	hCompDC = CreateCompatibleDC(hDC);

	HBITMAP hBitmap;

	hBitmap = CreateCompatibleBitmap(hDC, nW, nH);
	
	SelectObject(hCompDC, hBitmap);

	BITMAPINFOHEADER bmiHeader;
	bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmiHeader.biWidth = nW;
	bmiHeader.biHeight = nH;
	bmiHeader.biPlanes = 1;
	bmiHeader.biBitCount = 24;
	bmiHeader.biCompression = BI_RGB;
	bmiHeader.biSizeImage = (nW*3+3)/4*4  * nH;
	bmiHeader.biXPelsPerMeter = 2000;
	bmiHeader.biYPelsPerMeter = 2000;
	bmiHeader.biClrUsed = 0;
	bmiHeader.biClrImportant = 0;

	SetStretchBltMode(hCompDC, HALFTONE);

	StretchDIBits(hCompDC,
		0, 0,
		bmiHeader.biWidth,
		bmiHeader.biHeight,
		0,0,
		bmiHeader.biWidth,
		bmiHeader.biHeight,
		Image1D,
		(LPBITMAPINFO)&bmiHeader,
		DIB_RGB_COLORS,
		SRCCOPY);

	CFont font;
	font.CreatePointFont(150, _T("Arial"), NULL);
	
	if(bBig) SelectObject(hCompDC, font.GetSafeHandle());

	SetTextColor(hCompDC, Color);
	SetBkMode(hCompDC, TRANSPARENT);

	RECT Rt;
	Rt.left = x;
	Rt.top = y;
	
	DrawText(hCompDC, Msg, Msg.GetLength(), &Rt, DT_CALCRECT | nFormat);
	DrawText(hCompDC, Msg, Msg.GetLength(), &Rt, nFormat);

	BITMAPINFO bi;

	bi.bmiHeader.biSize = sizeof(bi.bmiHeader);
	bi.bmiHeader.biWidth = nW;
	bi.bmiHeader.biHeight = nH;
	bi.bmiHeader.biPlanes = 1;
	bi.bmiHeader.biBitCount = 24;
	bi.bmiHeader.biCompression = BI_RGB;
	bi.bmiHeader.biSizeImage = (nW*3+3)/4*4  * nH;
	bi.bmiHeader.biClrUsed = 0;
	bi.bmiHeader.biClrImportant = 0;

	GetDIBits(hCompDC, hBitmap, 0, nH, Image1D, &bi,
                   DIB_RGB_COLORS);

	DeleteObject(hBitmap);
	
	ReleaseDC(NULL, hCompDC);
	ReleaseDC(NULL, hDC);
	DeleteDC(hCompDC);
}

void DrawCross(BYTE **Image, int nW, int nH, CPoint pt, int Gray)
{
	DrawLine(Image, nW, nH, pt.x-5, pt.y, pt.x+5, pt.y, Gray);
	DrawLine(Image, nW, nH, pt.x-5, pt.y+1, pt.x+5, pt.y+1, Gray);
	DrawLine(Image, nW, nH, pt.x-5, pt.y-1, pt.x+5, pt.y-1, Gray);
	DrawLine(Image, nW, nH, pt.x, pt.y-5, pt.x, pt.y+5, Gray);
	DrawLine(Image, nW, nH, pt.x+1, pt.y-5, pt.x+1, pt.y+5, Gray);
	DrawLine(Image, nW, nH, pt.x-1, pt.y-5, pt.x-1, pt.y+5, Gray);
}

void BigDot(BYTE **ImageGray, int nW, int nH, int x, int y, BYTE Color)
{
	int i, j;

	for(j = -3 ; j <= 3 ; j++)
		for(i = -3 ; i <= 3 ; i++)
		{
			if(x+i >=0 && y+j >= 0 && x+i < nW && y+j < nH)
			{
				ImageGray[y+j][x+i] = Color;
			}
		}	
}

double Dist(int x1, int y1, int x2, int y2)
{
	return sqrt((double)(x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));
}

double FDist(double x1, double y1, double x2, double y2)
{
	return sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));
}

bool SaveImage2DColorJpeg(LPCTSTR FileName, BYTE **ImageRed, BYTE **ImageGreen, BYTE **ImageBlue, 
	int nW, int nH, bool bColor, int nQuality)
{
	int x, y, Pos;
	BYTE *Image1D;
	
	Image1D = new BYTE[GetBmp24Size(nW, nH)];

	for(y = 0 ; y < nH ; y++)
		for(x = 0 ; x < nW ; x++)
		{
			Pos = GetBmp24Pos(nW, nH, x, y);

			Image1D[Pos++] = ImageBlue[y][x];
			Image1D[Pos++] = ImageGreen[y][x];
			Image1D[Pos] = ImageRed[y][x];
		}

	bool bRtn = SaveJpeg(FileName, Image1D, nW, nH, bColor, nQuality);

	delete [] Image1D;

	return bRtn;
}

bool SaveImage2DColorBmp(LPCTSTR FileName, BYTE **ImageRed, BYTE **ImageGreen, BYTE **ImageBlue, 
	int nW, int nH)
{
	int x, y, Pos;
	BYTE *Image1D;
	
	Image1D = new BYTE[GetBmp24Size(nW, nH)];

	for(y = 0 ; y < nH ; y++)
		for(x = 0 ; x < nW ; x++)
		{
			Pos = GetBmp24Pos(nW, nH, x, y);

			Image1D[Pos++] = ImageBlue[y][x];
			Image1D[Pos++] = ImageGreen[y][x];
			Image1D[Pos] = ImageRed[y][x];
		}

	bool bRtn = SaveBmp(FileName, Image1D, nW, nH);

	delete [] Image1D;

	return bRtn;
}

void MedianBinaryFast2DGray(unsigned char **Image2DGray, int nW, int nH, int nSize, unsigned char **Image2DGrayOut)
{
	BYTE **ImageBinary2D;

	ImageBinary2D = cmatrix(nH, nW);

	int x, y, dx, dy;
	int Mean, LeftSum;
	int SideA, SideB;
	double Out;
	bool First = true;

	for(y = 0 ; y < nH ; y++)
		for(x = 0 ; x < nW ; x++)
			ImageBinary2D[y][x] = Image2DGray[y][x] > 128 ? 1 : 0;

	int *SumStrip = new int[nW];

	SideA = -1*nSize/2;
	SideB = (int)(nSize/2.+0.5) - 1;

	for(x = 0 ; x < nW ; x++)
	{
		SumStrip[x] = 0;

		for(dy = SideA ; dy <= SideB ; dy++)
			SumStrip[x] += ImageBinary2D[(0+dy+nH)%nH][x];
	}

	for(y = 0 ; y < nH ; y++)
		for(x = 0 ; x < nW ; x++)
		{
			if(x == 0)
			{
				Mean = 0;
				for(dx = SideA ; dx <= SideB ; dx++)
					Mean += SumStrip[(x+dx+nW)%nW];
				LeftSum = SumStrip[(x+SideA+nW)%nW];
			}
			else
			{
				Mean -= LeftSum;
				Mean += SumStrip[(x+SideB+nW)%nW];
				LeftSum = SumStrip[(x+SideA+nW)%nW];
			}

			if(x == nW-1)
			{
				for(dx = 0 ; dx < nW ; dx++)
					SumStrip[dx] += -ImageBinary2D[(y+SideA+nH)%nH][dx]+ImageBinary2D[(y+SideB+1+nH)%nH][dx];
			}


			Out = Mean/(double)(nSize*nSize);

			if(Out < 0.5)
				Image2DGrayOut[y][x] = 0;
			else
				Image2DGrayOut[y][x] = 255;
		}

	delete [] SumStrip;

	free_cmatrix(ImageBinary2D, nH, nW);
}

void ComputekCosine(CPoint *Boundary, int nCnt, int *LeftK, int *RightK, double *kCosine, double sigma, bool *bConvex, int xC, int yC)
{
	int i;
	double abs1, abs2, cosValue;

	const double Pi = acos(-1.);

	for(i = 0 ; i < nCnt ; i++)
	{
		abs1= sqrt((double)(Boundary[(i-LeftK[i]+nCnt)%nCnt].x - Boundary[i].x)*(double)(Boundary[(i-LeftK[i]+nCnt)%nCnt].x - Boundary[i].x) + 
			(double)(Boundary[(i-LeftK[i]+nCnt)%nCnt].y - Boundary[i].y)*(double)(Boundary[(i-LeftK[i]+nCnt)%nCnt].y - Boundary[i].y));

		abs2= sqrt((double)(Boundary[(i+RightK[i])%nCnt].x - Boundary[i].x)*(double)(Boundary[(i+RightK[i])%nCnt].x - Boundary[i].x) + 
			(double)(Boundary[(i+RightK[i])%nCnt].y - Boundary[i].y)*(double)(Boundary[(i+RightK[i])%nCnt].y - Boundary[i].y));

		cosValue = ((double)(Boundary[(i-LeftK[i]+nCnt)%nCnt].x - Boundary[i].x)*(double)(Boundary[(i+RightK[i])%nCnt].x - Boundary[i].x) + 
			(double)(Boundary[(i-LeftK[i]+nCnt)%nCnt].y - Boundary[i].y)*(double)(Boundary[(i+RightK[i])%nCnt].y - Boundary[i].y)) /
			(abs1*abs2);

		double dist0 = Dist(xC, yC, Boundary[i].x, Boundary[i].y);
		double dist1 = Dist(xC, yC, Boundary[(i-LeftK[i]+nCnt)%nCnt].x, Boundary[(i-LeftK[i]+nCnt)%nCnt].y);
		double dist2 = Dist(xC, yC, Boundary[(i+RightK[i])%nCnt].x, Boundary[(i+RightK[i])%nCnt].y);

		if(dist0 > (dist1+dist2)/2) bConvex[i] = true;
		else bConvex[i] = false;
		
		kCosine[i] = cosValue;

/*		double Average = 0;
		int k;
		int nAverageCnt = 0;


		for(k = MinValue(LeftK[i]/2, RightK[i]/2) ; k <= MinValue(LeftK[i], RightK[i]) ; k++)
		{
			abs1= sqrt((double)(Boundary[(i-k+nCnt)%nCnt].x - Boundary[i].x)*(double)(Boundary[(i-k+nCnt)%nCnt].x - Boundary[i].x) + 
			(double)(Boundary[(i-k+nCnt)%nCnt].y - Boundary[i].y)*(double)(Boundary[(i-k+nCnt)%nCnt].y - Boundary[i].y));
			abs2= sqrt((double)(Boundary[(i+k)%nCnt].x - Boundary[i].x)*(double)(Boundary[(i+k)%nCnt].x - Boundary[i].x) + 
				(double)(Boundary[(i+k)%nCnt].y - Boundary[i].y)*(double)(Boundary[(i+k)%nCnt].y - Boundary[i].y));
			cosValue = ((double)(Boundary[(i-k+nCnt)%nCnt].x - Boundary[i].x)*(double)(Boundary[(i+k)%nCnt].x - Boundary[i].x) + 
				(double)(Boundary[(i-k+nCnt)%nCnt].y - Boundary[i].y)*(double)(Boundary[(i+k)%nCnt].y - Boundary[i].y)) /
				(abs1*abs2);

			Average += cosValue;
			nAverageCnt++;
		}

		kCosine[i] = Average/nAverageCnt;*/
		
	}

	
	{
		const int MaxMaskSize = 20;
		double Gauss[MaxMaskSize];
		int MaskSize=MaxMaskSize-1;

		for(i = 0 ; i < MaxMaskSize ; i++)
		{
			Gauss[i] = MeanGaussFn((double)i, sigma);

			if(Gauss[i] < 0.00005)
			{
				MaskSize = i;
				break;
			}
		}

		double *NewkCosine;

		NewkCosine = new double[nCnt];

		for(i = 0 ; i < nCnt ; i++)
		{
			int k;

			NewkCosine[i] = kCosine[i]*Gauss[0];
			for(k = 1 ; k < MaskSize ; k++)
				NewkCosine[i] += kCosine[(i-k+nCnt)%nCnt]*Gauss[k] + kCosine[(i+k)%nCnt]*Gauss[k];

			if(NewkCosine[i] > 1) NewkCosine[i] = 1;
			else if(NewkCosine[i] < -1) NewkCosine[i] = -1;
		}

		for(i = 0 ; i < nCnt ; i++)
			kCosine[i] = NewkCosine[i];


		delete [] NewkCosine;
	}
}

void DrawArrow(BYTE **ImageGray, int nW, int nH, int x1, int y1, int x2, int y2, BYTE Color)
{
	DrawLine(ImageGray, nW, nH, x1, y1, x2, y2, Color);

	double Theta = atan2((double)(y1-y2), double(x1-x2));

	double Theta1 = Theta + acos(-1.)/6;
	double Theta2 = Theta - acos(-1.)/6;

	int xx0, yy0;

	xx0 = (int)(x2 + 5*cos(Theta));
	yy0 = (int)(y2 + 5*sin(Theta));

	int x, y;
	x = (int)(x2 + 7*cos(Theta1));
	y = (int)(y2 + 7*sin(Theta1));

	DrawLine(ImageGray, nW, nH, x2, y2, x, y, Color);

	x = (int)(x2 + 7*cos(Theta2));
	y = (int)(y2 + 7*sin(Theta2));

	DrawLine(ImageGray, nW, nH, x2, y2, x, y, Color);
}

void DrawBigArrow(BYTE **ImageGray, int nW, int nH, int x1, int y1, int x2, int y2, BYTE Color)
{
	DrawBigLine(ImageGray, nW, nH, x1, y1, x2, y2, Color);
	
	double Theta = atan2((double)(y1-y2), (double)(x1-x2));

	double Theta1 = Theta + acos(-1.)/6;
	double Theta2 = Theta - acos(-1.)/6;

	int xx0, yy0;

	xx0 = (int)(x2 + 5*cos(Theta));
	yy0 = (int)(y2 + 5*sin(Theta));

	int x, y;
	x = (int)(x2 + 7*cos(Theta1));
	y = (int)(y2 + 7*sin(Theta1));

	DrawBigLine(ImageGray, nW, nH, x2, y2, x, y, Color);

	x = (int)(x2 + 7*cos(Theta2));
	y = (int)(y2 + 7*sin(Theta2));

	DrawBigLine(ImageGray, nW, nH, x2, y2, x, y, Color);
}

void DrawBigLine(BYTE **ImageGray, int nW, int nH, int x0, int y0, int x1, int y1, BYTE Color)
{
	int dx, dy;

	for(dy = -1 ; dy <= 1; dy++)
		for(dx = -1 ; dx <= 1 ; dx++)
			DrawLine(ImageGray, nW, nH, x0+dx, y0+dy, x1+dx, y1+dy, Color);
}

void SetLabelBoundBox(int **Label, int nW, int nH, CRect *pBoundBox, int LabelCnt)
{
	int i;
	int x, y;

	for(i = 0 ; i < LabelCnt ; i++)
		pBoundBox[i] = CRect(nW, nH, 0, 0);

	for(y = 0 ; y < nH ; y++)
		for(x = 0 ; x < nW ; x++)
		{
			if(Label[y][x] < 0) continue;
			if(pBoundBox[Label[y][x]].left > x) pBoundBox[Label[y][x]].left = x;
			if(pBoundBox[Label[y][x]].top > y) pBoundBox[Label[y][x]].top = y;
			if(pBoundBox[Label[y][x]].right < x) pBoundBox[Label[y][x]].right = x;
			if(pBoundBox[Label[y][x]].bottom < y) pBoundBox[Label[y][x]].bottom = y;
		}
}

double ComputeSD(int *data, int cnt)
{
	int i;
	double SD;
	double Mean;

	Mean = 0.;
	for(i = 0 ; i < cnt ; i++)
		Mean += (double)data[i];

	Mean /= (double)cnt;

	SD = 0.;
	for(i = 0 ; i < cnt ; i++)
		SD += ((double)data[i] - Mean)*((double)data[i] - Mean);

	SD /= (double)cnt;

	return(sqrt(SD));
} 

void DrawRect(CRect rt, BYTE **Image2DGray, int nW, int nH, BYTE Color)
{
	int x, y;

	for(x = rt.left ; x <= rt.right ; x++)
	{
		y = rt.top;
		if(!(x < 0 || x >= nW || y < 0 || y >= nH))
			Image2DGray[y][x] = Color;

		y = rt.bottom;
		if(!(x < 0 || x >= nW || y < 0 || y >= nH))
			Image2DGray[y][x] = Color;
	}

	for(y = rt.top ; y <= rt.bottom ; y++)
	{
		x = rt.left;
		if(!(x < 0 || x >= nW || y < 0 || y >= nH))
			Image2DGray[y][x] = Color;

		x = rt.right;
		if(!(x < 0 || x >= nW || y < 0 || y >= nH))
			Image2DGray[y][x] = Color;
	}
}

void DrawRect1D(CRect rt, BYTE *Image1D, int nW, int nH, BYTE R, BYTE G, BYTE B)
{
	int x, y;
	int Pos;

	for(x = rt.left ; x <= rt.right ; x++)
	{
		y = rt.top;
		if(!(x < 0 || x >= nW || y < 0 || y >= nH))
		{
			Pos = GetBmp24Pos(nW, nH, x, y);
			Image1D[Pos++] = B;
			Image1D[Pos++] = G;
			Image1D[Pos] = R;
		}

		y = rt.bottom;
		if(!(x < 0 || x >= nW || y < 0 || y >= nH))
		{
			Pos = GetBmp24Pos(nW, nH, x, y);
			Image1D[Pos++] = B;
			Image1D[Pos++] = G;
			Image1D[Pos] = R;
		}
	}

	for(y = rt.top ; y <= rt.bottom ; y++)
	{
		x = rt.left;
		if(!(x < 0 || x >= nW || y < 0 || y >= nH))
		{
			Pos = GetBmp24Pos(nW, nH, x, y);
			Image1D[Pos++] = B;
			Image1D[Pos++] = G;
			Image1D[Pos] = R;
		}

		x = rt.right;
		if(!(x < 0 || x >= nW || y < 0 || y >= nH))
		{
			Pos = GetBmp24Pos(nW, nH, x, y);
			Image1D[Pos++] = B;
			Image1D[Pos++] = G;
			Image1D[Pos] = R;
		}
	}
}

void DrawBigRect(CRect rt, BYTE **Image2DGray, int nW, int nH, BYTE Color)
{
	int x, y;

	for(x = rt.left ; x <= rt.right ; x++)
	{
		for(y = rt.top-1 ; y <= rt.top+1 ; y++)
			if(!(x < 0 || x >= nW || y < 0 || y >= nH))
				Image2DGray[y][x] = Color;

		for(y = rt.bottom-1 ; y <= rt.bottom+1 ; y++)
			if(!(x < 0 || x >= nW || y < 0 || y >= nH))
				Image2DGray[y][x] = Color;
	}

	for(y = rt.top ; y <= rt.bottom ; y++)
	{
		for(x = rt.left-1 ; x <= rt.left+1 ; x++)
			if(!(x < 0 || x >= nW || y < 0 || y >= nH))
				Image2DGray[y][x] = Color;

		for(x = rt.right-1 ; x <= rt.right+1 ; x++)
			if(!(x < 0 || x >= nW || y < 0 || y >= nH))
				Image2DGray[y][x] = Color;
	}
}

void KMeanBinary(BYTE **Image, BYTE ***OutputGray, int nW, int nH, int nK) 
{
	int *Center;
	int *NewCenter;
	int *ClsCnt;
	int i, k;
	int mindist;
	int dist;
	int His[256] = {0}, Cls[256];
	BOOL End;
	int *Thre;
	int Iteration;
	int x, y;
	
	if(nK < 2) return;

	for(y = 0 ; y < nH; y++)
		for(x = 0 ; x < nW ; x++)
			His[Image[y][x]]++;

	Center = new int[nK];
	NewCenter = new int[nK];
	ClsCnt = new int[nK];
	Thre = new int[nK-1];

	Center[0] = 0; while(His[Center[0]++] <= 0); // Histogram이 존재하는 가장 작은 graylevel + 1
	Center[nK-1] = 255; while(His[Center[nK-1]--] <= 0); // Histogram이 존재하는 가장 큰 graylevel - 1

	for(i = 1 ; i < nK-1 ; i++)
		Center[i] = Center[0] + (Center[nK-1]-Center[0])/(nK-1)*i;

	End = FALSE;
	Iteration = 0;
	while(!End)
	{
		Iteration++;
		for(i = 0 ; i < nK ; i++)
			ClsCnt[i] = 0;

		for(k = 0 ; k < 256 ; k++)
		{
			if(His[k] > 0)
			{
				mindist = 256;
				for(i = 0 ; i < nK ; i++)
				{
					dist = abs(Center[i] - k);

					if(dist < mindist)
					{
						mindist = dist;
						Cls[k] = i;
					}
				}
				
				ClsCnt[Cls[k]] += His[k];
			}
		}

		for(i = 0 ; i < nK ; i++)
			if(ClsCnt[i] != 0)
				NewCenter[i] = 0;
			else 
				NewCenter[i] = Center[i];

		for(k = 0 ; k < 256 ; k++)
		{
			if(His[k] > 0) NewCenter[Cls[k]] += k*His[k];
		}

		for(i = 0 ; i < nK ; i++)
			if(ClsCnt[i] != 0)
				NewCenter[i] /= ClsCnt[i];

		End = 1;
		for(i = 0 ; i < nK ; i++)
		{
			if(NewCenter[i] == Center[i]) End *= 1;
			else End *= 0;

			Center[i] = NewCenter[i];
		}

	}

	for(i = 0 ; i < nK-1 ; i++)
	{
		Thre[i] = (Center[i] + Center[i+1])/2;
	}

	for(y = 0 ; y < nH ; y++)
		for(x = 0 ; x < nW ; x++)
		{
			for(i = 0 ; i < nK ; i++)
			{
				if(i == 0) {
					if(Image[y][x] < Thre[i])
						OutputGray[i][y][x] = 255;
					else
						OutputGray[i][y][x] = 0;
				}
				else if(i == nK-1) {
					if(Thre[i-1] <= Image[y][x])
						OutputGray[i][y][x] = 255;
					else
						OutputGray[i][y][x] = 0;
				}
				else if(Thre[i-1] <= Image[y][x] && Image[y][x] < Thre[i])
				{
					OutputGray[i][y][x] = 255;
				}
				else
				{
					OutputGray[i][y][x] = 0;
				}
			}
		}

	delete [] Center;
	delete [] NewCenter;
	delete [] ClsCnt;
	delete [] Thre;
}

void Fill(BYTE **Image, int nW, int nH)
{
	int x, y;
	int **Label;
	BYTE **Temp;

	Label = imatrix(nH, nW);
	Temp = cmatrix(nH, nW);

	for(y = 0 ; y < nH ; y++)
		for(x = 0 ; x < nW ; x++)
			Temp[y][x] = ~Image[y][x];	
	
	int Cnt = Labeling(Temp, Label, nW, nH, 0);

	for(y = 1; y < nH-1 ; y++)
		for(x = 1 ; x < nW-1 ; x++)
			if(Label[y][x] == 0) Image[y][x] = 0;
			else Image[y][x] = 255;


	free_imatrix(Label, nH, nW);
	free_cmatrix(Temp, nH, nW);
}

void RemoveSmallRegion(BYTE **Image, int nW, int nH, int nConnNumThre)
{
	int x, y;
	int **Label;

	Label = imatrix(nH, nW);
	
	
	int Cnt = Labeling(Image, Label, nW, nH, nConnNumThre);

	for(y = 0; y < nH ; y++)
		for(x = 0 ; x < nW ; x++)
			if(Label[y][x] >= 0) Image[y][x] = 255;
			else Image[y][x] = 0;


	free_imatrix(Label, nH, nW);
}


///// http://people.cs.uchicago.edu/~pff/dt/
#define INF 1E20

template <class T>
inline T square(const T &x) { return x*x; };

/* dt of 1d function using squared distance */
double *DistanceTransform(double *f, int n) {
  double *d = new double[n];
  int *v = new int[n];
  double *z = new double[n+1];
  int k = 0, q;

  v[0] = 0;
  z[0] = -INF;
  z[1] = +INF;
  for (q = 1; q <= n-1; q++) {
    double s  = ((f[q]+square(q))-(f[v[k]]+square(v[k])))/(2*q-2*v[k]);
    while (s <= z[k]) {
      k--;
      s  = ((f[q]+square(q))-(f[v[k]]+square(v[k])))/(2*q-2*v[k]);
    }
    k++;
    v[k] = q;
    z[k] = s;
    z[k+1] = +INF;
  }

  k = 0;
  for (q = 0; q <= n-1; q++) {
    while (z[k+1] < q)
      k++;
    d[q] = square(q-v[k]) + f[v[k]];
  }

  delete [] v;
  delete [] z;

  return d;
}

/* dt of 2d function using squared distance */
void DistanceTransform(double **Output, int nW, int nH)
{
	double *f = new double [nW > nH ? nW : nH];
	int x, y;

	for(x = 0 ; x < nW ; x++)
	{
		for(y = 0 ; y < nH ; y++)
			f[y] = Output[y][x];

		double *d = DistanceTransform(f, nH);

		for(y = 0 ; y < nH ; y++)
			Output[y][x] = d[y];

		delete [] d;
	}

	for(y = 0 ; y < nH ; y++)
	{
		for(x = 0 ; x < nW ; x++)
			f[x] = Output[y][x];

		double *d = DistanceTransform(f, nW);

		for(x = 0 ; x < nW ; x++)
			Output[y][x] = d[x];

		delete [] d;
	}

	delete [] f; 
}


/* dt of binary image using squared distance */
//static image<float> *dt(image<uchar> *im, uchar on = 1) 
void DistanceTransform(BYTE **Image, double **Output, int nW, int nH)
{
	int x, y;

	for(y = 0 ; y < nH ; y++)
		for(x = 0 ; x < nW ; x++)
			if(Image[y][x] > 128)
				Output[y][x] = 0;
			else
				Output[y][x] = INF;

	DistanceTransform(Output, nW, nH);
}

double VectorAbs(double x, double y)
{
	return sqrt(x*x + y*y);
}

void CirclePoints(BYTE **ImageGray, int nW, int nH, int cx, int cy, int x, int y, BYTE Color)
{    
    if (x == 0) {
		if(cy+y >= 0 && cy+y < nH && cx >= 0 && cx < nW)
			ImageGray[cy+y][cx]= Color;

		if(cy-y >= 0 && cy-y < nH && cx >= 0 && cx < nW)
			ImageGray[cy-y][cx]= Color;
		if(cy >= 0 && cy < nH && cx+y >= 0 && cx+y < nW)
			ImageGray[cy][cx+y]= Color;
		if(cy >= 0 && cy < nH && cx-y >= 0 && cx-y < nW)
			ImageGray[cy][cx-y]= Color;
    } else if (x == y) {
		if(cy+y >= 0 && cy+y < nH && cx+x >= 0 && cx+x < nW)
			ImageGray[cy+y][cx+x]= Color;

		if(cy+y >= 0 && cy+y < nH && cx-x >= 0 && cx-x < nW)
			ImageGray[cy+y][cx-x]= Color;
		if(cy-y >= 0 && cy-y < nH && cx+x >= 0 && cx+x < nW)
			ImageGray[cy-y][cx+x]= Color;
		if(cy-y >= 0 && cy-y < nH && cx-x >= 0 && cx-x < nW)
			ImageGray[cy-y][cx-x]= Color;
    } else if (x < y) {
		if(cy+y >= 0 && cy+y < nH && cx+x >= 0 && cx+x < nW)
			ImageGray[cy+y][cx+x]= Color;

		if(cy+y >= 0 && cy+y < nH && cx-x >= 0 && cx-x < nW)
			ImageGray[cy+y][cx-x]= Color;
		if(cy-y >= 0 && cy-y < nH && cx+x >= 0 && cx+x < nW)
			ImageGray[cy-y][cx+x]= Color;
		if(cy-y >= 0 && cy-y < nH && cx-x >= 0 && cx-x < nW)
			ImageGray[cy-y][cx-x]= Color;
		if(cy+x >= 0 && cy+x < nH && cx+y >= 0 && cx+y < nW)
			ImageGray[cy+x][cx+y]= Color;
		if(cy+x >= 0 && cy+x < nH && cx-y >= 0 && cx-y < nW)
			ImageGray[cy+x][cx-y]= Color;
		if(cy-x >= 0 && cy-x < nH && cx+y >= 0 && cx+y < nW)
			ImageGray[cy-x][cx+y]= Color;
		if(cy-x >= 0 && cy-x < nH && cx-y >= 0 && cx-y < nW)
			ImageGray[cy-x][cx-y]= Color;
    }
}

void DrawCircle(BYTE **ImageGray, int nW, int nH, int x1, int y1, int radius, BYTE Color)
{
	int x = 0;
    int y = radius;
    int p = (5 - radius*4)/4;
	
    CirclePoints(ImageGray, nW, nH, x1, y1, x, y, Color);

    while (x < y) {
        x++;
        if (p < 0) {
            p += 2*x+1;
        } else {
            y--;
            p += 2*(x-y)+1;
        }
        CirclePoints(ImageGray, nW, nH, x1, y1, x, y, Color);
    }
}

void DrawCircle2(BYTE **ImageGray, int nW, int nH, int x1, int y1, int radius, BYTE Color)
{
	int x = 0;
    int y = radius;
    int p = (5 - radius*4)/4;
	
    CirclePoints(ImageGray, nW, nH, x1, y1, x, y, Color);

    while (x < y) {
		x+=2;
        if (p < 0) {
            p += 2*x+1;
        } else {
			y-=2;
            p += 2*(x-y)+1;
        }
        CirclePoints(ImageGray, nW, nH, x1, y1, x, y, Color);
    }
}

void DrawFillCircle(BYTE **ImageGray, int nW, int nH, int x1, int y1, int radius, BYTE Color)
{
	int x, y;

	for(y = y1-radius ; y <= y1+radius ; y++)
		for(x = x1-radius ; x <= x1+radius ; x++)
		{
			if(Dist(x, y, x1, y1) <= radius)
			{
				if(x >= 0 && x < nW && y >= 0 && y < nH)
					ImageGray[y][x] = Color;
			}
		}
}

void FastMedianFilter(BYTE **ImageGray, BYTE **OutputGray, int nW, int nH, int nOneSideSize)
{
	// nOneSideSize = 1 => WinSize = 3
	// nOneSideSize = 2 -> WinSize = 5;

	int median, delta;
	int prev, next;
	int His[256] = {0};
	int direction = 1;
	int x, y;
	int x1, x2, y1, y2;
	int xx, yy;
	int i, sum;
	int inValue, outValue;

	for(y = 0 ; y < nH ; y++)
		for(x = 0 ; x < nW ; x++)
			OutputGray[y][x] = 0;

	int middle=((nOneSideSize*2+1)*(nOneSideSize*2+1)+1)/2;

	for(y = 0 ; y < nOneSideSize*2+1 ; y++)
		for(x = 0 ; x < nOneSideSize*2+1 ; x++)
			His[ImageGray[y][x]]++;

	sum = 0;
	for(i=0 ; i < 256 ; i++)
	{
		sum += His[i];
		if(sum >= middle)
			break;
	}
	median = i;
	delta = sum - His[i];

	OutputGray[nOneSideSize][nOneSideSize] = median;

	x = nOneSideSize+1;
	for(y = nOneSideSize ; y < nH-nOneSideSize ; y++)
	{
		y1 = y - nOneSideSize;
		y2 = y + nOneSideSize;

		for(; x >= nOneSideSize && x < nW-nOneSideSize ; x += direction)
		{
			prev = x-direction*(nOneSideSize+1);
			next = x+direction*nOneSideSize;

			for(yy = y1 ; yy <= y2 ; yy++)
			{

				outValue = ImageGray[yy][prev];
				inValue = ImageGray[yy][next];

				if(outValue == inValue)
					continue;
				His[outValue]--;
				if(outValue < median)
					delta--;
				His[inValue]++;
				if(inValue < median)
					delta++;
			}

			if(delta >= middle)
			{
				while(delta >= middle)
				{
					if(His[--median] > 0)
						delta -= His[median];
				}
			}
			else
			{
				while(delta + His[median] < middle)
				{
					if(His[median] > 0)
						delta += His[median];
					median++;
				}
			}

			OutputGray[y][x] = median;
		} 

		if(y < nH-nOneSideSize-1)
		{
			x -= direction;
			direction *= -1;

			prev = y1;
			next = y2+1;


			x1 = x - nOneSideSize;
			x2 = x + nOneSideSize;

			for(xx = x1; xx <= x2; xx++)
			{
				outValue = ImageGray[prev][xx];
				inValue = ImageGray[next][xx];

				if(outValue == inValue)
					continue;
				His[outValue]--;
				if(outValue < median)
					delta--;
				His[inValue]++;
				if(inValue < median)
					delta++;
			}

			if(delta >= middle)
			{
				while(delta >= middle)
				{
					if(His[--median] > 0)
						delta -= His[median];
				}
			}
			else
			{
				while(delta + His[median] < middle)
				{
					if(His[median] > 0)
						delta += His[median];
					median++;
				}
			}

			OutputGray[y+1][x] = median;
			x += direction;
		}
	}
}

void GetAffineParameters(CPoint D1, CPoint D2, CPoint D3, CPoint V1, CPoint V2, CPoint V3,
	double *pA1, double *pA2, double *pA0, double *pB1, double *pB2, double *pB0)
{
	double **T = dmatrix(3, 3);
	double **IT = dmatrix(3, 3);
	double **V = dmatrix(3, 3);

	T[0][0] = D1.x;
	T[0][1] = D1.y;
	T[0][2] = 1;

	T[1][0] = D2.x;
	T[1][1] = D2.y;
	T[1][2] = 1;

	T[2][0] = D3.x;
	T[2][1] = D3.y;
	T[2][2] = 1;

	V[0][0] = V1.x;  // visual
	V[0][1] = V1.y;
	V[0][2] = 1;

	V[1][0] = V2.x;
	V[1][1] = V2.y;
	V[1][2] = 1;

	V[2][0] = V3.x;
	V[2][1] = V3.y;
	V[2][2] = 1;

	double det;
	InverseMatrix(T, IT, 3, &det);


	*pA1 = IT[0][0] * V[0][0] + IT[0][1] * V[1][0] + IT[0][2] * V[2][0];
	*pA2 = IT[1][0] * V[0][0] + IT[1][1] * V[1][0] + IT[1][2] * V[2][0];
	*pA0 = IT[2][0] * V[0][0] + IT[2][1] * V[1][0] + IT[2][2] * V[2][0];

	*pB1 = IT[0][0] * V[0][1] + IT[0][1] * V[1][1] + IT[0][2] * V[2][1];
	*pB2 = IT[1][0] * V[0][1] + IT[1][1] * V[1][1] + IT[1][2] * V[2][1];
	*pB0 = IT[2][0] * V[0][1] + IT[2][1] * V[1][1] + IT[2][2] * V[2][1];


	free_dmatrix(T, 3, 3);
	free_dmatrix(IT, 3, 3);
	free_dmatrix(V, 3, 3);
}

void Affine(unsigned char **Image2DGray, int nW, int nH, unsigned char **Image2DGrayOut,
	double a1, double a2, double a0, double b1, double b2, double b0)
{
	int x, y;
	int xx, yy;

	for(y = 0 ; y < nH ; y++)
		for(x = 0 ; x < nW ; x++)
		{
			xx = (int)(a0 + x*a1 + y*a2 + 0.5);
			yy = (int)(b0 + x*b1 + y*b2 + 0.5);

			if(xx >= 0 && xx < nW && yy >= 0 && yy < nH)
				Image2DGrayOut[y][x] = Image2DGray[yy][xx];
			else
				Image2DGrayOut[y][x] = 0;

		}
}

void Bilinear(unsigned char **Image2DGray, int nW, int nH, unsigned char **Image2DGrayOut,
	double u0, double v0, double u1, double v1, double u2, double v2, double u3, double v3) 
{
	double u01, u23;
	double v03, v12;

	double doubleU, doubleV;

	int x, y;
	int mapX, mapY;
	double xRate, yRate;

	for(y = 0 ; y < nH ; y++)
	{
		yRate = (double)y/nH;
		for(x = 0 ; x < nW ; x++)
		{
			xRate = (double)x/nW;

			u01 = u0 + (u1 - u0) * xRate;
			u23 = u3 + (u2 - u3) * xRate;
			doubleU = u01 + (u23 - u01) * yRate;

			v03 = v0 + (v3 -v0) * yRate;
			v12 = v1 + (v2 -v1) * yRate;
			doubleV = v03 + (v12 - v03) * xRate;

			mapX = (int)(doubleU + 0.5);
			mapY = (int)(doubleV + 0.5);
			
			if(mapX >= 0 && mapX < nW && mapY >= 0 && mapY < nH)
				Image2DGrayOut[y][x] = Image2DGray[mapY][mapX];
			else
				Image2DGrayOut[y][x] = 0;
		}
	}
	////
}

void Mean2DGrayFast(unsigned char **Image2DGray, int nW, int nH, int nSize, unsigned char **Image2DGrayOut)
{
	int x, y, dx, dy;
	int Mean, LeftSum;
	int SideA, SideB;
	bool First = true;

	int *SumStrip = new int[nW];

	SideA = -1*nSize/2;
	SideB = (int)(nSize/2.+0.5) - 1;

	for(x = 0 ; x < nW ; x++)
	{
		SumStrip[x] = 0;

		for(dy = SideA ; dy <= SideB ; dy++)
			SumStrip[x] += Image2DGray[(0+dy+nH)%nH][x];
	}

	for(y = 0 ; y < nH ; y++)
		for(x = 0 ; x < nW ; x++)
		{
			if(x == 0)
			{
				Mean = 0;
				for(dx = SideA ; dx <= SideB ; dx++)
					Mean += SumStrip[(x+dx+nW)%nW];
				LeftSum = SumStrip[(x+SideA+nW)%nW];
			}
			else
			{
				Mean -= LeftSum;
				Mean += SumStrip[(x+SideB+nW)%nW];
				LeftSum = SumStrip[(x+SideA+nW)%nW];
			}

			if(x == nW-1)
			{
				for(dx = 0 ; dx < nW ; dx++)
					SumStrip[dx] += -Image2DGray[(y+SideA+nH)%nH][dx]+Image2DGray[(y+SideB+1+nH)%nH][dx];
			}

			Image2DGrayOut[y][x] = Mean/(nSize*nSize);
		}

	delete [] SumStrip;
}

void DcNotch2DGrayFast(BYTE **Image2DGray, BYTE **Image2DGrayOut, int nW, int nH, int nSize)
{
	int x, y, dx, dy;
	int Mean, Out, LeftSum;
	int SideA, SideB;
	bool First = true;
	BYTE *RowImage, *RowOut;
	int xx, yy1, yy2;

	int *SumStrip = new int[nW];

	SideA = -1*nSize/2;
	SideB = (int)(nSize/2.+0.5) - 1;

	for(x = 0 ; x < nW ; x++)
	{
		SumStrip[x] = 0;

		for(dy = SideA ; dy <= SideB ; dy++)
		{
			if(dy >= 0 && dy < nH)
				SumStrip[x] += Image2DGray[dy][x];
		}
	}

	for(y = 0 ; y < nH ; y++)
	{
		RowOut = Image2DGrayOut[y];
		RowImage = Image2DGray[y];
		for(x = 0 ; x < nW ; x++)
		{
			if(x == 0)
			{
				Mean = 0;
				for(dx = SideA ; dx <= SideB ; dx++)
				{
					xx = x+dx;
					if(xx >= 0 && xx < nW)
						Mean += SumStrip[xx];
				}
				xx = x+SideA;
				if(xx >= 0 && xx < nW)
					LeftSum = SumStrip[xx];
				else 
					LeftSum = 0;
			}
			else
			{
				Mean -= LeftSum;
				xx = x+SideB;
				if(xx >= 0 && xx < nW)
					Mean += SumStrip[xx];

				xx = x+SideA;
				if(xx >= 0 && xx < nW)
					LeftSum = SumStrip[xx];
				else
					LeftSum = 0;
			}

			if(x == nW-1)
			{
				yy1 = y+SideA;
				yy2 = y+SideB+1;
				for(dx = 0 ; dx < nW ; dx++)
				{
					if(yy1 >= 0 && yy1 < nH)
						SumStrip[dx] -= Image2DGray[yy1][dx];

					if(yy2 >= 0 && yy2 < nH)
						SumStrip[dx] += Image2DGray[yy2][dx];
				}
			}


			Out = RowImage[x] - Mean/(nSize*nSize) + 128;

			if(Out < 0)
				RowOut[x] = 0;
			else if(Out > 255)
				RowOut[x] = 255;
			else
				RowOut[x] = Out;
		}
	}

	delete [] SumStrip;
}

CPoint BMA(BYTE **CurrImageGray, BYTE **ReferImageGray, int left, int top, 
		 int nW, int nH, int nSearchLeft, int nSearchTop, int nSearchRight, int nSearchBottom, 
		 int BlockWidth, int BlockHeight)
{
	if(top < BlockHeight+nSearchBottom ||  top >= nH - BlockHeight-nSearchTop || 
		left < BlockWidth+nSearchRight || left >= nW - BlockWidth-nSearchLeft)
	{
		return 0;
	}

	int x, y, dx, dy;
	CPoint MinDx = CPoint(0, 0);
	double Diff, MinDiff = 255;
	int xx0, yy0, xx1, yy1;

	for(dy = -nSearchTop ; dy <= nSearchBottom ; dy++)
	{
		for(dx = -nSearchLeft ; dx <= nSearchRight ; dx++)
		{
			Diff = 0.;

			for(y = 0 ; y <= BlockHeight ; y++)
				for(x = 0 ; x <= BlockWidth ; x++)
				{
					yy0 = top+y;
					xx0 = left+x;

					xx1 = left+x+dx;
					yy1 = top+y+dy;

					Diff += abs(CurrImageGray[yy0][xx0] - ReferImageGray[yy1][xx1]);
				}

			Diff /= BlockWidth*BlockHeight;

			if(Diff < MinDiff)
			{
				MinDiff = Diff;
				MinDx = CPoint(dx, dy);
			}
		}
	}

	return MinDx;
}

void InversePerspectiveTable(int W, int H, int LW, int LH,
	POINT p0, POINT p1, POINT p2, POINT p3, POINT **Table)
{
	double delx1, dely1, delx2, dely2, delx3, dely3;
	double a11, a12, a13;
	double a21, a22, a23;
	double a31, a32, a33;

	int u, v;
	double floatu, floatv;
	int i, j;
	double x, y;
//	unsigned long sourcebase;
//	double dA, dB, dC, dD, dE, dF, dG, dH, dI;

	delx1 = p1.x - p2.x;
	dely1 = p1.y - p2.y;
	delx2 = p3.x - p2.x;
	dely2 = p3.y - p2.y;
	delx3 = p0.x - p1.x + p2.x - p3.x;
	dely3 = p0.y - p1.y + p2.y - p3.y;

	if(delx3 == 0.0 && dely3 == 0.0)
	{
		a11 = p1.x - p0.x;
		a21 = p2.x - p1.x;
		a31 = p0.x;
		a12 = p1.y - p0.y;
		a22 = p2.y - p1.y;
		a32 = p0.y;
		a13 = 0.0;
		a23 = 0.0;
		a33 = 1.0;
	}
	else
	{
		a13 = (delx3*dely2-delx2*dely3)/(delx1*dely2-dely1*delx2);
		a23 = (delx1*dely3-dely1*delx3)/(delx1*dely2-dely1*delx2);
		a11 = p1.x - p0.x + a13*p1.x;
		a21 = p3.x - p0.x + a23*p3.x;
		a31 = p0.x;
		a12 = p1.y - p0.y + a13*p1.y;
		a22 = p3.y - p0.y + a23*p3.y;
		a32 = p0.y;
		a33 = 1.0;
	}

//	dA = a22 * a33 - a32 * a23;
//	dB = a31 * a23 - a21 * a33;
//	dC = a21 * a32 - a31 * a22;
//	dD = a32 * a13 - a12 * a33;
//	dE = a11 * a33 - a31 * a13;
//	dF = a31 * a12 - a11 * a32;
//	dG = a12 * a23 - a22 * a13;
//	dH = a21 * a13 - a11 * a23;
//	dI = a11 * a22 - a21 * a12;

	for(i = 0 ; i < LH ; i++)
	{
		y = (double)i/(double)LH;
		for(j = 0 ; j < LW ; j++)
		{
			x = (double)j/(double)LW;
			floatu = ((a11*x + a21*y + a31)/(a13*x + a23*y + a33));// * (double)W;
			floatv = ((a12*x + a22*y + a32)/(a13*x + a23*y + a33));// * (double)H;

			u = (int)(floatu + 0.5);
			v = (int)(floatv + 0.5);

			Table[i][j].x = u;
			Table[i][j].y = v;

			if(u >= W) 
				Table[i][j].x = W-1;
			else if(u < 0)
				Table[i][j].x = 0;
			
			if(v >= H) 
				Table[i][j].y = H-1;
			else if(v < 0)
				Table[i][j].y = 0;
		}
	}
}

void InversePerspective(int W, int H, int LW, int LH,
	unsigned char **img, unsigned char **out, POINT **Table)
{
	int x, y, dx, dy;

	for(y = 0 ; y < LH ; y++)
		for(x = 0 ; x < LW ; x++)
		{
			dx = Table[y][x].x;
			dy = Table[y][x].y;

			if(dx < 0 || dy < 0) out[y][x] = 0;
			else out[y][x] = img[dy][dx];
		}
}


void PerspectiveTable(int W, int H, int TW, int TH, 
	POINT p0, POINT p1, POINT p2, POINT p3, POINT **Table)
{
	double delx1, dely1, delx2, dely2, delx3, dely3;
	double a11, a12, a13;
	double a21, a22, a23;
	double a31, a32, a33;

	int u, v;
	double floatu, floatv;
	int i, j;
	double x, y;
//	unsigned long sourcebase;
	double dA, dB, dC, dD, dE, dF, dG, dH, dI;

	delx1 = p1.x - p2.x;
	dely1 = p1.y - p2.y;
	delx2 = p3.x - p2.x;
	dely2 = p3.y - p2.y;
	delx3 = p0.x - p1.x + p2.x - p3.x;
	dely3 = p0.y - p1.y + p2.y - p3.y;

	if(delx3 == 0.0 && dely3 == 0.0)
	{
		a11 = p1.x - p0.x;
		a21 = p2.x - p1.x;
		a31 = p0.x;
		a12 = p1.y - p0.y;
		a22 = p2.y - p1.y;
		a32 = p0.y;
		a13 = 0.0;
		a23 = 0.0;
		a33 = 1.0;
	}
	else
	{
		a13 = (delx3*dely2-delx2*dely3)/(delx1*dely2-dely1*delx2);
		a23 = (delx1*dely3-dely1*delx3)/(delx1*dely2-dely1*delx2);
		a11 = p1.x - p0.x + a13*p1.x;
		a21 = p3.x - p0.x + a23*p3.x;
		a31 = p0.x;
		a12 = p1.y - p0.y + a13*p1.y;
		a22 = p3.y - p0.y + a23*p3.y;
		a32 = p0.y;
		a33 = 1.0;
	}

	dA = a22 * a33 - a32 * a23;
	dB = a31 * a23 - a21 * a33;
	dC = a21 * a32 - a31 * a22;
	dD = a32 * a13 - a12 * a33;
	dE = a11 * a33 - a31 * a13;
	dF = a31 * a12 - a11 * a32;
	dG = a12 * a23 - a22 * a13;
	dH = a21 * a13 - a11 * a23;
	dI = a11 * a22 - a21 * a12;

	for(i = 0 ; i < TH ; i++)
	{
		y = (double)i;///(double)H;
		for(j = 0 ; j < TW ; j++)
		{
			x = (double)j;///(double)W;
			floatu = ((dA*x + dB*y + dC)/(dG*x + dH*y + dI)) * (double)W;
			floatv = ((dD*x + dE*y + dF)/(dG*x + dH*y + dI)) * (double)H;

			u = (int)(floatu + 0.5);
			v = (int)(floatv + 0.5);

			Table[i][j].x = u;
			Table[i][j].y = v;

			if(u >= W)
				Table[i][j].x = W-1;
			else if(u < 0)
				Table[i][j].x = 0;
			
			if(v >= H)
				Table[i][j].y = H-1;
			else if(v < 0)
				Table[i][j].y = 0;
		}
	}
}

void Perspective(int W, int H, int TW, int TH, 
	unsigned char **img, unsigned char **out, POINT **Table)
{
	int x, y, dx, dy;

	for(y = 0 ; y < TH ; y++)
		for(x = 0 ; x < TW ; x++)
		{
			dx = Table[y][x].x;
			dy = Table[y][x].y;

			if(dx < 0 || dy < 0) out[y][x] = 0;
			else out[y][x] = img[dy][dx];
		}
}

void BilinearRadial(unsigned char **Image2DGray, int nW, int nH, unsigned char **Image2DGrayOut,
			  int topLeft) 
{
	int x, y;
	int mapX;
	//int mapY;

	topLeft = 300;
	for(y = 0 ; y < nH ; y++)
	{	
		for(x = 0 ; x < nW ; x++)
		{
			double fact = (x-nW/2.)*pow(fabs(nW/2.-x)/(nW/2.), 0.05);//*0.25*fabs( y)/pow(fabs( y), 0.8);//* (1./fabs(x-nW/2.))*1000;
			double dist = Dist(x, y, nW/2, nH-1);
			double MaxDist = Dist(0, 0, nW/2, nH-1);
		//	double fact = (x-nW/2.)*pow(dist/MaxDist, 1.2);
			mapX = (int)(nW/2.+ fact * (nW/2.) / ((topLeft)*y/(nH-1)+(nW/2.-topLeft)) + 0.5);
			
			if(mapX < 0)
				Image2DGrayOut[y][x] = Image2DGray[y][0];
			else if(mapX >= nW)
				Image2DGrayOut[y][x] = Image2DGray[y][nW-1];
			else
				Image2DGrayOut[y][x] = Image2DGray[y][mapX];

//			double fact2 = (nH-1-y)*pow((nH-1-y)/(nH-1.), 0.95);
//			mapY = (int)(nH-1-fact2+0.5);

//			if(mapX >= nW || mapY >= nH || mapX < 0 || mapY < 0)
//				Image2DGrayOut[y][x] = 0;
//			else
//				Image2DGrayOut[y][x] = Image2DGray[mapY][mapX];
				
		}
	}
	////
}

void InversePerspectivePt(int W, int H, int LW, int LH,
	POINT p0, POINT p1, POINT p2, POINT p3, double Ox, double Oy, double *Tx, double *Ty)
{
	double delx1, dely1, delx2, dely2, delx3, dely3;
	double a11, a12, a13;
	double a21, a22, a23;
	double a31, a32, a33;

	double x, y;
//	unsigned long sourcebase;
//	double dA, dB, dC, dD, dE, dF, dG, dH, dI;

	delx1 = p1.x - p2.x;
	dely1 = p1.y - p2.y;
	delx2 = p3.x - p2.x;
	dely2 = p3.y - p2.y;
	delx3 = p0.x - p1.x + p2.x - p3.x;
	dely3 = p0.y - p1.y + p2.y - p3.y;

	if(delx3 == 0.0 && dely3 == 0.0)
	{
		a11 = p1.x - p0.x;
		a21 = p2.x - p1.x;
		a31 = p0.x;
		a12 = p1.y - p0.y;
		a22 = p2.y - p1.y;
		a32 = p0.y;
		a13 = 0.0;
		a23 = 0.0;
		a33 = 1.0;
	}
	else
	{
		a13 = (delx3*dely2-delx2*dely3)/(delx1*dely2-dely1*delx2);
		a23 = (delx1*dely3-dely1*delx3)/(delx1*dely2-dely1*delx2);
		a11 = p1.x - p0.x + a13*p1.x;
		a21 = p3.x - p0.x + a23*p3.x;
		a31 = p0.x;
		a12 = p1.y - p0.y + a13*p1.y;
		a22 = p3.y - p0.y + a23*p3.y;
		a32 = p0.y;
		a33 = 1.0;
	}

//	dA = a22 * a33 - a32 * a23;
//	dB = a31 * a23 - a21 * a33;
//	dC = a21 * a32 - a31 * a22;
//	dD = a32 * a13 - a12 * a33;
//	dE = a11 * a33 - a31 * a13;
//	dF = a31 * a12 - a11 * a32;
//	dG = a12 * a23 - a22 * a13;
//	dH = a21 * a13 - a11 * a23;
//	dI = a11 * a22 - a21 * a12;

	y = (double)Oy/(double)LH;
	x = (double)Ox/(double)LW;
			
	*Tx = ((a11*x + a21*y + a31)/(a13*x + a23*y + a33));// * (double)W;
	*Ty = ((a12*x + a22*y + a32)/(a13*x + a23*y + a33));// * (double)H;
}

void DrawBoundary(BYTE **ImageGray, int nW, int nH, CPoint *Boundary, int Cnt, BYTE Color)
{
	int i;
	
	for(i = 0 ; i < Cnt ; i++)
	{
		if(Boundary[i].x < 0) continue;
		if(Boundary[i].y < 0) continue;
		if(Boundary[i].x >= nW) continue;
		if(Boundary[i].y >= nH) continue;

		ImageGray[Boundary[i].y][Boundary[i].x] = Color;
	}
}

double MeanAngleLabel(int **Label, int nLabelNum, CRect boundbox)
{
	double sumSine = 0., sumCosine = 0.;
	int cnt = 0;
	int x, y;
	double theta;
	double meanX = 0., meanY = 0.;

	for(y = boundbox.top ; y <= boundbox.bottom ; y++)
		for(x = boundbox.left ; x < boundbox.right ; x++)
			if(Label[y][x] == nLabelNum)
			{
				meanX += x;
				meanY += y;
				cnt++;
			}

	if(cnt > 0)
	{
		meanX /= cnt;
		meanY /= cnt;
	}

	for(y = boundbox.top ; y <= boundbox.bottom ; y++)
		for(x = boundbox.left ; x < boundbox.right ; x++)
			if(Label[y][x] == nLabelNum)
			{
				double angle = atan2(y-meanY, x-meanX);
				sumSine += sin(2.*atan2(y-meanY, x-meanX));
				sumCosine += cos(2.*atan2(y-meanY, x-meanX));
			}

	if(cnt == 0) theta = 0.;
	else theta = atan2(sumSine, sumCosine)/2.;

	return theta;
}

void Harris2DGray(unsigned char **Image2DGray, int nW, int nH, int nSize, double **Image2DGrayOut, double dThre, double dK)
{
	double GrdX, GrdY;
	double GrdX2, GrdY2, GrdXY;
	int x, y;
	int dx, dy;

	double **Temp;

	Temp = dmatrix(nH, nW);

	for (y = 0 ; y < nH; y++)
		for (x = 0 ; x < nW ; x++)
		{
			GrdX2=0.;
			GrdY2=0.;
			GrdXY=0.;

			for(dy = -1 * nSize/2 ; dy < (double)nSize/2. ; dy++)
				for(dx = -1 * nSize/2 ; dx < (double)nSize/2. ; dx++)
				{
					GrdX = (Image2DGray[(y+dy+nH)%nH][(x+dx+nW+1)%nW] - Image2DGray[(y+dy+nH)%nH][(x+dx+nW-1)%nW])/2.;
					GrdY = (Image2DGray[(y+dy+nH+1)%nH][(x+dx+nW)%nW] - Image2DGray[(y+dy+nH-1)%nH][(x+dx+nW)%nW])/2.;

					GrdX2 += GrdX*GrdX;
					GrdY2 += GrdY*GrdY;
					GrdXY += GrdX*GrdY;
				}
      
			GrdX2 /= nSize*2;
			GrdY2 /= nSize*2;
			GrdXY /= nSize*2;

			Temp[y][x] = (GrdX2*GrdY2 - GrdXY*GrdXY) -
				dK * (GrdX2+GrdY2)*(GrdX2+GrdY2);
		}

	for (y = 0 ; y < nH; y++)
		for (x = 0 ; x < nW ; x++)
		{
			if(dThre > 0)
			{
				if(Temp[y][x] > dThre &&
					Temp[y][x] >= Temp[(y+nH-1)%nH][(x+nW-1)%nW] &&
					Temp[y][x] >= Temp[(y+nH-1)%nH][(x+nW)%nW] &&
					Temp[y][x] >= Temp[(y+nH-1)%nH][(x+nW+1)%nW] &&
					Temp[y][x] >= Temp[(y+nH)%nH][(x+nW-1)%nW] &&
					Temp[y][x] >= Temp[(y+nH)%nH][(x+nW+1)%nW] &&
					Temp[y][x] >= Temp[(y+nH+1)%nH][(x+nW-1)%nW] &&
					Temp[y][x] >= Temp[(y+nH+1)%nH][(x+nW)%nW] &&
					Temp[y][x] >= Temp[(y+nH+1)%nH][(x+nW+1)%nW])
					Image2DGrayOut[y][x] = 255;
				else
					Image2DGrayOut[y][x] = 0;
			}
			else
				Image2DGrayOut[y][x] = Temp[y][x];

		}

	free_dmatrix(Temp, nH, nW);
}

void ShoulderBinary2DGray(unsigned char **Image2DGray, int nW, int nH, unsigned char **Image2DGrayOut)
{
	int x, y, i;
	int His[GRAY_CNT] = {0};
	int Max, Min;
	int MaxPos, MinPos;

	for(y = 0 ; y < nH ; y++)
		for(x = 0 ; x < nW ; x++)
			His[Image2DGray[y][x]]++;

	Max = 0;
	MaxPos = 0;
	for(i = 0 ; i < GRAY_CNT ; i++)
	{
		if(His[i] > Max)
		{
			Max = His[i];
			MaxPos = i;
		}
	}

	for(i = GRAY_CNT ; i >= 0 ; i--)
	{
		if(His[i] > 0)
		{
			Min = His[i];
			MinPos = i;
			break;
		}
	}

	if(MinPos == MaxPos) 
	{
		if(Image2DGrayOut == NULL)
		{
			for(y = 0 ; y < nH ; y++)
				for(x = 0 ; x < nW ; x++)
					Image2DGray[y][x] = 0;
		}
		else
		{
			for(y = 0 ; y < nH ; y++)
				for(x = 0 ; x < nW ; x++)
					Image2DGrayOut[y][x] = 0;
		}
		
		return;
	}

	int PrevMax;
	int Curr;
	int CurrPos;

	PrevMax = Min;

	double a, b, c;

	a = (Min - Max);
	b = (MaxPos - MinPos);
	c = (MinPos - MaxPos)*MaxPos - (Min - Max)*MinPos;

	int Thre;
	double MaxDist, Dist;

	Thre = MinPos;
	MaxDist = 0;
	for(i = MinPos ; i >= MaxPos ; i--)
	{
		Curr = His[i];
		CurrPos = i;

		if(i != MinPos)
		{
			if(His[i] < PrevMax)
				Curr = PrevMax;
		}
		
		Dist = fabs(a*CurrPos + b*Curr + c) / sqrt(a*a + b*b);

		if(Dist > MaxDist)
		{
			MaxDist = Dist;
			Thre = i;
		}

		if(His[i] > PrevMax)
			PrevMax = His[i];
	}

	if(Image2DGrayOut == NULL)
	{
		for(y = 0 ; y < nH ; y++)
			for(x = 0 ; x < nW ; x++)
			{
				if(Image2DGray[y][x] > Thre)
					Image2DGray[y][x] = 255;
				else
					Image2DGray[y][x] = 0;
			}
	}
	else
	{
		for(y = 0 ; y < nH ; y++)
			for(x = 0 ; x < nW ; x++)
			{
				if(Image2DGray[y][x] > Thre)
					Image2DGrayOut[y][x] = 255;
				else
					Image2DGrayOut[y][x] = 0;
			}
	}
}

void VectorMedianFilter(int **OrgX, int **OrgY, int **OutX, int **OutY, int nW, int nH)
{
	int x, y;
	int dx, dy, tx, ty;
	double Min;
	double Sum;

	for(y = 0 ; y < nH ; y++)
		for(x = 0 ; x < nW ; x++)
		{
			OutX[y][x] = 0;
			OutY[y][x] = 0;
		}

	for(y = 1 ; y < nH-1 ; y++)
		for(x = 1 ; x < nW-1 ; x++)
		{
			Min = (nW+nH)*9;

			OutX[y][x] = OrgX[y][x];
			OutY[y][x] = OrgY[y][x];
	
//			if(x1[y][x]*x1[y][x]+x2[y][x]*x1[y][x] < 2) continue;//

			for(dy = -1; dy < 2 ; dy++)
				for(dx = -1 ; dx < 2; dx++)
				{
					Sum = 0.;
					int Cnt;

					Cnt = 0;
					for(ty = -1 ; ty < 2; ty++)
						for(tx = -1 ; tx < 2; tx++)
						{
//							if(x1[y+ty][x+tx]*x1[y+ty][x+tx]+x2[y+ty][x+tx]*x1[y+ty][x+tx] == 0) continue;//

							Sum += sqrt((double)((OrgX[y+dy][x+dx] - OrgX[y+ty][x+tx])*(OrgX[y+dy][x+dx] - OrgX[y+ty][x+tx]) + 
								(OrgY[y+dy][x+dx] - OrgY[y+ty][x+tx])*(OrgY[y+dy][x+dx] - OrgY[y+ty][x+tx])));

							Cnt++;
						}

					if(Cnt == 0) continue;

					Sum /= (double)Cnt;
					if(Sum < Min)
					{
						Min = Sum;
						OutX[y][x] = OrgX[y+dy][x+dx];
						OutY[y][x] = OrgY[y+dy][x+dx];
					}
				}

		}
}

void YUV2RGB(BYTE *Y, BYTE *U, BYTE *V, BYTE **R, BYTE **G, BYTE **B, int nW, int nH, int nType)
{
	if(nType == 0) // YUV420
	{
		int x, y;
		int posY = 0, posUV = 0;

		for(y = 0 ; y < nH ; y++)
			for(x = 0 ; x < nW ; x++)
			{
				posUV = (nW/2)*(y/2) + (x/2);

				int b = (int)(1.164*(Y[posY] - 16)							+ 2.018*(U[posUV] - 128));
				int g = (int)(1.164*(Y[posY] - 16) - 0.813*(V[posUV] - 128) - 0.391*(U[posUV] - 128));
				int r = (int)(1.164*(Y[posY] - 16) + 1.596*(V[posUV] - 128));

				R[y][x] = r>255? 255 : r<0 ? 0 : r;
				G[y][x] = g>255? 255 : g<0 ? 0 : g;
				B[y][x] = b>255? 255 : b<0 ? 0 : b;

				posY++;
			}
	}
}

void ImageOperation(int nOperationType)
{
	int nW1, nH1, nPosX1, nPosY1,
		nW2, nH2, nPosX2, nPosY2,
		nW, nH;
	BYTE **ImageGray1, **ImageGray2, **OutputGray;
	BYTE **R1, **GL, **B1, **R2, **G2, **B2, **R, **G, **B;
	int x, y;

	// 영상 정보 읽기
	if(!GetCurrentImageInfo(&nW1, &nH1, &nPosX1, &nPosY1)) return;
	if(!GetCurrentImageInfo(&nW2, &nH2, &nPosX2, &nPosY2, 1)) return;

	nW = (nW1 > nW2) ? nW2 : nW1;
	nH = (nH1 > nH2) ? nH2 : nH1;

	ImageGray1 = cmatrix(nH1, nW1);
	R1 = cmatrix(nH1, nW1);
	GL = cmatrix(nH1, nW1);
	B1 = cmatrix(nH1, nW1);
	
	ImageGray2 = cmatrix(nH2, nW2);
	R2 = cmatrix(nH2, nW2);
	G2 = cmatrix(nH2, nW2);
	B2 = cmatrix(nH2, nW2);
	
	OutputGray = cmatrix(nH, nW);
	R = cmatrix(nH, nW);
	G = cmatrix(nH, nW);
	B = cmatrix(nH, nW);

	// 회색조 영상 읽기
	GetCurrentImageGray(ImageGray1);
	GetCurrentImageColor(R1, GL, B1);

	GetCurrentImageGray(ImageGray2, 1);
	GetCurrentImageColor(R2, G2, B2, 1);

	for(y = 0 ; y < nH ; y++)
		for(x = 0 ; x < nW ; x++)
		{
			if(nOperationType == 0)
			{
				R[y][x] = (R1[y][x]+R2[y][x])/2;
				G[y][x] = (GL[y][x]+G2[y][x])/2;
				B[y][x] = (B1[y][x]+B2[y][x])/2;
			}
			else if(nOperationType == 1)
			{
				R[y][x] = (R1[y][x]-R2[y][x]+GRAY_CNT-1)/2;
				G[y][x] = (GL[y][x]-G2[y][x]+GRAY_CNT-1)/2;
				B[y][x] = (B1[y][x]-B2[y][x]+GRAY_CNT-1)/2;
			}
			else if(nOperationType == 2)
				OutputGray[y][x] = 
					(ImageGray1[y][x] > GRAY_CNT/2 && ImageGray2[y][x] > GRAY_CNT/2) ? GRAY_CNT-1 : 0;
			else if(nOperationType == 3)
				OutputGray[y][x] = 
					(ImageGray1[y][x] > GRAY_CNT/2 || ImageGray2[y][x] > GRAY_CNT/2) ? GRAY_CNT-1 : 0;
			else if(nOperationType == 4)
			{
				R[y][x] = abs(R1[y][x]-R2[y][x]);
				G[y][x] = abs(GL[y][x]-G2[y][x]);
				B[y][x] = abs(B1[y][x]-B2[y][x]);
			}
		}

	if(nOperationType == 2 || nOperationType == 3)
		DisplayCimage2D(OutputGray, nW, nH, nPosX1+nW1, nPosY1);
	else
		DisplayCimage2DColor(R, G, B, nW, nH, nPosX1+nW1, nPosY1);
	
	free_cmatrix(ImageGray1, nH1, nW1);	
	free_cmatrix(R1, nH1, nW1);	
	free_cmatrix(GL, nH1, nW1);	
	free_cmatrix(B1, nH1, nW1);	

	free_cmatrix(ImageGray2, nH2, nW2);	
	free_cmatrix(R2, nH2, nW2);	
	free_cmatrix(G2, nH2, nW2);	
	free_cmatrix(B2, nH2, nW2);	

	free_cmatrix(OutputGray, nH, nW);	
	free_cmatrix(R, nH, nW);	
	free_cmatrix(G, nH, nW);	
	free_cmatrix(B, nH, nW);	
}

//----------------------------------------------------------------------------------------------------------

void MatToByteGray(BYTE* srcMatData, BYTE** desByte, int nH, int nW)
{
	int Pos = 0;

	for (int y = 0; y < nH; y++)
		for (int x = 0; x < nW; x++)
		{
			desByte[y][x] = srcMatData[Pos++];
		}
}

void ByteToMatGray(BYTE** srcByte, BYTE* desMatData, int nH, int nW)
{
	int Pos = 0;

	for (int y = 0; y < nH; y++)
		for (int x = 0; x < nW; x++)
		{
			desMatData[Pos++] = srcByte[y][x];
		}
}

void DoubleToMat(double** srcDouble, Mat& desMat, int nH, int nW)
{
	for (int y = 0; y < nH; y++)
		for (int x = 0; x < nW; x++)
		{
			desMat.at<double>(y, x) = srcDouble[y][x];
		}
}
BYTE** CreateOutputMatch(BYTE** ByteLeft, BYTE** ByteRight, int nHL, int nWL, int nHR, int nWR, vector<Point2f>& pointL, vector<Point2f>& pointR, int* pHO, int* pWO)
{
	int y, x;
	BYTE** tempByte;

	*pHO = (nHR > nHL)? nHR : nHL;
	*pWO = nWL + nWR;

	tempByte = cmatrix(*pHO, *pWO);

	// input image
	for (y = 0; y < nHL; y++)
		for (x = 0; x < nWL; x++)
		{
			tempByte[y][x] = ByteLeft[y][x];
		}

	for (y = 0; y < nHR; y++)
		for (x = nWL; x < *pWO; x++)
		{
			tempByte[y][x] = ByteRight[y][x-nWL];
		}

	for (int i = 0; i < pointL.size(); i++)
		DrawLine(tempByte, *pWO, *pHO, pointL[i].x, pointL[i].y, pointR[i].x+nWL, pointR[i].y, 255);

	return tempByte;
}

void SelectRandomPoint(vector<Point2f>&PointL, vector<Point2f>&PointR, vector<Point2f>& SelectPoint_L, vector<Point2f>& SelectPoint_R)
{
	int* tempIndex = new int[4];
	int nRandom_Max = PointL.size();
	int nTemp;
	bool bSuccess=false;

	RNG rng;
	
	for (int i = 0; i < 4; i++)
	{
		if (i == 0) tempIndex[i] = rng.uniform(0, nRandom_Max);
		else{
			nTemp = rng.uniform(0, nRandom_Max);

			for (int j = 0; j < i; j++) {
				if (nTemp == tempIndex[j]){
					nTemp = rng.uniform(0, nRandom_Max);
					j = -1;
				}
			}
			tempIndex[i] = nTemp;
		}
		
	}
	
	for (int i = 0; i < 4; i++)
	{
		SelectPoint_L.push_back(Point2f(PointL[tempIndex[i]].x, PointL[tempIndex[i]].y));
		SelectPoint_R.push_back(Point2f(PointR[tempIndex[i]].x, PointR[tempIndex[i]].y));
	}
	
	delete[] tempIndex;
	return;
}

void Kmean_DualHomography(vector<Point2f> &data, bool* clusterLabel, int nH, Point2f &topSeed, Point2f &btmSeed)
{
	int i, j, tmp, nChange=1;
	int sumX=0, avgX;
	int dataNum = data.size();
	double** distance;

	distance = dmatrix(dataNum,2);

	//topSeed, btmSeed 설정.
	for (i = 0; i < dataNum; i++)
		sumX += data[i].x;
	
	avgX = sumX / dataNum;

	topSeed.x = avgX; topSeed.y = 0;
	btmSeed.x = avgX; btmSeed.y = nH;

	while (nChange)
	{
		nChange = 0;
		//각 정보들 거리 계산.
		for (i = 0; i < dataNum; i++)
			for (j = 0; j < 2; j++)
			{
				if (j == 0)
					tmp = (topSeed.x - data[i].x) * (topSeed.x - data[i].x) + (topSeed.y - data[i].y) * (topSeed.y - data[i].y);
				if (j == 1)
					tmp = (btmSeed.x - data[i].x) * (btmSeed.x - data[i].x) + (btmSeed.y - data[i].y) * (btmSeed.y - data[i].y);

				distance[i][j] = sqrt(double(tmp));
			}

		//Label 할당. 0 : top, 1 : bottom
		for (i = 0; i < dataNum; i++)
		{
			if (distance[i][0] < distance[i][1]){
				if (clusterLabel[i] == 1) nChange++;

				clusterLabel[i] = 0;
			}

			if (distance[i][0] > distance[i][1]){
				if (clusterLabel[i] == 0) nChange++;

				clusterLabel[i] = 1;
			}
		}

		//다시 topSeed, btmSeed 계산.
		double sum_topX = 0, sum_topY = 0, sum_btmX = 0, sum_btmY = 0;
		int topNum = 0, btmNum = 0;

		for (i = 0; i < dataNum; i++)
		{
			if (clusterLabel[i] == 0) {
				sum_topX += data[i].x;
				sum_topY += data[i].y;
				topNum++;
			}

			if (clusterLabel[i] == 1) {
				sum_btmX += data[i].x;
				sum_btmY += data[i].y;
				btmNum++;
			}
		}

		topSeed.x = sum_topX / double(topNum);
		topSeed.y = sum_topY / double(topNum);
		btmSeed.x = sum_btmX / double(btmNum);
		btmSeed.y = sum_btmY / double(btmNum);
	}

	free_dmatrix(distance, dataNum, 2);
}

double GetDistance(double x1, double y1, double x2, double y2)
{
	double distance;
	distance = (((double(x1) - double(x2)) * (double(x1) - double(x2))) + ((double(y1) - double(y2)) * (double(y1) - double(y2))));
	distance = sqrt(distance);

	return distance;
}

double GetDistance_Manhatton(int x1, int y1, int x2, int y2)
{
	double distance;
	distance = abs(double(x1) - double(x2)) + abs(double(y1) - double(y2));

	return distance;
}

double GetDistance_Mahalanobis(int x1, int y1, int x2, int y2, double** m_cov)
{
	double dDistance;
	double dDiffX, dDiffY;
	double dTempX, dTempY;

	// ((p-u)^t  * m_cov^-1  * (p-u))^1/2
	dDiffX = double(x1) - double(x2);
	dDiffY = double(y1) - double(y2); // (p - u)

	m_cov=InverseMatrix2D(m_cov); // m_cov^-1

	dTempX = dDiffX*m_cov[0][0] + dDiffY*m_cov[1][0];
	dTempY = dDiffX*m_cov[0][1] + dDiffY*m_cov[1][1];


	dDistance = dTempX * dDiffX + dTempY * dDiffY;
	dDistance = sqrt(dDistance);

	return dDistance;
}


void GetCovMatrix2D(double ** covMat, vector<Point2f>& cluster)
{
	int i = 0, nSize = 0;
	double dX_sum = 0, dY_sum = 0, dXY_sum=0;
	double dX_mean, dY_mean;
	double dX_var, dY_var;
	double dXY_var;

	nSize = cluster.size();

	// Get X,Y's mean
	for (i = 0; i < nSize; i++)
	{
		dX_sum += cluster[i].x; 
		dY_sum += cluster[i].y;
	}

	dX_mean = dX_sum / nSize;
	dY_mean = dY_sum / nSize;

	// Get X,Y's variance
	dX_sum = 0; dY_sum = 0;

	for (i = 0; i < nSize; i++)
	{
		dX_sum += (dX_mean - cluster[i].x) * (dX_mean - cluster[i].x);
		dY_sum += (dY_mean - cluster[i].y) * (dY_mean - cluster[i].y);
	}

	dX_var = dX_sum / nSize;
	dY_var = dY_sum / nSize;

	// Get XY variance
	for (i = 0; i < nSize; i++)
	{
		dXY_sum += (cluster[i].x - dX_mean) * (cluster[i].y - dY_mean);
	}

	dXY_var = dXY_sum / nSize;

	// Get Covariance Matrix

	covMat[0][0] = dX_var; covMat[0][1] = dXY_var;
	covMat[1][0] = dXY_var; covMat[1][1] = dY_var;

	return ;
}

void GetCovMatrix2D(double ** covMat, vector<Point2f>& cluster, Point2f* mean)
{
	int i = 0, nSize = 0;
	double dX_sum = 0, dY_sum = 0, dXY_sum = 0;
	double dX_mean, dY_mean;
	double dX_var, dY_var;
	double dXY_var;

	nSize = cluster.size();

	// Get X,Y's mean
	for (i = 0; i < nSize; i++)
	{
		dX_mean = mean[i].x;
		dY_mean = mean[i].y;
	}

	// Get X,Y's variance
	dX_sum = 0; dY_sum = 0;

	for (i = 0; i < nSize; i++)
	{
		dX_sum += (dX_mean - cluster[i].x) * (dX_mean - cluster[i].x);
		dY_sum += (dY_mean - cluster[i].y) * (dY_mean - cluster[i].y);
	}

	dX_var = dX_sum / nSize;
	dY_var = dY_sum / nSize;

	// Get XY variance
	for (i = 0; i < nSize; i++)
	{
		dXY_sum += (cluster[i].x - dX_mean) * (cluster[i].y - dY_mean);
	}

	dXY_var = dXY_sum / nSize;

	// Get Covariance Matrix

	covMat[0][0] = dX_var; covMat[0][1] = dXY_var;
	covMat[1][0] = dXY_var; covMat[1][1] = dY_var;

	return;
}

void GetCovAndMean2D(double ** covMat, double * mean, vector<Point2f>& cluster)
{
	int i = 0, nSize = 0;
	double dX_sum = 0, dY_sum = 0, dXY_sum = 0;
	double dX_mean, dY_mean;
	double dX_var, dY_var;
	double dXY_var;

	nSize = cluster.size();

	// Get X,Y's mean
	for (i = 0; i < nSize; i++)
	{
		dX_sum += cluster[i].x;
		dY_sum += cluster[i].y;
	}

	dX_mean = dX_sum / nSize;
	dY_mean = dY_sum / nSize;

	mean[0] = dX_mean;
	mean[1] = dY_mean;

	// Get X,Y's variance
	dX_sum = 0; dY_sum = 0;

	for (i = 0; i < nSize; i++)
	{
		dX_sum += ((dX_mean - cluster[i].x) * (dX_mean - cluster[i].x));
		dY_sum += ((dY_mean - cluster[i].y) * (dY_mean - cluster[i].y));
	}

	dX_var = dX_sum / nSize;
	dY_var = dY_sum / nSize;

	// Get XY variance
	for (i = 0; i < nSize; i++)
	{
		dXY_sum += ((cluster[i].x - dX_mean) * (cluster[i].y - dY_mean));
	}

	dXY_var = dXY_sum / nSize;

	// Get Covariance Matrix

	covMat[0][0] = dX_var; covMat[0][1] = dXY_var;
	covMat[1][0] = dXY_var; covMat[1][1] = dY_var;

	return;
}

double** InverseMatrix2D(double** matrix)
{
	double dDet;
	double** M_inv;

	M_inv = dmatrix(2, 2);

	dDet = (matrix[0][0] * matrix[1][1]) - (matrix[0][1] * matrix[1][0]);
	
	M_inv[0][0] = matrix[1][1] / dDet; M_inv[0][1] = (-1 * matrix[0][1]) / dDet;
	M_inv[1][0] = (-1 * matrix[1][0]) / dDet; M_inv[1][1] = matrix[0][0] / dDet;

	return M_inv;
}

double DetermineMat2D(double** matrix)
{
	double tmp;
	tmp = (matrix[0][0] * matrix[1][1]) - (matrix[0][1] * matrix[1][0]);

	return tmp;
}

double** MultipleMatrix2D(double** mat1, double** mat2)
{
	double** tmpMat;

	tmpMat = dmatrix(2, 2);

	tmpMat[0][0] = (mat1[0][0] * mat2[0][0]) + (mat1[0][1] * mat2[1][0]);
	tmpMat[0][1] = (mat1[0][0] * mat2[0][1]) + (mat1[0][1] * mat2[1][1]);
	tmpMat[1][0] = (mat1[1][0] * mat2[0][0]) + (mat1[1][1] * mat2[1][0]);
	tmpMat[1][1] = (mat1[1][0] * mat2[0][1]) + (mat1[1][1] * mat2[1][1]);

	return tmpMat;
}

void GetPerspectivePoint(Point2f& input, Point2f& output, Mat& homoMat)
{
	double _x, _y, _w;

	_x = homoMat.at<double>(0, 0) * input.x + homoMat.at<double>(0, 1) * input.y + homoMat.at<double>(0, 2);
	_y = homoMat.at<double>(1, 0) * input.x + homoMat.at<double>(1, 1) * input.y + homoMat.at<double>(1, 2);
	_w = homoMat.at<double>(2, 0) * input.x + homoMat.at<double>(2, 1) * input.y + homoMat.at<double>(2, 2);

	if (_w == 0) _w = 0.00001;

	output.x = _x / _w;
	output.y = _y / _w;
}

void GetMatchingPoint(BYTE** ImageL, BYTE** ImageR, vector<Point2f>& GodPointL, vector<Point2f>& GodPointR, int nHR, int nWR, int nHL, int nWL)
{
	Mat M_ImageL(cvSize(nWL, nHL), CV_8UC1, cvScalar(0));
	Mat M_ImageR(cvSize(nWR, nHR), CV_8UC1, cvScalar(0));

	// Byte -> Mat

	ByteToMatGray(ImageL, M_ImageL.data, nHL, nWL);
	ByteToMatGray(ImageR, M_ImageR.data, nHR, nWR);

	//Detect the keypoints using SURF Detector

	int minHessian = 400;
	SiftFeatureDetector detector(800);
	std::vector< KeyPoint > keypoints_L, keypoints_R;
	detector.detect(M_ImageL, keypoints_L);
	detector.detect(M_ImageR, keypoints_R);

	//-- Step 2: Calculate descriptors (feature vectors)

	SiftDescriptorExtractor extractor;
	Mat descriptors_L, descriptors_R;
	extractor.compute(M_ImageL, keypoints_L, descriptors_L);
	extractor.compute(M_ImageR, keypoints_R, descriptors_R);

	//-- Step 3: Matching descriptor vectors using FLANN matcher

	FlannBasedMatcher matcher;
	std::vector< DMatch > matches;
	matcher.match(descriptors_L, descriptors_R, matches);
	double max_dist = 0; double min_dist = 100;


	//-- Quick calculation of max and min distances between keypoints
	for (int i = 0; i < descriptors_L.rows; i++)
	{
		double dist = matches[i].distance;
		if (dist < min_dist) min_dist = dist;
		if (dist > max_dist) max_dist = dist;
	}

	//-- Use only "good" matches (i.e. whose distance is less than 3*min_dist )

	std::vector< DMatch > good_matches;
	for (int i = 0; i < descriptors_L.rows; i++)
		if (matches[i].distance < 3 * min_dist)
			good_matches.push_back(matches[i]);

	for (unsigned int i = 0; i < good_matches.size(); i++)
	{
		//-- Get the keypoints from the good matches
		GodPointL.push_back(keypoints_L[good_matches[i].queryIdx].pt);
		GodPointR.push_back(keypoints_R[good_matches[i].trainIdx].pt);
	}
}


double GetAngleVecter(double* vector1, double* vector2)
{
	double dInner;
	double dMagVec1, dMagVec2;
	double dAngle;

	dInner = (vector1[0] * vector2[0]) + (vector1[1] * vector2[1]);
	dMagVec1 = sqrt(vector1[0] * vector1[0] + vector1[1] * vector1[1]);
	dMagVec2 = sqrt(vector2[0] * vector2[0] + vector2[1] * vector2[1]);

	dAngle = acos(dInner / (dMagVec1 * dMagVec2));

	if (dAngle < 0) acos(6.28 - (dInner / (dMagVec1 * dMagVec2)));
	
	return dAngle;
}

double GetAngleVecter(Point2f vector1, Point2f vector2)
{
	double dInner;
	double dMagVec1, dMagVec2;
	double dAngle;

	dInner = (vector1.y * vector2.y) + (vector1.x * vector2.x);
	dMagVec1 = sqrt(vector1.y * vector1.y + vector1.x * vector1.x);
	dMagVec2 = sqrt(vector2.y * vector2.y + vector2.x * vector2.x);

	dAngle = acos(dInner / (dMagVec1 * dMagVec2));

	//if (dAngle < 0) acos(6.28 - (dInner / (dMagVec1 * dMagVec2)));

	dAngle = dAngle * 360. / (3.141592*2);

	return dAngle;
}

void GaussianFilter(double **Filter, double sigma, int size)
{
	int center = size / 2;

	for (int i = -center; i <= center; i++)
		for (int j = -center; j <= center; j++)
		{
			Filter[i + center][j + center] = (double)exp(-(double)(i*i + j*j) / (2 * sigma*sigma)) / (2 * 3.141592*sigma*sigma);
		}
}

void BilinearInterpolation(double** image, double** output, int nW, int nH, double upRate)
{
	int nWU = nW*upRate;
	int nHU = nH*upRate;

	for (int y = 0; y<nHU - 1; y++)
		for (int x = 0; x < nWU - 1; x++)
		{
			double rateX = (nW - 1) / (double)(nWU - 1);
			double rateY = (nH - 1) / (double)(nHU - 1);
			double ramda = x*rateX - (int)(x*rateX);
			double mu = y*rateY - (int)(y*rateY);
			int xx = (int)(x*rateX);
			int yy = (int)(y*rateY);

			output[y][x] = ramda*mu*image[yy + 1][xx + 1] + ramda*(1 - mu)*image[yy][xx + 1] + (1 - ramda)*mu*image[yy + 1][xx] + (1 - ramda)*(1 - mu)*image[yy][xx];
		}
}

void MirrorPadding(BYTE **Result, double **Filter, BYTE **Input, int FSize, int nW, int nH)
{
	BYTE** mirror;
	int nHM, nWM;
	nHM = nH + FSize - 1;
	nWM = nW + FSize - 1;
	mirror = cmatrix(nHM, nWM);
	int yy, xx;
	double sum = 0;

	for (int y = 0; y<nHM; y++)
		for (int x = 0; x<nWM; x++)
		{
			if (x<nW + FSize / 2 && y<nH + FSize / 2)
			{
				yy = abs(y - FSize / 2);
				xx = abs(x - FSize / 2);
				mirror[y][x] = Input[yy][xx];
			}
			else
			{
				if (y >= nH + FSize / 2)
					yy = y - FSize / 2 - abs(nH - (y - FSize / 2)) - 1;
				else
					yy = abs(y - FSize / 2);
				if (x >= nW + FSize / 2)
					xx = x - FSize / 2 - abs(nW - (x - FSize / 2)) - 1;
				else
					xx = abs(x - FSize / 2);
				mirror[y][x] = Input[yy][xx];
			}
		}


	for(int y = FSize / 2; y<nH + FSize / 2; y++)
		for (int x = FSize / 2; x<nW + FSize / 2; x++)
		{
			yy = y - FSize / 2;
			xx = x - FSize / 2;
			for (int j = 0; j<FSize; j++)
			{
				for (int i = 0; i<FSize; i++)
				{
					sum += mirror[y - FSize / 2 + j][x - FSize / 2 + i] * Filter[j][i];
				}
			}
			if (sum<0) sum = 0;
			if (sum>255) sum = 255;
			Result[yy][xx] = sum;
			sum = 0;
		}


	free_cmatrix(mirror, nHM, nWM);
}

void MirrorPadding(double **Result, double **Filter, double **Input, int FSize, int nW, int nH)
{
	double** mirror;
	int nHM, nWM;
	nHM = nH + FSize - 1;
	nWM = nW + FSize - 1;
	mirror = dmatrix(nHM, nWM);
	int yy, xx;
	double sum = 0;

	for (int y = 0; y<nHM; y++)
		for (int x = 0; x<nWM; x++)
		{
			if (x<nW + FSize / 2 && y<nH + FSize / 2)
			{
				yy = abs(y - FSize / 2);
				xx = abs(x - FSize / 2);
				mirror[y][x] = Input[yy][xx];
			}
			else
			{
				if (y >= nH + FSize / 2)
					yy = y - FSize / 2 - abs(nH - (y - FSize / 2)) - 1;
				else
					yy = abs(y - FSize / 2);
				if (x >= nW + FSize / 2)
					xx = x - FSize / 2 - abs(nW - (x - FSize / 2)) - 1;
				else
					xx = abs(x - FSize / 2);
				mirror[y][x] = Input[yy][xx];
			}
		}


	for (int y = FSize / 2; y<nH + FSize / 2; y++)
		for (int x = FSize / 2; x<nW + FSize / 2; x++)
		{
			yy = y - FSize / 2;
			xx = x - FSize / 2;
			for (int j = 0; j<FSize; j++)
			{
				for (int i = 0; i<FSize; i++)
				{
					sum += mirror[y - FSize / 2 + j][x - FSize / 2 + i] * Filter[j][i];
				}
			}
			if (sum<0) sum = 0;
			if (sum>255) sum = 255;
			Result[yy][xx] = sum;
			sum = 0;
		}


	free_dmatrix(mirror, nHM, nWM);
}

void MirrorPadding(double **Result, double **Filter, BYTE **Input, int FSize, int nW, int nH)
{
	double** mirror;
	int nHM, nWM;
	nHM = nH + FSize - 1;
	nWM = nW + FSize - 1;
	mirror = dmatrix(nHM, nWM);
	int yy, xx;
	double sum = 0;

	for (int y = 0; y<nHM; y++)
		for (int x = 0; x<nWM; x++)
		{
			if (x<nW + FSize / 2 && y<nH + FSize / 2)
			{
				yy = abs(y - FSize / 2);
				xx = abs(x - FSize / 2);
				mirror[y][x] = Input[yy][xx];
			}
			else
			{
				if (y >= nH + FSize / 2)
					yy = y - FSize / 2 - abs(nH - (y - FSize / 2)) - 1;
				else
					yy = abs(y - FSize / 2);
				if (x >= nW + FSize / 2)
					xx = x - FSize / 2 - abs(nW - (x - FSize / 2)) - 1;
				else
					xx = abs(x - FSize / 2);
				mirror[y][x] = Input[yy][xx];
			}
		}


	for (int y = FSize / 2; y<nH + FSize / 2; y++)
		for (int x = FSize / 2; x<nW + FSize / 2; x++)
		{
			yy = y - FSize / 2;
			xx = x - FSize / 2;
			for (int j = 0; j<FSize; j++)
			{
				for (int i = 0; i<FSize; i++)
				{
					sum += mirror[y - FSize / 2 + j][x - FSize / 2 + i] * Filter[j][i];
				}
			}
			
			Result[yy][xx] = sum;
			sum = 0;
		}


	free_dmatrix(mirror, nHM, nWM);
}

void DisplayMat(double **Mat, int row, int col)
{
	int i, j;
	CString strElement;
	BYTE** imgMat;
	int nH, nW;

	nH = row * 15;
	nW = col * 100;
	imgMat = cmatrix(nH, nW);

	for (i = 0; i < nH; i++)
		for (j = 0; j < nW; j++)
		{
			imgMat[i][j] = 0;
		}

	for (i = 0; i < row; i++)
		for (j = 0; j < col; j++)
		{
			strElement.Format(_T("%f"), Mat[i][j]);
			DrawTextOnImag2DGray(imgMat, nW, nH, j * 100, i * 15, strElement, DT_LEFT, 255, false);
		}

	DisplayCimage2D(imgMat, nW, nH, 0, 0);

	delete[] imgMat;
}

void GetRandColorArray(int** arrColor, int numColor)
{
	int nRand;

	for (int i = 0; i < numColor; i++)
	{
		nRand = rand() % 255;
		arrColor[i][0] = nRand;
		nRand = rand() % 255;
		arrColor[i][1] = nRand;
		nRand = rand() % 255;
		arrColor[i][2] = nRand;
	}
}

void BilateralFilter(BYTE **ImageRed, BYTE **ImageGreen, BYTE **ImageBlue, BYTE **imgRedesult, BYTE **imgGreenesult, BYTE **imgBlueesult, int width, int height, int block_size)
{
	int row, col;
	float *spatial, *tonal;

	// Block Size는 홀수여야 한다.
	if (block_size % 2 == 0)
		block_size--;
	
	spatial = new float[block_size*block_size];
	tonal = new float[block_size*block_size];

	// make spatial mask
	int half_block_size = int(block_size / 2);
	double sum = 0;
	int index = 0;

	for (row = -half_block_size; row <= half_block_size; row++)
	{
		for (col = -half_block_size; col <= half_block_size; col++)
		{
			double ds = double(abs(row) + abs(col));
			float gi = (float)pow(2.0, (half_block_size * 2 - ds));
			spatial[index++] = gi;
			sum += gi;
			//TRACE("%f ", gi);
		}
		//TRACE("\n");
	}

	index = 0;
	for (row = -half_block_size; row <= half_block_size; row++)
	{
		for (col = -half_block_size; col <= half_block_size; col++)
		{
			spatial[index] = (float)((double)spatial[index++] / sum);
			//TRACE("%f ", spatial[row*block_size+col]);
		}
		//TRACE("\n");
	}



	// make tonal mask & filtering process
	int channel;
	float center_pixel_of_block;
	int edge_smoothing_ratio = 2;
	int start_row;
	int end_row;
	int start_col;
	int end_col;
	BYTE pixel;
	int dit;
	int fi;
	float denom, nomi;

	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
			for (int k = 0; k < 3; k++)
		{
			switch (k)
			{
			case 0:
				center_pixel_of_block = ImageRed[i][j];
				break;
			case 1:
				center_pixel_of_block = ImageGreen[i][j];
				break;
			case 2:
				center_pixel_of_block = ImageBlue[i][j];
			}

			if (i <= half_block_size)
				start_row = -i;
			else
				start_row = -half_block_size;


			if (i >= height - half_block_size)
				end_row = height - i - 1;
			else
				end_row = half_block_size;


			if (j <= half_block_size)
				start_col = -j;
			else
				start_col = -half_block_size;


			if (j >= width - half_block_size)
				end_col = width - j - 1;
			else
				end_col = half_block_size;

			// convolution
			index = 0;
			sum = 0;

			for (row = start_row; row <= end_row; row++)
			{

				for (col = start_col; col <= end_col; col++)
				{
					switch (k)
					{
					case 0:
						pixel = ImageRed[i+row][j+col];
						break;
					case 1:
						pixel = ImageGreen[i + row][j + col];
						break;
					case 2:
						pixel = ImageBlue[i + row][j + col];
					}

					dit = (int)abs(pixel - center_pixel_of_block);
					fi = (int)pow((double)(255 - dit), edge_smoothing_ratio);
					sum += (double)fi;
					tonal[index++] = (float)fi;

					//TRACE("%f ", spatial[row*block_size+col]);
				}
			}

			index = 0;

			denom = 0;
			nomi = 0;

			for (row = start_row; row <= end_row; row++)
			{
				for (col = start_col; col <= end_col; col++)
				{
					tonal[index] = (float)((double)tonal[index] / sum);
					//g_Message.LogMsg("%f, ", tonal[index-1]);

					switch (k)
					{
					case 0:
						pixel = ImageRed[i + row][j + col];
						break;
					case 1:
						pixel = ImageGreen[i + row][j + col];
						break;
					case 2:
						pixel = ImageBlue[i + row][j + col];
					}
					denom += spatial[index] * tonal[index] * pixel;
					nomi += spatial[index] * tonal[index];
					index++;
				}
				//g_Message.LogMsg("\n");
			}

			switch (k)
			{
			case 0:
				imgRedesult[i][j] = (BYTE)(denom / nomi);
				break;
			case 1:
				imgGreenesult[i][j] = (BYTE)(denom / nomi);
				break;
			case 2:
				imgBlueesult[i][j] = (BYTE)(denom / nomi);
			}
		}

	delete[] spatial;
	delete[] tonal;
}

void FillForMean(BYTE** ImageBinary, int nW, int nH)
{
	int y, x;
	int ** fillData;
	int minXRegion, maxXRegion;
	int tmp;
	bool bFillStart = false;

	fillData = imatrix(nH, 2);

	minXRegion = nW;
	maxXRegion = 0;

	for (y = 0; y < nH; y++)
	{
		for (x = 0; x < nW - 1; x++)
		{
			if (ImageBinary[y][x] != ImageBinary[y][x + 1])
			{
				tmp = x;
				if (minXRegion > tmp) minXRegion = x;
				if (maxXRegion < tmp) maxXRegion = x;
			}
		}

		fillData[y][0] = minXRegion;
		fillData[y][1] = maxXRegion;

		minXRegion = nW;
		maxXRegion = 0;
	}

	for (y = 0; y < nH; y++)
		for (x = 0; x < nW; x++)
		{
			if (fillData[y][0] == x) bFillStart = true;
			if (fillData[y][1] == x) bFillStart = false;

			if (bFillStart == true) ImageBinary[y][x] = 255;
		}
}
// Fruc.cpp: implementation of the CFruc class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ImageProcessing.h"
#include "Fruc.h"

#include "Image.h"

#include <cmath>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFruc::CFruc()
{
	Init(320, 320);
}

CFruc::~CFruc()
{
	Closing();
}

void CFruc::Init(int nW, int nH)
{
	m_nW = nW;
	m_nH = nH;

	m_CurrR = cmatrix(m_nH, m_nW);
	m_CurrG = cmatrix(m_nH, m_nW);
	m_CurrB = cmatrix(m_nH, m_nW);

	m_PrevR = cmatrix(m_nH, m_nW);
	m_PrevG = cmatrix(m_nH, m_nW);
	m_PrevB = cmatrix(m_nH, m_nW);

	m_InterR = cmatrix(m_nH, m_nW);
	m_InterG = cmatrix(m_nH, m_nW);
	m_InterB = cmatrix(m_nH, m_nW);

	m_RefR = cmatrix(m_nH, m_nW);
	m_RefG = cmatrix(m_nH, m_nW);
	m_RefB = cmatrix(m_nH, m_nW);

	m_TempR = cmatrix(m_nH, m_nW);
	m_TempG = cmatrix(m_nH, m_nW);
	m_TempB = cmatrix(m_nH, m_nW);
}

void CFruc::Closing()
{
	free_cmatrix(m_CurrR, m_nH, m_nW);
	free_cmatrix(m_CurrG, m_nH, m_nW);
	free_cmatrix(m_CurrB, m_nH, m_nW);

	free_cmatrix(m_PrevR, m_nH, m_nW);
	free_cmatrix(m_PrevG, m_nH, m_nW);
	free_cmatrix(m_PrevB, m_nH, m_nW);

	free_cmatrix(m_InterR, m_nH, m_nW);
	free_cmatrix(m_InterG, m_nH, m_nW);
	free_cmatrix(m_InterB, m_nH, m_nW);

	free_cmatrix(m_RefR, m_nH, m_nW);
	free_cmatrix(m_RefG, m_nH, m_nW);
	free_cmatrix(m_RefB, m_nH, m_nW);

	free_cmatrix(m_TempR, m_nH, m_nW);
	free_cmatrix(m_TempG, m_nH, m_nW);
	free_cmatrix(m_TempB, m_nH, m_nW);
}

static void Gaussian5Filter(BYTE **ImageGray, BYTE **OutputGray, int nW, int nH)
{
	int x, y;
	int dx, dy;
	int nSum;
	int xx, yy;

	int Gaussian[5][5] = {2, 4, 5, 4, 2,
		4, 9, 12, 9, 4,
		5, 12, 15, 12, 5,
		4, 9, 12, 9, 4,
		2, 4, 5, 4, 2};

	for(y = 0 ; y < nH ; y++)
		for(x = 0 ; x < nW ; x++)
		{
			nSum = 0;
			for(dy = -2 ; dy <= 2 ; dy++)
				for(dx = -2 ; dx <=2 ; dx++)
				{
					xx = x+dx;
					yy = y+dy;

					if(xx < 0) xx = 0;
					if(xx >= nW) xx = nW-1;
					if(yy < 0) yy = 0;
					if(yy >= nH) yy = nH-1;

					nSum += ImageGray[yy][xx]*Gaussian[dy+2][dx+2];
				}
			
			OutputGray[y][x] = nSum/159;
		}
}

static double GetPsnrRgb(BYTE **R, BYTE **G, BYTE **B, BYTE **R2, BYTE **G2, BYTE **B2, int nW, int nH)
{
	int x, y;
	double Mse = 0.;
	double Mse1 = 0, Mse2 = 0, Mse3 = 0;

	for(y = 0 ; y < nH ; y++)
		for(x = 0 ; x < nW ; x++)
		{	
//			Mse += (R[y][x] - R2[y][x])*(R[y][x] - R2[y][x]) +
//				(G[y][x] - G2[y][x])*(G[y][x] - G2[y][x]) +
//				(B[y][x] - B2[y][x])*(B[y][x] - B2[y][x]);

			Mse1 += (R[y][x] - R2[y][x])*(R[y][x] - R2[y][x]);
			Mse2 += (G[y][x] - G2[y][x])*(G[y][x] - G2[y][x]);
			Mse3 += (B[y][x] - B2[y][x])*(B[y][x] - B2[y][x]);

		}

	Mse1 /= nW*nH;
	Mse2 /= nW*nH;
	Mse3 /= nW*nH;

	Mse = (Mse1+Mse2+Mse3)/3;

	if(Mse == 0) return 100;

	return 10*log10(255*255/Mse);
}

static double GetPsnrYuv(BYTE **R, BYTE **G, BYTE **B, BYTE **R2, BYTE **G2, BYTE **B2, int nW, int nH)
{
	int x, y;
	double Mse = 0.;
	double Mse1 = 0, Mse2 = 0, Mse3 = 0;

	for(y = 0 ; y < nH ; y++)
		for(x = 0 ; x < nW ; x++)
		{	
			double y1 = 0.257*R[y][x]	+ 0.504*G[y][x] + 0.098*B[y][x] + 16;
			double v1 = 0.439*R[y][x]  - 0.368*G[y][x] - 0.071*B[y][x] + 128;
			double u1 = -0.148*R[y][x] - 0.291*G[y][x] + 0.439*B[y][x] + 128;

			y1 = y1>255? 255 : y1<0 ? 0 : y1;
			v1 = v1>255? 255 : v1<0 ? 0 : v1;
			u1 = u1>255? 255 : u1<0 ? 0 : u1;

			double y2 = 0.257*R2[y][x]	+ 0.504*G2[y][x] + 0.098*B2[y][x] + 16;
			double v2 = 0.439*R2[y][x] - 0.368*G2[y][x] - 0.071*B2[y][x] + 128;
			double u2 = -0.148*R2[y][x] - 0.291*G2[y][x] + 0.439*B2[y][x] + 128;

			y2 = y2>255? 255 : y2<0 ? 0 : y2;
			v2 = v2>255? 255 : v2<0 ? 0 : v2;
			u2 = u2>255? 255 : u2<0 ? 0 : u2;

			Mse1 += (y1-y2)*(y1-y2);
			Mse2 += (u1-u2)*(u1-u2);
			Mse3 += (v1-v2)*(v1-v2);
		}

	Mse1 /= nW*nH;
	Mse2 /= nW*nH;
	Mse3 /= nW*nH;

	Mse = (Mse1+Mse2+Mse3)/3;

	if(Mse == 0) return 100;

	return 10*log10(255*255/Mse);
}

static double GetPsnrY(BYTE **R, BYTE **G, BYTE **B, BYTE **R2, BYTE **G2, BYTE **B2, int nW, int nH)
{
	int x, y;
	double Mse = 0.;
	
	for(y = 0 ; y < nH ; y++)
		for(x = 0 ; x < nW ; x++)
		{	
			double y1 = 0.257*R[y][x] + 0.504*G[y][x] + 0.098*B[y][x] +16;
			double y2 = 0.257*R2[y][x] + 0.504*G2[y][x] + 0.098*B2[y][x] +16;

			Mse += (y1-y2)*(y1-y2);
		}


	Mse /= nW*nH;
	
	if(Mse == 0) return 100;

	return 10*log10(255*255/Mse);
}

static CPoint BMA(BYTE **CurrImageGray, BYTE **ReferImageGray, int left, int top, 
		 int nW, int nH, int nSearchLeft, int nSearchTop, int nSearchRight, int nSearchBottom, 
		 int BlockWidth, int BlockHeight, double *pMinDiff)
{
	if(top < BlockHeight+nSearchBottom ||  top >= nH - BlockHeight-nSearchTop || 
		left < BlockWidth+nSearchRight || left >= nW - BlockWidth-nSearchLeft)
	{
		*pMinDiff = 255;
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

			for(y = 0 ; y < BlockHeight ; y++)
				for(x = 0 ; x < BlockWidth ; x++)
				{
					yy0 = top+y;
					xx0 = left+x;

					xx1 = left+x+dx;
					yy1 = top+y+dy;

					Diff += abs(CurrImageGray[yy0][xx0] - ReferImageGray[yy1][xx1]);
				}

			Diff /= BlockWidth*BlockHeight;

			if(Diff < MinDiff || (Diff == MinDiff && dx == 0 && dy == 0))
			{
				MinDiff = Diff;
				MinDx = CPoint(dx, dy);
			}
		}
	}

	*pMinDiff = MinDiff;
	return MinDx;
}

void GetInterFrame(BYTE **Prev, BYTE **Curr, BYTE **Inter, int nW, int nH, int nMask, int nSearch, int nGrow, int nMeGap, bool bDiffRegion, int nThreDiff)
{
	const int MAX_CAND = 150;
	BYTE **Candidate[MAX_CAND];
	double **Weight[MAX_CAND];
	int **CandidateIndex;
	BYTE **Diff2D;
//	BYTE SortTemp[MAX_CAND];

	int x, y;
	int i;

	Diff2D = cmatrix(nH, nW);

	for(y = 0 ; y < nH ; y++)
		for(x = 0 ; x < nW ; x++)
		{
			if(bDiffRegion)
			{
				Diff2D[y][x] = abs(Prev[y][x] - Curr[y][x]) >= nThreDiff ? 255 : 0;
			}
		}

	if(bDiffRegion)
		DisplayCimage2D(Diff2D, nW, nH, nW*2, nH*2, false, true);

	for(i = 0 ; i < MAX_CAND ; i++)
	{
		Candidate[i] = cmatrix(nH, nW);
		Weight[i] = dmatrix(nH, nW);
	}
	CandidateIndex = imatrix(nH, nW);

	for(y = 0 ; y < nH ; y++)
		for(x = 0 ; x < nW ; x++)
		{
			CandidateIndex[y][x] = 0;
		}

	
	int xx, yy;

	for(y = 0 ; y < nH ; y+=nMeGap)
	{
		for(x = 0 ; x < nW ; x+=nMeGap)
		{
			double MinDiff;
			CPoint Mv;

			if(bDiffRegion && Diff2D[y][x] == 0)
			{
				Mv = CPoint(0, 0);
			}
			else
			{
				Mv = BMA(Curr, Prev, x-nMask, y-nMask, nW, nH, nSearch, nSearch, nSearch, nSearch, nMask*2+1, nMask*2+1, &MinDiff);
				if(MinDiff > 30) Mv = CPoint(0, 0);
			}

			if(MinDiff > 30) Mv = CPoint(0, 0);

			for(yy = -nGrow ; yy <= nGrow ; yy++)
				for(xx = -nGrow ; xx <= nGrow ; xx++)
				{
					int xPos = x + Mv.x/2 + xx;
					int yPos = y + Mv.y/2 + yy;

					int xOrgPos = x+xx;
					int yOrgPos = y+yy;

					if(xPos >= 0 && xPos < nW && yPos >= 0 && yPos < nH &&
						xOrgPos >= 0 && xOrgPos < nW && yOrgPos >= 0 && yOrgPos < nH)
					{
						if(CandidateIndex[yPos][xPos] >= MAX_CAND) continue;

						Weight[CandidateIndex[yPos][xPos]][yPos][xPos] = 1.-Dist(xx, yy, 0, 0)/(nGrow*sqrt((double)2)+1);//pow(1.-fabs(xx)/(nGrow+1), 1./3);
						Candidate[CandidateIndex[yPos][xPos]][yPos][xPos] = Curr[yOrgPos][xOrgPos];
						CandidateIndex[yPos][xPos]++;
					}
				}
		}
	}

	for(y = 0 ; y < nH ; y+=nMeGap)
	{
		for(x = 0 ; x < nW ; x+=nMeGap)
		{
			double MinDiff;
			CPoint Mv;

			if(bDiffRegion && Diff2D[y][x] == 0)
			{
				Mv = CPoint(0, 0);
			}
			else
			{
				Mv = BMA(Prev, Curr, x-nMask, y-nMask, nW, nH, nSearch, nSearch, nSearch, nSearch, nMask*2+1, nMask*2+1, &MinDiff);
				if(MinDiff > 30) Mv = CPoint(0, 0);
			}

			for(yy = -nGrow ; yy <= nGrow ; yy++)
				for(xx = -nGrow ; xx <= nGrow ; xx++)
				{
					int xPos = x + Mv.x/2 + xx;
					int yPos = y + Mv.y/2 + yy;

					int xOrgPos = x+xx;
					int yOrgPos = y+yy;

					if(xPos >= 0 && xPos < nW && yPos >= 0 && yPos < nH &&
						xOrgPos >= 0 && xOrgPos < nW && yOrgPos >= 0 && yOrgPos < nH)
					{
						if(CandidateIndex[yPos][xPos] >= MAX_CAND) continue;

						Weight[CandidateIndex[yPos][xPos]][yPos][xPos] = 1.-Dist(xx, yy, 0, 0)/(nGrow*sqrt((double)2)+1);//pow(1.-fabs(xx)/(nGrow+1), 1./3);
						Candidate[CandidateIndex[yPos][xPos]][yPos][xPos] = Prev[yOrgPos][xOrgPos];
						CandidateIndex[yPos][xPos]++;
					}
				}
		}
	}
	
	for(y = 0 ; y < nH ; y++)
		for(x = 0 ; x < nW ; x++)
		{
			if(CandidateIndex[y][x] == 0)
				Inter[y][x] = Curr[y][x];
			else
			{

				double Sum = 0, SumWeight = 0;
				for(i = 0 ; i < CandidateIndex[y][x] ; i++)
				{
					Sum += Candidate[i][y][x]*Weight[i][y][x];
					SumWeight += Weight[i][y][x];
				}

				if(SumWeight == 0)
					Inter[y][x] = Curr[y][x];
				else
					Inter[y][x] = (BYTE)(Sum / SumWeight);
			}
		}
		
	for(i = 0 ; i < MAX_CAND ; i++)
	{
		free_cmatrix(Candidate[i], nH, nW);
		free_dmatrix(Weight[i], nH, nW);
	}
	free_imatrix(CandidateIndex, nH, nW);

	free_cmatrix(Diff2D, nH, nW);
}

void GetColorInterFrameByMotionY(BYTE **PrevR, BYTE **PrevG, BYTE **PrevB, 
								 BYTE **CurrR, BYTE **CurrG, BYTE **CurrB,
								 BYTE **InterR, BYTE **InterG, BYTE **InterB, 
								 int nW, int nH, int nMask, int nSearch, int nGrow, int nMeGap, bool bDiffRegion, int nThreDiff)
{
	const int MAX_CAND = 150;
	BYTE **CandidateR[MAX_CAND], **CandidateG[MAX_CAND], **CandidateB[MAX_CAND];
	double **Weight[MAX_CAND];
	int **CandidateIndex;
	BYTE **PrevGray, **CurrGray, **DiffGray;
//	BYTE SortTemp[MAX_CAND];

	int x, y;
	int i;

	PrevGray = cmatrix(nH, nW);
	CurrGray = cmatrix(nH, nW);
	DiffGray = cmatrix(nH, nW);

	for(y = 0 ; y < nH ; y++)
		for(x = 0 ; x < nW ; x++)
		{
			PrevGray[y][x] = (PrevR[y][x]+PrevG[y][x]+PrevB[y][x])/3;
			CurrGray[y][x] = (CurrR[y][x]+CurrG[y][x]+CurrB[y][x])/3;

			if(bDiffRegion)
			{
				DiffGray[y][x] = abs(PrevGray[y][x] - CurrGray[y][x]) >= nThreDiff ? 255 : 0;
			}
		}

	if(bDiffRegion)
		DisplayCimage2D(DiffGray, nW, nH, nW*2, nH*2, false, true);

	for(i = 0 ; i < MAX_CAND ; i++)
	{
		CandidateR[i] = cmatrix(nH, nW);
		CandidateG[i] = cmatrix(nH, nW);
		CandidateB[i] = cmatrix(nH, nW);

		Weight[i] = dmatrix(nH, nW);
	}
	CandidateIndex = imatrix(nH, nW);

	for(y = 0 ; y < nH ; y++)
		for(x = 0 ; x < nW ; x++)
		{
			CandidateIndex[y][x] = 0;
		}

	
	int xx, yy;

	for(y = 0 ; y < nH ; y+=nMeGap)
	{
		for(x = 0 ; x < nW ; x+=nMeGap)
		{
			double MinDiff;
			CPoint Mv;

			if(bDiffRegion && DiffGray[y][x] == 0)
			{
				Mv = CPoint(0, 0);
			}
			else
			{
				Mv = BMA(CurrGray, PrevGray, x-nMask, y-nMask, nW, nH, nSearch, nSearch, nSearch, nSearch, nMask*2+1, nMask*2+1, &MinDiff);
				if(MinDiff > 30) Mv = CPoint(0, 0);
			}

			for(yy = -nGrow ; yy <= nGrow ; yy++)
				for(xx = -nGrow ; xx <= nGrow ; xx++)
				{
					int xPos = x + Mv.x/2 + xx;
					int yPos = y + Mv.y/2 + yy;

					int xOrgPos = x+xx;
					int yOrgPos = y+yy;

					if(xPos >= 0 && xPos < nW && yPos >= 0 && yPos < nH &&
						xOrgPos >= 0 && xOrgPos < nW && yOrgPos >= 0 && yOrgPos < nH)
					{
						if(CandidateIndex[yPos][xPos] >= MAX_CAND) continue;

						Weight[CandidateIndex[yPos][xPos]][yPos][xPos] = 1.-Dist(xx, yy, 0, 0)/(nGrow*sqrt((double)2)+1);//pow(1.-fabs(xx)/(nGrow+1), 1./3);
						CandidateR[CandidateIndex[yPos][xPos]][yPos][xPos] = CurrR[yOrgPos][xOrgPos];
						CandidateG[CandidateIndex[yPos][xPos]][yPos][xPos] = CurrG[yOrgPos][xOrgPos];
						CandidateB[CandidateIndex[yPos][xPos]][yPos][xPos] = CurrB[yOrgPos][xOrgPos];
						CandidateIndex[yPos][xPos]++;
					}
				}
		}
	}
	
	for(y = 0 ; y < nH ; y+=nMeGap)
	{
		for(x = 0 ; x < nW ; x+=nMeGap)
		{
			double MinDiff;
			CPoint Mv;

			if(bDiffRegion && DiffGray[y][x] == 0)
			{
				Mv = CPoint(0, 0);
			}
			else
			{
				Mv = BMA(PrevGray, CurrGray, x-nMask, y-nMask, nW, nH, nSearch, nSearch, nSearch, nSearch, nMask*2+1, nMask*2+1, &MinDiff);
				if(MinDiff > 30) Mv = CPoint(0, 0);
			}

			if(MinDiff > 30) Mv = CPoint(0, 0);

			for(yy = -nGrow ; yy <= nGrow ; yy++)
				for(xx = -nGrow ; xx <= nGrow ; xx++)
				{
					int xPos = x + Mv.x/2 + xx;
					int yPos = y + Mv.y/2 + yy;

					int xOrgPos = x+xx;
					int yOrgPos = y+yy;

					if(xPos >= 0 && xPos < nW && yPos >= 0 && yPos < nH &&
						xOrgPos >= 0 && xOrgPos < nW && yOrgPos >= 0 && yOrgPos < nH)
					{
						if(CandidateIndex[yPos][xPos] >= MAX_CAND) continue;

						Weight[CandidateIndex[yPos][xPos]][yPos][xPos] = 1.-Dist(xx, yy, 0, 0)/(nGrow*sqrt((double)2)+1);//pow(1.-fabs(xx)/(nGrow+1), 1./3);
						CandidateR[CandidateIndex[yPos][xPos]][yPos][xPos] = PrevR[yOrgPos][xOrgPos];
						CandidateG[CandidateIndex[yPos][xPos]][yPos][xPos] = PrevG[yOrgPos][xOrgPos];
						CandidateB[CandidateIndex[yPos][xPos]][yPos][xPos] = PrevB[yOrgPos][xOrgPos];
						CandidateIndex[yPos][xPos]++;
					}
				}
		}
	}
	
	for(y = 0 ; y < nH ; y++)
		for(x = 0 ; x < nW ; x++)
		{
			if(CandidateIndex[y][x] == 0)
			{
				InterR[y][x] = CurrR[y][x];
				InterG[y][x] = CurrG[y][x];
				InterB[y][x] = CurrB[y][x];
			}
			else
			{

				double SumR = 0, SumG = 0, SumB = 0, SumWeight = 0;
				for(i = 0 ; i < CandidateIndex[y][x] ; i++)
				{
					SumR += CandidateR[i][y][x]*Weight[i][y][x];
					SumG += CandidateG[i][y][x]*Weight[i][y][x];
					SumB += CandidateB[i][y][x]*Weight[i][y][x];

					SumWeight += Weight[i][y][x];
				}

				if(SumWeight == 0)
				{
					//InterR[y][x] = CurrR[y][x];
					//InterG[y][x] = CurrG[y][x];
					//InterB[y][x] = CurrB[y][x];
				}
				else
				{
					InterR[y][x] = (BYTE)(SumR / SumWeight);
					InterG[y][x] = (BYTE)(SumG / SumWeight);
					InterB[y][x] = (BYTE)(SumB / SumWeight);
				}
			}
		}
		
	for(i = 0 ; i < MAX_CAND ; i++)
	{
		free_cmatrix(CandidateR[i], nH, nW);
		free_cmatrix(CandidateG[i], nH, nW);
		free_cmatrix(CandidateB[i], nH, nW);
		free_dmatrix(Weight[i], nH, nW);
	}
	free_imatrix(CandidateIndex, nH, nW);

	free_cmatrix(CurrGray, nH, nW);
	free_cmatrix(PrevGray, nH, nW);
	free_cmatrix(DiffGray, nH, nW);
}

void CFruc::Run(BYTE *Image1D, int nW, int nH, int nInterGap, int nMask, int nSearch, int nGrow, int nMeGap, int nUseColor, int nThreDiff, 
				bool bViewPsnr, int nPsnrType, bool bFirst)
{
	if(nW != m_nW || nH != m_nH)
	{
		Closing();
		Init(nW, nH);
	}

	if(bFirst)
	{
		m_nFrameCnt = 0;

		int k;
		for(k = 0 ; k < 5 ; k++)
		{
			m_Psnr[k] = 0;
			m_TimeDiff[k] = 0;
		}
		m_nCnt = 0;
	}

	MakeImage1DToImage2DColor(Image1D, true, nW, nH, m_CurrR, m_CurrG, m_CurrB);

	if(m_nFrameCnt == 0)
	{
		int y;

		for(y = 0 ; y < m_nH ; y++)
		{
			memcpy(m_PrevR[y], m_CurrR[y], m_nW);
			memcpy(m_PrevG[y], m_CurrG[y], m_nW);
			memcpy(m_PrevB[y], m_CurrB[y], m_nW);
		}
	}
	else if(m_nFrameCnt%nInterGap == 0)
	{
		if(bViewPsnr)
		{
			int y;
			CString Msg;
			double PSNR;

			if(nPsnrType == 0)
				PSNR = GetPsnrRgb(m_RefR, m_RefG, m_RefB, m_PrevR, m_PrevG, m_PrevB, m_nW, m_nH);
			else if(nPsnrType == 1)
				PSNR = GetPsnrYuv(m_RefR, m_RefG, m_RefB, m_PrevR, m_PrevG, m_PrevB, m_nW, m_nH);
			else if(nPsnrType == 2)
				PSNR = GetPsnrY(m_RefR, m_RefG, m_RefB, m_PrevR, m_PrevG, m_PrevB, m_nW, m_nH);

			m_Psnr[0] += PSNR;

			Msg.Format("Prev %5.2lf(%5.2lf)dB", PSNR, m_Psnr[0]/(m_nCnt+1));

			for(y = 0 ; y < m_nH ; y++)
			{
				memcpy(m_InterR[y], m_PrevR[y], m_nW);
				memcpy(m_InterG[y], m_PrevG[y], m_nW);
				memcpy(m_InterB[y], m_PrevB[y], m_nW);
			}

			DrawTextOnImag2DGray(m_InterR, nW, nH, 0, 0, Msg, DT_LEFT, 255, true);
			DrawTextOnImag2DGray(m_InterG, nW, nH, 0, 0, Msg, DT_LEFT, 0, true);
			DrawTextOnImag2DGray(m_InterB, nW, nH, 0, 0, Msg, DT_LEFT, 0, true);

			DisplayCimage2DColor(m_InterR, m_InterG, m_InterB, m_nW, m_nH, 0, m_nH, false, true);

			if(nPsnrType == 0)
				PSNR = GetPsnrRgb(m_RefR, m_RefG, m_RefB, m_CurrR, m_CurrG, m_CurrB, m_nW, m_nH);
			else if(nPsnrType == 1)
				PSNR = GetPsnrYuv(m_RefR, m_RefG, m_RefB, m_CurrR, m_CurrG, m_CurrB, m_nW, m_nH);
			else if(nPsnrType == 2)
				PSNR = GetPsnrY(m_RefR, m_RefG, m_RefB, m_CurrR, m_CurrG, m_CurrB, m_nW, m_nH);

			m_Psnr[1] += PSNR;

			Msg.Format("Curr %5.2lf(%5.2lf)dB", PSNR, m_Psnr[1]/(m_nCnt+1));

			for(y = 0 ; y < m_nH ; y++)
			{
				memcpy(m_InterR[y], m_CurrR[y], m_nW);
				memcpy(m_InterG[y], m_CurrG[y], m_nW);
				memcpy(m_InterB[y], m_CurrB[y], m_nW);
			}

			DrawTextOnImag2DGray(m_InterR, nW, nH, 0, 0, Msg, DT_LEFT, 255, true);
			DrawTextOnImag2DGray(m_InterG, nW, nH, 0, 0, Msg, DT_LEFT, 0, true);
			DrawTextOnImag2DGray(m_InterB, nW, nH, 0, 0, Msg, DT_LEFT, 0, true);

			DisplayCimage2DColor(m_InterR, m_InterG, m_InterB, m_nW, m_nH, m_nW*2, m_nH, false, true);
		}

		DisplayCimage2DColor(m_PrevR, m_PrevG, m_PrevB, m_nW, m_nH, 0, 0, false, true);
		DisplayCimage2DColor(m_RefR, m_RefG, m_RefB, m_nW, m_nH, m_nW, 0, false, true);
		DisplayCimage2DColor(m_CurrR, m_CurrG, m_CurrB, m_nW, m_nH, m_nW*2, 0, false, true);

		int x, y;
		double PSNR, TimeDiff;

		QueryPerformanceFrequency((LARGE_INTEGER*)&m_TimeFreq);
		QueryPerformanceCounter((LARGE_INTEGER*)&m_TimeStart);

		for(y = 0 ; y < m_nH ; y++)
			for(x = 0 ; x < m_nW ; x++)
			{
				m_InterR[y][x] = (m_PrevR[y][x] + m_CurrR[y][x])/2;
				m_InterG[y][x] = (m_PrevG[y][x] + m_CurrG[y][x])/2;
				m_InterB[y][x] = (m_PrevB[y][x] + m_CurrB[y][x])/2;
			}

		QueryPerformanceCounter((LARGE_INTEGER*)&m_TimeEnd);

		if(bViewPsnr)
		{
			CString Msg;

			TimeDiff = (double)(m_TimeEnd - m_TimeStart)/(double)m_TimeFreq*1000; 
			m_TimeDiff[2] += TimeDiff;

			if(nPsnrType == 0)
				PSNR = GetPsnrRgb(m_RefR, m_RefG, m_RefB, m_InterR, m_InterG, m_InterB, m_nW, m_nH);
			else if(nPsnrType == 1)
				PSNR = GetPsnrYuv(m_RefR, m_RefG, m_RefB, m_InterR, m_InterG, m_InterB, m_nW, m_nH);
			else if(nPsnrType == 2)
				PSNR = GetPsnrY(m_RefR, m_RefG, m_RefB, m_InterR, m_InterG, m_InterB, m_nW, m_nH);

			m_Psnr[2] += PSNR;

			Msg.Format("Average %5.2lf(%5.2lf)dB\n  %5.2lf(%5.2lf)ms", PSNR, m_Psnr[2]/(m_nCnt+1), TimeDiff, m_TimeDiff[2]/(m_nCnt+1));
			DrawTextOnImag2DGray(m_InterR, nW, nH, 0, 0, Msg, DT_LEFT, 255, true);
			DrawTextOnImag2DGray(m_InterG, nW, nH, 0, 0, Msg, DT_LEFT, 0, true);
			DrawTextOnImag2DGray(m_InterB, nW, nH, 0, 0, Msg, DT_LEFT, 0, true);
		}
		
		DisplayCimage2DColor(m_InterR, m_InterG, m_InterB, m_nW, m_nH, m_nW, m_nH, false, true);
	
		for(y = 0 ; y < m_nH ; y++)
			for(x = 0 ; x < m_nW ; x++)
			{
				m_InterR[y][x] =0;
				m_InterG[y][x] = 0;
				m_InterB[y][x] = 0;
			}

		QueryPerformanceFrequency((LARGE_INTEGER*)&m_TimeFreq);
		QueryPerformanceCounter((LARGE_INTEGER*)&m_TimeStart);

		if(nUseColor == 0)
		{
			GetInterFrame(m_PrevR, m_CurrR, m_InterR, m_nW, m_nH, nMask, nSearch, nGrow, nMeGap, nThreDiff==0?false:true, nThreDiff);
			GetInterFrame(m_PrevG, m_CurrG, m_InterG, m_nW, m_nH, nMask, nSearch, nGrow, nMeGap, nThreDiff==0?false:true, nThreDiff);
			GetInterFrame(m_PrevB, m_CurrB, m_InterB, m_nW, m_nH, nMask, nSearch, nGrow, nMeGap, nThreDiff==0?false:true, nThreDiff);
		}
		else if(nUseColor == 1)
			GetColorInterFrameByMotionY(m_PrevR, m_PrevG, m_PrevB, 
				m_CurrR, m_CurrG, m_CurrB, m_InterR, m_InterG, m_InterB,
				m_nW, m_nH, nMask, nSearch, nGrow, nMeGap, nThreDiff==0?false:true, nThreDiff);

		QueryPerformanceCounter((LARGE_INTEGER*)&m_TimeEnd);

		if(bViewPsnr)
		{
			CString Msg;

			TimeDiff = (double)(m_TimeEnd - m_TimeStart)/(double)m_TimeFreq*1000; 
			m_TimeDiff[3] += TimeDiff;

			if(nPsnrType == 0)
				PSNR = GetPsnrRgb(m_RefR, m_RefG, m_RefB, m_InterR, m_InterG, m_InterB, m_nW, m_nH);
			else if(nPsnrType == 1)
				PSNR = GetPsnrYuv(m_RefR, m_RefG, m_RefB, m_InterR, m_InterG, m_InterB, m_nW, m_nH);
			else if(nPsnrType == 2)
				PSNR = GetPsnrY(m_RefR, m_RefG, m_RefB, m_InterR, m_InterG, m_InterB, m_nW, m_nH);
			
			m_Psnr[3] += PSNR;

			Msg.Format("Proposed %5.2lf(%5.2lf)dB\n  %5.2lf(%5.2lf)ms", PSNR, m_Psnr[3]/(m_nCnt+1), TimeDiff, m_TimeDiff[3]/(m_nCnt+1));
			DrawTextOnImag2DGray(m_InterR, nW, nH, 0, 0, Msg, DT_LEFT, 255, true);
			DrawTextOnImag2DGray(m_InterG, nW, nH, 0, 0, Msg, DT_LEFT, 0, true);
			DrawTextOnImag2DGray(m_InterB, nW, nH, 0, 0, Msg, DT_LEFT, 0, true);
		}

		DisplayCimage2DColor(m_InterR, m_InterG, m_InterB, m_nW, m_nH, m_nW, m_nH*2, false, true);

		for(y = 0 ; y < m_nH ; y++)
		{
			memcpy(m_PrevR[y], m_CurrR[y], m_nW);
			memcpy(m_PrevG[y], m_CurrG[y], m_nW);
			memcpy(m_PrevB[y], m_CurrB[y], m_nW);
		}

		m_nCnt++;
	}
	else if(m_nFrameCnt%nInterGap == nInterGap/2)
	{
		int y;
		for(y = 0 ; y < m_nH ; y++)
		{
			memcpy(m_RefR[y], m_CurrR[y], m_nW);
			memcpy(m_RefG[y], m_CurrG[y], m_nW);
			memcpy(m_RefB[y], m_CurrB[y], m_nW);
		}
	}

	m_nFrameCnt++;
}
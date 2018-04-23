// SR.cpp: implementation of the CSR class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ImageProcessing.h"
#include "SR.h"

#include "MainFrm.h"		// Ãß°¡

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

CSR::CSR()
{

}

CSR::~CSR()
{

}

static void SobelDbl(BYTE **ImageGray, double **Output, int nW, int nH)
{
	int x, y;
	double nGx, nGy;

	for(y = 0 ; y < nH ; y++)
		for(x = 0 ; x < nW ; x++)
		{
			if(x <= 0 || x >= nW-1 || y <= 0 || y >= nH-1)
				Output[y][x] = 0;
			else
			{
				nGx = ImageGray[y-1][x+1] + 2*ImageGray[y][x+1] + ImageGray[y+1][x+1]
					- ImageGray[y-1][x-1] - 2*ImageGray[y][x-1] - ImageGray[y+1][x-1];

				nGy = ImageGray[y+1][x-1] + 2*ImageGray[y+1][x] + ImageGray[y+1][x+1]
					- ImageGray[y-1][x-1] - 2*ImageGray[y-1][x] - ImageGray[y-1][x+1];
				
				Output[y][x] = (fabs(nGx) + fabs(nGy))/8.;
			}
		}
}


static double EdgePSNR(BYTE **R, BYTE **G, BYTE **B, BYTE **OutR, BYTE **OutG, BYTE **OutB, int nW, int nH)
{
	double **RE1 = dmatrix(nH, nW);
	double **GE1 = dmatrix(nH, nW);
	double **BE1 = dmatrix(nH, nW);

	double **RE2 = dmatrix(nH, nW);
	double **GE2 = dmatrix(nH, nW);
	double **BE2 = dmatrix(nH, nW);

	SobelDbl(R, RE1, nW, nH);
	SobelDbl(G, GE1, nW, nH);
	SobelDbl(B, BE1, nW, nH);

	SobelDbl(OutR, RE2, nW, nH);
	SobelDbl(OutG, GE2, nW, nH);
	SobelDbl(OutB, BE2, nW, nH);

	double Mse = 0, PSNR;
	double Threshold = 260./8.;
	int x, y;
	int nEPs = 0, nEPh = 0 , nEPc = 0;
	for(y = 0 ; y < nH ; y++)
		for(x = 0 ; x < nW ; x++)
		{
			
		/*	Mse += (RE1[y][x] - RE2[y][x])*(RE1[y][x] - RE2[y][x]) +
				(GE1[y][x] - GE2[y][x])*(GE1[y][x] - GE2[y][x]) +
				(BE1[y][x] - BE2[y][x])*(BE1[y][x] - BE2[y][x]);*/

			if(RE1[y][x] >= Threshold || GE1[y][x] >= Threshold || BE1[y][x] >= Threshold) 
			{
				Mse += (R[y][x] - OutR[y][x])*(R[y][x] - OutR[y][x])
					+ (G[y][x] - OutG[y][x])*(G[y][x] - OutG[y][x])
					+ (B[y][x] - OutB[y][x])*(B[y][x] - OutB[y][x]);

				nEPs++;

				if(RE2[y][x] >= Threshold || GE2[y][x] >= Threshold || BE2[y][x] >= Threshold) 
					nEPc++;
			}

			if(RE2[y][x] >= Threshold || GE2[y][x] >= Threshold || BE2[y][x] >= Threshold) 
				nEPh++;
			
		}

	Mse /= nW*nH*3;
	PSNR = 20*log10(255/sqrt(Mse));

	free_dmatrix(RE1, nH, nW);
	free_dmatrix(GE1, nH, nW);
	free_dmatrix(BE1, nH, nW);

	free_dmatrix(RE2, nH, nW);
	free_dmatrix(GE2, nH, nW);
	free_dmatrix(BE2, nH, nW);

	if(PSNR > 40) PSNR *= 0.8;
	else if(PSNR >= 35) PSNR *= 0.9;
	else if(PSNR < 25 && (double)nEPc/nEPs < 0.35 && (double)nEPh/nEPs < 0.13) PSNR = PSNR-60*(0.1225-((double)nEPc/nEPs)*((double)nEPc/nEPs));

	return PSNR;
}


static void Sort(BYTE Data[], int size){
    int i, k;
	BYTE t;
    
    for(i=1 ; i<size ; i++){
        t = Data[i];

        for(k=i-1 ; k>=0 ; k--){
            if(Data[k] > t)
                Data[k+1] = Data[k];
            else
                break;
        }

        Data[k+1] = t;
    }
}

void NewInterpolation(BYTE **ImageGray, int nW, int nH, BYTE **OutputGray, int nOW, int nOH, int nMask, int nSearch, int nGrow)
{
	const int MAX_CAND = 150;
	BYTE **Candidate[MAX_CAND];
	double **Weight[MAX_CAND];
	int **CandidateIndex;
//	BYTE SortTemp[MAX_CAND];

	int x, y;
	int i;

	double RateW = (double)nOW/nW;
	double RateH = (double)nOH/nH;

	for(i = 0 ; i < MAX_CAND ; i++)
	{
		Candidate[i] = cmatrix(nOH, nOW);
		Weight[i] = dmatrix(nOH, nOW);
	}
	CandidateIndex = imatrix(nOH, nOW);

	for(y = 0 ; y < nOH ; y++)
		for(x = 0 ; x < nOW ; x++)
		{
			CandidateIndex[y][x] = 0;
		}

	
	int dx, dy, xx, yy;

	for(y = 0 ; y < nH-1 ; y++)
	{
		for(x = nMask ; x < nW-nMask ; x++)
		{
			int nDiff = 0;
			double MinMad = 255;
			for(xx = -nSearch ; xx <= nSearch ; xx++)
			{
				if(x-nSearch-nMask < 0) continue;
				if(x+nSearch+nMask >= nW) continue;

				double Mad = 0;
				for(dx = -nMask ; dx <= nMask ; dx++)
					Mad += abs(ImageGray[y][x+dx] - ImageGray[y+1][x+dx+xx]);

				Mad /= nMask*2+1;

				if(Mad < MinMad || (Mad == MinMad && xx == 0)) {
					MinMad = Mad;
					nDiff = xx;
				}
			}

			if(MinMad > 50) nDiff = 0;

			for(xx = -nGrow ; xx <= nGrow ; xx++)
			{
				for(yy = (int)(y*RateH) ; yy < (int)((y+1)*RateH) ; yy++)
				{
					double Slope = (yy - (int)(y*RateH))/RateH;
					int xPos = (int)(x*RateW) + (int)(nDiff*Slope*RateW) + xx;
					int xOrgPos = x + (int)(xx/RateW);


					if(xPos >= 0 && xPos < nOW && xOrgPos >= 0 && xOrgPos < nW && yy >= 0 && yy < nOH)
					{
						if(CandidateIndex[yy][xPos] >= MAX_CAND) continue;

						Weight[CandidateIndex[yy][xPos]][yy][xPos] = pow(1.-fabs((double)xx)/(nGrow+1), 1./3);
						Candidate[CandidateIndex[yy][xPos]][yy][xPos] = ImageGray[y][xOrgPos];
						CandidateIndex[yy][xPos]++;
					}
				}
			}
		}
	}
	
	for(y = 1 ; y < nH ; y++)
	{
		for(x = nMask ; x < nW-nMask ; x++)
		{
			int nDiff = 0;
			double MinMad = 255;
			for(xx = -nSearch ; xx <= nSearch ; xx++)
			{
				if(x-nSearch-nMask < 0) continue;
				if(x+nSearch+nMask >= nW) continue;

				double Mad = 0;
				for(dx = -nMask ; dx <= nMask ; dx++)
					Mad += abs(ImageGray[y][x+dx] - ImageGray[y-1][x+dx+xx]);

				Mad /= nMask*2+1;

				if(Mad < MinMad || (Mad == MinMad && xx == 0)) {
					MinMad = Mad;
					nDiff = xx;
				}
			}

			if(MinMad > 50) nDiff = 0;

			for(xx = -nGrow ; xx <= nGrow ; xx++)
			{
				for(yy = (int)(y*RateH) ; yy > (int)((y-1)*RateH) ; yy--)
				{
					double Slope = ((int)(y*RateH) - yy)/RateH;
					int xPos = (int)(x*RateW) + (int)(nDiff*Slope*RateW) + xx;
					int xOrgPos = x + (int)(xx/RateW);


					if(xPos >= 0 && xPos < nOW && xOrgPos >= 0 && xOrgPos < nW && yy >= 0 && yy < nOH)
					{
						if(CandidateIndex[yy][xPos] >= MAX_CAND) continue;

						Weight[CandidateIndex[yy][xPos]][yy][xPos] = pow(1.-fabs((double)xx)/(nGrow+1), 1./3);
						Candidate[CandidateIndex[yy][xPos]][yy][xPos] = ImageGray[y][xOrgPos];
						CandidateIndex[yy][xPos]++;
					}
				}
			}
		}
	}

	for(x = 0 ; x < nW-1 ; x++)
	{
		for(y = nMask ; y < nH-nMask ; y++)
		{
			int nDiff = 0;
			double MinMad = 255;
			for(yy = -nSearch ; yy <= nSearch ; yy++)
			{
				if(y-nSearch-nMask < 0) continue;
				if(y+nSearch+nMask >= nH) continue;

				double Mad = 0;
				for(dy = -nMask ; dy <= nMask ; dy++)
					Mad += abs(ImageGray[y+dy][x] - ImageGray[y+dy+yy][x+1]);

				Mad /= nMask*2+1;

				if(Mad < MinMad || (Mad == MinMad && yy == 0)) {
					MinMad = Mad;
					nDiff = yy;
				}
			}

			if(MinMad > 50) nDiff = 0;

			for(yy = -nGrow ; yy <= nGrow ; yy++)
			{
				for(xx = (int)(x*RateW) ; xx < (int)((x+1)*RateW) ; xx++)
				{
					double Slope = (xx - (int)(x*RateW))/RateW;
					int yPos = (int)(y*RateH) + (int)(nDiff*Slope*RateH) + yy;
					int yOrgPos = y + (int)(yy/RateH);

					if(yPos >= 0 && yPos < nOH && yOrgPos >= 0 && yOrgPos < nH && xx >= 0 && xx < nOW)
					{
						if(CandidateIndex[yPos][xx] >= MAX_CAND) continue;

						Weight[CandidateIndex[yPos][xx]][yPos][xx] = pow(1.-fabs((double)yy)/(nGrow+1), 1./3);
						Candidate[CandidateIndex[yPos][xx]][yPos][xx] = ImageGray[yOrgPos][x];
						CandidateIndex[yPos][xx]++;
					}
				}
			}

		}
	}
	
	for(x = 1 ; x < nW ; x++)
	{
		for(y = nMask ; y < nH-nMask ; y++)
		{
			int nDiff = 0;
			double MinMad = 255;
			for(yy = -nSearch ; yy <= nSearch ; yy++)
			{
				if(y-nSearch-nMask < 0) continue;
				if(y+nSearch+nMask >= nH) continue;

				double Mad = 0;
				for(dy = -nMask ; dy <= nMask ; dy++)
					Mad += abs(ImageGray[y+dy][x] - ImageGray[y+dy+yy][x-1]);

				Mad /= nMask*2+1;

				if(Mad < MinMad || (Mad == MinMad && yy == 0)) {
					MinMad = Mad;
					nDiff = yy;
				}
			}

			if(MinMad > 50) nDiff = 0;

			for(yy = -nGrow ; yy <= nGrow ; yy++)
			{
				for(xx = (int)(x*RateW) ; xx > (int)((x-1)*RateW) ; xx--)
				{
					double Slope = ((int)(x*RateW)-xx)/RateW;
					int yPos = (int)(y*RateH) + (int)(nDiff*Slope*RateH) + yy;
					int yOrgPos = y + (int)(yy/RateH);

					if(yPos >= 0 && yPos < nOH && yOrgPos >= 0 && yOrgPos < nH && xx >= 0 && xx < nOW)
					{
						if(CandidateIndex[yPos][xx] >= MAX_CAND) continue;

						Weight[CandidateIndex[yPos][xx]][yPos][xx] = pow(1.-fabs((double)yy)/(nGrow+1), 1./3);
						Candidate[CandidateIndex[yPos][xx]][yPos][xx] = ImageGray[yOrgPos][x];
						CandidateIndex[yPos][xx]++;
					}
				}
			}

		}
	}
	
	for(y = 0 ; y < nOH ; y++)
		for(x = 0 ; x < nOW ; x++)
		{
			/*if(y%2 == 0 && x%2 == 0)
				OutputGray[y][x] = ImageGray[y/2][x/2];
			else*/ 
			if(CandidateIndex[y][x] == 0)
				OutputGray[y][x] = ImageGray[(int)(y/RateH)][(int)(x/RateW)];
			else
			{

				double Sum = 0, SumWeight = 0;
				for(i = 0 ; i < CandidateIndex[y][x] ; i++)
				{
					Sum += Candidate[i][y][x]*Weight[i][y][x];
					SumWeight += Weight[i][y][x];
//					SortTemp[i] = Candidate[i][y][x];
				}

//				Sort(SortTemp, CandidateIndex[y][x]);

				//OutputGray[y][x] = SortTemp[CandidateIndex[y][x]/2];
				//OutputGray[y][x] = (BYTE)(Sum / CandidateIndex[y][x]);
				if(SumWeight == 0)
				{
					OutputGray[y][x] = ImageGray[(int)(y/RateH)][(int)(x/RateW)];
				}
				else
					OutputGray[y][x] = (BYTE)(Sum / SumWeight);
					//OutputGray[y][x] = SortTemp[CandidateIndex[y][x]/2];
			}
		}
		
	for(i = 0 ; i < MAX_CAND ; i++)
	{
		free_cmatrix(Candidate[i], nOH, nOW);
		free_dmatrix(Weight[i], nOH, nOW);
	}
	free_imatrix(CandidateIndex, nOH, nOW);
}

void NewInterpolation(BYTE **ImageGray, int nW, int nH, BYTE **OutputGray)
{
	const int MAX_CAND = 50;
	BYTE **Candidate[MAX_CAND];
	double **Weight[MAX_CAND];
	int **CandidateIndex;
	BYTE SortTemp[MAX_CAND];

	int x, y;
	int i;

	for(i = 0 ; i < MAX_CAND ; i++)
	{
		Candidate[i] = cmatrix(nH*2, nW*2);
		Weight[i] = dmatrix(nH*2, nW*2);;
	}
	CandidateIndex = imatrix(nH*2, nW*2);

	for(y = 0 ; y < nH*2 ; y++)
		for(x = 0 ; x < nW*2 ; x++)
		{
			OutputGray[y][x] = ImageGray[y/2][x/2];
			CandidateIndex[y][x] = 0;
		}

	
	int dx, dy, xx, yy;
	int nSearch = 3;

	for(y = 0 ; y < nH-1 ; y++)
	{
		for(x = 1 ; x < nW-1 ; x++)
		{
			int nDiff = 0;
			double MinMad = 255;
			for(xx = -nSearch ; xx <= nSearch ; xx++)
			{
				if(x-nSearch-1 < 0) continue;
				if(x+nSearch+1 >= nW) continue;

				double Mad = 0;
				for(dx = -1 ; dx <= 1 ; dx++)
					Mad += abs(ImageGray[y][x+dx] - ImageGray[y+1][x+dx+xx]);

				Mad /= 3;

				if(Mad < MinMad || (Mad == MinMad && xx == 0)) {
					MinMad = Mad;
					nDiff = xx;
				}
			}

			if(MinMad > 50) nDiff = 0;

			for(xx = -nSearch/2 ; xx <= nSearch/2 ; xx++)
			{
				int xPos = x*2+(nDiff*2)/2 + xx;
				int xOrgPos = x + xx/2;

				if(xPos >= 0 && xPos < nW*2 && xOrgPos >= 0 && xOrgPos < nW)
				{
					if(CandidateIndex[y*2+1][xPos] >= MAX_CAND) continue;

					OutputGray[y*2+1][xPos] = ImageGray[y][xOrgPos];
					Weight[CandidateIndex[y*2+1][xPos]][y*2+1][xPos] = 1.5-fabs((double)xx)/(nSearch/2);
					Candidate[CandidateIndex[y*2+1][xPos]++][y*2+1][xPos] = ImageGray[y][xOrgPos];
				}

				xPos = x*2+(0*2)/2 + xx;
				if(xPos >= 0 && xPos < nW*2 && xOrgPos >= 0 && xOrgPos < nW)
				{
					if(CandidateIndex[y*2][xPos] >= MAX_CAND) continue;

					Weight[CandidateIndex[y*2][xPos]][y*2][xPos] = 1.5-fabs((double)xx)/(nSearch/2);
					Candidate[CandidateIndex[y*2][xPos]++][y*2][xPos] = ImageGray[y][xOrgPos];
				}
			}
		}
	}
	
	for(y = 0 ; y < nH*2 ; y++)
		for(x = 0 ; x < nW*2 ; x++)
		{
			OutputGray[y][x] = ImageGray[y/2][x/2];
		}

	for(x = 0 ; x < nW-1 ; x++)
	{
		for(y = 1 ; y < nH-1 ; y++)
		{
			int nDiff = 0;
			double MinMad = 255;
			for(yy = -nSearch ; yy <= nSearch ; yy++)
			{
				if(y-nSearch-1 < 0) continue;
				if(y+nSearch+1 >= nH) continue;

				double Mad = 0;
				for(dy = -1 ; dy <= 1 ; dy++)
					Mad += abs(ImageGray[y+dy][x] - ImageGray[y+dy+yy][x+1]);

				Mad /= 3;

				if(Mad < MinMad || (Mad == MinMad && yy == 0)) {
					MinMad = Mad;
					nDiff = yy;
				}
			}

			if(MinMad > 50) nDiff = 0;

			for(yy = -nSearch/2 ; yy <= nSearch/2 ; yy++)
			{
				int yPos = y*2+(nDiff*2)/2 + yy;
				int yOrgPos = y + yy/2;

				if(yPos >= 0 && yPos < nH*2 && yOrgPos >= 0 && yOrgPos < nH)
				{
					if(CandidateIndex[yPos][x*2+1] >= MAX_CAND) continue;

					OutputGray[yPos][x*2+1] = ImageGray[yOrgPos][x];
					Weight[CandidateIndex[yPos][x*2+1]][yPos][x*2+1] = 1.5-fabs((double)yy)/(nSearch/2);
					Candidate[CandidateIndex[yPos][x*2+1]++][yPos][x*2+1] = ImageGray[yOrgPos][x];
				}

				yPos = y*2+(0*2)/2 + yy;
				if(yPos >= 0 && yPos < nH*2 && yOrgPos >= 0 && yOrgPos < nH)
				{
					if(CandidateIndex[yPos][x*2] >= MAX_CAND) continue;

					Weight[CandidateIndex[yPos][x*2]][yPos][x*2] = 1.5-fabs((double)yy)/(nSearch/2);
					Candidate[CandidateIndex[yPos][x*2]++][yPos][x*2] = ImageGray[yOrgPos][x];
				}
			}

		}
	}
	
	for(y = 0 ; y < nH*2 ; y++)
		for(x = 0 ; x < nW*2 ; x++)
		{
			if(y%2 == 0 && x%2 == 0)
				OutputGray[y][x] = ImageGray[y/2][x/2];
			else if(CandidateIndex[y][x] == 0)
				OutputGray[y][x] = ImageGray[y/2][x/2];
			else
			{

				double Sum = 0, SumWeight = 0;
				for(i = 0 ; i < CandidateIndex[y][x] ; i++)
				{
					Sum += Candidate[i][y][x]*Weight[i][y][x];
					SumWeight += Weight[i][y][x];
					SortTemp[i] = Candidate[i][y][x];
				}

//				Sort(SortTemp, CandidateIndex[y][x]);

				//OutputGray[y][x] = SortTemp[CandidateIndex[y][x]/2];
				//OutputGray[y][x] = (BYTE)(Sum / CandidateIndex[y][x]);
				if(SumWeight == 0)
				{
					OutputGray[y][x] = ImageGray[y/2][x/2];
					//AfxMessageBox("A");
				}
				else
					OutputGray[y][x] = (BYTE)(Sum / SumWeight);
			}
		}
		
	for(i = 0 ; i < MAX_CAND ; i++)
	{
		free_cmatrix(Candidate[i], nH*2, nW*2);
		free_dmatrix(Weight[i], nH*2, nW*2);
	}
	free_imatrix(CandidateIndex, nH*2, nW*2);
}

static double lerp(double v1, double v2, double ratio)
{
   return v1*(1-ratio)+v2*ratio;
}

static void Bilinear(BYTE **ImageGray, int nW, int nH, BYTE **OutputGray, int nOW, int nOH)
{
	int x, y;
	
	for(y = 0 ; y < nOH ; y++)
		for(x = 0 ; x < nOW ; x++)
		{
			double u = x/((double)nOW/nW);
			double v = y/((double)nOH/nH);

			int u0, v0, u1, v1;
			double du, dv;

			u0 = (int)floor(u);
			v0 = (int)floor(v);

			v1 = v0+1;
			u1 = u0+1;

			if(u1 >= nW) u1 = nW-1;
			if(v1 >= nH) v1 = nH-1;
  
			du=u-u0;
			dv=v-v0;

			int r0, r1, r2, r3;

			r0 = ImageGray[v0][u0];
			r1 = ImageGray[v0][u1];
			r2 = ImageGray[v1][u1];
			r3 = ImageGray[v1][u0];

			OutputGray[y][x] = (BYTE)(lerp (lerp (r0, r1, du), lerp (r3, r2, du), dv));
		}
}

double Cubic(double offset, double v0, double v1, double v2, double v3)
{
	
	return   (((( -7 * v0 + 21 * v1 - 21 * v2 + 7 * v3 ) * offset +
		( 15 * v0 - 36 * v1 + 27 * v2 - 6 * v3 ) ) * offset +
		( -9 * v0 + 9 * v2 ) ) * offset + (v0 + 16 * v1 + v2) ) / 18.0;
}
double GetCubicRow(int x, int y, double offset, BYTE **ImageGray, int nW, int nH)
{
	int i0, i1, i2, i3;
	int x1, x2, x3;
	
	if(y < 0) y = 0;
	if(y >= nH) y = nH-1;
	
	if(x < 0) x = 0;
	x1 = x+1;
	if(x1 >= nW) x1 = x;
	x2 = x1+1;
	if(x2 >= nW) x2 = x1;
	x3 = x2+1;
	if(x3 >= nW) x3 = x2;
	
	i0 = ImageGray[y][x];
	i1 = ImageGray[y][x1];
	i2 = ImageGray[y][x2];
	i3 = ImageGray[y][x3];
	
	return Cubic(offset, i0, i1, i2, i3);
}

double GetBicubic(double x, double y, BYTE **ImageGray, int nW, int nH)
{
	int xi,yi;
	double dx,dy;
	double i0, i1, i2, i3;
	
	xi = (int)floor(x);
	yi = (int)floor(y);
	dx=x-xi;
	dy=y-yi;
	
	i0 = GetCubicRow(xi-1, yi-1, dx, ImageGray, nW, nH);
	i1 = GetCubicRow(xi-1, yi,   dx, ImageGray, nW, nH);
	i2 = GetCubicRow(xi-1, yi+1, dx, ImageGray, nW, nH);
	i3 = GetCubicRow(xi-1, yi+2, dx, ImageGray, nW, nH);
	
	return Cubic(dy, i0, i1, i2, i3);
}
void Bicublic(BYTE **ImageGray, int nW, int nH, BYTE **OutputGray, int nOW, int nOH)
{ 
	int x, y;
	for(y = 0 ; y < nOH ; y++)
		for(x = 0 ; x < nOW ; x++)
		{
			double u = x*(double)nW/nOW;
			double v = y*(double)nH/nOH;
			
			double out = GetBicubic(u, v, ImageGray, nW, nH);
			if(out < 0)
				OutputGray[y][x] = 0;
			else if(out > 255)
				OutputGray[y][x] = 255;
			else
				OutputGray[y][x] = (BYTE)out;
		}
}

void CSR::Run(BYTE *Image1D, int nW, int nH, int RefX, int RefY, int nZoom, int nMask, int nSearch, int nGrow, bool bViewPsnr, bool bFirst)
{
	RefX -= nW/2/4/2;
	RefY -= nH/2/4/2;

	int k;

	if(bFirst)
	{
		for(k = 0 ; k < 4 ; k++)
		{
			m_Psnr[k] = 0;
			m_Epsnr[k] = 0;
			m_TimeDiff[k] = 0;
		}
		m_nCnt = 0;
	}

	CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd();
	CString Msg;

	BYTE **R, **G, **B;
	BYTE **OutR, **OutG, **OutB;
	BYTE **OutR2, **OutG2, **OutB2;
	BYTE **CompR, **CompG, **CompB;
	BYTE **R2, **G2, **B2;

	R = cmatrix(nH, nW);
	G = cmatrix(nH, nW);
	B = cmatrix(nH, nW);

	OutR = cmatrix(nH, nW);
	OutG = cmatrix(nH, nW);
	OutB = cmatrix(nH, nW);

	OutR2 = cmatrix(nH, nW);
	OutG2 = cmatrix(nH, nW);
	OutB2 = cmatrix(nH, nW);

	CompR = cmatrix(nH/2, nW/2);
	CompG = cmatrix(nH/2, nW/2);
	CompB = cmatrix(nH/2, nW/2);

	R2 = cmatrix(nH/nZoom, nW/nZoom);
	G2 = cmatrix(nH/nZoom, nW/nZoom);
	B2 = cmatrix(nH/nZoom, nW/nZoom);

	MakeImage1DToImage2DColor(Image1D, true, nW, nH, R, G, B);

	int x, y;
	for(y = 0 ; y < nH/nZoom ; y++)
		for(x = 0 ; x < nW/nZoom ; x++)
		{
			R2[y][x] = (R[y*nZoom][x*nZoom]);//+R[y*2+1][x*2]+R[y*2][x*2+1]+R[y*2+1][x*2+1])/4;
			G2[y][x] = (G[y*nZoom][x*nZoom]);//+G[y*2+1][x*2]+G[y*2][x*2+1]+G[y*2+1][x*2+1])/4;
			B2[y][x] = (B[y*nZoom][x*nZoom]);//+B[y*2+1][x*2]+B[y*2][x*2+1]+B[y*2+1][x*2+1])/4;
		}

		
////////////
	double TimeDiff[4];
	double Mse[4], PSNR[4], EPSNR[4];

	FILE *fp;
	fp = fopen("PSNR.dat", "a");
	for(k = 0 ; k < 4 ; k++)
	{
		QueryPerformanceFrequency((LARGE_INTEGER*)&m_TimeFreq);
		QueryPerformanceCounter((LARGE_INTEGER*)&m_TimeStart);

		if(k == 0)
		{
			for(y = 0 ; y < nH ; y++)
				for(x = 0 ; x < nW ; x++)
				{
					if(x/nZoom < nW/nZoom && y/nZoom < nH/nZoom)
					{
						OutR[y][x] = R2[y/nZoom][x/nZoom];
						OutG[y][x] = G2[y/nZoom][x/nZoom];
						OutB[y][x] = B2[y/nZoom][x/nZoom];
					}
					else
					{
						OutR[y][x] = 0;
						OutG[y][x] = 0;
						OutB[y][x] = 0;
					}
				}
		}
		else if(k == 1)
		{
			Bilinear(R2, nW/nZoom, nH/nZoom, OutR, nW, nH);
			Bilinear(G2, nW/nZoom, nH/nZoom, OutG, nW, nH);
			Bilinear(B2, nW/nZoom, nH/nZoom, OutB, nW, nH);

		}
		else if(k == 2)
		{
			Bicublic(R2, nW/nZoom, nH/nZoom, OutR, nW, nH);
			Bicublic(G2, nW/nZoom, nH/nZoom, OutG, nW, nH);
			Bicublic(B2, nW/nZoom, nH/nZoom, OutB, nW, nH);
		}
		else if(k == 3)
		{
			NewInterpolation(R2, nW/nZoom, nH/nZoom, OutR, nW, nH, nMask, nSearch, nGrow);
			NewInterpolation(G2, nW/nZoom, nH/nZoom, OutG, nW, nH, nMask, nSearch, nGrow);
			NewInterpolation(B2, nW/nZoom, nH/nZoom, OutB, nW, nH, nMask, nSearch, nGrow);
		}


		QueryPerformanceCounter((LARGE_INTEGER*)&m_TimeEnd);
		TimeDiff[k] = (double)(m_TimeEnd - m_TimeStart)/(double)m_TimeFreq*1000; 
		m_TimeDiff[k] += TimeDiff[k];

		if(k == 3)
		{
			Bilinear(R2, nW/nZoom, nH/nZoom, OutR2, nW, nH);
			Bilinear(G2, nW/nZoom, nH/nZoom, OutG2, nW, nH);
			Bilinear(B2, nW/nZoom, nH/nZoom, OutB2, nW, nH);

			for(y = 0 ; y < nH ; y++)
				for(x = 0 ; x < nW ; x++)
				{
					if(abs(OutR[y][x]-R[y][x]) >= abs(OutR2[y][x]-R[y][x]))
						OutR2[y][x] = 255;
					else
						OutR2[y][x] = 0;

					if(abs(OutG[y][x]-G[y][x]) >= abs(OutG2[y][x]-G[y][x]))
						OutG2[y][x] = 255;
					else
						OutG2[y][x] = 0;

					if(abs(OutB[y][x]-B[y][x]) >= abs(OutB2[y][x]-B[y][x]))
						OutB2[y][x] = 255;
					else
						OutB2[y][x] = 0;

				}

			DisplayCimage2DColor(OutR2, OutG2, OutB2, nW, nH, nW*3, nH, false, true);
		}

		for(y = 0 ; y < nH/2 ; y++)
			for(x = 0 ; x < nW/2 ; x++)
			{
				int CompX = RefX+x/4;
				int CompY = RefY+y/4;

				if(CompX >= 0 && CompX < nW && CompY >= 0 && CompY < nH)
				{
					CompR[y][x] = OutR[CompY][CompX];
					CompG[y][x] = OutG[CompY][CompX];
					CompB[y][x] = OutB[CompY][CompX];
				}
				else
				{
					CompR[y][x] = 0;
					CompG[y][x] = 0;
					CompB[y][x] = 0;
				}
			}

		if(bViewPsnr)
		{
			Mse[k] = 0;
			for(y = 0 ; y < nH ; y++)
				for(x = 0 ; x < nW ; x++)
				{	
					Mse[k] += (R[y][x] - OutR[y][x])*(R[y][x] - OutR[y][x]) +
						(G[y][x] - OutG[y][x])*(G[y][x] - OutG[y][x]) +
						(B[y][x] - OutB[y][x])*(B[y][x] - OutB[y][x]);
				}


			Mse[k] /= nW*nH*3;
			PSNR[k] = 20*log10(255/sqrt(Mse[k]));
			m_Psnr[k] += PSNR[k];

			EPSNR[k] = EdgePSNR(R, G, B, OutR, OutG, OutB, nW, nH);
			m_Epsnr[k] += EPSNR[k];

			if(k == 0)
				Msg.Format("NN %5.2lf(%5.2lf) %5.2lf(%5.2lf)dB   \n  %5.2lf(%5.2lf)ms", PSNR[k], m_Psnr[k]/(m_nCnt+1), EPSNR[k], m_Epsnr[k]/(m_nCnt+1), TimeDiff[k], m_TimeDiff[k]/(m_nCnt+1));
			else if(k == 1)
				Msg.Format("Bilinear %5.2lf(%5.2lf) %5.2lf(%5.2lf)dB   \n  %5.2lf(%5.2lf)ms", PSNR[k], m_Psnr[k]/(m_nCnt+1), EPSNR[k], m_Epsnr[k]/(m_nCnt+1), TimeDiff[k], m_TimeDiff[k]/(m_nCnt+1));
			else if(k == 2)
				Msg.Format("Bicubic %5.2lf(%5.2lf) %5.2lf(%5.2lf)dB   \n  %5.2lf(%5.2lf)ms", PSNR[k], m_Psnr[k]/(m_nCnt+1), EPSNR[k], m_Epsnr[k]/(m_nCnt+1), TimeDiff[k], m_TimeDiff[k]/(m_nCnt+1));
			else if(k == 3)
				Msg.Format("Proposed %5.2lf(%5.2lf) %5.2lf(%5.2lf)dB   \n  %5.2lf(%5.2lf)ms", PSNR[k], m_Psnr[k]/(m_nCnt+1), EPSNR[k], m_Epsnr[k]/(m_nCnt+1), TimeDiff[k], m_TimeDiff[k]/(m_nCnt+1));

			DrawTextOnImag2DGray(OutR, nW, nH, 0, 0, Msg, DT_LEFT, 255, true);
			DrawTextOnImag2DGray(OutG, nW, nH, 0, 0, Msg, DT_LEFT, 0, true);
			DrawTextOnImag2DGray(OutB, nW, nH, 0, 0, Msg, DT_LEFT, 0, true);

			if(fp) fprintf(fp, "%lf, %lf, ", PSNR[k], EPSNR[k]);
		}

		if(k == 0)
		{
			DisplayCimage2DColor(OutR, OutG, OutB, nW, nH, nW, 0, false, true);
			DisplayCimage2DColor(CompR, CompG, CompB, nW/2, nH/2, 0, nH*2, false, true);
		}
		else if(k == 1) 
		{
			DisplayCimage2DColor(OutR, OutG, OutB, nW, nH, nW*2, 0, false, true);
			DisplayCimage2DColor(CompR, CompG, CompB, nW/2, nH/2, nW/2, nH*2, false, true);
		}
		else if(k == 2) 
		{
			DisplayCimage2DColor(OutR, OutG, OutB, nW, nH, nW, nH, false, true);
			DisplayCimage2DColor(CompR, CompG, CompB, nW/2, nH/2, 0, nH*2+nH/2, false, true);
		}
		else if(k == 3) 
		{
			DisplayCimage2DColor(OutR, OutG, OutB, nW, nH, nW*2, nH, false, true);
			DisplayCimage2DColor(CompR, CompG, CompB, nW/2, nH/2, nW/2, nH*2+nH/2, false, true);
		}
	}

	if(fp) {
		fprintf(fp, "\n");
		fclose(fp);
	}
///////////
	for(y = 0 ; y < nH/2 ; y++)
		for(x = 0 ; x < nW/2 ; x++)
		{
			int CompX = RefX+x/4;
			int CompY = RefY+y/4;

			if(CompX >= 0 && CompX < nW && CompY >= 0 && CompY < nH)
			{
				CompR[y][x] = R[CompY][CompX];
				CompG[y][x] = G[CompY][CompX];
				CompB[y][x] = B[CompY][CompX];
			}
			else
			{
				CompR[y][x] = 0;
				CompG[y][x] = 0;
				CompB[y][x] = 0;
			}
		}
	DisplayCimage2DColor(CompR, CompG, CompB, nW/2, nH/2, nW, nH*2, false, true);

	DisplayCimage1D(Image1D, nW, nH, 0, nH, false, true);

	DrawLine1D(Image1D, nW, nH, RefX, RefY, RefX+nW/2/4, RefY, 255, 0, 0);
	DrawLine1D(Image1D, nW, nH, RefX+nW/2/4, RefY, RefX+nW/2/4, RefY+nH/2/4, 255, 0, 0);
	DrawLine1D(Image1D, nW, nH, RefX+nW/2/4, RefY+nH/2/4, RefX, RefY+nH/2/4, 255, 0, 0);
	DrawLine1D(Image1D, nW, nH, RefX, RefY+nH/2/4, RefX, RefY, 255, 0, 0);

	DisplayCimage1D(Image1D, nW, nH, 0, 0, false, true);

	DisplayCimage2DColor(R2, G2, B2, nW/nZoom, nH/nZoom, nW*2, nH*2, false, true);

	if(bViewPsnr)
	{
		for(k = 0 ; k < 4 ; k++)
		{
			pMain->DlgPrintf("Method(%d): %5.2f(%5.2f) %5.2f(%5.2f)dB %5.2f(%5.2f)ms", k, PSNR[k], m_Psnr[k]/(m_nCnt+1), EPSNR[k], m_Epsnr[k]/(m_nCnt+1), TimeDiff[k], m_TimeDiff[k]/(m_nCnt+1));
		}
	}
	
	free_cmatrix(R, nH, nW);
	free_cmatrix(G, nH, nW);
	free_cmatrix(B, nH, nW);

	free_cmatrix(OutR, nH, nW);
	free_cmatrix(OutG, nH, nW);
	free_cmatrix(OutB, nH, nW);

	free_cmatrix(OutR2, nH, nW);
	free_cmatrix(OutG2, nH, nW);
	free_cmatrix(OutB2, nH, nW);

	free_cmatrix(CompR, nH/2, nW/2);
	free_cmatrix(CompG, nH/2, nW/2);
	free_cmatrix(CompB, nH/2, nW/2);

	free_cmatrix(R2, nH/nZoom, nW/nZoom);
	free_cmatrix(G2, nH/nZoom, nW/nZoom);
	free_cmatrix(B2, nH/nZoom, nW/nZoom);

	m_nCnt++;
}
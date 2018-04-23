// Stitch.cpp: implementation of the CStitch class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ImageProcessing.h"
#include "Stitch.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#include "Image.h"
#include <cmath>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CStitch::CStitch()
{
	m_nImageIndex = -1;
}

CStitch::~CStitch()
{

}

void CStitch::Closing()
{
	int i;
	for(i = 0 ; i <= m_nImageIndex ; i++)
	{
		free_cmatrix(m_Image[i].Red, m_Image[i].nH, m_Image[i].nW);
		free_cmatrix(m_Image[i].Green, m_Image[i].nH, m_Image[i].nW);
		free_cmatrix(m_Image[i].Blue, m_Image[i].nH, m_Image[i].nW);
		free_cmatrix(m_Image[i].Gray, m_Image[i].nH, m_Image[i].nW);
		free_cmatrix(m_Image[i].Temp, m_Image[i].nH, m_Image[i].nW);

		free_imatrix(m_Image[i].Label, m_Image[i].nH, m_Image[i].nW);

		if(m_Image[i].nLabelCnt > 0)
		{
			delete [] m_Image[i].m2;
			delete [] m_Image[i].m3;

			delete [] m_Image[i].LabelBox;
		}
	}

	m_nImageIndex = -1;
}

static void LabelMoment(int **Label, int LabelCnt, int nW, int nH, double *m2, double *m3)
{
	double *mu00, *mu20, *mu02, *mu11, *mu30, *mu03, *mu21, *mu12, r;
	CPoint *Center;
	int *Cnt;
	int x, y, i;

	Center = new CPoint[LabelCnt];
	Cnt = new int[LabelCnt];

	mu00 = new double[LabelCnt];
	mu20 = new double[LabelCnt];
	mu02 = new double[LabelCnt];
	mu11 = new double[LabelCnt];
	mu30 = new double[LabelCnt];
	mu03 = new double[LabelCnt];
	mu12 = new double[LabelCnt];
	mu21 = new double[LabelCnt];

	for(i = 0 ; i < LabelCnt ; i++)
	{
		Cnt[i] = 0;
		Center[i].x = 0;
		Center[i].y = 0;

		mu00[i] = 0;
		mu20[i] = 0;
		mu02[i] = 0;
		mu11[i] = 0;
		mu30[i] = 0;
		mu03[i] = 0;
		mu12[i] = 0;
		mu21[i] = 0;
	}

	for(y = 0 ; y < nH ; y++)
		for(x = 0 ; x < nW ; x++)
		{
			i = Label[y][x];

			if(i < 0) continue;

			Cnt[i]++;
			Center[i].x += x;
			Center[i].y += y;
		}

	for(i = 0 ; i < LabelCnt ; i++)
	{
		Center[i].x /= Cnt[i];
		Center[i].y /= Cnt[i];
	}

	for(y = 0 ; y < nH ; y++)
		for(x = 0 ; x < nW ; x++)
		{
			i = Label[y][x];

			if(i < 0) continue;
			
			mu00[i] += pow((double)(x-Center[i].x), 0)*pow((double)(y-Center[i].y), 0);
			mu20[i] += pow((double)(x-Center[i].x), 2)*pow((double)(y-Center[i].y), 0);
			mu02[i] += pow((double)(x-Center[i].x), 0)*pow((double)(y-Center[i].y), 2);
			mu11[i] += pow((double)(x-Center[i].x), 1)*pow((double)(y-Center[i].y), 1);
			mu30[i] += pow((double)(x-Center[i].x), 3)*pow((double)(y-Center[i].y), 0);
			mu03[i] += pow((double)(x-Center[i].x), 0)*pow((double)(y-Center[i].y), 3);
			mu21[i] += pow((double)(x-Center[i].x), 2)*pow((double)(y-Center[i].y), 1);
			mu12[i] += pow((double)(x-Center[i].x), 1)*pow((double)(y-Center[i].y), 2);
		}

	for(i = 0 ; i < LabelCnt ; i++)
	{
		mu20[i] /= Cnt[i];
		mu02[i] /= Cnt[i];
		mu11[i] /= Cnt[i];
		mu30[i] /= Cnt[i];
		mu03[i] /= Cnt[i];
		mu12[i] /= Cnt[i];
		mu21[i] /= Cnt[i];

		r = pow(mu20[i]+mu02[i], 0.5);
		m2[i] = (pow(mu20[i]-mu02[i], 2) + 4*mu11[i]*mu11[i])/pow(r, 4);
		m3[i] = (pow(mu30[i]-3*mu12[i], 2) + pow(3*mu21[i]-mu03[i], 2))/pow(r, 6);
	}

	delete [] Center;
	delete [] Cnt;

	delete [] mu00;
	delete [] mu20;
	delete [] mu02;
	delete [] mu11;
	delete [] mu30;
	delete [] mu03;
	delete [] mu21;
	delete [] mu12;
}

static void GetAffineParameters(CPoint *p1, CPoint *p2, int nCnt,
	double *pA1, double *pA2, double *pA0, double *pB1, double *pB2, double *pB0)
{
//	*pA1 = *pA2 = *pA0 = *pB1 = *pB2 = *pB0 = 0;

	double **T = dmatrix(3, 3);
	double **IT = dmatrix(3, 3);
	double **V = dmatrix(3, 3);

	int i1, i2, i3;
	int nCase = 0;

	for(i1 = 0 ; i1 < nCnt ; i1++)
		for(i2 = i1+1 ; i2 < nCnt ; i2++)
			for(i3 = i2+1 ; i3 < nCnt ; i3++)
			  nCase++;

	double *a1 = new double[nCase];
	double *a2 = new double[nCase];
	double *a0 = new double[nCase];

	double *b1 = new double[nCase];
	double *b2 = new double[nCase];
	double *b0 = new double[nCase];

	nCase = 0;
	for(i1 = 0 ; i1 < nCnt ; i1++)
		for(i2 = i1+1 ; i2 < nCnt ; i2++)
			for(i3 = i2+1 ; i3 < nCnt ; i3++)
			{
	T[0][0] = p1[i1].x;
	T[0][1] = p1[i1].y;
	T[0][2] = 1;

	T[1][0] = p1[i2].x;
	T[1][1] = p1[i2].y;
	T[1][2] = 1;

	T[2][0] = p1[i3].x;
	T[2][1] = p1[i3].y;
	T[2][2] = 1;

	V[0][0] = p2[i1].x;
	V[0][1] = p2[i1].y;
	V[0][2] = 1;

	V[1][0] = p2[i2].x;
	V[1][1] = p2[i2].y;
	V[1][2] = 1;

	V[2][0] = p2[i3].x;
	V[2][1] = p2[i3].y;
	V[2][2] = 1;

	double det;
	InverseMatrix(T, IT, 3, &det);


	a1[nCase] = IT[0][0] * V[0][0] + IT[0][1] * V[1][0] + IT[0][2] * V[2][0];
	a2[nCase] = IT[1][0] * V[0][0] + IT[1][1] * V[1][0] + IT[1][2] * V[2][0];
	a0[nCase] = IT[2][0] * V[0][0] + IT[2][1] * V[1][0] + IT[2][2] * V[2][0];

	b1[nCase] = IT[0][0] * V[0][1] + IT[0][1] * V[1][1] + IT[0][2] * V[2][1];
	b2[nCase] = IT[1][0] * V[0][1] + IT[1][1] * V[1][1] + IT[1][2] * V[2][1];
	b0[nCase] = IT[2][0] * V[0][1] + IT[2][1] * V[1][1] + IT[2][2] * V[2][1];

	nCase++;
			}

	*pA1 = a1[0];
	*pA2 = a2[0];
	*pA0 = a0[0];

	*pB1 = b1[0];
	*pB2 = b2[0];
	*pB0 = b0[0];

	free_dmatrix(T, 3, 3);
	free_dmatrix(IT, 3, 3);
	free_dmatrix(V, 3, 3);
}


void CStitch::Run(BYTE *Image1D, int nW, int nH, bool bFirstFrame)
{
	if(bFirstFrame)
	{
		Closing();
		m_nImageIndex = 0;
	}
	else
		m_nImageIndex++;

	m_Image[m_nImageIndex].nW = nW;
	m_Image[m_nImageIndex].nH = nH;
	m_Image[m_nImageIndex].Red = cmatrix(nH, nW);
	m_Image[m_nImageIndex].Green = cmatrix(nH, nW);
	m_Image[m_nImageIndex].Blue = cmatrix(nH, nW);
	m_Image[m_nImageIndex].Gray = cmatrix(nH, nW);
	m_Image[m_nImageIndex].Temp = cmatrix(nH, nW);

	m_Image[m_nImageIndex].Label = imatrix(nH, nW);

	MakeImage1DToImage2DColor(Image1D, true, nW, nH, m_Image[m_nImageIndex].Red, m_Image[m_nImageIndex].Green, m_Image[m_nImageIndex].Blue);

	int x, y;
	for(y = 0 ; y < nH ; y++)
		for(x = 0 ; x < nW ; x++)
			m_Image[m_nImageIndex].Gray[y][x] = (m_Image[m_nImageIndex].Red[y][x] + m_Image[m_nImageIndex].Green[y][x] + m_Image[m_nImageIndex].Blue[y][x])/3;


	Mean2DGrayFast(m_Image[m_nImageIndex].Gray, nW, nH, 3, m_Image[m_nImageIndex].Temp);
	DcNotch2DGrayFast(m_Image[m_nImageIndex].Temp, m_Image[m_nImageIndex].Gray, nW, nH, 35);

	for(y = 0 ; y < nH ; y++)
		for(x = 0 ; x < nW ; x++)
		{
			if(x < 35 || x >= nW-35 || y < 35 || y >= nH-35)
				m_Image[m_nImageIndex].Gray[y][x] = 0;
			else
				m_Image[m_nImageIndex].Gray[y][x] = m_Image[m_nImageIndex].Gray[y][x] > 128+25 ? 255 : 0;
		}
	

	DisplayCimage2D(m_Image[m_nImageIndex].Gray, nW, nH, 0, nH, false, true);

	m_Image[m_nImageIndex].nLabelCnt = Labeling(m_Image[m_nImageIndex].Gray, m_Image[m_nImageIndex].Label, nW, nH, 25);
	int nLabelCnt = m_Image[m_nImageIndex].nLabelCnt;

	if(nLabelCnt <= 0)
		return;

	m_Image[m_nImageIndex].m2 = new double[nLabelCnt];
	m_Image[m_nImageIndex].m3 = new double[nLabelCnt];
	m_Image[m_nImageIndex].LabelBox = new CRect[nLabelCnt];

	SetLabelBoundBox(m_Image[m_nImageIndex].Label, nW, nH, m_Image[m_nImageIndex].LabelBox, nLabelCnt);

	LabelMoment(m_Image[m_nImageIndex].Label, nLabelCnt, nW, nH, m_Image[m_nImageIndex].m2, m_Image[m_nImageIndex].m3);

	if(!bFirstFrame)
	{
		BYTE **MatchLine;
		int nMH = nH > m_Image[m_nImageIndex-1].nH ? nH : m_Image[m_nImageIndex-1].nH;
		int nMW = nW+m_Image[m_nImageIndex-1].nW;
		MatchLine = cmatrix(nMH, nMW);

		for(y = 0 ; y < nMH ; y++)
			for(x = 0 ; x < nMW ; x++)
				MatchLine[y][x] = 0;

		for(y = 0 ; y < m_Image[m_nImageIndex-1].nH ; y++)
			for(x = 0 ; x < m_Image[m_nImageIndex-1].nW ; x++)
			{
				MatchLine[y][x] = m_Image[m_nImageIndex-1].Gray[y][x];
			}

		for(y = 0 ; y < nH ; y++)
			for(x = 0 ; x < nW ; x++)
			{
				MatchLine[y][x+m_Image[m_nImageIndex-1].nW] = m_Image[m_nImageIndex].Gray[y][x];
			}


		int i, k;

		int nLabelCnt0 = m_Image[m_nImageIndex-1].nLabelCnt;

		CPoint *p1 = new CPoint[nLabelCnt];
		CPoint *p2 = new CPoint[nLabelCnt];
		int SimCnt = 0;
		
		for(i = 0 ; i < nLabelCnt ; i++)
		{
			double MinSim;
			int MatchIndex;
			for(k = 0 ; k < nLabelCnt0 ; k++)
			{
				if(k == 0) {
					MinSim = fabs(m_Image[m_nImageIndex].m2[i] - m_Image[m_nImageIndex-1].m2[k]) 
						+ fabs(m_Image[m_nImageIndex].m3[i] - m_Image[m_nImageIndex-1].m3[k]);
					MinSim /= 2;
					MatchIndex = 0;
				}
				else
				{
					double Sim = fabs(m_Image[m_nImageIndex].m2[i] - m_Image[m_nImageIndex-1].m2[k]) 
						+ fabs(m_Image[m_nImageIndex].m3[i] - m_Image[m_nImageIndex-1].m3[k]);
					Sim /= 2;
					if(Sim < MinSim) {
						MinSim = Sim;
						MatchIndex = k;
					}
				}
			}

			if(MinSim < 0.0075)
			{
				DrawLine(MatchLine, nMW, nMH, 
					m_Image[m_nImageIndex].LabelBox[i].CenterPoint().x+m_Image[m_nImageIndex-1].nW, m_Image[m_nImageIndex].LabelBox[i].CenterPoint().y, 
					m_Image[m_nImageIndex-1].LabelBox[MatchIndex].CenterPoint().x, m_Image[m_nImageIndex-1].LabelBox[MatchIndex].CenterPoint().y, 
					128);

				p1[SimCnt] = m_Image[m_nImageIndex].LabelBox[i].CenterPoint();
				p2[SimCnt] = m_Image[m_nImageIndex-1].LabelBox[MatchIndex].CenterPoint();
				SimCnt++;
			}
		}

		double a1, a2, a0, b1, b2, b0;
		GetAffineParameters(p1, p2, SimCnt,
			&a1, &a2, &a0, &b1, &b2, &b0);

		delete [] p1;
		delete [] p2;

		DisplayCimage2D(MatchLine, nMW, nMH, 0, 0, false, true);

		Affine(m_Image[m_nImageIndex].Gray, nW, nH, m_Image[m_nImageIndex].Temp, a1, a2, a0, b1, b2, b0);
		DisplayCimage2D(m_Image[m_nImageIndex].Temp, nW, nH, nW, nH, false, true);

		free_cmatrix(MatchLine, nMH, nMW);

	}
}


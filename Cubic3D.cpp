// Cubic3D.cpp: implementation of the CCubic3D class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ImageProcessing.h"
#include "Cubic3D.h"

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

CCubic3D::CCubic3D()
{
	m_Red = m_Green = m_Blue = NULL;
}

CCubic3D::~CCubic3D()
{
	if(m_Red)
		free_cmatrix(m_Red, m_nH, m_nW);

	if(m_Green)
		free_cmatrix(m_Green, m_nH, m_nW);

	if(m_Blue)
		free_cmatrix(m_Blue, m_nH, m_nW);
}

void CCubic3D::Set(int nW, int nH, double MinX, double MaxX, double MinY, double MaxY, double MinZ, double MaxZ)
{
	m_nW = nW;
	m_nH = nH;

	m_Red = cmatrix(m_nH, m_nW);
	m_Green = cmatrix(m_nH, m_nW);
	m_Blue = cmatrix(m_nH, m_nW);

	if(m_nW > m_nH) {
		m_nStartY = 0 ; 
		m_nEndY = m_nH-1;

		m_nStartX = (m_nW-m_nH)/2;
		m_nEndX = m_nStartX + m_nEndY;

		m_nLengthZ = m_nH/4;
	}
	else
	{
		m_nStartX = 0 ; 
		m_nEndX = m_nW-1;

		m_nStartY = (m_nH-m_nW)/2;
		m_nEndY = m_nStartY + m_nEndX;

		m_nLengthZ = m_nW/4;
	}

	m_nFrontLeft = m_nStartX;
	m_nFrontTop = m_nStartY+m_nLengthZ;
	m_nFrontRight = m_nEndX-m_nLengthZ;
	m_nFrontBottom = m_nEndY;

	m_nRearLeft = m_nStartX+m_nLengthZ;
	m_nRearTop = m_nStartY;//+m_nLengthZ;
	m_nRearRight = m_nEndX;//-m_nLengthZ;
	m_nRearBottom = m_nEndY-m_nLengthZ;

	m_MinX = MinX;
	m_MaxX = MaxX;
	m_MinY = MinY;
	m_MaxY = MaxY;
	m_MinZ = MinZ;
	m_MaxZ = MaxZ;

	m_CenterX = (m_MaxX+m_MinX)/2;
	m_CenterY = (m_MaxY+m_MinY)/2;
	m_CenterZ = (m_MaxZ+m_MinZ)/2;

	m_RangeX = m_MaxX - m_MinX;
	m_RangeY = m_MaxY - m_MinY;
	m_RangeZ = m_MaxZ - m_MinZ;

	if(m_RangeX > m_RangeY && m_RangeX > m_RangeZ)
		m_RangeMax = m_RangeX;
	else if(m_RangeY > m_RangeZ && m_RangeY > m_RangeX)
		m_RangeMax = m_RangeY;
	else
		m_RangeMax = m_RangeZ;

	m_RangeRate = m_RangeMax/(m_nLengthZ*3.);
}

void CCubic3D::Reset()
{
	if(m_Red)
	{
		free_cmatrix(m_Red, m_nH, m_nW);
		m_Red = NULL;
	}

	if(m_Green)
	{
		free_cmatrix(m_Green, m_nH, m_nW);
		m_Green = NULL;
	}

	if(m_Blue)
	{
		free_cmatrix(m_Blue, m_nH, m_nW);
		m_Blue = NULL;
	}
}

void CCubic3D::ClearImage(BYTE R, BYTE G, BYTE B)
{
	int x, y;

	for(y = 0 ; y < m_nH ; y++)
		for(x = 0 ; x < m_nW ; x++)
		{
			m_Red[y][x] = R;
			m_Green[y][x] = G;
			m_Blue[y][x] = B;
		}
}

void CCubic3D::DrawCubic(BYTE R, BYTE G, BYTE B)
{
	::DrawLine(m_Red, m_nW, m_nH, m_nFrontLeft, m_nFrontTop, m_nFrontRight, m_nFrontTop, R);
	::DrawLine(m_Green, m_nW, m_nH, m_nFrontLeft, m_nFrontTop, m_nFrontRight, m_nFrontTop, G);
	::DrawLine(m_Blue, m_nW, m_nH, m_nFrontLeft, m_nFrontTop, m_nFrontRight, m_nFrontTop, B);

	::DrawLine(m_Red, m_nW, m_nH, m_nFrontRight, m_nFrontTop, m_nFrontRight, m_nFrontBottom, R);
	::DrawLine(m_Green, m_nW, m_nH, m_nFrontRight, m_nFrontTop, m_nFrontRight, m_nFrontBottom, G);
	::DrawLine(m_Blue, m_nW, m_nH, m_nFrontRight, m_nFrontTop, m_nFrontRight, m_nFrontBottom, B);

	::DrawLine(m_Red, m_nW, m_nH, m_nFrontRight, m_nFrontBottom, m_nFrontLeft, m_nFrontBottom, R);
	::DrawLine(m_Green, m_nW, m_nH, m_nFrontRight, m_nFrontBottom, m_nFrontLeft, m_nFrontBottom, G);
	::DrawLine(m_Blue, m_nW, m_nH, m_nFrontRight, m_nFrontBottom, m_nFrontLeft, m_nFrontBottom, B);

	::DrawLine(m_Red, m_nW, m_nH, m_nFrontLeft, m_nFrontBottom, m_nFrontLeft, m_nFrontTop, R);
	::DrawLine(m_Green, m_nW, m_nH, m_nFrontLeft, m_nFrontBottom, m_nFrontLeft, m_nFrontTop, G);
	::DrawLine(m_Blue, m_nW, m_nH, m_nFrontLeft, m_nFrontBottom, m_nFrontLeft, m_nFrontTop, B);

	
	
	::DrawLine(m_Red, m_nW, m_nH, m_nRearLeft, m_nRearTop, m_nRearRight, m_nRearTop, R);
	::DrawLine(m_Green, m_nW, m_nH, m_nRearLeft, m_nRearTop, m_nRearRight, m_nRearTop, G);
	::DrawLine(m_Blue, m_nW, m_nH, m_nRearLeft, m_nRearTop, m_nRearRight, m_nRearTop, B);

	::DrawLine(m_Red, m_nW, m_nH, m_nRearRight, m_nRearTop, m_nRearRight, m_nRearBottom, R);
	::DrawLine(m_Green, m_nW, m_nH, m_nRearRight, m_nRearTop, m_nRearRight, m_nRearBottom, G);
	::DrawLine(m_Blue, m_nW, m_nH, m_nRearRight, m_nRearTop, m_nRearRight, m_nRearBottom, B);

	::DrawLine(m_Red, m_nW, m_nH, m_nRearRight, m_nRearBottom, m_nRearLeft, m_nRearBottom, R);
	::DrawLine(m_Green, m_nW, m_nH, m_nRearRight, m_nRearBottom, m_nRearLeft, m_nRearBottom, G);
	::DrawLine(m_Blue, m_nW, m_nH, m_nRearRight, m_nRearBottom, m_nRearLeft, m_nRearBottom, B);

	::DrawLine(m_Red, m_nW, m_nH, m_nRearLeft, m_nRearBottom, m_nRearLeft, m_nRearTop, R);
	::DrawLine(m_Green, m_nW, m_nH, m_nRearLeft, m_nRearBottom, m_nRearLeft, m_nRearTop, G);
	::DrawLine(m_Blue, m_nW, m_nH, m_nRearLeft, m_nRearBottom, m_nRearLeft, m_nRearTop, B);



	::DrawLine(m_Red, m_nW, m_nH, m_nFrontLeft, m_nFrontTop, m_nRearLeft, m_nRearTop, R);
	::DrawLine(m_Green, m_nW, m_nH, m_nFrontLeft, m_nFrontTop, m_nRearLeft, m_nRearTop, G);
	::DrawLine(m_Blue, m_nW, m_nH, m_nFrontLeft, m_nFrontTop, m_nRearLeft, m_nRearTop, B);

	::DrawLine(m_Red, m_nW, m_nH, m_nFrontRight, m_nFrontTop, m_nRearRight, m_nRearTop, R);
	::DrawLine(m_Green, m_nW, m_nH, m_nFrontRight, m_nFrontTop, m_nRearRight, m_nRearTop, G);
	::DrawLine(m_Blue, m_nW, m_nH, m_nFrontRight, m_nFrontTop, m_nRearRight, m_nRearTop, B);

	::DrawLine(m_Red, m_nW, m_nH, m_nFrontLeft, m_nFrontBottom, m_nRearLeft, m_nRearBottom, R);
	::DrawLine(m_Green, m_nW, m_nH, m_nFrontLeft, m_nFrontBottom, m_nRearLeft, m_nRearBottom, G);
	::DrawLine(m_Blue, m_nW, m_nH, m_nFrontLeft, m_nFrontBottom, m_nRearLeft, m_nRearBottom, B);

	::DrawLine(m_Red, m_nW, m_nH, m_nFrontRight, m_nFrontBottom, m_nRearRight, m_nRearBottom, R);
	::DrawLine(m_Green, m_nW, m_nH, m_nFrontRight, m_nFrontBottom, m_nRearRight, m_nRearBottom, G);
	::DrawLine(m_Blue, m_nW, m_nH, m_nFrontRight, m_nFrontBottom, m_nRearRight, m_nRearBottom, B);
}

void CCubic3D::GetCubicPosition(double x3D, double y3D, double z3D, int *ImageX, int *ImageY)
{
	double x, y;
	
	x = m_nStartX + (x3D-m_MinX+(m_RangeMax-m_RangeX)/2.)/m_RangeRate;
	y = m_nEndY - (y3D-m_MinY+(m_RangeMax-m_RangeY)/2.)/m_RangeRate;

	*ImageX = (int)(x + m_nLengthZ*(z3D-m_MinZ+(m_RangeMax-m_RangeZ)/2.)/m_RangeMax+0.5);
	*ImageY = (int)(y - m_nLengthZ*(z3D-m_MinZ+(m_RangeMax-m_RangeZ)/2.)/m_RangeMax+0.5);
}

void CCubic3D::DrawCross(double x3D, double y3D, double z3D, BYTE R, BYTE G, BYTE B)
{
	int x, y;

	GetCubicPosition(x3D, y3D, z3D, &x, &y);

	::DrawCross(m_Red, m_nW, m_nH, CPoint(x, y), R);
	::DrawCross(m_Green, m_nW, m_nH, CPoint(x, y), G);
	::DrawCross(m_Blue, m_nW, m_nH, CPoint(x, y), B);
}

void CCubic3D::DrawLine(double x3D1, double y3D1, double z3D1, double x3D2, double y3D2, double z3D2, BYTE R, BYTE G, BYTE B)
{
	int x1, y1, x2, y2;

	GetCubicPosition(x3D1, y3D1, z3D1, &x1, &y1);
	GetCubicPosition(x3D2, y3D2, z3D2, &x2, &y2);

	::DrawLine(m_Red, m_nW, m_nH, x1, y1, x2, y2, R);
	::DrawLine(m_Green, m_nW, m_nH, x1, y1, x2, y2, G);
	::DrawLine(m_Blue, m_nW, m_nH, x1, y1, x2, y2, B);
}

void CCubic3D::DrawArrow(double x3D1, double y3D1, double z3D1, double x3D2, double y3D2, double z3D2, BYTE R, BYTE G, BYTE B)
{
	int x1, y1, x2, y2;

	GetCubicPosition(x3D1, y3D1, z3D1, &x1, &y1);
	GetCubicPosition(x3D2, y3D2, z3D2, &x2, &y2);

	::DrawArrow(m_Red, m_nW, m_nH, x1, y1, x2, y2, R);
	::DrawArrow(m_Green, m_nW, m_nH, x1, y1, x2, y2, G);
	::DrawArrow(m_Blue, m_nW, m_nH, x1, y1, x2, y2, B);
}

void CCubic3D::DrawNormal(double x3D1, double y3D1, double z3D1, double x3D2, double y3D2, double z3D2, double x3D3, double y3D3, double z3D3, BYTE R, BYTE G, BYTE B)
{
	double xV1, yV1, zV1, xV2, yV2, zV2;
	double xN, yN, zN;
	double xC, yC, zC, xE, yE, zE;

	xV1 = x3D1 - x3D2;
	yV1 = y3D1 - y3D2;
	zV1 = z3D1 - z3D2;

	xV2 = x3D2 - x3D3;
	yV2 = y3D2 - y3D3;
	zV2 = z3D2 - z3D3;

	xN = yV1*zV2 - zV1*yV2;
	yN = zV1*xV2 - xV1*zV2;
	zN = xV1*yV2 - yV1*xV2;

	double Distance = sqrt(xN*xN + yN*yN + zN*zN);

	xN = xN/Distance*m_RangeMax/4.;
	yN = yN/Distance*m_RangeMax/4.;
	zN = zN/Distance*m_RangeMax/4.;

	xC = (x3D1+x3D2+x3D3)/3;
	yC = (y3D1+y3D2+y3D3)/3;
	zC = (z3D1+z3D2+z3D3)/3;

	xE = xC + xN;
	yE = yC + yN;
	zE = zC + zN;

	DrawArrow(xC, yC, zC, xE, yE, zE, R, G, B);
}
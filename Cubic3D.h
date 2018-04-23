// Cubic3D.h: interface for the CCubic3D class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CUBIC3D_H__2C351051_BBAB_46A2_9F85_822EFC48E18E__INCLUDED_)
#define AFX_CUBIC3D_H__2C351051_BBAB_46A2_9F85_822EFC48E18E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CCubic3D  
{
public:
	int m_nW, m_nH;
	BYTE **m_Red, **m_Green, **m_Blue;
	int m_nStartX, m_nEndX, m_nStartY, m_nEndY;
	int m_nLengthZ;
	int m_nFrontLeft, m_nFrontTop, m_nFrontRight, m_nFrontBottom;
	int m_nRearLeft, m_nRearTop, m_nRearRight, m_nRearBottom;

	double m_MinX, m_MaxX, m_MinY, m_MaxY, m_MinZ, m_MaxZ;
	double m_CenterX, m_CenterY, m_CenterZ;
	double m_RangeX, m_RangeY, m_RangeZ, m_RangeMax, m_RangeRate;

	CCubic3D();
	virtual ~CCubic3D();

	void Set(int nW, int nH, double MinX, double MaxX, double MinY, double MaxY, double MinZ, double MaxZ);
	void Reset();
	void ClearImage(BYTE R, BYTE G, BYTE B);
	void DrawCubic(BYTE R, BYTE G, BYTE B);
	void GetCubicPosition(double x3D, double y3D, double z3D, int *ImageX, int *ImageY);
	void DrawCross(double x3D, double y3D, double z3D, BYTE R, BYTE G, BYTE B);
	void DrawLine(double x3D1, double y3D1, double z3D1, double x3D2, double y3D2, double z3D2, BYTE R, BYTE G, BYTE B);
	void DrawArrow(double x3D1, double y3D1, double z3D1, double x3D2, double y3D2, double z3D2, BYTE R, BYTE G, BYTE B);
	void DrawNormal(double x3D1, double y3D1, double z3D1, double x3D2, double y3D2, double z3D2, double x3D3, double y3D3, double z3D3, BYTE R, BYTE G, BYTE B);

};

#endif // !defined(AFX_CUBIC3D_H__2C351051_BBAB_46A2_9F85_822EFC48E18E__INCLUDED_)

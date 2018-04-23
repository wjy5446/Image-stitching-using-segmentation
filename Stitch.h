// Stitch.h: interface for the CStitch class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STITCH_H__6FBFD100_3C5D_4A7D_9B16_D87307395BA6__INCLUDED_)
#define AFX_STITCH_H__6FBFD100_3C5D_4A7D_9B16_D87307395BA6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define MAX_STITCH_IMAGE	50

struct SColorImage2D {
	int nW, nH;
	BYTE **Red, **Green, **Blue, **Gray, **Temp;
	int **Label;
	int nLabelCnt;
	CRect *LabelBox;
	double *m2, *m3;
};

class CStitch  
{
public:
	CStitch();
	virtual ~CStitch();

	SColorImage2D m_Image[MAX_STITCH_IMAGE];
	int m_nImageIndex;

	void Closing();
	void Run(BYTE *Image1D, int nW, int nH, bool bFirstFrame);
};

#endif // !defined(AFX_STITCH_H__6FBFD100_3C5D_4A7D_9B16_D87307395BA6__INCLUDED_)

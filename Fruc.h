// Fruc.h: interface for the CFruc class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FRUC_H__DA919124_3353_41F0_AD0B_D65D37CC5397__INCLUDED_)
#define AFX_FRUC_H__DA919124_3353_41F0_AD0B_D65D37CC5397__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CFruc  
{
public:
	CFruc();
	virtual ~CFruc();

	void Init(int nW, int nH);
	void Closing();
	void Run(BYTE *Image1D, int nW, int nH, int nInterGap, int nMask, int nSearch, int nGrow, int nMeGap, int nUseColor, int nThreDiff, 
				bool bViewPsnr, int nPsnrType, bool bFirst);

	int m_nW, m_nH;

	BYTE **m_CurrR, **m_CurrG, **m_CurrB;
	BYTE **m_PrevR, **m_PrevG, **m_PrevB;
	BYTE **m_InterR, **m_InterG, **m_InterB;
	BYTE **m_RefR, **m_RefG, **m_RefB;
	BYTE **m_TempR, **m_TempG, **m_TempB;

	int m_nFrameCnt;

	int m_nCnt;
	double m_Psnr[5];
	double m_TimeDiff[5];

	_int64 m_TimeFreq, m_TimeStart, m_TimeEnd;
};

#endif // !defined(AFX_FRUC_H__DA919124_3353_41F0_AD0B_D65D37CC5397__INCLUDED_)

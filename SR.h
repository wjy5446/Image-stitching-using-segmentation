// SR.h: interface for the CSR class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SR_H__6004AE12_5B61_4487_B925_A5B1262F1E5D__INCLUDED_)
#define AFX_SR_H__6004AE12_5B61_4487_B925_A5B1262F1E5D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSR  
{
public:
	CSR();
	virtual ~CSR();

	void Run(BYTE *Image1D, int nW, int nH, int RefX, int RefY, int nZoom, int nMask, int nSearch, int nGrow, bool bViewPsnr, bool bFirst);

	int m_nCnt;
	double m_Psnr[4];
	double m_Epsnr[4];
	double m_TimeDiff[4];

	_int64 m_TimeFreq, m_TimeStart, m_TimeEnd;

};

#endif // !defined(AFX_SR_H__6004AE12_5B61_4487_B925_A5B1262F1E5D__INCLUDED_)

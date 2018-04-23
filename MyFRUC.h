#pragma once

#include <vector>
#define MB_SIZE 2
#define SEARCH_RANGE 3

typedef struct motionvector
{
	int x;        /* MV의 가로성분 값 */
	int y;        /* MV의 세로성분 값 */
	int sad;		/* MV의 SAD 값*/
  motionvector()
  {
	x=y=sad=0;
  };
} MotionVector;



typedef struct macroblock
{
	//int pixelValue[MB_SIZE][MB_SIZE];
	MotionVector motion;		/* MB의 MotionVector */
	CPoint po;// MB의 좌측 상단 위치


	macroblock& operator =(macroblock macro)
	{
		motion.x = macro.motion.x;
		motion.y = macro.motion.y;
		motion.sad = macro.motion.sad;

		/*for(int i = 0 ; i<MB_SIZE;i++)
		{
			for(int j = 0 ;j<MB_SIZE;j++)
			{
				pixelValue[i][j]=macro.pixelValue[i][j];
			}
		}*/
		po.x = macro.po.x;
		po.y = macro.po.y;
		return *this;
	}
} Macroblock;


typedef struct _block{
	CPoint mv;
	int leftTopx;
	int leftTopy;

	_block & operator =(_block bl){
	
		mv.x = bl.mv.x;
		mv.y = bl.mv.y;
		leftTopx = bl.leftTopx;
		leftTopy = bl.leftTopy;
		return *this;
	};
	BOOL operator <(_block bl){
		if( mv.x*mv.x+mv.y*mv.y < bl.mv.x*bl.mv.x + bl.mv.y*bl.mv.y )
			return TRUE;
		else
			return FALSE;
	};

}block;

typedef struct _newblock{
	CPoint mv;
	CPoint mv_b;
	int leftTopx;
	int leftTopy;
	int Wx;
	int Wy;
	_newblock & operator =(_newblock bl){
	
		mv.x = bl.mv.x;
		mv.y = bl.mv.y;
		mv_b.x = bl.mv_b.x;
		mv_b.y = bl.mv_b.y;
		leftTopx = bl.leftTopx;
		leftTopy = bl.leftTopy;
		//Wx = bl.Wx;
		//Wy = bl.Wy;
		return *this;
	};
	BOOL operator <(_block bl){
		if( mv.x*mv.x+mv.y*mv.y < bl.mv.x*bl.mv.x + bl.mv.y*bl.mv.y )
			return TRUE;
		else
			return FALSE;
	};

}newblock;




class MyFRUC
{
public:
	MyFRUC(void);
	~MyFRUC(void);

	void Init(int nW, int nH);
	void Closing();
	void Run(BYTE *Image1D, int nW, int nH, int nInterGap, int nMask, int nSearch, int nGrow, int nMeGap, int nUseColor, int nThreDiff, 
				bool bViewPsnr, int nPsnrType, bool bFirst);
	void GetInterpolatedFrame(BYTE ** PrevR,BYTE ** PrevG,BYTE ** PrevB,BYTE ** CurrR,BYTE ** CurrG, BYTE** CurrB,BYTE ** InterR,BYTE ** InterG, BYTE** InterB, int nW, int nH, int sizeMB,int search_Range);
	void GetFrameWithBilateralME(BYTE ** PrevR,BYTE ** PrevG,BYTE ** PrevB,BYTE ** CurrR,BYTE ** CurrG, BYTE** CurrB,BYTE ** InterR,BYTE ** InterG, BYTE** InterB, int nW, int nH, int sizeMB,int search_Range,int refineMode);
	void GetFrameWithBilateralME2(BYTE ** PrevR,BYTE ** PrevG,BYTE ** PrevB,BYTE ** CurrR,BYTE ** CurrG, BYTE** CurrB,BYTE ** InterR,BYTE ** InterG, BYTE** InterB, int nW, int nH, int sizeMB,int search_Range,int nMeGap);
	void GetFrameWithNEWBilateralME(BYTE ** PrevR,BYTE ** PrevG,BYTE ** PrevB,BYTE ** CurrR,BYTE ** CurrG, BYTE** CurrB,BYTE ** InterR,BYTE ** InterG, BYTE** InterB, int nW, int nH, int sizeMB,int search_Range,int nMeGap);
	
	int m_nW, m_nH;

	BYTE **m_CurrR, **m_CurrG, **m_CurrB;
	BYTE **m_PrevR, **m_PrevG, **m_PrevB;
	BYTE **m_InterR, **m_InterG, **m_InterB;
	BYTE **m_RefR, **m_RefG, **m_RefB;
	BYTE **m_TempR, **m_TempG, **m_TempB;
	
	int m_nFrameCnt;

	int m_nCnt;
	double m_Psnr[7];
	double m_TimeDiff[7];

	_int64 m_TimeFreq, m_TimeStart, m_TimeEnd;

	std::vector<Macroblock>	Prevgroup_MB;
	std::vector<Macroblock>  Tempgroup_MB;
	std::vector<block> MBgroupForBME;
	std::vector<block> tempgroup;
	std::vector<newblock> MBgroupBackward;
	std::vector<newblock> MBgroupforward;
};


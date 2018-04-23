#include "stdafx.h"
#include "MyFRUC.h"
#include "Image.h"
#include "Fruc.h"
#include<cmath>



MyFRUC::MyFRUC(void)
{
	Init(320, 320);
}


MyFRUC::~MyFRUC(void)
{
}


void MyFRUC::Init(int nW, int nH)
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

void MyFRUC::Closing()
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

static double GetPsnrRgb(BYTE **R, BYTE **G, BYTE **B, BYTE **R2, BYTE **G2, BYTE **B2, int nW, int nH)
{
	int x, y;
	double Mse = 0.;
	double Mse1 = 0, Mse2 = 0, Mse3 = 0;

	for(y = 0 ; y < nH ; y++)
		for(x = 0 ; x < nW ; x++)
		{
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

void static medianFilter(BYTE**src,BYTE**dst, int nW, int nH, int maskSize)
{
	int * mask = new int[(2*maskSize+1)*(2*maskSize+1)-1];
	int min,cnt;
	for(int y = 0 ;y<nH;y++){
		for(int x = 0;x<nW;x++){
		   min =cnt= 0;
			
		   for(int init = 0 ;init < maskSize*maskSize-1 ;init++)
			   mask[init] = 0;


		   for(int ymask = - maskSize;ymask <= maskSize;ymask++){
			   for(int xmask = - maskSize;xmask <= maskSize;xmask++){
					if(y+ymask>=0 &&y+ymask<nH && x+xmask>=0 && x+xmask<nW && !(ymask ==0 && xmask ==0))
					{
						if(cnt ==0){
						mask[cnt] =src[y+ymask][x+xmask];
						cnt++;
						}
						else{
						
							for(int compare = 0;compare <cnt;compare++){
							
								if(mask[0] > src[y+ymask][x+xmask]){
									for(int i =0;i<cnt;i++)
									{
										mask[i+1] =mask[i];
									}
									mask[0] = src[y+ymask][x+xmask];
								
								}
								else{
									if(cnt ==1)
									{
										mask[1] = src[y+ymask][x+xmask];
									}
									for(int i =1;i<cnt;i++)
									{
										if(mask[i]>src[y+ymask][x+xmask]){
											for(int j = i;j< cnt;j++)
												mask[j+1] =mask[j];
											mask[i] = src[y+ymask][x+xmask];
											break;
										}
									    if(cnt-1 == i)
											mask[cnt] = src[y+ymask][x+xmask];
										
									}
								
								}
							}
							cnt++;
						}
					}
			   }
		   }

		   dst[y][x] = mask[((2*maskSize+1)*(2*maskSize+1)-1)/2 +1];

		}
	}






	 delete [] mask;

}



static CPoint BilateralME(BYTE **CurrImageGray, BYTE **PrevImageGray, int lefttopX, int lefttopY, 
		 int nW, int nH, int nSearchLeft, int nSearchTop, int nSearchRight, int nSearchBottom, 
		 int BlockWidth, int BlockHeight, double *pMinDiff)
{
	if(lefttopX-nSearchLeft <0 ||  lefttopY-nSearchTop<0 ||  lefttopX+BlockWidth+nSearchRight>=nW || 
		lefttopY+BlockHeight+nSearchBottom>=nH )
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
					yy0 = lefttopY+y+dy;
					xx0 = lefttopX+x+dx;

					xx1 = lefttopX+x-dx;
					yy1 = lefttopY+y-dy;

					Diff += abs(CurrImageGray[yy0][xx0] - PrevImageGray[yy1][xx1]);
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

static CPoint NEWBilateralME(BYTE **CurrImageGray, BYTE **PrevImageGray, int lefttopX, int lefttopY, 
		 int nW, int nH, int nSearchLeft, int nSearchTop, int nSearchRight, int nSearchBottom, 
		 int BlockWidth, int BlockHeight, double *pMinDiff, int *WY,int *WX)
{
	if(lefttopX-nSearchLeft <0 ||  lefttopY-nSearchTop<0 ||  lefttopX+BlockWidth+nSearchRight>=nW || 
		lefttopY+BlockHeight+nSearchBottom>=nH )
	{
		*pMinDiff = 255;
		*WY= *WX =0;
		return 0;
	}

	int x, y, dx, dy;
	CPoint MinDx = CPoint(0, 0);
	double Diff, MinDiff = 255;
	int xx0, yy0, xx1, yy1;
	(*WY)= (*WX) =0;
	for(dy = -nSearchTop ; dy <= nSearchBottom ; dy++)
	{
		for(dx = -nSearchLeft ; dx <= nSearchRight ; dx++)
		{
			
			for(int Wy =-2;Wy<=2;Wy++)
				for(int Wx =-2;Wx<=2;Wx++)
					{	
						if(lefttopX-nSearchLeft+Wx <0 ||  lefttopY-nSearchTop+Wy<0 ||  lefttopX+BlockWidth+nSearchRight+Wx>=nW || 
		lefttopY+BlockHeight+nSearchBottom+Wy>=nH )
						continue;
						Diff = 0.;
					for(y = 0 ; y < BlockHeight ; y++)
						for(x = 0 ; x < BlockWidth ; x++)
						{
							yy0 = lefttopY+y+dy+Wy;
							xx0 = lefttopX+x+dx+Wx;

							xx1 = lefttopX+x-dx;
							yy1 = lefttopY+y-dy;

							Diff += abs(CurrImageGray[yy0][xx0] - PrevImageGray[yy1][xx1]);
						}

						Diff /= BlockWidth*BlockHeight;

						if(Diff < MinDiff || (Diff == MinDiff && (Wx == 0 && Wy == 0)) )
						 {
							MinDiff = Diff;
							MinDx = CPoint(dx, dy);
							(*WY) = Wy;
							(*WX) = Wx;
						 }

						
					}
			
		}
	}

	*pMinDiff = MinDiff;
	return MinDx;
}





static block** blockmatrix(int nH,int nW,int sizeMB)
{
	block ** MVGroup;
	
	MVGroup = new block*[nH/(2*sizeMB+1)+1];

	for(int y = 0 ;y<nH/(2*sizeMB+1)+1; y++)
		MVGroup[y] = new block[nW/(2*sizeMB+1)+1];
	
	return MVGroup;
}

static void free_blockmatrix(block**matrix,int nH,int nW,int sizeMB)
{
	for(int y = 0 ;y<nH/(2*sizeMB+1)+1; y++)
		delete [] matrix[y];
	delete [] matrix;
}

static void MotionVectorRefinement(block ** MVgroup,BYTE ** Curr,BYTE ** Prev,int nMVx,int nMVy,int BlockWidth, int BlockHeight,int nW, int nH )
{
	std::vector<block> temp;
	double minMAD;
	double MAD;
	int check;
	CPoint minMV;
	for(int y = 0 ;y<nMVy;y++)
		for(int x =0;x<nMVx;x++){
			for(int ymask=-1;ymask<=1;ymask++){
				for(int xmask=-1;xmask<=1;xmask++){
					if(x+xmask>=0 && x+xmask<nMVx && y+ymask>=0 && y+ymask<nMVy){
						temp.push_back(MVgroup[y+ymask][x+xmask]);
					}
				}
			}
			minMAD = 255;
			minMV =CPoint(0,0);
			for(int num = 0;num< temp.size();num++){
				check =1;
				MAD = 0;
				for(int yblock = 0;yblock<BlockHeight;yblock++)
					for(int xblock = 0;xblock<BlockWidth;xblock++){
						if(MVgroup[y][x].leftTopy + temp[num].mv.y + yblock >=0 &&MVgroup[y][x].leftTopy + temp[num].mv.y + yblock <nH && MVgroup[y][x].leftTopx + temp[num].mv.x + xblock >=0 && MVgroup[y][x].leftTopx + temp[num].mv.x + xblock <nW &&
							MVgroup[y][x].leftTopy - temp[num].mv.y + yblock >=0 && MVgroup[y][x].leftTopy - temp[num].mv.y + yblock <nH &&	MVgroup[y][x].leftTopx - temp[num].mv.x + xblock >=0 &&	MVgroup[y][x].leftTopx - temp[num].mv.x + xblock <nW	)
						MAD += abs(Curr[MVgroup[y][x].leftTopy + temp[num].mv.y + yblock][MVgroup[y][x].leftTopx + temp[num].mv.x + xblock]- Prev[MVgroup[y][x].leftTopy - temp[num].mv.y + yblock][MVgroup[y][x].leftTopx - temp[num].mv.x + xblock]);
						else
							check = 0;
					}
					MAD/=(BlockHeight*BlockWidth);
					if(minMAD>MAD &&check)
					{
						minMAD = MAD;
						minMV = CPoint(temp[num].mv.x,temp[num].mv.y);
					}
			}

			MVgroup[y][x].mv = minMV;
			temp.clear();


		}
}


static void FindMVforMinimumMAD(BYTE ** CurrGray, BYTE ** PrevGray,int lefttopX,int lefttopY,CPoint mv_f1,CPoint mv_b1,CPoint mv_f2,CPoint mv_b2, int nW, int nH,int blockwidth,int blockheight,CPoint * mv_f, CPoint *mv_b)
{
	double MAD1,MAD2;
	MAD1=MAD2 =0;
	int check1,check2;
	check1 = check2 =0;
	for(int y =0;y<blockheight;y++)
	{
		for(int x = 0;x<blockwidth;x++)
		{
			if(lefttopY + mv_f1.y+y < 0 || lefttopY + mv_f1.y+y >=nH || lefttopX+mv_f1.x+x < 0 || lefttopX+mv_f1.x+x >=nW || lefttopY + mv_b1.y+y < 0 || lefttopY + mv_b1.y+y >=nH || lefttopX+mv_b1.x+x < 0 || lefttopX+mv_b1.x+x >=nW)
			{
				check1 = 1; break;
			}
			if(lefttopY + mv_f2.y+y < 0 || lefttopY + mv_f2.y+y >=nH || lefttopX+mv_f2.x+x < 0 || lefttopX+mv_f2.x+x >=nW || lefttopY + mv_b2.y+y < 0 || lefttopY + mv_b2.y+y >=nH || lefttopX+mv_b2.x+x < 0 || lefttopX+mv_b2.x+x >=nW)
			{
				check2 = 1; break;
			}
			MAD1+=abs(CurrGray[lefttopY + mv_f1.y+y][lefttopX+mv_f1.x+x] - PrevGray[lefttopY+mv_b1.y+y][lefttopX+mv_b1.x+x]);
			MAD2+=abs(CurrGray[lefttopY + mv_f2.y+y][lefttopX+mv_f2.x+x] - PrevGray[lefttopY+mv_b2.y+y][lefttopX+mv_b2.x+x]);
		}
		if(check1 || check2)
			break;
	}
	MAD1/=blockheight*blockwidth;
	MAD2/=blockheight*blockwidth;
	if(check1 && !check2)
	{
		*mv_f = mv_f2;
		*mv_b = mv_b2;
	}
	else if(!check1 && check2)
	{
		*mv_f = mv_f1;
		*mv_b = mv_b1;
	}
	else if(check1 && check2)
	{
		*mv_f = CPoint(0,0);
		*mv_b = CPoint(0,0);
	}
	else
	{
		if(MAD1<MAD2)
		{
			*mv_f = mv_f1;
			*mv_b = mv_b1;
		}
		else
		{
			*mv_f = mv_f2;
			*mv_b = mv_b2;
		}
	}
	return ;
}

static void ModifiedOBMC(BYTE ** InputR,BYTE ** InputG,BYTE ** InputB,int nW, int nH, int w,int blocksize,std::vector<newblock> & G_block, BYTE ** OutR,BYTE ** OutG,BYTE ** OutB)
{
	int **NumOfOverlapping;
	BYTE **candidateR[4];
	BYTE **candidateG[4];
	BYTE **candidateB[4];
	int sumR,sumG,sumB;
	double val1,val2;

 	NumOfOverlapping = imatrix(nH,nW);
	for(int i = 0;i<4;i++)
	{	candidateR[i] = cmatrix(nH,nW);
		candidateG[i] = cmatrix(nH,nW);
		candidateB[i] = cmatrix(nH,nW);
	}

	
	for(int y = 0;y< nH;y++)
		for(int x = 0 ;x<nW;x++)
		 NumOfOverlapping[y][x] = 0;
		

 
	for(int y = 0;y< nH;y+=blocksize)
		for(int x = 0 ;x<nW;x+=blocksize)
		{
			for(int by = 0-w;by< blocksize+w;by++)
				for(int bx = 0-w;bx< blocksize+w;bx++)
				{
					if(y+by >=0 && y+by<nH && x+bx >=0 && x+bx <nW)
					{
						val1 = sqrt((double)(G_block[y*(nW/blocksize+1)+x].mv.x * G_block[y*(nW/blocksize+1)+x].mv.x + G_block[y*(nW/blocksize+1)+x].mv.y * G_block[y*(nW/blocksize+1)+x].mv.y));
						val2 = sqrt((double)(G_block[y*(nW/blocksize+1)+x].mv_b.x * G_block[y*(nW/blocksize+1)+x].mv_b.x + G_block[y*(nW/blocksize+1)+x].mv_b.y *G_block[y*(nW/blocksize+1)+x].mv_b.y) );
						if (val1 >= 2.2&& val2 >=2.2 )
				        {
						if(by >= -w && by <= -1 && 0<= bx && bx<=blocksize-1) //1
						{
							candidateR[NumOfOverlapping[y+by][x+bx]][y+by][x+bx] = InputR[y][x+bx];
							candidateG[NumOfOverlapping[y+by][x+bx]][y+by][x+bx] = InputG[y][x+bx];
							candidateB[NumOfOverlapping[y+by][x+bx]][y+by][x+bx] = InputB[y][x+bx];
							NumOfOverlapping[y+by][x+bx]++;
						}
						else if(by >= 0 && by <= blocksize-1 && -w <= bx && bx<= -1)//2
						{
							candidateR[NumOfOverlapping[y+by][x+bx]][y+by][x+bx] = InputR[y+by][x];
							candidateG[NumOfOverlapping[y+by][x+bx]][y+by][x+bx] = InputG[y+by][x];
							candidateB[NumOfOverlapping[y+by][x+bx]][y+by][x+bx] = InputB[y+by][x];
							NumOfOverlapping[y+by][x+bx]++;
						}
						else if(by >= blocksize && by <= blocksize+w-1 && 0 <= bx && bx<= blocksize-1)//3
						{
							candidateR[NumOfOverlapping[y+by][x+bx]][y+by][x+bx] = InputR[y+blocksize-1][x+bx];
							candidateG[NumOfOverlapping[y+by][x+bx]][y+by][x+bx] = InputG[y+blocksize-1][x+bx];
							candidateB[NumOfOverlapping[y+by][x+bx]][y+by][x+bx] = InputB[y+blocksize-1][x+bx];
							NumOfOverlapping[y+by][x+bx]++;
						}
						else if(by >= 0 && by <= blocksize-1 && blocksize <= bx && bx<= blocksize+w-1)//4
						{
							candidateR[NumOfOverlapping[y+by][x+bx]][y+by][x+bx] = InputR[y+by][x+blocksize-1];
							candidateG[NumOfOverlapping[y+by][x+bx]][y+by][x+bx] = InputG[y+by][x+blocksize-1];
							candidateB[NumOfOverlapping[y+by][x+bx]][y+by][x+bx] = InputB[y+by][x+blocksize-1];
							NumOfOverlapping[y+by][x+bx]++;
						}
						else if(by >= -w && by <= -1 && -w <= bx && bx<= -1)//5
						{
							candidateR[NumOfOverlapping[y+by][x+bx]][y+by][x+bx] = InputR[y][x];
							candidateG[NumOfOverlapping[y+by][x+bx]][y+by][x+bx] = InputG[y][x];
							candidateB[NumOfOverlapping[y+by][x+bx]][y+by][x+bx] = InputB[y][x];
							NumOfOverlapping[y+by][x+bx]++;
						}
						else if(by >= -w && by <= -1 && blocksize <= bx && bx<= blocksize+w-1)//6
						{
							candidateR[NumOfOverlapping[y+by][x+bx]][y+by][x+bx] = InputR[y][x+blocksize-1];
							candidateG[NumOfOverlapping[y+by][x+bx]][y+by][x+bx] = InputG[y][x+blocksize-1];
							candidateB[NumOfOverlapping[y+by][x+bx]][y+by][x+bx] = InputB[y][x+blocksize-1];
							NumOfOverlapping[y+by][x+bx]++;
						}
						else if(by >= blocksize && by <= blocksize+w-1 && -w <= bx && bx<= -1)//7
						{
							candidateR[NumOfOverlapping[y+by][x+bx]][y+by][x+bx] = InputR[y+blocksize-1][x];
							candidateG[NumOfOverlapping[y+by][x+bx]][y+by][x+bx] = InputG[y+blocksize-1][x];
							candidateB[NumOfOverlapping[y+by][x+bx]][y+by][x+bx] = InputB[y+blocksize-1][x];
							NumOfOverlapping[y+by][x+bx]++;
						}
						else if(by >= blocksize && by <= blocksize+w-1 && blocksize <= bx && bx<= blocksize+w-1)//8
						{
							candidateR[NumOfOverlapping[y+by][x+bx]][y+by][x+bx] = InputR[y+blocksize-1][x+blocksize-1];
							candidateG[NumOfOverlapping[y+by][x+bx]][y+by][x+bx] = InputG[y+blocksize-1][x+blocksize-1];
							candidateB[NumOfOverlapping[y+by][x+bx]][y+by][x+bx] = InputB[y+blocksize-1][x+blocksize-1];
							NumOfOverlapping[y+by][x+bx]++;
						}
						else
						{
							candidateR[NumOfOverlapping[y+by][x+bx]][y+by][x+bx] = InputR[y+by][x+bx];
							candidateG[NumOfOverlapping[y+by][x+bx]][y+by][x+bx] = InputG[y+by][x+bx];
							candidateB[NumOfOverlapping[y+by][x+bx]][y+by][x+bx] = InputB[y+by][x+bx];
							NumOfOverlapping[y+by][x+bx]++;
						}
					  }
						
						else
						{
							if(by>=0 && by<blocksize && bx>=0 && bx < blocksize){
							candidateR[NumOfOverlapping[y+by][x+bx]][y+by][x+bx] = InputR[y+by][x+bx];
							candidateG[NumOfOverlapping[y+by][x+bx]][y+by][x+bx] = InputG[y+by][x+bx];
							candidateB[NumOfOverlapping[y+by][x+bx]][y+by][x+bx] = InputB[y+by][x+bx];
							NumOfOverlapping[y+by][x+bx]++;
							}
						}
				}
			}

				
		}
	

		
	for(int y = 0;y< nH;y++)
		for(int x = 0 ;x<nW;x++)
		{
			sumR =0;
			sumG =0;
			sumB =0;
			for(int num =0 ;num < NumOfOverlapping[y][x];num++)
			{
				sumR += candidateR[num][y][x];
				sumG += candidateG[num][y][x];
				sumB += candidateB[num][y][x];
			}
			OutR[y][x] = sumR/NumOfOverlapping[y][x];
			OutG[y][x] = sumG/NumOfOverlapping[y][x];
			OutB[y][x] = sumB/NumOfOverlapping[y][x];
		}



		for(int i = 0;i<4;i++)
		{	free_cmatrix(candidateR[i],nH,nW);
			free_cmatrix(candidateG[i],nH,nW);
			free_cmatrix(candidateB[i],nH,nW);
		}
		free_imatrix(NumOfOverlapping,nH,nW);
}
void MyFRUC::GetFrameWithBilateralME(BYTE ** PrevR,BYTE ** PrevG,BYTE ** PrevB,BYTE ** CurrR,BYTE ** CurrG, BYTE** CurrB,BYTE ** InterR,BYTE ** InterG, BYTE** InterB, int nW, int nH, int sizeMB,int search_Range,int refineMode)
{
	block temp;
	int tSAD = 0;
	int minsad;
	BYTE **PrevGray;
	BYTE **CurrGray;
	BYTE **DiffGray;
	PrevGray=cmatrix( nH, nW);
	CurrGray =cmatrix( nH, nW);
	DiffGray =cmatrix( nH, nW);

	
	block ** MVGroup = blockmatrix( nH, nW,sizeMB);
	
	for(int y = 0;y<nH;y++)
		for(int x = 0;x<nW;x++)
		{
			PrevGray[y][x] = (PrevR[y][x]+PrevG[y][x]+PrevB[y][x])/3;
			CurrGray[y][x] = (CurrR[y][x]+CurrG[y][x]+CurrB[y][x])/3;
		}


	for(int y = 0;y < nH;y+=2*sizeMB+1)
		for(int x = 0;x<nW;x+=2*sizeMB+1){
			CPoint mv;
			double Mindiff;
			
			mv = BilateralME(CurrGray,PrevGray,x,y,nW,nH,search_Range,search_Range,search_Range,search_Range,2*sizeMB+1,2*sizeMB+1,&Mindiff);
			if(Mindiff>30 )mv= CPoint(0,0);
			
			temp.leftTopx = x;
			temp.leftTopy = y;
			temp.mv = mv;
			
			if(refineMode){
			MVGroup[y/(2*sizeMB+1)][x/(2*sizeMB+1)] = temp;
			}
			else
			MBgroupForBME.push_back(temp);
		}

		if(refineMode){
			MotionVectorRefinement(MVGroup,CurrGray,PrevGray,nW/(2*sizeMB+1)+1,nH/(2*sizeMB+1)+1,2*sizeMB+1,2*sizeMB+1,nW,nH);
			
			for(int MVy = 0;MVy<nH/(2*sizeMB+1)+1;MVy++)
				for(int MVx = 0;MVx<nW/(2*sizeMB+1)+1;MVx++){
					
				 for(int y = 0 ; y < 2*sizeMB+1 ; y++)
					for(int x = 0 ; x < 2*sizeMB+1 ; x++)
					{
						if(MVGroup[MVy][MVx].leftTopy+y >= nH || MVGroup[MVy][MVx].leftTopx+x  >=nW || MVGroup[MVy][MVx].leftTopy+MVGroup[MVy][MVx].mv.y+y <0 || MVGroup[MVy][MVx].leftTopx+MVGroup[MVy][MVx].mv.x+x <0 || 
							MVGroup[MVy][MVx].leftTopy-MVGroup[MVy][MVx].mv.y+y<0 || MVGroup[MVy][MVx].leftTopx-MVGroup[MVy][MVx].mv.x+x<0  ) continue;
						InterR[MVGroup[MVy][MVx].leftTopy+y][MVGroup[MVy][MVx].leftTopx+x] = (CurrR[MVGroup[MVy][MVx].leftTopy+MVGroup[MVy][MVx].mv.y+y][MVGroup[MVy][MVx].leftTopx+MVGroup[MVy][MVx].mv.x+x]+PrevR[MVGroup[MVy][MVx].leftTopy-MVGroup[MVy][MVx].mv.y+y][MVGroup[MVy][MVx].leftTopx-MVGroup[MVy][MVx].mv.x+x])/2;
						InterG[MVGroup[MVy][MVx].leftTopy+y][MVGroup[MVy][MVx].leftTopx+x] = (CurrG[MVGroup[MVy][MVx].leftTopy+MVGroup[MVy][MVx].mv.y+y][MVGroup[MVy][MVx].leftTopx+MVGroup[MVy][MVx].mv.x+x]+PrevG[MVGroup[MVy][MVx].leftTopy-MVGroup[MVy][MVx].mv.y+y][MVGroup[MVy][MVx].leftTopx-MVGroup[MVy][MVx].mv.x+x])/2;
						InterB[MVGroup[MVy][MVx].leftTopy+y][MVGroup[MVy][MVx].leftTopx+x] = (CurrB[MVGroup[MVy][MVx].leftTopy+MVGroup[MVy][MVx].mv.y+y][MVGroup[MVy][MVx].leftTopx+MVGroup[MVy][MVx].mv.x+x]+PrevB[MVGroup[MVy][MVx].leftTopy-MVGroup[MVy][MVx].mv.y+y][MVGroup[MVy][MVx].leftTopx-MVGroup[MVy][MVx].mv.x+x])/2;
					
					}
					
				}
			
		}

		else{
		for(int cnt = 0;cnt < MBgroupForBME.size();cnt++){
			
			for(int y = 0 ; y < 2*sizeMB+1 ; y++)
				for(int x = 0 ; x < 2*sizeMB+1 ; x++)
				{
					if(MBgroupForBME[cnt].leftTopy+y >= nH || MBgroupForBME[cnt].leftTopx+x >=nW) continue;
					InterR[MBgroupForBME[cnt].leftTopy+y][MBgroupForBME[cnt].leftTopx+x] = (CurrR[MBgroupForBME[cnt].leftTopy+MBgroupForBME[cnt].mv.y+y][MBgroupForBME[cnt].leftTopx+MBgroupForBME[cnt].mv.x+x]+PrevR[MBgroupForBME[cnt].leftTopy-MBgroupForBME[cnt].mv.y+y][MBgroupForBME[cnt].leftTopx-MBgroupForBME[cnt].mv.x+x])/2;
					InterG[MBgroupForBME[cnt].leftTopy+y][MBgroupForBME[cnt].leftTopx+x] = (CurrG[MBgroupForBME[cnt].leftTopy+MBgroupForBME[cnt].mv.y+y][MBgroupForBME[cnt].leftTopx+MBgroupForBME[cnt].mv.x+x]+PrevG[MBgroupForBME[cnt].leftTopy-MBgroupForBME[cnt].mv.y+y][MBgroupForBME[cnt].leftTopx-MBgroupForBME[cnt].mv.x+x])/2;
					InterB[MBgroupForBME[cnt].leftTopy+y][MBgroupForBME[cnt].leftTopx+x] = (CurrB[MBgroupForBME[cnt].leftTopy+MBgroupForBME[cnt].mv.y+y][MBgroupForBME[cnt].leftTopx+MBgroupForBME[cnt].mv.x+x]+PrevB[MBgroupForBME[cnt].leftTopy-MBgroupForBME[cnt].mv.y+y][MBgroupForBME[cnt].leftTopx-MBgroupForBME[cnt].mv.x+x])/2;
					
				}
		}
		}


	

	


	free_cmatrix(CurrGray, nH, nW);
	free_cmatrix(PrevGray, nH, nW);
	free_cmatrix(DiffGray, nH, nW);
	
	
	free_blockmatrix(MVGroup,nH, nW,sizeMB);
		
	MBgroupForBME.clear();

}


void MyFRUC::GetFrameWithBilateralME2(BYTE ** PrevR,BYTE ** PrevG,BYTE ** PrevB,BYTE ** CurrR,BYTE ** CurrG, BYTE** CurrB,BYTE ** InterR,BYTE ** InterG, BYTE** InterB, int nW, int nH, int sizeMB,int search_Range,int nMeGap)
{
	block temp;
	int tSAD = 0;
	int minsad;
	BYTE **PrevGray;
	BYTE **CurrGray;
	BYTE **DiffGray;
	PrevGray=cmatrix( nH, nW);
	CurrGray =cmatrix( nH, nW);
	DiffGray =cmatrix( nH, nW);


	for(int y = 0;y<nH;y++)
		for(int x = 0;x<nW;x++)
		{
			PrevGray[y][x] = (PrevR[y][x]+PrevG[y][x]+PrevB[y][x])/3;
			CurrGray[y][x] = (CurrR[y][x]+CurrG[y][x]+CurrB[y][x])/3;
		}


	for(int y = 0;y < nH;y+=nMeGap)
		for(int x = 0;x<nW;x+=nMeGap){
			CPoint mv;
			double Mindiff;
			
			mv = BilateralME(CurrGray,PrevGray,x,y,nW,nH,search_Range,search_Range,search_Range,search_Range,2*sizeMB+1,2*sizeMB+1,&Mindiff);
			if(Mindiff>30 )mv= CPoint(0,0);
			
			temp.leftTopx = x;
			temp.leftTopy = y;
			temp.mv = mv;

			MBgroupForBME.push_back(temp);
		}


	



		for(int cnt = 0;cnt < MBgroupForBME.size();cnt++){
			
			for(int y = 0 ; y < 2*sizeMB+1 ; y++)
				for(int x = 0 ; x < 2*sizeMB+1 ; x++)
				{
					if(MBgroupForBME[cnt].leftTopy+y >= nH || MBgroupForBME[cnt].leftTopx+x >=nW) continue;
					InterR[MBgroupForBME[cnt].leftTopy+y][MBgroupForBME[cnt].leftTopx+x] = (CurrR[MBgroupForBME[cnt].leftTopy-MBgroupForBME[cnt].mv.y+y][MBgroupForBME[cnt].leftTopx-MBgroupForBME[cnt].mv.x+x]+PrevR[MBgroupForBME[cnt].leftTopy+MBgroupForBME[cnt].mv.y+y][MBgroupForBME[cnt].leftTopx+MBgroupForBME[cnt].mv.x+x])/2;
					InterG[MBgroupForBME[cnt].leftTopy+y][MBgroupForBME[cnt].leftTopx+x] = (CurrG[MBgroupForBME[cnt].leftTopy-MBgroupForBME[cnt].mv.y+y][MBgroupForBME[cnt].leftTopx-MBgroupForBME[cnt].mv.x+x]+PrevG[MBgroupForBME[cnt].leftTopy+MBgroupForBME[cnt].mv.y+y][MBgroupForBME[cnt].leftTopx+MBgroupForBME[cnt].mv.x+x])/2;
					InterB[MBgroupForBME[cnt].leftTopy+y][MBgroupForBME[cnt].leftTopx+x] = (CurrB[MBgroupForBME[cnt].leftTopy-MBgroupForBME[cnt].mv.y+y][MBgroupForBME[cnt].leftTopx-MBgroupForBME[cnt].mv.x+x]+PrevB[MBgroupForBME[cnt].leftTopy+MBgroupForBME[cnt].mv.y+y][MBgroupForBME[cnt].leftTopx+MBgroupForBME[cnt].mv.x+x])/2;
					
				}
		}





	free_cmatrix(CurrGray, nH, nW);
	free_cmatrix(PrevGray, nH, nW);
	free_cmatrix(DiffGray, nH, nW);

	MBgroupForBME.clear();
}

void MyFRUC::GetFrameWithNEWBilateralME(BYTE ** PrevR,BYTE ** PrevG,BYTE ** PrevB,BYTE ** CurrR,BYTE ** CurrG, BYTE** CurrB,BYTE ** InterR,BYTE ** InterG, BYTE** InterB, int nW, int nH, int sizeMB,int search_Range,int nMeGap)
{
	newblock temp;
	int tSAD = 0;
	int minsad;
	CPoint mv_f;
	CPoint mv_b;
	CPoint mv1_f,mv1_b,mv2_f,mv2_b,mv3_f,mv3_b;
	BYTE **PrevGray;
	BYTE **CurrGray;
	BYTE **DiffGray;
	PrevGray=cmatrix( nH, nW);
	CurrGray =cmatrix( nH, nW);
	DiffGray =cmatrix( nH, nW);
	int WX,WY;

	for(int y = 0;y<nH;y++)
		for(int x = 0;x<nW;x++)
		{
			PrevGray[y][x] = (PrevR[y][x]+PrevG[y][x]+PrevB[y][x])/3;
			CurrGray[y][x] = (CurrR[y][x]+CurrG[y][x]+CurrB[y][x])/3;
		}


	for(int y = 0;y < nH;y+=2*sizeMB+1)
		for(int x = 0;x<nW;x+=2*sizeMB+1){
			CPoint mv;
			double Mindiff;

			temp.leftTopx = x;
			temp.leftTopy = y;
			
			mv = NEWBilateralME(CurrGray,PrevGray,x,y,nW,nH,search_Range,search_Range,search_Range,search_Range,2*sizeMB+1,2*sizeMB+1,&Mindiff,&WY,&WX);
			if(Mindiff>30 )mv= CPoint(0,0);
			
			mv1_f = CPoint(mv.x+WX,mv.y+WY);
			mv1_b = CPoint(-mv.x,-mv.y);

			mv = NEWBilateralME(PrevGray,CurrGray,x,y,nW,nH,search_Range,search_Range,search_Range,search_Range,2*sizeMB+1,2*sizeMB+1,&Mindiff,&WY,&WX);
			if(Mindiff>30 )mv= CPoint(0,0);
		
			mv2_b = CPoint(mv.x+WX,mv.y+WY);
			mv2_f= CPoint(-mv.x,-mv.y);

	

			mv =BilateralME(CurrGray,PrevGray,x,y,nW,nH,search_Range,search_Range,search_Range,search_Range,2*sizeMB+1,2*sizeMB+1,&Mindiff);
			if(Mindiff>30 )mv= CPoint(0,0);

			mv3_b = CPoint(-mv.x,-mv.y);
			mv3_f = CPoint(mv.x,mv.y);

			//FindMVforMinimumMAD(CurrGray,PrevGray,x,y, mv1_f, mv1_b, mv2_f, mv2_b, nW, nH, 2*sizeMB+1,2*sizeMB+1, &mv_f, &mv_b);
			//FindMVforMinimumMAD(CurrGray,PrevGray,x,y, mv1_f, mv1_b, mv3_f, mv3_b, nW, nH, 2*sizeMB+1,2*sizeMB+1, &mv_f, &mv_b);
			//temp.mv = CPoint(mv1_f.x*0.25 +mv2_f.x*0.25 + mv3_f.x*0.5, mv1_f.y*0.25 +mv2_f.y*0.25 + mv3_f.y*0.5);
			//temp.mv_b = CPoint(mv1_b.x*0.25 +mv2_b.x*0.25 + mv3_b.x*0.5, mv1_b.y*0.25 +mv2_b.y*0.25 + mv3_b.y*0.5);
			//temp.mv = CPoint(mv1_f.x*0.25 + mv3_f.x*0.75,mv1_f.y*0.25 + mv3_f.y*0.75);
			//temp.mv_b = CPoint(mv2_b.x*0.25 + mv3_b.x*0.75,mv2_b.y*0.25 + mv3_b.y*0.75); 
			temp.mv = mv3_f;
			temp.mv_b = mv3_b;
			MBgroupBackward.push_back(temp);

		}







		for(int cnt = 0;cnt < MBgroupBackward.size();cnt++){
			
			for(int y = 0 ; y < 2*sizeMB+1 ; y++)
				for(int x = 0 ; x < 2*sizeMB+1 ; x++)
				{
					
					if(MBgroupBackward[cnt].leftTopy+y >= nH || MBgroupBackward[cnt].leftTopx+x  >=nW || MBgroupBackward[cnt].leftTopy+MBgroupBackward[cnt].mv.y+y <0 || MBgroupBackward[cnt].leftTopx+MBgroupBackward[cnt].mv.x+x <0 || 
						MBgroupBackward[cnt].leftTopy+MBgroupBackward[cnt].mv_b.y+y <0 || MBgroupBackward[cnt].leftTopx+MBgroupBackward[cnt].mv_b.x+x<0  ) continue;
					InterR[MBgroupBackward[cnt].leftTopy+y][MBgroupBackward[cnt].leftTopx+x] = (CurrR[MBgroupBackward[cnt].leftTopy+MBgroupBackward[cnt].mv.y+y][MBgroupBackward[cnt].leftTopx+MBgroupBackward[cnt].mv.x+x]+PrevR[MBgroupBackward[cnt].leftTopy+MBgroupBackward[cnt].mv_b.y+y][MBgroupBackward[cnt].leftTopx+MBgroupBackward[cnt].mv_b.x+x])/2;
					InterG[MBgroupBackward[cnt].leftTopy+y][MBgroupBackward[cnt].leftTopx+x] = (CurrG[MBgroupBackward[cnt].leftTopy+MBgroupBackward[cnt].mv.y+y][MBgroupBackward[cnt].leftTopx+MBgroupBackward[cnt].mv.x+x]+PrevG[MBgroupBackward[cnt].leftTopy+MBgroupBackward[cnt].mv_b.y+y][MBgroupBackward[cnt].leftTopx+MBgroupBackward[cnt].mv_b.x+x])/2;
					InterB[MBgroupBackward[cnt].leftTopy+y][MBgroupBackward[cnt].leftTopx+x] = (CurrB[MBgroupBackward[cnt].leftTopy+MBgroupBackward[cnt].mv.y+y][MBgroupBackward[cnt].leftTopx+MBgroupBackward[cnt].mv.x+x]+PrevB[MBgroupBackward[cnt].leftTopy+MBgroupBackward[cnt].mv_b.y+y][MBgroupBackward[cnt].leftTopx+MBgroupBackward[cnt].mv_b.x+x])/2;
					/*
					if(MBgroupforward[cnt].leftTopy+y >= nH || MBgroupforward[cnt].leftTopx+x  >=nW || MBgroupforward[cnt].leftTopy+MBgroupforward[cnt].mv.y+y <0 || MBgroupforward[cnt].leftTopx+MBgroupforward[cnt].mv.x+x <0 || 
						MBgroupforward[cnt].leftTopy+MBgroupforward[cnt].mv_b.y+y <0 || MBgroupforward[cnt].leftTopx+MBgroupforward[cnt].mv_b.x+x<0  ) continue;
					InterR[MBgroupforward[cnt].leftTopy+y][MBgroupforward[cnt].leftTopx+x] = (CurrR[MBgroupforward[cnt].leftTopy+MBgroupforward[cnt].mv.y+y][MBgroupforward[cnt].leftTopx+MBgroupforward[cnt].mv.x+x]+PrevR[MBgroupforward[cnt].leftTopy+MBgroupforward[cnt].mv_b.y+y][MBgroupforward[cnt].leftTopx+MBgroupforward[cnt].mv_b.x+x])/2;
					InterG[MBgroupforward[cnt].leftTopy+y][MBgroupforward[cnt].leftTopx+x] = (CurrG[MBgroupforward[cnt].leftTopy+MBgroupforward[cnt].mv.y+y][MBgroupforward[cnt].leftTopx+MBgroupforward[cnt].mv.x+x]+PrevG[MBgroupforward[cnt].leftTopy+MBgroupforward[cnt].mv_b.y+y][MBgroupforward[cnt].leftTopx+MBgroupforward[cnt].mv_b.x+x])/2;
					InterB[MBgroupforward[cnt].leftTopy+y][MBgroupforward[cnt].leftTopx+x] = (CurrB[MBgroupforward[cnt].leftTopy+MBgroupforward[cnt].mv.y+y][MBgroupforward[cnt].leftTopx+MBgroupforward[cnt].mv.x+x]+PrevB[MBgroupforward[cnt].leftTopy+MBgroupforward[cnt].mv_b.y+y][MBgroupforward[cnt].leftTopx+MBgroupforward[cnt].mv_b.x+x])/2;
					*/
				}
		}

		ModifiedOBMC(InterR,InterG,InterB,nW,nH,2,2*sizeMB+1,MBgroupBackward,InterR,InterG,InterB);
		
	free_cmatrix(CurrGray, nH, nW);
	free_cmatrix(PrevGray, nH, nW);
	free_cmatrix(DiffGray, nH, nW);

	MBgroupBackward.clear();
}


void MyFRUC::GetInterpolatedFrame(BYTE ** PrevR,BYTE ** PrevG,BYTE ** PrevB,BYTE ** CurrR,BYTE ** CurrG, BYTE** CurrB,BYTE ** InterR,BYTE ** InterG, BYTE** InterB, int nW, int nH, int sizeMB,int search_Range)
{
	Macroblock temp;
	int tSAD = 0;
	int minsad;
	BYTE **PrevGray;
	BYTE **CurrGray;
	BYTE **DiffGray;
	PrevGray=cmatrix( nH, nW);
	CurrGray =cmatrix( nH, nW);
	DiffGray =cmatrix( nH, nW);
	
	for(int y = 0;y<nH;y++)
		for(int x = 0;x<nW;x++)
		{
			PrevGray[y][x] = (PrevR[y][x]+PrevG[y][x]+PrevB[y][x])/3;
			CurrGray[y][x] = (CurrR[y][x]+CurrG[y][x]+CurrB[y][x])/3;
		}


	for(int MBy = 0 ; MBy< nH/sizeMB ;MBy++){  //현재 프레임의 MB group 생성
	
		for(int MBx = 0 ;MBx<nW/sizeMB ; MBx++)
		{
	  if(-search_Range+ MBy*sizeMB>=0 && search_Range+ MBy*sizeMB +sizeMB-1 < nH &&  -search_Range+MBx*sizeMB >= 0 && search_Range+MBx*sizeMB+sizeMB-1 <nW)
		{
//*********************************************
	  
		for(int y = -search_Range;y<=search_Range;y++)
		{
			for(int x =-search_Range;x<=search_Range;x++)
			{
				
				for(int i = 0 ;i<sizeMB;i++)
				{
					for(int j = 0 ; j<sizeMB;j++)
					{
					
						tSAD += abs(PrevGray[i+y+ MBy*sizeMB][j+x+MBx*sizeMB] - CurrGray[i+ MBy*sizeMB][j+MBx*sizeMB]);
					}
				}
				temp.po.y = y+MBy*sizeMB;
				temp.po.x = x+MBx*sizeMB;
				temp.motion.sad = tSAD;
				temp.motion.x = -x;
				temp.motion.y = -y;
				Tempgroup_MB.push_back(temp);

				tSAD = 0;
				
			}
		}

		minsad = Tempgroup_MB[0].motion.sad;
		int check = 0; //최소 SAD를 갖는 이전 frame의 MB 확인
		for(int i = 1 ;i<Tempgroup_MB.size();i++)
		{
			if(minsad>Tempgroup_MB[i].motion.sad)
			{
				minsad = Tempgroup_MB[i].motion.sad;
				check = i;
			}

		}
	//	if(Tempgroup_MB[check].motion.sad >10)
		
		Prevgroup_MB.push_back(Tempgroup_MB[check]);
		Tempgroup_MB.clear();
//******************************************
		
	  }
			//Currgroup_MB.push_back(temp);
		}
	}
	///////////////////////화면 생성 부분
	//frame의 테두리 부분 처리 -평균
	int x,y;

	//이전 프레임의 블록의 MV의 1/2 만큼 블록을 이동한다.
	

	for(int i = 0 ;i<Prevgroup_MB.size(); i++)
	{
		for(y = 0;y< sizeMB;y++ )
		{
			for(x = 0 ;x<sizeMB;x++)
			{
				InterR[Prevgroup_MB[i].po.y+(Prevgroup_MB[i].motion.y/2)+y][Prevgroup_MB[i].po.x+(Prevgroup_MB[i].motion.x/2)+x]	 = PrevR[Prevgroup_MB[i].po.y+y][Prevgroup_MB[i].po.x+x];
				InterG[Prevgroup_MB[i].po.y+(Prevgroup_MB[i].motion.y/2)+y][Prevgroup_MB[i].po.x+(Prevgroup_MB[i].motion.x/2)+x]	 = PrevG[Prevgroup_MB[i].po.y+y][Prevgroup_MB[i].po.x+x];
				InterB[Prevgroup_MB[i].po.y+(Prevgroup_MB[i].motion.y/2)+y][Prevgroup_MB[i].po.x+(Prevgroup_MB[i].motion.x/2)+x]	 = PrevB[Prevgroup_MB[i].po.y+y][Prevgroup_MB[i].po.x+x];
			}
		}
	}


	for( y = 0;y<sizeMB;y++){ // 윗부분
	
		for( x = 0;x<nW;x++)
		{
			InterR[y][x] = (PrevR[y][x]+CurrR[y][x])/2;
			InterG[y][x] = (PrevG[y][x]+CurrG[y][x])/2;
			InterB[y][x] = (PrevB[y][x]+CurrB[y][x])/2;
		}
	}
	
	for( y = sizeMB;y<nH-sizeMB;y++){ // 중간 부분1
	
		for( x = 0;x<sizeMB;x++)
		{
			InterR[y][x] = (PrevR[y][x]+CurrR[y][x])/2;
			InterG[y][x] = (PrevG[y][x]+CurrG[y][x])/2;
			InterB[y][x] = (PrevB[y][x]+CurrB[y][x])/2;
		}
	}

	for( y = sizeMB;y<nH-sizeMB;y++){ // 중간 부분2
	
		for( x = nW-sizeMB;x<nW;x++)
		{
			InterR[y][x] = (PrevR[y][x]+CurrR[y][x])/2;
			InterG[y][x] = (PrevG[y][x]+CurrG[y][x])/2;
			InterB[y][x] = (PrevB[y][x]+CurrB[y][x])/2;
		}
	}

	for( y = nH-sizeMB;y<nH;y++){ // 아래 부분
	
		for( x = 0;x<nW;x++)
		{
			InterR[y][x] = (PrevR[y][x]+CurrR[y][x])/2;
			InterG[y][x] = (PrevG[y][x]+CurrG[y][x])/2;
			InterB[y][x] = (PrevB[y][x]+CurrB[y][x])/2;
		}
	}

	

	Prevgroup_MB.clear();



	free_cmatrix(CurrGray, nH, nW);
	free_cmatrix(PrevGray, nH, nW);
	free_cmatrix(DiffGray, nH, nW);

}




void MyFRUC::Run(BYTE *Image1D, int nW, int nH, int nInterGap, int nMask, int nSearch, int nGrow, int nMeGap, int nUseColor, int nThreDiff, 
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
		for(k = 0 ; k < 7 ; k++)
		{
			m_Psnr[k] = 0;
			m_TimeDiff[k] = 0;
		}
		m_nCnt = 0;
	}

	MakeImage1DToImage2DColor(Image1D, true, nW, nH, m_CurrR, m_CurrG, m_CurrB);


	if(bFirst)
	{
		for(int y = 0 ;y< nH;y++)
			for(int x = 0 ;x<nW; x++)
			{
				m_PrevR[y][x] = m_CurrR[y][x];
				m_PrevG[y][x] = m_CurrG[y][x];
				m_PrevB[y][x] = m_CurrB[y][x];
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
				m_InterR[y][x] = 0;
				m_InterG[y][x] = 0;
				m_InterB[y][x] = 0;
			}


		QueryPerformanceFrequency((LARGE_INTEGER*)&m_TimeFreq);
		QueryPerformanceCounter((LARGE_INTEGER*)&m_TimeStart);

		


		if(nUseColor == 0)
		{
			GetFrameWithNEWBilateralME(m_PrevR,m_PrevG,m_PrevB,m_CurrR, m_CurrG, m_CurrB,m_InterR, m_InterG, m_InterB, m_nW, m_nH,nMask, nSearch,0);
		
			//GetInterpolatedFrame(m_PrevR,m_PrevG,m_PrevB,m_CurrR, m_CurrG, m_CurrB,m_InterR, m_InterG, m_InterB, m_nW, m_nH,nMask, nSearch);
			
		}
		else if(nUseColor == 1)
			/*GetColorInterFrameByMotionY(m_PrevR, m_PrevG, m_PrevB, 
				m_CurrR, m_CurrG, m_CurrB, m_InterR, m_InterG, m_InterB,
				m_nW, m_nH, nMask, nSearch, nGrow, nMeGap, nThreDiff==0?false:true, nThreDiff);*/;
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

			Msg.Format("Bilateral with OBMC %5.2lf(%5.2lf)dB\n  %5.2lf(%5.2lf)ms", PSNR, m_Psnr[3]/(m_nCnt+1), TimeDiff, m_TimeDiff[3]/(m_nCnt+1));
			DrawTextOnImag2DGray(m_InterR, nW, nH, 0, 0, Msg, DT_LEFT, 255, true);
			DrawTextOnImag2DGray(m_InterG, nW, nH, 0, 0, Msg, DT_LEFT, 0, true);
			DrawTextOnImag2DGray(m_InterB, nW, nH, 0, 0, Msg, DT_LEFT, 0, true);
		}


	

		DisplayCimage2DColor(m_InterR, m_InterG, m_InterB, m_nW, m_nH, m_nW, m_nH*2, false, true);

	
		for(y = 0 ; y < m_nH ; y++)
			for(x = 0 ; x < m_nW ; x++)
			{
				m_InterR[y][x] = 0;
				m_InterG[y][x] = 0;
				m_InterB[y][x] = 0;
			}

		//

		QueryPerformanceFrequency((LARGE_INTEGER*)&m_TimeFreq);
		QueryPerformanceCounter((LARGE_INTEGER*)&m_TimeStart);

		GetFrameWithBilateralME(m_PrevR,m_PrevG,m_PrevB,m_CurrR, m_CurrG, m_CurrB,m_InterR, m_InterG, m_InterB, m_nW, m_nH,nMask, nSearch,0);
		


		QueryPerformanceCounter((LARGE_INTEGER*)&m_TimeEnd);

		if(bViewPsnr)
		{
			CString Msg;

			TimeDiff = (double)(m_TimeEnd - m_TimeStart)/(double)m_TimeFreq*1000; 
			m_TimeDiff[4] += TimeDiff;

			if(nPsnrType == 0)
				PSNR = GetPsnrRgb(m_RefR, m_RefG, m_RefB, m_InterR, m_InterG, m_InterB, m_nW, m_nH);
			else if(nPsnrType == 1)
				PSNR = GetPsnrYuv(m_RefR, m_RefG, m_RefB, m_InterR, m_InterG, m_InterB, m_nW, m_nH);
			else if(nPsnrType == 2)
				PSNR = GetPsnrY(m_RefR, m_RefG, m_RefB, m_InterR, m_InterG, m_InterB, m_nW, m_nH);
			
			m_Psnr[4] += PSNR;

			Msg.Format("Bilateral %5.2lf(%5.2lf)dB\n  %5.2lf(%5.2lf)ms", PSNR, m_Psnr[4]/(m_nCnt+1), TimeDiff, m_TimeDiff[4]/(m_nCnt+1));
			DrawTextOnImag2DGray(m_InterR, nW, nH, 0, 0, Msg, DT_LEFT, 255, true);
			DrawTextOnImag2DGray(m_InterG, nW, nH, 0, 0, Msg, DT_LEFT, 0, true);
			DrawTextOnImag2DGray(m_InterB, nW, nH, 0, 0, Msg, DT_LEFT, 0, true);
		}

		DisplayCimage2DColor(m_InterR, m_InterG, m_InterB, m_nW, m_nH, m_nW*2, m_nH*2, false, true);





		for(y = 0 ; y < m_nH ; y++)
			for(x = 0 ; x < m_nW ; x++)
			{
				m_InterR[y][x] = 0;
				m_InterG[y][x] = 0;
				m_InterB[y][x] = 0;
			}


		QueryPerformanceFrequency((LARGE_INTEGER*)&m_TimeFreq);
		QueryPerformanceCounter((LARGE_INTEGER*)&m_TimeStart);

		GetFrameWithBilateralME2(m_PrevR,m_PrevG,m_PrevB,m_CurrR, m_CurrG, m_CurrB,m_InterR, m_InterG, m_InterB, m_nW, m_nH,nMask, nSearch,nMeGap);

		QueryPerformanceCounter((LARGE_INTEGER*)&m_TimeEnd);

		if(bViewPsnr)
		{
			CString Msg;

			TimeDiff = (double)(m_TimeEnd - m_TimeStart)/(double)m_TimeFreq*1000; 
			m_TimeDiff[5] += TimeDiff;

			if(nPsnrType == 0)
				PSNR = GetPsnrRgb(m_RefR, m_RefG, m_RefB, m_InterR, m_InterG, m_InterB, m_nW, m_nH);
			else if(nPsnrType == 1)
				PSNR = GetPsnrYuv(m_RefR, m_RefG, m_RefB, m_InterR, m_InterG, m_InterB, m_nW, m_nH);
			else if(nPsnrType == 2)
				PSNR = GetPsnrY(m_RefR, m_RefG, m_RefB, m_InterR, m_InterG, m_InterB, m_nW, m_nH);
			
			m_Psnr[5] += PSNR;

			Msg.Format("Wrong Bilateral %5.2lf(%5.2lf)dB\n  %5.2lf(%5.2lf)ms", PSNR, m_Psnr[5]/(m_nCnt+1), TimeDiff, m_TimeDiff[5]/(m_nCnt+1));
			DrawTextOnImag2DGray(m_InterR, nW, nH, 0, 0, Msg, DT_LEFT, 255, true);
			DrawTextOnImag2DGray(m_InterG, nW, nH, 0, 0, Msg, DT_LEFT, 0, true);
			DrawTextOnImag2DGray(m_InterB, nW, nH, 0, 0, Msg, DT_LEFT, 0, true);
		}

		DisplayCimage2DColor(m_InterR, m_InterG, m_InterB, m_nW, m_nH, 0, m_nH*2, false, true);




		
		for(y = 0 ; y < m_nH ; y++)
			for(x = 0 ; x < m_nW ; x++)
			{
				m_InterR[y][x] = 0;
				m_InterG[y][x] = 0;
				m_InterB[y][x] = 0;
			}


		QueryPerformanceFrequency((LARGE_INTEGER*)&m_TimeFreq);
		QueryPerformanceCounter((LARGE_INTEGER*)&m_TimeStart);

		GetFrameWithBilateralME(m_PrevR,m_PrevG,m_PrevB,m_CurrR, m_CurrG, m_CurrB,m_InterR, m_InterG, m_InterB, m_nW, m_nH,nMask, nSearch,1);

		QueryPerformanceCounter((LARGE_INTEGER*)&m_TimeEnd);

		if(bViewPsnr)
		{
			CString Msg;

			TimeDiff = (double)(m_TimeEnd - m_TimeStart)/(double)m_TimeFreq*1000; 
			m_TimeDiff[6] += TimeDiff;

			if(nPsnrType == 0)
				PSNR = GetPsnrRgb(m_RefR, m_RefG, m_RefB, m_InterR, m_InterG, m_InterB, m_nW, m_nH);
			else if(nPsnrType == 1)
				PSNR = GetPsnrYuv(m_RefR, m_RefG, m_RefB, m_InterR, m_InterG, m_InterB, m_nW, m_nH);
			else if(nPsnrType == 2)
				PSNR = GetPsnrY(m_RefR, m_RefG, m_RefB, m_InterR, m_InterG, m_InterB, m_nW, m_nH);
			
			m_Psnr[6] += PSNR;

			Msg.Format("Bilateral+vectorRefinement %5.2lf(%5.2lf)dB\n  %5.2lf(%5.2lf)ms", PSNR, m_Psnr[6]/(m_nCnt+1), TimeDiff, m_TimeDiff[6]/(m_nCnt+1));
			DrawTextOnImag2DGray(m_InterR, nW, nH, 0, 0, Msg, DT_LEFT, 255, true);
			DrawTextOnImag2DGray(m_InterG, nW, nH, 0, 0, Msg, DT_LEFT, 0, true);
			DrawTextOnImag2DGray(m_InterB, nW, nH, 0, 0, Msg, DT_LEFT, 0, true);
		}

		DisplayCimage2DColor(m_InterR, m_InterG, m_InterB, m_nW, m_nH, 0, m_nH*3, false, true);


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
		for(int y = 0 ;y< nH;y++)
			for(int x = 0 ;x<nW; x++)
			{
				m_RefR[y][x] = m_CurrR[y][x];
				m_RefG[y][x] = m_CurrG[y][x];
				m_RefB[y][x] = m_CurrB[y][x];
			}
	
	}

	m_nFrameCnt++;
}
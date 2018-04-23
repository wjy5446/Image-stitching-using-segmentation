// Bpn.cpp: implementation of the CBpn class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Bpn.h"
#include <cstdlib>
#include <ctime>
#include <cmath>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBpn::CBpn(int nLayerCntInput, int *NodeCntInput, double dGainInput)
{
	int i, j;
	nLayerCnt = nLayerCntInput;

	NodeCnt = new int[nLayerCnt];
	
	for(i = 0 ; i < nLayerCnt ; i++)
		NodeCnt[i] = NodeCntInput[i];

	Node = new double *[nLayerCnt];
	for(i = 0 ; i < nLayerCnt ; i++)
		Node[i] = new double[NodeCnt[i]];

	Weight = new double **[nLayerCnt];
	for(i = 1 ; i < nLayerCnt ; i++)	// 0: 사용하지 않음
	{
		Weight[i] = new double *[NodeCnt[i]];
		for(j = 0 ; j < NodeCnt[i] ; j++)
			Weight[i][j] = new double [NodeCnt[i-1]]; 
	}

	Delta = new double *[nLayerCnt];
	for(i = 1 ; i < nLayerCnt ; i++)	// 0: 사용하지 않음
		Delta[i] = new double[NodeCnt[i]];

	dGain = dGainInput;
}

CBpn::~CBpn()
{
	int i, j;

	for(i = 1 ; i < nLayerCnt ; i++)
		delete [] Delta[i];
	delete [] Delta;

	for(i = 1 ; i < nLayerCnt ; i++)
	{
		for(j = 0 ; j < NodeCnt[i] ; j++)
			delete [] Weight[i][j];
		delete [] Weight[i];
	}
	delete [] Weight;

	for(i = 0 ; i < nLayerCnt ; i++)
		delete [] Node[i];
	delete [] Node;

	delete [] NodeCnt;
}


void CBpn::InitWeight()
{
	int i, j, k;

	srand((unsigned int)time(NULL));

	for(i = 1 ; i < nLayerCnt ; i++)	
		for(j = 0 ; j < NodeCnt[i] ; j++)
			for(k = 0 ; k < NodeCnt[i-1] ; k++)
				Weight[i][j][k] = (double)rand()/RAND_MAX - 0.5;
}

double CBpn::ActivationFn(double x)
{
	return 1./(1.+exp(-1. * x));
}

double CBpn::DifferentialActivationFn(double x)
{
	return x*(1.-x);
}

int CBpn::ComputeNet(double *Input)
{
	int i, j, k;
	double NodeSum;
	double Max;
	int MaxPos;

	for(j = 0 ; j < NodeCnt[0] ; j++)
		Node[0][j] = Input[j];

	for(i = 1 ; i < nLayerCnt ; i++)
	{
		for(j = 0 ; j < NodeCnt[i] ; j++)
		{
			NodeSum = 0;
			for(k = 0 ; k < NodeCnt[i-1] ; k++)
				NodeSum += Node[i-1][k] * Weight[i][j][k];

			Node[i][j] = ActivationFn(NodeSum);
		}
	}

	for(j = 0 ; j < NodeCnt[nLayerCnt-1] ; j++)
	{
		if(j == 0) {
			Max = Node[nLayerCnt-1][j];
			MaxPos = 0;
		} 
		else if(Node[nLayerCnt-1][j] > Max)
		{
			Max = Node[nLayerCnt-1][j];
			MaxPos = j;
		}
	}

	return MaxPos;
}

void CBpn::ComputeDelta(double *Output)
{
	int i, j, k;

	for(i = nLayerCnt-1 ; i > 0 ; i--)
	{
		if(i == nLayerCnt-1)
		{
			for(j = 0 ; j < NodeCnt[i] ; j++)
				Delta[i][j] = (Output[j] - Node[i][j]) * DifferentialActivationFn(Node[i][j]);
		}
		else
		{
			for(j = 0 ; j < NodeCnt[i] ; j++)
			{
				Delta[i][j] = 0;
				for(k = 0 ; k < NodeCnt[i+1] ; k++)
					Delta[i][j] += Delta[i+1][k] * Weight[i+1][k][j];

				Delta[i][j] *= DifferentialActivationFn(Node[i][j]);
			}
		}
	}
}

void CBpn::UpdateWeight()
{
	int i, j, k;

	for(i = nLayerCnt-1 ; i > 0 ; i--)
	{
		for(j = 0 ; j < NodeCnt[i] ; j++)
		{
			for(k = 0 ; k < NodeCnt[i-1] ; k++)
			{
				Weight[i][j][k] += dGain * Delta[i][j] * Node[i-1][k];
			}
		}
	}
}

int CBpn::Train(double *Input, double *Output)
{
	int MaxPos = ComputeNet(Input);
	
	ComputeDelta(Output);
	UpdateWeight();

	return MaxPos;
}

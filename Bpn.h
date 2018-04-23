// Bpn.h: interface for the CBpn class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BPN_H__CB0F7ED8_8AD4_4FD5_B5D9_C60C939F5548__INCLUDED_)
#define AFX_BPN_H__CB0F7ED8_8AD4_4FD5_B5D9_C60C939F5548__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CBpn  
{
private:
	int nLayerCnt;		// 층의 개수: 입력 + 출력 + 은닉
	int *NodeCnt;		// 층당 노드 수: NodeCnt[층번호]
	double **Node;		// 노드: Node[층번호][노드 번호]
	double ***Weight;	// 가중치: Weight[층번호][노드번호1][노드번호2]
						// 노드번호1은 현재 층의 노드 번호, 노드 번호2는 이전 층의 노드 번호
	double **Delta;		// delta: Delta[층번호][노드번호]
	double dGain;		// 이득

public:
	CBpn(int nLayerCntInput, int *NodeCntInput, double dGainInput);	// 생성자: 층수, 노드수, 이득 입력 + 메모리 할당

	static double ActivationFn(double x);				// 활성 함수
	static double DifferentialActivationFn(double x);	// 활성 함수의 미분

	void InitWeight();								// 가중치 초기화
	int ComputeNet(double *Input);					// 입력에 대한 출력 계산
	void ComputeDelta(double *Output);				// Delta 계산
	void UpdateWeight();							// 가중치 갱신
	int Train(double *Input, double *Output);		// 훈련: ComputeNet + ComputeDelta + UpdateDelta
	virtual ~CBpn();

};

#endif // !defined(AFX_BPN_H__CB0F7ED8_8AD4_4FD5_B5D9_C60C939F5548__INCLUDED_)

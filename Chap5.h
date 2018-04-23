#if !defined(AFX_CHAP5_H__9DFE9431_17F2_4381_8EFC_20BC4E87B438__INCLUDED_)
#define AFX_CHAP5_H__9DFE9431_17F2_4381_8EFC_20BC4E87B438__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Chap5.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CChap5 dialog

class CChap5 : public CDialog
{
// Construction
public:
	CChap5(CWnd* pParent = NULL);   // standard constructor

	BOOL m_bCreated;

// Dialog Data
	//{{AFX_DATA(CChap5)
	enum { IDD = IDD_CHAP5 };
	CComboBox	m_ShapeCtrl;
	CComboBox	m_UpDownCtrl;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChap5)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CChap5)
	virtual BOOL OnInitDialog();
	afx_msg void OnCancel();
	afx_msg void OnThresholdingPtile();
	afx_msg void OnThresholdingValley();
	afx_msg void OnThresholdingIterate();
	afx_msg void OnHough();
	afx_msg void OnTurtle();
	afx_msg void OnKCosine();
	afx_msg void OnDilation();
	afx_msg void OnErosion();
	afx_msg void OnClose();
	afx_msg void OnOpen();
	afx_msg void OnLabeling();
	afx_msg void OnThinning();
	afx_msg void OnLabelingMoment();
	afx_msg void OnMeanShift();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHAP5_H__9DFE9431_17F2_4381_8EFC_20BC4E87B438__INCLUDED_)

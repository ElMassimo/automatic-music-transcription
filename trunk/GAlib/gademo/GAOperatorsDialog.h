#if !defined(AFX_GAOPERATORSDIALOG_H__CF762341_A631_11D2_B853_00104B6A8728__INCLUDED_)
#define AFX_GAOPERATORSDIALOG_H__CF762341_A631_11D2_B853_00104B6A8728__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// GAOperatorsDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGAOperatorsDialog dialog

class CGAOperatorsDialog : public CDialog
{
// Construction
public:
	CGAOperatorsDialog(CWnd* pParent = NULL);   // standard constructor
    BOOL Create();
	void configure();

	int m_cross;
	int m_mut;

// Dialog Data
	//{{AFX_DATA(CGAOperatorsDialog)
	enum { IDD = IDD_OPERATORS };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGAOperatorsDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CWnd* m_pParent;

	// Generated message map functions
	//{{AFX_MSG(CGAOperatorsDialog)
	afx_msg void OnApply();
	afx_msg void OnClose();
	afx_msg void OnRevert();
	afx_msg void OnSelectCrossAverage();
	afx_msg void OnSelectCrossBLX();
	afx_msg void OnSelectCrossOnePT();
	afx_msg void OnSelectCrossTwoPT();
	afx_msg void OnSelectCrossUniform();
	afx_msg void OnSelectMutBoundary();
	afx_msg void OnSelectMutFlip();
	afx_msg void OnSelectMutGaussian();
	afx_msg void OnSelectMutUniform();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GAOPERATORSDIALOG_H__CF762341_A631_11D2_B853_00104B6A8728__INCLUDED_)

#if !defined(AFX_GAPARAMETERSDIALOG_H__78558003_9865_11D2_836E_00C04F8F199B__INCLUDED_)
#define AFX_GAPARAMETERSDIALOG_H__78558003_9865_11D2_836E_00C04F8F199B__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// GAParametersDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGAParametersDialog dialog

class CGAParametersDialog : public CDialog
{
// Construction
public:
	CGAParametersDialog(CWnd* pParent = NULL);   // standard constructor
    BOOL Create();

// Dialog Data
	//{{AFX_DATA(CGAParametersDialog)
	enum { IDD = IDD_FORMVIEW };
	float	m_pcross;
	float	m_pmut;
	int		m_popsize;
	int		m_ngen;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGAParametersDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CWnd* m_pParent;


	// Generated message map functions
	//{{AFX_MSG(CGAParametersDialog)
	afx_msg void OnRevert();
	afx_msg void OnApply();
	afx_msg void OnClose();
	afx_msg void OnChangePmut();
	afx_msg void OnChangePcross();
	afx_msg void OnChangeNgen();
	afx_msg void OnChangePopsize();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GAPARAMETERSDIALOG_H__78558003_9865_11D2_836E_00C04F8F199B__INCLUDED_)

// gademo.h : main header file for the GADEMO application
//

#if !defined(AFX_GADEMO_H__27ACB975_9765_11D2_836E_00C04F8F199B__INCLUDED_)
#define AFX_GADEMO_H__27ACB975_9765_11D2_836E_00C04F8F199B__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CGademoApp:
// See gademo.cpp for the implementation of this class
//

class CGademoApp : public CWinApp
{
public:
	CGademoApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGademoApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CGademoApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GADEMO_H__27ACB975_9765_11D2_836E_00C04F8F199B__INCLUDED_)

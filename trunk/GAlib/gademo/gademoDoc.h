// gademoDoc.h : interface of the CGademoDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_GADEMODOC_H__27ACB97B_9765_11D2_836E_00C04F8F199B__INCLUDED_)
#define AFX_GADEMODOC_H__27ACB97B_9765_11D2_836E_00C04F8F199B__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


class CGademoDoc : public CDocument
{
protected: // create from serialization only
	CGademoDoc();
	DECLARE_DYNCREATE(CGademoDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGademoDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CGademoDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CGademoDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GADEMODOC_H__27ACB97B_9765_11D2_836E_00C04F8F199B__INCLUDED_)

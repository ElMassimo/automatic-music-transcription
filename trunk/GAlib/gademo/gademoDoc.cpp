// gademoDoc.cpp : implementation of the CGademoDoc class
//

#include "stdafx.h"
#include "gademo.h"

#include "gademoDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGademoDoc

IMPLEMENT_DYNCREATE(CGademoDoc, CDocument)

BEGIN_MESSAGE_MAP(CGademoDoc, CDocument)
	//{{AFX_MSG_MAP(CGademoDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGademoDoc construction/destruction

CGademoDoc::CGademoDoc()
{
	// TODO: add one-time construction code here

}

CGademoDoc::~CGademoDoc()
{
}

BOOL CGademoDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CGademoDoc serialization

void CGademoDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CGademoDoc diagnostics

#ifdef _DEBUG
void CGademoDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CGademoDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CGademoDoc commands

// MapEditDoc.cpp : implementation of the CMapEditDoc class
//

#include "stdafx.h"
#include "MapEdit.h"

#include "MapEditDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMapEditDoc

IMPLEMENT_DYNCREATE(CMapEditDoc, CDocument)

BEGIN_MESSAGE_MAP(CMapEditDoc, CDocument)
	//{{AFX_MSG_MAP(CMapEditDoc)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMapEditDoc construction/destruction

CMapEditDoc::CMapEditDoc() 
{
}

CMapEditDoc::~CMapEditDoc()
{
}

BOOL CMapEditDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	TRACE0("New Doc\n");

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}

BOOL CMapEditDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	TRACE1("Load Doc %s\n",lpszPathName);
	
	// TODO: Add your specialized creation code here
	
	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CMapEditDoc serialization

void CMapEditDoc::Serialize(CArchive& ar)
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
// CMapEditDoc diagnostics

#ifdef _DEBUG
void CMapEditDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMapEditDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

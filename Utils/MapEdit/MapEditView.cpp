// MapEditView.cpp : implementation of the CMapEditView class
//

#include "stdafx.h"
#include "MapEdit.h"

#include "MapEditDoc.h"
#include "MapEditView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMapEditView

IMPLEMENT_DYNCREATE(CMapEditView, CView)

BEGIN_MESSAGE_MAP(CMapEditView, CView)
	//{{AFX_MSG_MAP(CMapEditView)
	ON_WM_SETFOCUS()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMapEditView construction/destruction

CMapEditView::CMapEditView()
{
	// TODO: add construction code here

}

CMapEditView::~CMapEditView()
{
}

BOOL CMapEditView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CMapEditView drawing

void CMapEditView::OnDraw(CDC* pDC)
{
	CMapEditDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CMapEditView printing

BOOL CMapEditView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CMapEditView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CMapEditView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CMapEditView diagnostics

#ifdef _DEBUG
void CMapEditView::AssertValid() const
{
	CView::AssertValid();
}

void CMapEditView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMapEditDoc* CMapEditView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMapEditDoc)));
	return (CMapEditDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMapEditView message handlers

void CMapEditView::OnSetFocus(CWnd* pOldWnd) 
{
CMapEditDoc	*CurDoc=GetDocument();
	CView::OnSetFocus(pOldWnd);
	theApp.SetCurrent(CurDoc);

	CurDoc->UpdateAll();	// woohoo, that was easy

	// TODO: Add your message handler code here
	
}

// MapEditView.cpp : implementation of the CMapEditView class
//

#include	"stdafx.h"
#include	<gl\gl.h>
#include	<gl\glu.h>
#include	"GLEnabledView.h"
#include	"MapEdit.h"

#include	"MapEditDoc.h"
#include	"MapEditView.h"
#include	"MainFrm.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMapEditView

IMPLEMENT_DYNCREATE(CMapEditView, CGLEnabledView)

BEGIN_MESSAGE_MAP(CMapEditView, CGLEnabledView)
	//{{AFX_MSG_MAP(CMapEditView)
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_MBUTTONDOWN()
	ON_WM_MBUTTONUP()
	ON_WM_MOUSEWHEEL()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_SETFOCUS()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMapEditView construction/destruction

CMapEditView::CMapEditView()
{
}

CMapEditView::~CMapEditView()
{
}

/////////////////////////////////////////////////////////////////////////////
void CMapEditView::VideoMode(ColorsNumber & c, ZAccuracy & z, BOOL & dbuf)
{
	c=MILLIONS;
	z=NORMAL;
	dbuf=TRUE;
}

/////////////////////////////////////////////////////////////////////////////
void CMapEditView::OnCreateGL()
{
		glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
		glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				// Background Color
		glClearDepth(1.0f);									// Depth Buffer Setup
		glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
		glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations
		glEnable(GL_BLEND);									// Enable Alpha Channel
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	// Alpha Blend Style
}


/////////////////////////////////////////////////////////////////////////////

void CMapEditView::OnDrawGL()
{
	GetDocument()->Render();
}

/////////////////////////////////////////////////////////////////////////////
void CMapEditView::OnSizeGL(int cx, int cy)
{
//		cy=cx/1.5f;
		glViewport(0,0,cx,cy);

// update the camera
 		glPushMatrix();
			glMatrixMode(GL_PROJECTION);
				glLoadIdentity();
				SetupPersMatrix();
			glMatrixMode(GL_MODELVIEW);
		glPopMatrix();
		GetDocument()->UpdateView();
}

/////////////////////////////////////////////////////////////////////////////
void	CMapEditView::SetupPersMatrix()
{
	gluPerspective(40.0,m_dAspectRatio,0.1f, 100.0f);
	glTranslatef(0.0f,0.0f,-1.f);
}


/////////////////////////////////////////////////////////////////////////////
// CMapEditView diagnostics

#ifdef _DEBUG
void CMapEditView::AssertValid() const
{
	CGLEnabledView::AssertValid();
}

void CMapEditView::Dump(CDumpContext& dc) const
{
	CGLEnabledView::Dump(dc);
}

CMapEditDoc* CMapEditView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMapEditDoc)));
	return (CMapEditDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMapEditView message handlers

void CMapEditView::OnLButtonDown(UINT nFlags, CPoint point)				{GetDocument()->LButtonControl(nFlags,point,TRUE);}
void CMapEditView::OnLButtonUp(UINT nFlags, CPoint point)				{GetDocument()->LButtonControl(nFlags,point,FALSE);}
void CMapEditView::OnMButtonDown(UINT nFlags, CPoint point)				{GetDocument()->MButtonControl(nFlags,point,TRUE);}
void CMapEditView::OnMButtonUp(UINT nFlags, CPoint point)				{GetDocument()->MButtonControl(nFlags,point,FALSE);}
BOOL CMapEditView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)	{GetDocument()->MouseWheel(nFlags,zDelta,pt) ;return(0);}
void CMapEditView::OnRButtonDown(UINT nFlags, CPoint point)				{GetDocument()->RButtonControl(nFlags,point,TRUE);}
void CMapEditView::OnRButtonUp(UINT nFlags, CPoint point)				{GetDocument()->RButtonControl(nFlags,point,FALSE);}
void CMapEditView::OnMouseMove(UINT nFlags, CPoint point)				{GetDocument()->MouseMove(nFlags, point);}


void CMapEditView::OnSetFocus(CWnd* pOldWnd) 
{
		CGLEnabledView::OnSetFocus(pOldWnd);
		theApp.SetCurrent(GetDocument());
}

void CMapEditView::OnInitialUpdate() 
{
	CGLEnabledView::OnInitialUpdate();
	GetDocument()->SetView(this);	
	
}


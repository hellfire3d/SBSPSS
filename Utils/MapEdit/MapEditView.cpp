// MapEditView.cpp : implementation of the CMapEditView class
//

#include	"stdafx.h"
#include	<gl\gl.h>
#include	<gl\glu.h>
#include	<gl\glut.h>
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
	ON_WM_SETFOCUS()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_MBUTTONDOWN()
	ON_WM_MBUTTONUP()
	ON_WM_MOUSEWHEEL()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_TOGGLE_TILEVIEW, OnToggleTileview)
	ON_COMMAND(ID_TOOLBAR_GRID, OnToggleGrid)
	ON_COMMAND(ID_MIRRORX, OnMirrorx)
	ON_COMMAND(ID_MIRRORY, OnMirrory)
	ON_COMMAND(ID_ACTIVEBRUSH_LEFT, OnActivebrushLeft)
	ON_COMMAND(ID_ACTIVEBRUSH_RIGHT, OnActivebrushRight)
	ON_COMMAND(ID_MAP_SETSIZE, OnMapSetSize)
	ON_COMMAND(ID_2D_3D_TOGGLE, On2d3dToggle)
	ON_COMMAND(ID_TOOLBAR_TILEPALETTE, OnToggleTileview)
	ON_COMMAND(ID_TOGGLE_GRID, OnToggleGrid)
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
	c=MILLIONS;	// ask for 65355 colors...
	z=NORMAL;		// ...16 bit Z-buffer...
	dbuf=TRUE;		// ...double-buffering
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
	GetDocument()->Render(this);
}

/////////////////////////////////////////////////////////////////////////////
void CMapEditView::OnSizeGL(int cx, int cy)
{
		glViewport(0,0,cx,cy);

// update the camera
 		glPushMatrix();
			glMatrixMode(GL_PROJECTION);
				glLoadIdentity();
				SetupPersMatrix();
			glMatrixMode(GL_MODELVIEW);
		glPopMatrix();
		GetDocument()->UpdateView(this);
}

/////////////////////////////////////////////////////////////////////////////
void	CMapEditView::SetupPersMatrix()
{
	gluPerspective(40.0,m_dAspectRatio,0.1f, 100.0f);
	glTranslatef(0.0f,0.0f,-4.f);

//	GLint viewport[4];

//	glGetIntegerv( GL_VIEWPORT, viewport );
//	glMatrixMode( GL_PROJECTION );
//	glLoadIdentity();
//	glOrtho( 0.0f, viewport[2]-viewport[0],viewport[3]-viewport[1], 0.0f, 0.1f, 100.0f);
//	glOrtho( 0.0f, +30,+20, 0, 0.1f, 100.0f);
//	glTranslatef(0.0f,0.0f,-0.f);

//	glMatrixMode( GL_MODELVIEW );
//	glLoadIdentity();
//	glTranslatef( 0.375, 0.375, 0.0 );

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

void CMapEditView::OnSetFocus(CWnd* pOldWnd) 
{
CMapEditDoc	*CurDoc=GetDocument();
	CGLEnabledView::OnSetFocus(pOldWnd);
	theApp.SetCurrent(CurDoc);
	CurDoc->UpdateAll(this);
//	CurDoc->UpdateAllViews(this);
}

/*********************************************************************************/
/*********************************************************************************/
/*********************************************************************************/
void CMapEditView::OnLButtonDown(UINT nFlags, CPoint point)				{GetDocument()->LButtonControl(this,nFlags,point,TRUE);}
void CMapEditView::OnLButtonUp(UINT nFlags, CPoint point)				{GetDocument()->LButtonControl(this,nFlags,point,FALSE);}
void CMapEditView::OnMButtonDown(UINT nFlags, CPoint point)				{GetDocument()->MButtonControl(this,nFlags,point,TRUE);}
void CMapEditView::OnMButtonUp(UINT nFlags, CPoint point)				{GetDocument()->MButtonControl(this,nFlags,point,FALSE);}
BOOL CMapEditView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)	{GetDocument()->MouseWheel(this,nFlags,zDelta,pt) ;return(0);}
void CMapEditView::OnRButtonDown(UINT nFlags, CPoint point)				{GetDocument()->RButtonControl(this,nFlags,point,TRUE);}
void CMapEditView::OnRButtonUp(UINT nFlags, CPoint point)				{GetDocument()->RButtonControl(this,nFlags,point,FALSE);}
void CMapEditView::OnMouseMove(UINT nFlags, CPoint point)				{GetDocument()->MouseMove(this,nFlags, point);}
void CMapEditView::OnToggleTileview()									{GetDocument()->ToggleTileView(this);}
void CMapEditView::OnToggleGrid()										{GetDocument()->ToggleGrid(this);}
void CMapEditView::OnMirrorx()											{GetDocument()->MirrorX(this);}
void CMapEditView::OnMirrory()											{GetDocument()->MirrorY(this);}

void CMapEditView::OnActivebrushLeft()									{GetDocument()->ActiveBrushLeft(this);}
void CMapEditView::OnActivebrushRight() 								{GetDocument()->ActiveBrushRight(this);}

void CMapEditView::OnMapSetSize()			 							{GetDocument()->MapSetSize(this);}

void CMapEditView::On2d3dToggle()										{GetDocument()->Toggle2d3d(this);}


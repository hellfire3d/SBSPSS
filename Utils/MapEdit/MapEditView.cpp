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
	ON_COMMAND(ID_TOOLBAR_LAYERBAR, OnToolbarLayerbar)
	ON_COMMAND(ID_TOOLBAR_TILEPALETTE, OnToolbarTilepalette)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_CURSORXY, OnStatusCursorXY)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMapEditView construction/destruction

CMapEditView::CMapEditView()
{
CMainFrame	*Frm=(CMainFrame*)AfxGetApp()->GetMainWnd();
CDialogBar	*LayerBar=Frm->GetLayerBar();
	
}

CMapEditView::~CMapEditView()
{
}

/////////////////////////////////////////////////////////////////////////////
void CMapEditView::VideoMode(ColorsNumber & c, ZAccuracy & z, BOOL & dbuf)
{
	c=THOUSANDS;	// ask for 65355 colors...
	z=NORMAL;		// ...16 bit Z-buffer...
	dbuf=TRUE;		// ...double-buffering
}

/////////////////////////////////////////////////////////////////////////////
void CMapEditView::OnCreateGL()
{
		glEnable(GL_TEXTURE_2D);       // Enable Texture Mapping
		glShadeModel(GL_SMOOTH);       // Enable Smooth Shading
		glClearColor(0.0f, 0.0f, 0.0f, 0.5f);    // Black Background
		glClearDepth(1.0f);         // Depth Buffer Setup
		glEnable(GL_DEPTH_TEST);       // Enables Depth Testing
		glDepthFunc(GL_LEQUAL);        // The Type Of Depth Testing To Do
		glEnable(GL_LIGHT0);        // Quick And Dirty Lighting (Assumes Light0 Is SetUp)
		glEnable(GL_LIGHTING);        // Enable Lighting
		glEnable(GL_COLOR_MATERIAL);      // Enable Material Coloring
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); // Really Nice Perspective Calculations

		Core.Init(this);

}


/////////////////////////////////////////////////////////////////////////////
void CMapEditView::OnDrawGL()
{
		Core.Render();
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
		Core.Redraw();
}

/////////////////////////////////////////////////////////////////////////////
void CMapEditView::SetupPersMatrix()
{
		gluPerspective(40.0,m_dAspectRatio,0.1f, 100.0f);
		glTranslatef(0.0f,0.0f,-4.f);

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
	UpdateAll();
}

/*********************************************************************************/
/*********************************************************************************/
/*********************************************************************************/
void CMapEditView::UpdateAll() 
{
	Core.UpdateAll();
}

/*********************************************************************************/
void CMapEditView::OnStatusCursorXY(CCmdUI *pCmdUI)
{
CPoint	&XY=Core.GetCursorPos();
CString XYStr;
		pCmdUI->Enable();
		if (XY.x!=-1 && XY.y!=-1)
			XYStr.Format( "%d\t%d", XY.x,XY.y);
		pCmdUI->SetText(XYStr); 
}

/*********************************************************************************/
/*********************************************************************************/
/*********************************************************************************/
void CMapEditView::OnLButtonDown(UINT nFlags, CPoint point)				{Core.LButtonControl(nFlags,point,TRUE);}
void CMapEditView::OnLButtonUp(UINT nFlags, CPoint point)				{Core.LButtonControl(nFlags,point,FALSE);}
void CMapEditView::OnMButtonDown(UINT nFlags, CPoint point)				{Core.MButtonControl(nFlags,point,TRUE);}
void CMapEditView::OnMButtonUp(UINT nFlags, CPoint point)				{Core.MButtonControl(nFlags,point,FALSE);}
BOOL CMapEditView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)	{Core.MouseWheel(nFlags,zDelta,pt) ;return(0);}
void CMapEditView::OnRButtonDown(UINT nFlags, CPoint point)				{Core.RButtonControl(nFlags,point,TRUE);}
void CMapEditView::OnRButtonUp(UINT nFlags, CPoint point)				{Core.RButtonControl(nFlags,point,FALSE);}
void CMapEditView::OnMouseMove(UINT nFlags, CPoint point)				{Core.MouseMove(nFlags, point);}

void CMapEditView::OnToolbarLayerbar()									{Core.ToggleLayerView();}
void CMapEditView::OnToolbarTilepalette()								{Core.ToggleTileView();}



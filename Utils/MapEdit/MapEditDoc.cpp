// MapEditDoc.cpp : implementation of the CMapEditDoc class
//

#include "stdafx.h"
#include "MapEdit.h"

#include "MapEditDoc.h"
#include "MapSizeDlg.h"

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
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_CURSORXY, OnStatusCursorXY)
	ON_COMMAND(ID_EXPORT, OnExport)
	ON_COMMAND(ID_ZOOM_IN, OnZoomIn)
	ON_COMMAND(ID_ZOOM_OUT, OnZoomOut)
	ON_COMMAND(ID_TOGGLE_TILEVIEW, OnToggleTileview)
	ON_COMMAND(ID_TOOLBAR_GRID, OnToggleGrid)
	ON_COMMAND(ID_MIRRORX, OnMirrorx)
	ON_COMMAND(ID_MIRRORY, OnMirrory)
	ON_COMMAND(ID_ACTIVEBRUSH_LEFT, OnActivebrushLeft)
	ON_COMMAND(ID_ACTIVEBRUSH_RIGHT, OnActivebrushRight)
	ON_COMMAND(ID_MAP_SETSIZE, OnMapSetSize)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_COMMAND(ID_TOOLBAR_TILEPALETTE, OnToggleTileview)
	ON_COMMAND(ID_TOGGLE_GRID, OnToggleGrid)
	ON_COMMAND(ID_2D_3D_TOGGLE, On2d3dToggle)
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
	if (!CDocument::OnNewDocument()) return FALSE;

	return(Core.New());

//	return TRUE;
}

BOOL CMapEditDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	return TRUE;
}

void CMapEditDoc::OnCloseDocument() 
{
		Core.GUIRemoveAll();	
		CDocument::OnCloseDocument();
}


/////////////////////////////////////////////////////////////////////////////
// CMapEditDoc serialization
void CMapEditDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		Core.Save(ar.GetFile());
	}
	else
	{
		Core.Load(ar.GetFile());
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

/*********************************************************************************/
/*********************************************************************************/
/*********************************************************************************/
/*********************************************************************************/
void	CMapEditDoc::SetView(CMapEditView *View)
{
//CMapEditView *LastView=Core.GetView();
		Core.SetView(View);
//		if (LastView!=View) 
			UpdateView();
}

/*********************************************************************************/
CMapEditView	*CMapEditDoc::GetView()
{
		return(Core.GetView());
}

/*********************************************************************************/
void	CMapEditDoc::UpdateView()
{
		Core.UpdateView();
}

/*********************************************************************************/
void	CMapEditDoc::Render()	
{
		Core.Render();
}

/*********************************************************************************/
void	CMapEditDoc::UpdateAll()
{
		Core.UpdateAll();
}

/*********************************************************************************/
void	CMapEditDoc::OnStatusCursorXY(CCmdUI *pCmdUI)
{
CPoint	&XY=Core.GetCursorPos();
CString XYStr;
		pCmdUI->Enable();
		if (XY.x!=-1 && XY.y!=-1)
			XYStr.Format( "%d   %d", XY.x,XY.y);
		pCmdUI->SetText(XYStr); 
}

/*********************************************************************************/
void	CMapEditDoc::GUIUpdate()
{
		Core.GUIUpdate();

}

/*********************************************************************************/
void	CMapEditDoc::GUIChanged()
{
		Core.GUIChanged();
		UpdateView();
}

/*********************************************************************************/
/*********************************************************************************/
/*** Windows Message Handlers ****************************************************/
/*********************************************************************************/
/*********************************************************************************/
void	CMapEditDoc::LButtonControl(UINT nFlags, CPoint &point,BOOL DownFlag)	{Core.LButtonControl(nFlags,point,DownFlag);}
void	CMapEditDoc::MButtonControl(UINT nFlags, CPoint &point,BOOL DownFlag)	{Core.MButtonControl(nFlags,point,DownFlag);}
void	CMapEditDoc::RButtonControl(UINT nFlags, CPoint &point,BOOL DownFlag)	{Core.RButtonControl(nFlags,point,DownFlag);}
void	CMapEditDoc::MouseWheel(UINT nFlags, short zDelta, CPoint &point)		{Core.MouseWheel(nFlags,zDelta,point);}
void	CMapEditDoc::MouseMove(UINT nFlags, CPoint &point)						{Core.MouseMove(nFlags,point);}

void	CMapEditDoc::OnToggleTileview()											{Core.UpdateTileView(true); FocusView();}
void	CMapEditDoc::OnToggleGrid()												{Core.UpdateGrid(TRUE); FocusView();}
void	CMapEditDoc::OnMirrorx()												{Core.MirrorX(); FocusView();}
void	CMapEditDoc::OnMirrory()												{Core.MirrorY();FocusView();}
void	CMapEditDoc::OnEditCopy()												{Core.CopySelection(); FocusView();}
void	CMapEditDoc::OnEditPaste()												{Core.PasteSelection(); FocusView(); }
void	CMapEditDoc::OnActivebrushLeft()										{Core.ActiveBrushLeft();}
void	CMapEditDoc::OnActivebrushRight() 										{Core.ActiveBrushRight();}
void	CMapEditDoc::On2d3dToggle()												{Core.Toggle2d3d();}
void	CMapEditDoc::OnZoomIn()													{Core.Zoom(-0.1f); UpdateView();}
void	CMapEditDoc::OnZoomOut()												{Core.Zoom(+0.1f); UpdateView();}


/*********************************************************************************/
/*********************************************************************************/
/*********************************************************************************/
void	CMapEditDoc::SetMode(int NewMode)
{
		Core.SetMode(NewMode);
		Core.GUIUpdate();
		FocusView();
}

/*********************************************************************************/
void	CMapEditDoc::SetLayer(int Layer)
{
		Core.SetLayer(Layer);
		FocusView();
}

/*********************************************************************************/
void	CMapEditDoc::AddLayer(int Layer)
{
		Core.AddLayer(Layer);
		FocusView();
}

/*********************************************************************************/
void	CMapEditDoc::DeleteLayer(int Layer)
{
		Core.DeleteLayer(Layer);
		FocusView();
}

/*********************************************************************************/
/*********************************************************************************/
/*********************************************************************************/
void CMapEditDoc::OnExport() 
{
char		BASED_CODE Filter[]= "Export Data Type (*.MEX)|*.MEX|All Files (*.*)|*.*||";
CFileDialog	Dlg(FALSE,"*.MEX",NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,Filter);

		if (Dlg.DoModal()!=IDOK) return;

char	Filename[256];
		sprintf(Filename,"%s",Dlg.GetPathName());
		
		Core.Export(Filename);
}

/*********************************************************************************/
/*** Tilebank Functions **********************************************************/
/*********************************************************************************/
void	CMapEditDoc::TileBankLoad()
{
char		BASED_CODE GinFilter[]= "All Tile Files (*.Gin; *.Bmp)|*.gin;*.Bmp|3d Tile Files (*.Gin)|*.Gin|2d Tile Files (*.Bmp)|*.Bmp|All Files (*.*)|*.*||";
CFileDialog	Dlg(TRUE,"Gin",NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,GinFilter);

		if (Dlg.DoModal()!=IDOK) return;

char	Filename[256];
		sprintf(Filename,"%s",Dlg.GetPathName());
		Core.TileBankLoad(Filename);
		UpdateView();
		FocusView();
}

/*********************************************************************************/
void	CMapEditDoc::TileBankDelete()
{
		Core.TileBankDelete();
		UpdateView();
		FocusView();
}

/*********************************************************************************/
void	CMapEditDoc::TileBankReload()
{
		Core.TileBankReload();
		UpdateView();
		FocusView();
}
/*********************************************************************************/
void	CMapEditDoc::TileBankSet()
{
		Core.TileBankSet();
		UpdateView();
		FocusView();
}

/*********************************************************************************/
void	CMapEditDoc::OnMapSetSize()
{
CMapSizeDlg	Dlg;

		Dlg.m_Width=Core.GetMapWidth();
		Dlg.m_Height=Core.GetMapHeight();
		
		if (Dlg.DoModal()!=IDOK) return;

		Core.SetMapSize(Dlg.m_Width,Dlg.m_Height);
}


/*********************************************************************************/
void	CMapEditDoc::FocusView()
{
		theApp.GetMainWnd()->SetFocus();		// Put control back to Window :o)
}


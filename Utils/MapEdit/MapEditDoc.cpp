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
	ON_COMMAND(ID_EXPORT_AGB, OnExportAgb)
	ON_COMMAND(ID_EXPORT_PSX, OnExportPsx)
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
	TRACE0("New Doc\n");

	return(Core.New());

//	return TRUE;
}

BOOL CMapEditDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	TRACE1("Load Doc %s\n",lpszPathName);

	return TRUE;
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
void	CMapEditDoc::UpdateView(CMapEditView *View)
{
		Core.UpdateView(View);
}

/*********************************************************************************/
void	CMapEditDoc::Render(CMapEditView *View)	
{
		Core.Render(View);
}

/*********************************************************************************/
void	CMapEditDoc::UpdateAll(CMapEditView *View)
{
		Core.UpdateAll(View);
}

/*********************************************************************************/
void	CMapEditDoc::OnStatusCursorXY(CCmdUI *pCmdUI)
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
/*** Windows Message Handlers ****************************************************/
/*********************************************************************************/
/*********************************************************************************/
void	CMapEditDoc::LButtonControl(CMapEditView *View,UINT nFlags, CPoint &point,BOOL DownFlag)
{
	Core.LButtonControl(View,nFlags,point,DownFlag);
}

/*********************************************************************************/
void	CMapEditDoc::MButtonControl(CMapEditView *View,UINT nFlags, CPoint &point,BOOL DownFlag)
{
	Core.MButtonControl(View,nFlags,point,DownFlag);
}

/*********************************************************************************/
void	CMapEditDoc::RButtonControl(CMapEditView *View,UINT nFlags, CPoint &point,BOOL DownFlag)
{
	Core.RButtonControl(View,nFlags,point,DownFlag);
}

/*********************************************************************************/
void	CMapEditDoc::MouseWheel(CMapEditView *View,UINT nFlags, short zDelta, CPoint &point)
{
	Core.MouseWheel(View,nFlags,zDelta,point);
}

/*********************************************************************************/
void	CMapEditDoc::MouseMove(CMapEditView *View,UINT nFlags, CPoint &point)
{
		Core.MouseMove(View,nFlags,point);
}

/*********************************************************************************/
void	CMapEditDoc::ToggleTileView(CMapEditView *View)
{
		Core.UpdateTileView(View,TRUE);
		Core.UpdateAll(View);
		FocusView();
}

/*********************************************************************************/
void	CMapEditDoc::ToggleGrid(CMapEditView *View)
{
		Core.UpdateGrid(View,TRUE);
		FocusView();
}

/*********************************************************************************/
void	CMapEditDoc::MirrorX(CMapEditView *View)
{
		Core.MirrorX(View);
		FocusView();
}

/*********************************************************************************/
void	CMapEditDoc::MirrorY(CMapEditView *View)
{
		Core.MirrorY(View);
		FocusView();
}

/*********************************************************************************/
void	CMapEditDoc::SetMode(int NewMode)
{
		Core.SetMode(NewMode);
		FocusView();
}

/*********************************************************************************/
void	CMapEditDoc::SetLayer(int Layer)
{
		Core.SetLayer(Layer);
		FocusView();
}

/*********************************************************************************/
/*********************************************************************************/
/*********************************************************************************/
void CMapEditDoc::OnExportAgb() 
{
char		BASED_CODE AGBFilter[]= "AGB Data Type (*.c)|*.c|All Files (*.*)|*.*||";
CFileDialog	Dlg(FALSE,"*.c",NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,AGBFilter);

		if (Dlg.DoModal()!=IDOK) return;

char	Filename[256];
		sprintf(Filename,"%s",Dlg.GetPathName());
		

		Core.ExportAGB(Filename);
}

/*********************************************************************************/
void CMapEditDoc::OnExportPsx() 
{
	
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
		UpdateAllViews(NULL);
		FocusView();
}

/*********************************************************************************/
void	CMapEditDoc::TileBankReload()
{
		Core.TileBankReload();
		UpdateAllViews(NULL);
		FocusView();
}

/*********************************************************************************/
void	CMapEditDoc::TileBankSet()
{
		Core.TileBankSet();
		UpdateAllViews(NULL);
		FocusView();
}

/*********************************************************************************/
void	CMapEditDoc::ActiveBrushLeft(CMapEditView *View)
{
		Core.ActiveBrushLeft(View);
}

/*********************************************************************************/
void	CMapEditDoc::ActiveBrushRight(CMapEditView *View)
{
		Core.ActiveBrushRight(View);
}

/*********************************************************************************/
void	CMapEditDoc::MapSetSize(CMapEditView *View)
{
CMapSizeDlg	Dlg;

		Dlg.m_Width=Core.GetMapWidth();
		Dlg.m_Height=Core.GetMapHeight();
		
		if (Dlg.DoModal()!=IDOK) return;

		Core.SetMapSize(View,Dlg.m_Width,Dlg.m_Height);
}

/*********************************************************************************/
void	CMapEditDoc::Toggle2d3d(CMapEditView *View)
{
	Core.Toggle2d3d(View);
}

/*********************************************************************************/
void	CMapEditDoc::FocusView()
{
		theApp.GetMainWnd()->SetFocus();		// Put control back to Window :o)
		
}
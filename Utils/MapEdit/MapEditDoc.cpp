// MapEditDoc.cpp : implementation of the CMapEditDoc class
//

#include "stdafx.h"
#include "MapEdit.h"
#include "Mainfrm.h"

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
	ON_BN_CLICKED(IDC_LAYERBAR_NEW, OnLayerbarNew)
	ON_BN_CLICKED(IDC_LAYERBAR_DELETE, OnLayerbarDelete)
	ON_BN_CLICKED(IDC_LAYERBAR_UP, OnLayerbarUp)
	ON_BN_CLICKED(IDC_LAYERBAR_DOWN, OnLayerbarDown)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMapEditDoc construction/destruction

CMapEditDoc::CMapEditDoc() 
{
CMainFrame	*Frm=(CMainFrame*)AfxGetApp()->GetMainWnd();
			LayerBar=Frm->GetLayerBar();


}

CMapEditDoc::~CMapEditDoc()
{
}

BOOL CMapEditDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

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


/***************************************************************************/
void CMapEditDoc::UpdateAll() 
{
	UpdateLayerBar();

}
/***************************************************************************/
/*** Layer Commands ********************************************************/
/***************************************************************************/
void CMapEditDoc::OnLayerbarNew() 
{
	Core.LayerAdd();
	UpdateLayerBar();
}

/***************************************************************************/
void CMapEditDoc::OnLayerbarDelete() 
{
int	Sel=GetLayerCurSel();

		if (Sel==-1) return;
		Core.LayerDelete(Sel);
		UpdateLayerBar();
}

/***************************************************************************/
void CMapEditDoc::OnLayerbarUp() 
{
int	Sel=GetLayerCurSel();

	if (Sel==-1) return;
	if (Sel>0)
	{
		Core.LayerMoveUp(Sel);
		UpdateLayerBar();
	}

}

/***************************************************************************/
void CMapEditDoc::OnLayerbarDown() 
{
int	Sel=GetLayerCurSel();
	if (Sel==-1) return;
	if (Sel<GetLayerCount()-1)
	{
		Core.LayerMoveDown(GetLayerCurSel());
		UpdateLayerBar();
	}
}

/***************************************************************************/
void CMapEditDoc::UpdateLayerBar()
{
int			LayerCount=Core.LayerGetCount();
CListBox	*Dlg=(CListBox *)LayerBar->GetDlgItem(IDC_LAYERBAR_LIST);
int			CurSel=Dlg->GetCurSel();

		Dlg->ResetContent();

		for (int i=0;i<LayerCount;i++)
		{
			sLayer	const &ThisLayer=Core.LayerGet(i);
			Dlg->AddString(ThisLayer.Name);
		}
		Dlg->SetCurSel(CurSel);

}

/***************************************************************************/
int	CMapEditDoc::GetLayerCurSel()
{
CListBox	*Dlg=(CListBox *)LayerBar->GetDlgItem(IDC_LAYERBAR_LIST);
		return(Dlg->GetCurSel());

}

/***************************************************************************/
int	CMapEditDoc::GetLayerCount()
{
CListBox	*Dlg=(CListBox *)LayerBar->GetDlgItem(IDC_LAYERBAR_LIST);
	return(Dlg->GetCount());

}

/***************************************************************************/
/***************************************************************************/
/***************************************************************************/

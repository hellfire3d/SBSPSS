/***********************/
/*** Map Editor Core ***/
/***********************/


#include	"stdafx.h"
#include	"gl3d.h"
#include	<gl\gl.h>
#include	<gl\glu.h>
#include	<gl\glut.h>
#include	"GLEnabledView.h"

#include	"MapEdit.h"
#include	"MapEditDoc.h"
#include	"MapEditView.h"
#include	"MainFrm.h"

#include	"NewMapGUI.h"

#include	"Core.h"
#include	"Layer.h"
#include	"LayerTile.h"
#include	"utils.h"

#include	"ExportAGB.h"
#include	"ExportPSX.h"


/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
CCore::CCore()
{
		CurrentMousePos=CPoint(0,0);
}

/*****************************************************************************/
CCore::~CCore()
{
int	ListSize=Layer.size();
		for (int i=0; i<ListSize; i++) delete Layer[i];
}

/*****************************************************************************/
void	CCore::Init()
{
CMainFrame	*Frm=(CMainFrame*)AfxGetApp()->GetMainWnd();
CMultiBar	*ParamBar=Frm->GetParamBar();

			ParamBar->RemoveAll();
// Add default param bar items			
			ParamBar->Add(Frm->GetLayerList(),IDD_LAYER_LIST_DIALOG,TRUE,TRUE);
			UpdateParamBar();
			UpdateAll(NULL);
}

/*****************************************************************************/
BOOL	CCore::New()
{
CNewMapGUI	Dlg;
int		Width,Height;
		Dlg.m_Width=TileLayerMinWidth;
		Dlg.m_Height=TileLayerMinHeight;

		Dlg.m_Back=TRUE;
		Dlg.m_Mid=TRUE;
		Dlg.m_Fore=FALSE;
		
		if (Dlg.DoModal()!=IDOK) return FALSE;
		Width=Dlg.m_Width;
		Height=Dlg.m_Height;

// Create Tile Layers
//															Type					Width					Height		Scale	3d?		Resizable?
		if (Dlg.m_Back)		Layer.push_back(new CLayerTile(	CLayerTile::Back,		32,						32,			4.0f,	FALSE,	FALSE));
		if (Dlg.m_Mid)		Layer.push_back(new CLayerTile(	CLayerTile::Mid,		Width,					Height,		2.0f,	FALSE,	TRUE));
							Layer.push_back(new CLayerTile(	CLayerTile::Action,		Width,					Height,		1.0f,	TRUE,	TRUE));
		if (Dlg.m_Fore)		Layer.push_back(new CLayerTile(	CLayerTile::Fore,		Width,					Height,		0.5f,	FALSE,	TRUE));

		ActiveLayer=FindActionLayer();
		MapCam=Vec(0,0,0);
		MapCamOfs=Vec(-15,10,0);
		TileCam=Vec(0,0,0);
		TileCamOfs=Vec(-15,10,0);
		TileViewFlag=FALSE;
		GridFlag=TRUE;
		Is3dFlag=TRUE;
		Init();
		return(TRUE);
}

/*****************************************************************************/

void	CCore::Load(CFile *File)
{
float	Version;

		File->Read(&Version,sizeof(float));
		TRACE1("Load Version %g\n",Version);

		if (Version>=1.0)
		{
			File->Read(&MapCam,sizeof(Vec));
			File->Read(&MapCamOfs,sizeof(Vec));
			File->Read(&TileCam,sizeof(Vec));
			File->Read(&TileCamOfs,sizeof(Vec));
	
			File->Read(&TileViewFlag,sizeof(BOOL));
			File->Read(&GridFlag,sizeof(BOOL));
			File->Read(&Is3dFlag,sizeof(BOOL));
		}
		if (Version>=1.1)
		{

		}


// Layers
int		LayerCount;
		File->Read(&LayerCount,sizeof(int));
		File->Read(&ActiveLayer,sizeof(int));

		for (int i=0;i<LayerCount;i++)
		{
			int	Type;

			File->Read(&Type,sizeof(int));
			switch (Type)
			{
			case LAYER_TYPE_TILE:
				Layer.push_back(new CLayerTile(File,Version));
				break;

			}
		}
		TileBank.Load(File,Version);


// Check Layers
int		MapWidth=Layer[FindActionLayer()]->GetWidth();
int		MapHeight=Layer[FindActionLayer()]->GetHeight();
		for (i=0;i<LayerCount;i++)
		{
			Layer[i]->CheckLayerSize(MapWidth,MapHeight);
		}

		Init();
}

/*****************************************************************************/
void	CCore::Save(CFile *File)
{
// Version 1
		File->Write(&FileVersion,sizeof(float));

		File->Write(&MapCam,sizeof(Vec));
		File->Write(&MapCamOfs,sizeof(Vec));
		File->Write(&TileCam,sizeof(Vec));
		File->Write(&TileCamOfs,sizeof(Vec));

		File->Write(&TileViewFlag,sizeof(BOOL));
		File->Write(&GridFlag,sizeof(BOOL));
		File->Write(&Is3dFlag,sizeof(BOOL));


// Layers
int		LayerCount=Layer.size();
		File->Write(&LayerCount,sizeof(int));
		File->Write(&ActiveLayer,sizeof(int));

		for (int i=0;i<LayerCount;i++)
		{
			int	Type=Layer[i]->GetType();
			File->Write(&Type,sizeof(int));
			Layer[i]->Save(File);
		}
	TileBank.Save(File);

}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
void	CCore::Render(CMapEditView *View,BOOL ForceRender)
{
		if (TileBank.NeedLoad()) TileBank.LoadTileSets(this);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear Screen
		if (TileViewFlag)
		{
			RenderTileView(View);
		}
		else
		{
			RenderLayers(View);
		}

}

/*****************************************************************************/
void	CCore::RenderLayers(CMapEditView *View)
{
Vec		&ThisCam=GetCam();
int		ListSize=Layer.size();
	
		for (int i=0;i<ListSize;i++)
		{
			if (Layer[i]->IsVisible())
				{
				Layer[i]->Render(this,ThisCam,Is3dFlag);
				if (GridFlag) Layer[i]->RenderGrid(this,ThisCam,i==ActiveLayer);
				}
		}
		
		Layer[ActiveLayer]->RenderCursor(this,ThisCam,Is3dFlag);
		
// Get Cursor Pos
		LastCursorPos=CursorPos;
		Layer[ActiveLayer]->FindCursorPos(this,View,GetCam(),CurrentMousePos);
}

/////////////////////////////////////////////////////////////////////////////
void	CCore::RenderTileView(CMapEditView *View)
{
Vec		&ThisCam=GetCam();

		TileBank.RenderSet(this,ThisCam,Is3dFlag);


// Get Cursor Pos
		TileBank.FindCursorPos(this,View,GetCam(),CurrentMousePos);
}

/*****************************************************************************/
/*** Control *****************************************************************/
/*****************************************************************************/
void	CCore::SetMode(int NewMode)
{
BOOL	RedrawFlag=FALSE;

		RedrawFlag=Layer[ActiveLayer]->SetMode(NewMode);
		//if (RedrawFlag) View->Invalidate();
}

/*****************************************************************************/
void	CCore::LButtonControl(CMapEditView *View,UINT nFlags, CPoint &point,BOOL DownFlag)
{
BOOL	RedrawFlag=FALSE;

		if (TileViewFlag)
		{
			if (nFlags & MK_RBUTTON)
				RedrawFlag=TileBank.SelectCancel();
			else
				RedrawFlag=TileBank.SelectL(DownFlag);
		}
		else
		{
			RedrawFlag=Layer[ActiveLayer]->LButtonControl(this,View,nFlags,CursorPos,DownFlag);
		}
		TileBank.SetActiveBrushL();

		if (RedrawFlag) View->Invalidate();
}

/*****************************************************************************/
void	CCore::MButtonControl(CMapEditView *View,UINT nFlags, CPoint &point,BOOL DownFlag)
{
		LastMousePos=point;
}

/*****************************************************************************/
void	CCore::RButtonControl(CMapEditView *View,UINT nFlags, CPoint &point,BOOL DownFlag)
{
BOOL	RedrawFlag=FALSE;

		if (TileViewFlag)
		{
			if (nFlags & MK_LBUTTON)
				RedrawFlag=TileBank.SelectCancel();
			else
				RedrawFlag=TileBank.SelectR(DownFlag);
		}
		else
		{
			RedrawFlag=Layer[ActiveLayer]->RButtonControl(this,View,nFlags,CursorPos,DownFlag);
		}
		TileBank.SetActiveBrushR();

		if (RedrawFlag) View->Invalidate();
}

/*****************************************************************************/
void	CCore::MouseWheel(CMapEditView *View,UINT nFlags, short zDelta, CPoint &pt) 
{
		if (zDelta>0)
			UpdateView(View,Vec(0,0,1.0f));
		else
			UpdateView(View,Vec(0,0,-1.0f));
}

/*****************************************************************************/
void	CCore::MouseMove(CMapEditView *View,UINT nFlags, CPoint &point) 
{
Vec		Ofs(0,0,0);

Vec		&ThisCam=GetCam();
// check if active doc
		if (theApp.GetCurrent()!=View->GetDocument()) return;

		CurrentMousePos=point;

// Handle Drag Movement
		if (nFlags & MK_MBUTTON)
			{
			float	XS,YS;
			RECT	ThisRect;

			View->GetWindowRect(&ThisRect);
			XS=ThisCam.z*4;//*Layer[ActiveLayer]->GetLayerZPos();
			YS=ThisCam.z*4;//*Layer[ActiveLayer]->GetLayerZPos();
			XS/=((ThisRect.right-ThisRect.left));
			YS/=((ThisRect.bottom-ThisRect.top));
	
			Ofs.x=LastMousePos.x-CurrentMousePos.x;
			Ofs.y=LastMousePos.y-CurrentMousePos.y;
			LastMousePos=CurrentMousePos;
	
			Ofs.x*=XS;
			Ofs.y*=YS;
/*			if (nFlags & MK_CONTROL)
			{ // Move Ofs
				Vec	&CamOfs=GetCamOfs();
				Ofs.y=-Ofs.y;
				CamOfs+=Ofs;
				UpdateView(View);
			}
			else
*/			{
				UpdateView(View,Ofs);
			}
			}
		else
		{	// Mouse still moved, so need to redraw windows, to get CursorPos (And pos render)
			View->Invalidate();
			if (TileViewFlag)
			{
			}
			else
			{
			Layer[ActiveLayer]->MouseMove(this,View,nFlags,CursorPos);
			}
		}

}


/*****************************************************************************/
/*** Layers ******************************************************************/
/*****************************************************************************/
void	CCore::UpdateParamBar()
{
CMainFrame	*Frm=(CMainFrame*)AfxGetApp()->GetMainWnd();
CToolBar	*ToolBar=Frm->GetToolBar();
CMultiBar	*ParamBar=Frm->GetParamBar();

			ParamBar->RemoveAll();
			Layer[ActiveLayer]->InitGUI(this);
			Layer[ActiveLayer]->UpdateGUI(this);

			ParamBar->Update();

}

/*****************************************************************************/
void		CCore::UpdateLayerGUI(CMapEditView *View)
{
CMainFrame	*Frm=(CMainFrame*)AfxGetApp()->GetMainWnd();
CMultiBar	*ParamBar=Frm->GetParamBar();
CLayerList	*List=(CLayerList*)Frm->GetDialog(IDD_LAYER_LIST_DIALOG);
int			ListSize=Layer.size();

			List->ListBox.ResetContent();

			for (int i=0; i<ListSize; i++)
			{
				List->ListBox.AddString(Layer[i]->GetName());
			}
// Now sets checks (silly MSoft bug!!)
			for (i=0; i<ListSize; i++)
			{
				List->ListBox.SetCheck(i,Layer[i]->IsVisible());
			}
			List->ListBox.SetCurSel(ActiveLayer);
}

/*****************************************************************************/
void		CCore::SetLayer(int NewLayer)
{
CMainFrame	*Frm=(CMainFrame*)AfxGetApp()->GetMainWnd();
CMultiBar	*ParamBar=Frm->GetParamBar();
CLayerList	*List=(CLayerList*)Frm->GetDialog(IDD_LAYER_LIST_DIALOG);

		Layer[ActiveLayer]->SetVisible(List->ListBox.GetCheck(ActiveLayer));
		if (ActiveLayer!=NewLayer)
		{
			ActiveLayer=NewLayer;
			Layer[ActiveLayer]->SetVisible(List->ListBox.GetCheck(ActiveLayer));
		}

}

/*****************************************************************************/
/*** Grid ********************************************************************/
/*****************************************************************************/
void	CCore::UpdateGrid(CMapEditView *View,BOOL Toggle)
{
CMainFrame	*Frm=(CMainFrame*)AfxGetApp()->GetMainWnd();
CToolBar	*ToolBar=Frm->GetToolBar();

			if (Toggle) GridFlag=!GridFlag;

			ToolBar->GetToolBarCtrl().PressButton(ID_TOOLBAR_GRID,GridFlag);
			UpdateView(View);
}

/*****************************************************************************/
/*** TileBank ****************************************************************/
/*****************************************************************************/
void	CCore::UpdateTileView(CMapEditView *View,BOOL Toggle)
{
CMainFrame	*Frm=(CMainFrame*)AfxGetApp()->GetMainWnd();
CToolBar	*ToolBar=Frm->GetToolBar();
CMultiBar	*ParamBar=Frm->GetParamBar();

			if (Toggle) TileViewFlag=!TileViewFlag;
			ParamBar->RemoveAll();
			ToolBar->GetToolBarCtrl().PressButton(ID_TOOLBAR_TILEPALETTE,TileViewFlag);
			UpdateParamBar();
			UpdateView(View);
}

/*****************************************************************************/
void	CCore::TileBankLoad(char *Filename)
{
		TileBank.AddTileSet(Filename);
		TileBank.UpdateGUI(this,TileViewFlag);
		UpdateView(NULL);
}

/*****************************************************************************/
void	CCore::TileBankDelete()
{
		TileBank.Delete();
		UpdateView(NULL);
}

/*****************************************************************************/
void	CCore::TileBankReload()
{
		TileBank.Reload();
		TexCache.Purge();
		UpdateView(NULL);
}

/*****************************************************************************/
void	CCore::TileBankSet()
{
CMainFrame		*Frm=(CMainFrame*)AfxGetApp()->GetMainWnd();
CLayerTileGUI	*Dlg=(CLayerTileGUI*)Frm->GetDialog(IDD_LAYERTILE_GUI);

		TileBank.SetCurrent(Dlg->m_List.GetCurSel());
}

/*****************************************************************************/
void	CCore::MirrorX(CMapEditView *View)
{
		if (!TileViewFlag) 
		{
			Layer[ActiveLayer]->MirrorX(this);
			UpdateView(View);
		}
}

/*****************************************************************************/
void	CCore::MirrorY(CMapEditView *View)
{
		if (!TileViewFlag) 
		{
			Layer[ActiveLayer]->MirrorY(this);
			UpdateView(View);
		}
}

/*****************************************************************************/
void	CCore::ActiveBrushLeft(CMapEditView *View)
{
		TileBank.SetActiveBrushL();
		UpdateView(View);
}

/*****************************************************************************/
void	CCore::ActiveBrushRight(CMapEditView *View)
{
		TileBank.SetActiveBrushR();
		UpdateView(View);
}

/*****************************************************************************/
BOOL	CCore::IsTileValid(int Set,int Tile)
{
		return(TileBank.IsTileValid(Set,Tile));
}

/*****************************************************************************/
/*** Misc ********************************************************************/
/*****************************************************************************/
Vec		&CCore::GetCam()
{
		if (TileViewFlag)
			return(TileCam);
		else
			return(MapCam);

}

/*****************************************************************************/
Vec		&CCore::GetCamOfs()
{
		if (TileViewFlag)
			return(TileCamOfs);
		else
			return(MapCamOfs);

}

/*****************************************************************************/
void	CCore::UpdateGUI(CMapEditView *View)
{
		UpdateLayerGUI(View);
		UpdateGrid(View);

		Layer[ActiveLayer]->UpdateGUI(this);

}

/*****************************************************************************/
void	CCore::UpdateAll(CMapEditView *View)
{
		UpdateGUI(View);
		UpdateView(View);
}

/*****************************************************************************/
void	CCore::UpdateView(CMapEditView *View,Vec Ofs)
{
Vec		&ThisCam=GetCam();

		Ofs.y=-Ofs.y;
		ThisCam+=Ofs;
		if (ThisCam.z>-1) ThisCam.z=-1;
		if (View) View->Invalidate();
}		


/*****************************************************************************/
void	CCore::SetMapSize(CMapEditView *View,int Width,int Height)
{
	if (Width==GetMapWidth() && Height==GetMapHeight()) return;

int		ListSize=Layer.size();

	for (int i=0; i<ListSize; i++) 
	{
		Layer[i]->Resize(Width,Height);
	}

	UpdateView(View);
}

/*****************************************************************************/
void	CCore::Toggle2d3d(CMapEditView *View)
{
		Is3dFlag=!Is3dFlag;
		UpdateView(View);
//		ExportPSX("c:/temp/test.pme");
}

/*****************************************************************************/
int		CCore::FindLayer(int Type,int SubType)
{
int	ListSize=Layer.size();

		for (int i=0; i<ListSize; i++)
		{
			if (Layer[i]->GetType()==Type)
				if (SubType==-1 || Layer[i]->GetSubType()==SubType)
					return(i);
		}
	return(-1);
}

/*****************************************************************************/
int		CCore::FindActionLayer()
{
int	Idx=FindLayer(LAYER_TYPE_TILE,CLayerTile::Action);

	return(Idx);
}

/*****************************************************************************/

Vec		CCore::OffsetCam(Vec &Cam,float DivVal)
{
Vec	ThisCam;

	ThisCam=Cam/DivVal;
	ThisCam.z=Cam.z;
	ThisCam+=GetCamOfs();

	return(ThisCam);
}

/*****************************************************************************/
void	CCore::ExportAGB(char *Filename)
{
int		LayerCount=Layer.size();
char	ExportName[256];
		
		SetFileExt(Filename,ExportName,"c");

CExportAGB	Exp(ExportName);

		for (int i=0;i<LayerCount;i++)
		{
			Layer[i]->Export(this,Exp);
		}
		Exp.ExportTiles(this);
		Exp.ExportPalette();
}

/*****************************************************************************/
void	CCore::ExportPSX(char *Filename)
{

int		LayerCount=Layer.size();
char	ExportName[256];
		
		SetFileExt(Filename,ExportName,"PME");

CExportPSX	Exp(ExportName);

/*		for (int i=0;i<LayerCount;i++)
		{
			Layer[i]->Export(this,Exp);
		}
*/
		Layer[FindActionLayer()]->Export(this,Exp);

		Exp.ExportTiles(this);
		Exp.ExportTexList(this);

}
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

#include	"Core.h"
#include	"Layer.h"
#include	"LayerTile.h"

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
CCore::CCore()
{
	for (int i=0; i<LAYER_TYPE_MAX; i++) Layers[i]=0;

	TileViewFlag=FALSE;
	GridFlag=TRUE;
	CurrentMousePos=CPoint(0,0);
	ActiveLayer=0;
	MapCam=Vec(0,0,0);
	TileCam=Vec(0,0,0);
	Is3dFlag=TRUE;

}

/*****************************************************************************/
CCore::~CCore()
{
	for (int i=0; i<LAYER_TYPE_MAX; i++) if (Layers[i]) delete Layers[i];
}

/*****************************************************************************/
void	CCore::Init()
{
CMainFrame	*Frm=(CMainFrame*)AfxGetApp()->GetMainWnd();
CMultiBar	*ParamBar=Frm->GetParamBar();

			ParamBar->RemoveAll();
// Add default parram bar items			
			ParamBar->Add(Frm->GetLayerList(),IDD_LAYER_LIST_DIALOG,TRUE,TRUE);
			ParamBar->Add(Frm->GetTileSetDlg(),IDD_TILESET_DIALOG,TRUE,TRUE);
			UpdateParamBar();
//			Layers[ActiveLayer]->InitGUI(this);
//			ParamBar->Update();
}

/*****************************************************************************/
void	CCore::NewMap()
{
// Create Gfx Layers
//												Name	Width					Height					SizeDiv	ViewDiv	3d?		Resizable?
	Layers[LAYER_TYPE_BACK]=	new CLayerTile(	"Back",	32,						32,						1.0f,	4.0f,	FALSE,	FALSE);
	Layers[LAYER_TYPE_MID]=		new CLayerTile(	"Mid",	TileLayerDefaultWidth,	TileLayerDefaultHeight,	2.0f,	2.0f,	FALSE,	TRUE);
	Layers[LAYER_TYPE_ACTION]=	new CLayerTile(	"Action",TileLayerDefaultWidth,	TileLayerDefaultHeight,	1.0f,	1.0f,	TRUE,	TRUE);
	Layers[LAYER_TYPE_FORE]=	new CLayerTile(	"Fore",	TileLayerDefaultWidth,	TileLayerDefaultHeight,	0.5f,	0.5f,	FALSE,	TRUE);

	ActiveLayer=LAYER_TYPE_ACTION;
	MapCam=Vec(0,0,0);
	TileCam=Vec(0,0,0);
	Init();
}

/*****************************************************************************/
void	CCore::OpenMap()
{
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
		
		for (int i=0;i<LAYER_TYPE_MAX;i++)
		{
			Layers[i]->Render(this,ThisCam,Is3dFlag);
		}
		
		Layers[ActiveLayer]->RenderCursor(this,ThisCam,Is3dFlag);
		if (GridFlag) Layers[ActiveLayer]->RenderGrid(this,ThisCam);
		
// Get Cursor Pos
		LastCursorPos=CursorPos;
		Layers[ActiveLayer]->FindCursorPos(this,View,GetCam(),CurrentMousePos);
}

/*****************************************************************************/
void	CCore::RenderTileView(CMapEditView *View)
{
Vec		&ThisCam=GetCam();

		TileBank.RenderSet(this,ThisCam,Is3dFlag);
//		TileBank.RenderCursor(this,ThisCam,Is3dFlag);

// Get Cursor Pos
		TileBank.FindCursorPos(this,View,GetCam(),CurrentMousePos);
}

/*****************************************************************************/
/*** Control *****************************************************************/
/*****************************************************************************/
void	CCore::SetMode(int NewMode)
{
BOOL	RedrawFlag=FALSE;

		RedrawFlag=Layers[ActiveLayer]->SetMode(NewMode);
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
			RedrawFlag=Layers[ActiveLayer]->LButtonControl(this,View,nFlags,CursorPos,DownFlag);
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
			RedrawFlag=Layers[ActiveLayer]->RButtonControl(this,View,nFlags,CursorPos,DownFlag);
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
			XS=ThisCam.z*4;//*Layers[ActiveLayer]->GetLayerZPos();
			YS=ThisCam.z*4;//*Layers[ActiveLayer]->GetLayerZPos();
			XS/=((ThisRect.right-ThisRect.left));
			YS/=((ThisRect.bottom-ThisRect.top));
	
			Ofs.x=LastMousePos.x-CurrentMousePos.x;
			Ofs.y=LastMousePos.y-CurrentMousePos.y;
			LastMousePos=CurrentMousePos;
	
			Ofs.x*=XS;
			Ofs.y*=YS;

			UpdateView(View,Ofs);
			}
		else
		{	// Mouse still moved, so need to redraw windows, to get CursorPos (And pos render)
			View->Invalidate();
			if (TileViewFlag)
			{
			}
			else
			{
			Layers[ActiveLayer]->MouseMove(this,View,nFlags,CursorPos);
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
// Add default parram bar items			
			if (TileViewFlag)
			{
//				TileBank.InitGUI(this);
			}
			else
			{
				Layers[ActiveLayer]->InitGUI(this);
			}
			
			ParamBar->Update();

}

/*****************************************************************************/
void	CCore::SetActiveLayer(int i)
{
//	UpdateParamBar(NULL,ParamViewFlag);
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
void	CCore::TileBankReload()
{
		TileBank.Reload();
		TexCache.Purge();
		UpdateView(NULL);
}

/*****************************************************************************/
void	CCore::TileBankSet()
{
CMainFrame	*Frm=(CMainFrame*)AfxGetApp()->GetMainWnd();
CTileSetDlg	*TileSetDlg=(CTileSetDlg*)Frm->GetDialog(IDD_TILESET_DIALOG);

		TileBank.SetCurrent(TileSetDlg->TileSetList.GetCurSel());
}

/*****************************************************************************/
void	CCore::MirrorX(CMapEditView *View)
{
		if (!TileViewFlag) 
		{
			Layers[ActiveLayer]->MirrorX(this);
			UpdateView(View);
		}
}

/*****************************************************************************/
void	CCore::MirrorY(CMapEditView *View)
{
		if (!TileViewFlag) 
		{
			Layers[ActiveLayer]->MirrorY(this);
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
void	CCore::UpdateAll(CMapEditView *View)
{
		UpdateView(View);
		UpdateGrid(View);

		TileBank.UpdateGUI(this,TileViewFlag);
		Layers[ActiveLayer]->UpdateGUI(this);

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

	for (int i=0; i<LAYER_TYPE_MAX; i++) 
	{
//		Layers[i]->Resize(Width,Height);
	}
		Layers[LAYER_TYPE_ACTION]->Resize(Width,Height);

	UpdateView(View);
}
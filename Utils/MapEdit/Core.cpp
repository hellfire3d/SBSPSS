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
#include	"utils.h"


/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
CCore::CCore()
{
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
int	ListSize=Layer.size();
	for (int i=0; i<ListSize; i++) delete Layer[i];
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
//			Layer[ActiveLayer]->InitGUI(this);
//			ParamBar->Update();
			UpdateAll(NULL);
}

/*****************************************************************************/
void	CCore::New()
{
// Create Gfx Layers
//									Name	Width					Height					SizeDiv	ViewDiv	3d?		Resizable?
	Layer.push_back(new CLayerTile(	"Back",	32,						32,						1.0f,	4.0f,	FALSE,	FALSE));
	Layer.push_back(new CLayerTile(	"Mid",	TileLayerDefaultWidth,	TileLayerDefaultHeight,	2.0f,	2.0f,	FALSE,	TRUE));
	Layer.push_back(new CLayerTile(	"Action",TileLayerDefaultWidth,	TileLayerDefaultHeight,	1.0f,	1.0f,	TRUE,	TRUE));
	Layer.push_back(new CLayerTile(	"Fore",	TileLayerDefaultWidth,	TileLayerDefaultHeight,	0.5f,	0.5f,	FALSE,	TRUE));

	ActiveLayer=LAYER_ACTION;
	MapCam=Vec(0,0,0);
	TileCam=Vec(0,0,0);
	Init();
}

/*****************************************************************************/
void	CCore::Load(CFile *File)
{
float	Version;

		File->Read(&Version,sizeof(float));

		File->Read(&MapCam,sizeof(Vec));
		File->Read(&TileCam,sizeof(Vec));

		File->Read(&TileCam,sizeof(Vec));

		File->Read(&TileViewFlag,sizeof(BOOL));
		File->Read(&GridFlag,sizeof(BOOL));
		File->Read(&Is3dFlag,sizeof(BOOL));


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
	Init();

}

/*****************************************************************************/
void	CCore::Save(CFile *File)
{

		File->Write(&FileVersion,sizeof(float));

		File->Write(&MapCam,sizeof(Vec));
		File->Write(&TileCam,sizeof(Vec));

		File->Write(&TileCam,sizeof(Vec));

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
			Layer[i]->Render(this,ThisCam,Is3dFlag);
		}
		
		Layer[ActiveLayer]->RenderCursor(this,ThisCam,Is3dFlag);
		if (GridFlag) Layer[ActiveLayer]->RenderGrid(this,ThisCam);
		
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
// Add default parram bar items			
			if (TileViewFlag)
			{
//				TileBank.InitGUI(this);
			}
			else
			{
				Layer[ActiveLayer]->InitGUI(this);
			}
			
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
//				List->ListBox.SetCheck(i,Layer[i]->IsVisible());
			}
// Now sets checks (silly MSoft bug!!)
			for (i=0; i<ListSize; i++)
			{
				List->ListBox.SetCheck(i,Layer[i]->IsVisible());
			}

}

/*****************************************************************************/
/*
void	CCore::SetActiveLayer(int i)
{
//	UpdateParamBar(NULL,ParamViewFlag);
}
*/
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
void	CCore::UpdateGUI(CMapEditView *View)
{
		UpdateLayerGUI(View);
		UpdateGrid(View);

		TileBank.UpdateGUI(this,TileViewFlag);
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
/***********************/
/*** Map Editor Core ***/
/***********************/


#include	"stdafx.h"
#include	<Vector3.h>
#include	<gl\gl.h>
#include	<gl\glu.h>
#include	"GLEnabledView.h"

#include	"MapEdit.h"
#include	"MapEditDoc.h"
#include	"MapEditView.h"
#include	"MainFrm.h"

#include	"NewMapGUI.h"
#include	"AddLayerDlg.h"

#include	"Core.h"
#include	"Layer.h"
#include	"LayerTile.h"
#include	"LayerCollision.h"
#include	"LayerShade.h"
#include	"utils.h"

#include	"Export.h"
#include	"LayerList.h"

const Vector3	DefaultCamPos(0.0f,0.0f,0.9f);

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
CCore::CCore()
{
		CurrentMousePos=CPoint(0,0);
		CurrentView=NULL;
}

/*****************************************************************************/
CCore::~CCore()
{
int	ListSize=Layer.size();
		for (int i=0; i<ListSize; i++) delete Layer[i];
}

/*****************************************************************************/
BOOL	CCore::New()
{
CNewMapGUI	Dlg;
int		Width,Height;
		Dlg.m_Width=TileLayerMinWidth;
		Dlg.m_Height=TileLayerMinHeight;

#ifndef _DEBUG
		if (Dlg.DoModal()!=IDOK) return FALSE;
#endif
		Width=Dlg.m_Width;
		Height=Dlg.m_Height;

// Create Tile Layers
		AddLayer(LAYER_TYPE_TILE,LAYER_SUBTYPE_ACTION, Width, Height);
		AddLayer(LAYER_TYPE_TILE,LAYER_SUBTYPE_SCRATCH, Width, Height);

		ActiveLayer=FindLayer(LAYER_TYPE_TILE,LAYER_SUBTYPE_ACTION);
		MapCam=DefaultCamPos;
		TileCam=DefaultCamPos;
		TileViewFlag=FALSE;
		GridFlag=TRUE;
		Is3dFlag=TRUE;
		return(TRUE);
}

/*****************************************************************************/
void	CCore::Load(CFile *File)
{
int		Version;

		File->Read(&Version,sizeof(int));
		if (Version>100000) Version=1;	// Check fix for changing version to int from float

		if (Version<FileVersion)
		{
			CString mexstr;
			mexstr.Format("Old File Format\n\nPlease re-save\n");
			AfxMessageBox(mexstr,MB_OK | MB_ICONEXCLAMATION);

		}

		TRACE1("Load Version %i\n",Version);

		File->Read(&MapCam,sizeof(Vector3));
		File->Read(&MapCamOfs,sizeof(Vector3));
		File->Read(&TileCam,sizeof(Vector3));
		File->Read(&TileCamOfs,sizeof(Vector3));
	
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
				AddLayer(new CLayerTile(File,Version));
				break;
			case LAYER_TYPE_COLLISION:
				AddLayer(new CLayerCollision(File,Version));
				break;
			case LAYER_TYPE_SHADE:
				AddLayer(new CLayerShade(File,Version));
				break;
			default:
				ASSERT(!"poos");
			}
		}
		TileBank.Load(File,Version);


// Check Layers
int		MapWidth=ActionLayer->GetWidth();
int		MapHeight=ActionLayer->GetHeight();
		for (i=0;i<LayerCount;i++)
		{
			Layer[i]->CheckLayerSize(MapWidth,MapHeight);
		}

}

/*****************************************************************************/
void	CCore::Save(CFile *File)
{
// Version 1
		File->Write(&FileVersion,sizeof(int));

		File->Write(&MapCam,sizeof(Vector3));
		File->Write(&MapCamOfs,sizeof(Vector3));
		File->Write(&TileCam,sizeof(Vector3));
		File->Write(&TileCamOfs,sizeof(Vector3));

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
BOOL	CCore::Question(char *Txt)
{
CString Str;
	Str.Format(Txt);
int	Ret=AfxMessageBox(Str,MB_YESNO , MB_ICONQUESTION);
	
	if (Ret==IDYES) return(true);
	return(false);

}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
void	CCore::Render(BOOL ForceRender)
{
		if (!CurrentView) 
		{
			TRACE0("No View\n");
			UpdateAll();
			return;
		}
		if (TileBank.NeedLoad()) TileBank.LoadTileSets(this);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear Screen
		if (TileViewFlag)
		{
			RenderTileView();
		}
		else
		{
			RenderLayers();
		}

}

/*****************************************************************************/
void	CCore::RenderLayers()
{
Vector3	&ThisCam=GetCam();
int		ListSize=Layer.size();
int		StartLayer,EndLayer;

		StartLayer=0;
		EndLayer=ListSize;

		while (Layer[StartLayer]->IsUnique()) StartLayer++;

		if (Layer[ActiveLayer]->IsUnique())
		{
			StartLayer=ActiveLayer;
			EndLayer=StartLayer+1;
		}

		for (int i=StartLayer; i<EndLayer; i++)
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
		Layer[ActiveLayer]->FindCursorPos(this,GetCam(),CurrentMousePos);
}

/*****************************************************************************/
void	CCore::RenderTileView()
{
Vector3	&ThisCam=GetCam();

		GetTileBank().RenderSet(this,ThisCam,Is3dFlag);
		GetTileBank().FindCursorPos(this,GetCam(),CurrentMousePos);
}

/*****************************************************************************/
/*** Control *****************************************************************/
/*****************************************************************************/
void	CCore::SetMode(int NewMode)
{
BOOL	RedrawFlag=FALSE;
		RedrawFlag=Layer[ActiveLayer]->SetMode(NewMode);
}

/*****************************************************************************/
void	CCore::LButtonControl(UINT nFlags, CPoint &point,BOOL DownFlag)
{
BOOL	RedrawFlag=FALSE;

		if (TileViewFlag)
		{
			if (nFlags & MK_RBUTTON)
				RedrawFlag=GetTileBank().SelectCancel();
			else
				RedrawFlag=GetTileBank().SelectL(DownFlag);
		}
		else
		{
			if (Layer[ActiveLayer]->IsVisible())
			{
				RedrawFlag=Layer[ActiveLayer]->LButtonControl(this,nFlags,CursorPos,DownFlag);
			}
		}
		GetTileBank().SetActiveBrushL();

		if (RedrawFlag) RedrawView();
}

/*****************************************************************************/
void	CCore::MButtonControl(UINT nFlags, CPoint &point,BOOL DownFlag)
{
}

/*****************************************************************************/
void	CCore::RButtonControl(UINT nFlags, CPoint &point,BOOL DownFlag)
{
BOOL	RedrawFlag=FALSE;

		if (TileViewFlag)
		{
			if (nFlags & MK_LBUTTON)
				RedrawFlag=GetTileBank().SelectCancel();
			else
				RedrawFlag=GetTileBank().SelectR(DownFlag);
		}
		else
		{
			if (Layer[ActiveLayer]->IsVisible())
			{
				RedrawFlag=Layer[ActiveLayer]->RButtonControl(this,nFlags,CursorPos,DownFlag);
			}
		}
		GetTileBank().SetActiveBrushR();

		if (RedrawFlag) RedrawView();
}

/*****************************************************************************/
void	CCore::Zoom(float Dst) 
{
Vector3	Ofs;
		Ofs.Zero();
		Ofs.z=Dst;
		UpdateView(&Ofs);
}


/*****************************************************************************/
void	CCore::MouseWheel(UINT nFlags, short zDelta, CPoint &pt) 
{
		if (zDelta>0) 
			Zoom(-0.1f);
		else
			Zoom(+0.1f);
}

/*****************************************************************************/
void	CCore::MouseMove(UINT nFlags, CPoint &point) 
{
Vector3	Ofs;
Vector3	&ThisCam=GetCam();
// check if active doc
		Ofs.Zero();
		if (theApp.GetCurrent()!=CurrentView->GetDocument()) return;
		GetWorkingPath();
		CurrentMousePos=point;

// Handle Drag Movement
		if (nFlags & MK_MBUTTON || nFlags & MK_SHIFT)
			{
			float	XS,YS;
			RECT	ThisRect;
			float	MoveSpd=GetZoomW();

			if (nFlags & MK_CONTROL)
			{
				MoveSpd*=4;
			}
			CurrentView->GetWindowRect(&ThisRect);
			XS=ThisCam.z*MoveSpd;
			YS=ThisCam.z*MoveSpd;
			XS/=((ThisRect.right-ThisRect.left));
			YS/=((ThisRect.bottom-ThisRect.top));
	
			Ofs.x=LastMousePos.x-CurrentMousePos.x;
			Ofs.y=LastMousePos.y-CurrentMousePos.y;
	
			Ofs.x*=XS;
			Ofs.y*=YS;
			UpdateView(&Ofs);
			}
		else
		{	
			if (TileViewFlag)
			{
			}
			else
			{
				if (Layer[ActiveLayer]->IsVisible())
				{
					Layer[ActiveLayer]->MouseMove(this,nFlags,CursorPos);
				}
			}
// Mouse still moved, so need to redraw windows, to get CursorPos
			RedrawView();
		}
		LastMousePos=CurrentMousePos;
}


/*****************************************************************************/
/*** Layers ******************************************************************/
/*****************************************************************************/
void	CCore::UpdateParamBar()
{
CMainFrame	*Frm=(CMainFrame*)AfxGetMainWnd();
CMultiBar	*ParamBar=Frm->GetParamBar();

			GUIRemoveAll();
			GUIAdd(LayerList,IDD_LAYER_LIST_DIALOG);
			Layer[ActiveLayer]->GUIInit(this);
			GUIUpdate();
}

/*****************************************************************************/
void		CCore::UpdateLayerGUI()
{
int			ListSize=Layer.size();

			if (LayerList.ListBox)
			{
				LayerList.ListBox.ResetContent();

				for (int i=0; i<ListSize; i++)
				{
					LayerList.ListBox.AddString(Layer[i]->GetName());
				}
// Now sets checks (silly MSoft bug!!)
				for (i=0; i<ListSize; i++)
				{
					LayerList.ListBox.SetCheck(i,Layer[i]->IsVisible());
				}
				LayerList.ListBox.SetCurSel(ActiveLayer);
			}
}

/*****************************************************************************/
void		CCore::SetLayer(int NewLayer,bool Force)
{
int		LayerCount=Layer.size()-1;
int		LastLayer=ActiveLayer;

		if (NewLayer<0) NewLayer=0;

// If toggling layer, dont change the layer
		if ((int)LayerList.ListBox.GetCheck(NewLayer)!=(int)Layer[NewLayer]->IsVisible())
		{
			Layer[NewLayer]->SetVisible(LayerList.ListBox.GetCheck(NewLayer));
			LayerList.ListBox.SetCurSel(ActiveLayer);
		}
		else
		{
			bool	IsCol=Layer[NewLayer]->GetType()==LAYER_TYPE_COLLISION;
			TileBank.SetCollision(IsCol);
			ActiveLayer=NewLayer;
		}

		if (LastLayer!=ActiveLayer || Force)
		{
			if (TileViewFlag) TileViewFlag=false;
			if (LastLayer<=LayerCount) Layer[LastLayer]->GUIKill(this);
			Layer[ActiveLayer]->GUIInit(this);
			GUIUpdate();
		}
		RedrawView();
}

/*****************************************************************************/
int			CCore::AddLayer(CLayer *NewLayer)
{
int			ListSize=Layer.size();
int			NewIdx=CLayer::GetLayerIdx(NewLayer->GetType(),NewLayer->GetSubType());
int			Idx=ListSize;

		TRACE3("Add Layer %i %i @ %i\n",NewLayer->GetType(),NewLayer->GetSubType(),NewIdx);

		for (Idx=0; Idx<ListSize; Idx++)
		{
			int ListIdx=CLayer::GetLayerIdx(Layer[Idx]->GetType(),Layer[Idx]->GetSubType());
			if (NewIdx<ListIdx) break;
		}

		Layer.insert(Layer.begin() + Idx,NewLayer);
		
		if (NewLayer->GetType()==LAYER_TYPE_TILE && NewLayer->GetSubType()==LAYER_SUBTYPE_ACTION) ActionLayer=NewLayer;
		return(Idx);
}


/*****************************************************************************/
int		CCore::AddLayer(int Type, int SubType, int Width, int Height)
{
int		Idx;
		switch (Type)
		{
			case LAYER_TYPE_TILE:
				Idx=AddLayer(new CLayerTile(SubType, Width,Height));
				break;
			case LAYER_TYPE_COLLISION:
				Idx=AddLayer(new CLayerCollision(SubType, Width,Height));
				break;
			case LAYER_TYPE_SHADE:
				Idx=AddLayer(new CLayerShade(SubType, Width,Height));
				break;
			default:
				ASSERT(!"AddLayer - Invalid Layer Type");
				break;
		}
		return(Idx);
}

/*****************************************************************************/
void		CCore::AddLayer(int CurrentLayer)
{
std::vector<int>	List;
CAddLayerDlg		Dlg;
int					NewLayerId=0;
int					Sel;

// Build Unused List
		Dlg.Sel=&Sel;
		Sel=0;
		for (int i=0; i<CLayer::InfoTableSize; i++)
		{
			if (FindLayer(CLayer::InfoTable[i].Type,CLayer::InfoTable[i].SubType)==-1)
			{
				List.push_back(i);
				Dlg.StrList.push_back(CLayer::InfoTable[i].Name);
			}
		}
		if (Dlg.DoModal()!=IDOK) return;

		NewLayerId=List[Sel];

		TRACE2("Add Layer %i %s\n",NewLayerId,CLayer::InfoTable[NewLayerId].Name);

int		Width=ActionLayer->GetWidth();
int		Height=ActionLayer->GetHeight();

int		Idx=AddLayer(CLayer::InfoTable[NewLayerId].Type,CLayer::InfoTable[NewLayerId].SubType,Width,Height);
		if (ActiveLayer>=Idx) ActiveLayer++;

		SetLayer(Idx,true);
}

/*****************************************************************************/
void		CCore::DeleteLayer(int CurrentLayer)
{
		if (Layer[CurrentLayer]->CanDelete())
		{
			Layer[CurrentLayer]->GUIKill(this);
			delete Layer[CurrentLayer];
			Layer.erase(Layer.begin() + CurrentLayer);
			SetLayer(CurrentLayer-1,true);
			TRACE1("Deleted Layer %i\n",CurrentLayer);
		}
		else
		{
			TRACE1("Cant Delete Layer %i\n",CurrentLayer);
		}
}

/*****************************************************************************/
/*** Grid ********************************************************************/
/*****************************************************************************/
void	CCore::UpdateGrid(BOOL Toggle)
{
			if (Toggle) GridFlag=!GridFlag;
			RedrawView();
}

/*****************************************************************************/
/*** TileBank ****************************************************************/
/*****************************************************************************/
void	CCore::UpdateTileView(BOOL Toggle)
{
		if (!Layer[ActiveLayer]->HasTileView()) return;
		if (TileViewFlag && !TileBank.CanClose()) return;

		if (Toggle) 
		{
			TileViewFlag=!TileViewFlag;
		}

		if (TileViewFlag)
		{
			Layer[ActiveLayer]->GUIKill(this);
			TileBank.GUIInit(this);
		}
		else
		{
			TileBank.GUIKill(this);
			Layer[ActiveLayer]->GUIInit(this);
		}
		GUIUpdate();
		RedrawView();
}

/*****************************************************************************/
void	CCore::TileBankLoad(char *Filename)
{
		TileBank.AddTileSet(Filename);
		TileBank.GUIUpdate(this);
		RedrawView();
}

/*****************************************************************************/
void	CCore::TileBankDelete()
{
		if (Question("Delete Current Tile Bank\n\nAll used tiles in current set will be set to blank\nAre you sure?"))
			{
			int		SetCount=TileBank.GetSetCount();
			int		Current=TileBank.GetCurrent();
			int		i,ListSize=Layer.size();

				for (i=0;i<ListSize;i++)
				{
					Layer[i]->DeleteSet(Current);
				}
				TileBank.Delete();

				for (int Set=Current+1; Set<SetCount; Set++)
				{
		
					for (i=0;i<ListSize;i++)
					{
						Layer[i]->RemapSet(Set,Set-1);
					}
				}
				RedrawView();
			}
}

/*****************************************************************************/
void	CCore::TileBankReload()
{
		TileBank.Reload();
		TexCache.Purge();
		RedrawView();
}

/*****************************************************************************/
void	CCore::TileBankSet()
{
		TileBank.SetCurrent();
}

/*****************************************************************************/
void	CCore::MirrorX()
{
		if (TileViewFlag) return;
		Layer[ActiveLayer]->MirrorX(this);
		RedrawView();
}

/*****************************************************************************/
void	CCore::MirrorY()
{
		if (TileViewFlag) return;
		Layer[ActiveLayer]->MirrorY(this);
		RedrawView();
}

/*****************************************************************************/
void	CCore::ActiveBrushLeft()
{
		GetTileBank().SetActiveBrushL();
		RedrawView();
}

/*****************************************************************************/
void	CCore::ActiveBrushRight()
{
		GetTileBank().SetActiveBrushR();
		RedrawView();
}

/*****************************************************************************/
BOOL	CCore::IsTileValid(int Set,int Tile)
{
		return(TileBank.IsTileValid(Set,Tile));
}

/*****************************************************************************/
void	CCore::CopySelection()
{
		Layer[ActiveLayer]->CopySelection(this);
		RedrawView();
}

/*****************************************************************************/
void	CCore::PasteSelection()
{
		Layer[ActiveLayer]->PasteSelection(this);
		RedrawView();
}

/*****************************************************************************/
/*** Misc ********************************************************************/
/*****************************************************************************/
Vector3	&CCore::GetCam()
{
		if (TileViewFlag)
			return(TileCam);
		else
			return(MapCam);

}

/*****************************************************************************/
void	CCore::SetScale()
{
float	XS=GetZoomW();
float	YS=GetZoomH();
float	ZS=XS/YS;

		ScaleVector.x=1.0f/XS;
		ScaleVector.y=1.0f/XS;
		ScaleVector.z=1.0f/XS;
}

/*****************************************************************************/
float	CCore::GetZoomW()
{
Vector3	&ThisCam=GetCam();

		return((float)SCREEN_MAP_WIDTH/ThisCam.z);
}

/*****************************************************************************/
float	CCore::GetZoomH()
{
Vector3	&ThisCam=GetCam();

		return((float)SCREEN_MAP_HEIGHT/ThisCam.z);
}

/*****************************************************************************/
void	CCore::ResetView()
{
Vector3	&ThisCam=GetCam();

		ThisCam=DefaultCamPos;
		UpdateView();
}

/*****************************************************************************/
/*** GUI *********************************************************************/
/*****************************************************************************/
void	CCore::GUIAdd(CDialog &Dlg,int ID,bool Visible,bool Lock)
{
CMainFrame	*Frm=(CMainFrame*)AfxGetMainWnd();
CMultiBar	*ParamBar=Frm->GetParamBar();

		ParamBar->Add(Dlg,ID,Visible,Lock);
}

/*****************************************************************************/
void	CCore::GUIRemove(CDialog &Dlg,int ID,bool Force)
{
CMainFrame	*Frm=(CMainFrame*)AfxGetMainWnd();
CMultiBar	*ParamBar=Frm->GetParamBar();

			ParamBar->Remove(Dlg,Force);
}

/*****************************************************************************/
void	CCore::GUIRemoveAll(bool Force)
{
CMainFrame	*Frm=(CMainFrame*)AfxGetMainWnd();
CMultiBar	*ParamBar=Frm->GetParamBar();

			ParamBar->RemoveAll(Force);
}

/*****************************************************************************/
void	CCore::GUIUpdate()
{
CMainFrame	*Frm=(CMainFrame*)AfxGetMainWnd();
CMultiBar	*ParamBar=Frm->GetParamBar();

		UpdateLayerGUI();
		UpdateGrid();
		Layer[ActiveLayer]->GUIUpdate(this);
		ParamBar->Update();
}

/*****************************************************************************/
void	CCore::GUIChanged()
{
		Layer[ActiveLayer]->GUIChanged(this);
}

/*****************************************************************************/
void	CCore::UpdateAll()
{
		UpdateParamBar();
		RedrawView();
}

/*****************************************************************************/
void	CCore::RedrawView()
{
		if (CurrentView) CurrentView->Invalidate();
}

/*****************************************************************************/
void	CCore::UpdateView(Vector3 *Ofs)
{
		if (Ofs)
		{
			Vector3	&ThisCam=GetCam();

			ThisCam.x+=Ofs->x;
			ThisCam.y+=Ofs->y;
			ThisCam.z-=Ofs->z;
			if (!TileViewFlag)
				{
				if (ThisCam.x<0) ThisCam.x=0;
				if (ThisCam.y<0) ThisCam.y=0;
			}
			if (ThisCam.z<0.1) ThisCam.z=0.1f;
		}

		SetScale();
		RedrawView();
}		

/*****************************************************************************/
void	CCore::SetMapSize(int Width,int Height)
{
		if (Width==GetMapWidth() && Height==GetMapHeight()) return;

int		ListSize=Layer.size();

		for (int i=0; i<ListSize; i++) 
		{
			Layer[i]->Resize(Width,Height);
		}

		RedrawView();
}

/*****************************************************************************/
void	CCore::Toggle2d3d()
{
		Is3dFlag=!Is3dFlag;
		RedrawView();
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

Vector3	CCore::OffsetCam(Vector3 &Cam,float DivVal)
{
Vector3	ThisCam=Cam;

	ThisCam=Cam/DivVal;
	ThisCam.z=Cam.z;

	return(ThisCam);
}

/*****************************************************************************/
void	CCore::Export(char *Filename)
{

int		LayerCount=Layer.size();
char	ExportName[256];
		
		SetFileExt(Filename,ExportName,"MEX");

CExport	Exp(ExportName);

		for (int i=0;i<LayerCount;i++)
		{
			if (Layer[i]->CanExport())
			{
				Layer[i]->Export(this,Exp);
			}
		}

		Exp.ExportTiles(this);
		Exp.ExportStrList(this);

}


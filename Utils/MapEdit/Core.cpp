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

#include	"Core.h"
#include	"Elem.h"
#include	"Layer.h"
#include	"LayerTile.h"
#include	"LayerCollision.h"
#include	"LayerShade.h"
#include	"LayerThing.h"
#include	"LayerActor.h"
#include	"LayerItem.h"
#include	"LayerPlatform.h"
#include	"utils.h"

#include	"Export.h"
#include	"GUILayerList.h"
#include	"GUIAddLayer.h"
#include	"GUINewMap.h"

#include	<IniClass.h>

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
CCore::CCore()
{
		CurrentMousePos=CPoint(0,0);
		MapCam=DefaultCamPos;
		SpareFlag=false;
		GridFlag=true;
		Is3dFlag=true;
		CurrentView=NULL;
		CursorPos.x=CursorPos.y=-1;
		CurrentLayer=0;

GString	Filename;
		IconBank=new CElemBank(16,16,false,false);

		GetExecPath(Filename);
		Filename+=theApp.GetConfigStr("FileLocation","Iconz");
		IconBank->AddSet(Filename);
		RenderTGAFlag=false;
}

/*****************************************************************************/
CCore::~CCore()
{
		IconBank->CleanUp();
		delete IconBank;
int	ListSize=Layer.size();
		for (int i=0; i<ListSize; i++) delete Layer[i];
}

/*****************************************************************************/
bool	CCore::New()
{
CGUINewMap	Dlg;
int		Width,Height;
		Dlg.m_Width=TileLayerMinWidth+2;
		Dlg.m_Height=TileLayerMinHeight+2;

#ifndef _DEBUG
		if (Dlg.DoModal()!=IDOK) return FALSE;
#endif
		Width=Dlg.m_Width;
		Height=Dlg.m_Height;

// Create Tile Layers
		AddLayer(LAYER_TYPE_TILE,LAYER_SUBTYPE_ACTION, Width, Height);
#ifdef _DEBUG
//		AddLayer(LAYER_TYPE_SHADE,LAYER_SUBTYPE_NONE, Width, Height);
//		AddLayer(LAYER_TYPE_TRIGGER,LAYER_SUBTYPE_NONE, Width, Height);
//		AddLayer(LAYER_TYPE_PLATFORM,LAYER_SUBTYPE_NONE, Width, Height);
		AddLayer(LAYER_TYPE_RGB,LAYER_SUBTYPE_NONE, Width, Height);
//		AddLayer(LAYER_TYPE_FX,LAYER_SUBTYPE_NONE, Width, Height);
//		AddLayer(LAYER_TYPE_ACTOR,LAYER_SUBTYPE_NONE, Width, Height);
//		AddLayer(LAYER_TYPE_ITEM,LAYER_SUBTYPE_NONE, Width, Height);
#endif

		for (int i=0; i<Layer.size(); i++)
		{
			Layer[i]->InitSubView(this);
		}

		ActiveLayer=FindLayer(LAYER_TYPE_TILE,LAYER_SUBTYPE_ACTION);
#ifdef _DEBUG
		ActiveLayer=FindLayer(LAYER_TYPE_HAZARD,LAYER_SUBTYPE_NONE);
		if (ActiveLayer<0) ActiveLayer=0;
#endif
		CurrentLayer=Layer[ActiveLayer];
		return(TRUE);
}

/*****************************************************************************/
void	CCore::Load(CFile *File)
{
int		Version,i;
BOOL	F;
Vector3	DuffVector;

		File->Read(&Version,sizeof(int));
		if (Version>100000) Version=1;	// Check fix for changing version to int from float
#ifndef _DEBUG
		if (Version<FileVersion)
		{
			CString mexstr;
			mexstr.Format("Old File Format\n\nPlease re-save\n");
			AfxMessageBox(mexstr,MB_OK | MB_ICONEXCLAMATION);
		}
#endif

		TRACE1("Load Version %i\n",Version);

		File->Read(&MapCam,sizeof(Vector3));
		if (Version<4)
		{
			File->Read(&MapCamOfs,sizeof(Vector3));
			File->Read(&DuffVector,sizeof(Vector3));
			File->Read(&DuffVector,sizeof(Vector3));
		}
	
		File->Read(&F,sizeof(BOOL));  SpareFlag=F!=0;
		File->Read(&F,sizeof(BOOL));  GridFlag=F!=0;
		File->Read(&F,sizeof(BOOL));  Is3dFlag=F!=0;

// Layers
int		LayerCount;
		File->Read(&LayerCount,sizeof(int));
		File->Read(&ActiveLayer,sizeof(int));

		for (i=0;i<LayerCount;i++)
		{
			CLayer	*Lyr=CLayer::LoadLayer(File,Version);
			TRACE1("Loaded %s\n",Lyr->GetName());
			AddLayer(Lyr);
		}

		for (i=0; i<Layer.size(); i++)
		{
			Layer[i]->InitSubView(this);
		}
		GetTileBank()->Load(File,Version);
		CurrentLayer=Layer[ActiveLayer];

// Check Layers
int		MapWidth=ActionLayer->GetWidth();
int		MapHeight=ActionLayer->GetHeight();
		for (i=0;i<LayerCount;i++)
		{
			Layer[i]->CheckLayerSize(MapWidth,MapHeight);
		}

}

/*****************************************************************************/
void	CCore::Validate(int Type)
{
int		LayerCount=Layer.size();
		for (int i=0;i<LayerCount;i++)
		{
			if (Layer[i]->GetType()==Type)
				Layer[i]->Validate(this);
		}
}

/*****************************************************************************/
void	CCore::Save(CFile *File)
{
BOOL	F;
// Version 1
		File->Write(&FileVersion,sizeof(int));

		File->Write(&MapCam,sizeof(Vector3));

		F=SpareFlag;	File->Write(&F,sizeof(BOOL));
		F=GridFlag;		File->Write(&F,sizeof(BOOL));
		F=Is3dFlag;		File->Write(&F,sizeof(BOOL));

// Layers
int		LayerCount=Layer.size();
		File->Write(&LayerCount,sizeof(int));
		File->Write(&ActiveLayer,sizeof(int));

		for (int i=0;i<LayerCount;i++)
		{
			Layer[i]->CLayer::Save(File);
			Layer[i]->Save(File);
		}
	GetTileBank()->Save(File);
}

/*****************************************************************************/
bool	CCore::Question(char *Txt)
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
void	CCore::Render(bool ForceRender)
{
Vector3	&ThisCam=GetCam();
		if (!CurrentView)
		{
			TRACE0("No View\n");
			UpdateAll();
			return;
		}
		if (IconBank->NeedLoad()) IconBank->LoadAllSets(this);
		if (RenderTGAFlag) RenderToTga();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear Screen

		RenderLayers(IsSubView() || CurrentLayer->IsUnique());
}

/*****************************************************************************/
void	CCore::RenderLayers(bool OneShot)
{
Vector3	&ThisCam=GetCam();
int		ListSize=Layer.size();
int		StartLayer,EndLayer;
CLayer	*ThisLayer;

		if (OneShot)
		{
			StartLayer=ActiveLayer;
			EndLayer=StartLayer+1;
			ThisLayer=CurrentLayer;
		}
		else
		{
			StartLayer=0;
			EndLayer=ListSize;
			while (Layer[StartLayer]->IsUnique()) StartLayer++;
			ThisLayer=Layer[StartLayer];
		}

		for (int i=StartLayer; i<EndLayer; i++)
		{
			ThisLayer->LoadGfx(this);
			if (ThisLayer->IsVisible())
			{
				ThisLayer->Render(this,ThisCam,Is3dFlag);
				if (GridFlag) ThisLayer->RenderGrid(this,ThisCam,i==ActiveLayer);
			}
			if (i<EndLayer-1) ThisLayer=Layer[i+1];
			
		}
		CurrentLayer->RenderCursor(this,ThisCam,Is3dFlag);
// Get Cursor Pos
		LastCursorPos=CursorPos;
		CurrentLayer->FindCursorPos(this,GetCam(),CurrentMousePos);

}

/*****************************************************************************/
void	CCore::RenderNumber(int No)
{
int		T=No/10;
int		U=No%10;		

		glPushMatrix();
		IconBank->RenderElem(0,T,0,false);
		glTranslatef(0.5f,0,0);
		IconBank->RenderElem(0,U,0,false);
		glPopMatrix();

}

/*****************************************************************************/
/*** Control *****************************************************************/
/*****************************************************************************/
void	CCore::LButtonControl(UINT nFlags, CPoint &point,bool DownFlag)
{
		if (CurrentLayer->IsVisible())
		{
			CurrentLayer->LButtonControl(this,nFlags,CursorPos,DownFlag);
		}
		Command(CmdMsg_ActiveBrushLeft,0,0);

		RedrawView();
}

/*****************************************************************************/
void	CCore::MButtonControl(UINT nFlags, CPoint &point,bool DownFlag)
{
}

/*****************************************************************************/
void	CCore::RButtonControl(UINT nFlags, CPoint &point,bool DownFlag)
{
		if (CurrentLayer->IsVisible())
		{
			CurrentLayer->RButtonControl(this,nFlags,CursorPos,DownFlag);
		}
		Command(CmdMsg_ActiveBrushRight,0,0);
		RedrawView();
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
			if (CurrentLayer->IsVisible())
			{
				CurrentLayer->MouseMove(this,nFlags,CursorPos);
			}
// Mouse still moved, so need to redraw windows, to get CursorPos
			RedrawView();
		}
		LastMousePos=CurrentMousePos;
}


/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
void	CCore::Command(int CmdMsg,int Param0,int Param1)
{
bool	RedrawFlag=false;
		switch(CmdMsg)
		{
		case CmdMsg_ToggleSubView:
			ToggleSubView();
			break;
		case CmdMsg_ToggleGrid:		
			ToggleGrid(); 
			break;
		case CmdMsg_Toggle2d:	
			Toggle2d3d();
			break;
		case CmdMsg_ZoomIn:
			Zoom(-0.1f);
			break;
		case CmdMsg_ZoomOut:
			Zoom(+0.1f);
			break;
		case CmdMsg_ResetView:
			SetCamPos(DefaultCamPos);
			break;
		case CmdMsg_SetLayer:
			SetLayer(Param0);
			break;
		case CmdMsg_AddLayer:
			AddLayer(Param0);
			break;
		case CmdMsg_DeleteLayer:
			DeleteLayer(Param0);
			break;

		case CmdMsg_Report:
			GetTileBank()->Report();
			break;

// Pass remaining to Active Layer
		default:
			RedrawFlag=CurrentLayer->Command(CmdMsg,this,Param0,Param1);
			break;

		}
		RedrawView();

}

/*****************************************************************************/
/*** Layers ******************************************************************/
/*****************************************************************************/
void	CCore::UpdateParamBar()
{
CMainFrame		*Frm=(CMainFrame*)AfxGetMainWnd();
CGUIMultiBar	*ParamBar=Frm->GetParamBar();

			GUIRemoveAll();
			GUIAdd(LayerList,IDD_LAYER_LIST);
			CurrentLayer->GUIInit(this);
			GUIUpdate();
}

/*****************************************************************************/
void		CCore::UpdateLayerGUI()
{
int			ListSize=Layer.size();

			if (LayerList.m_List)
			{
				LayerList.m_List.ResetContent();

				for (int i=0; i<ListSize; i++)
				{
					LayerList.m_List.AddString(Layer[i]->GetName());
				}
// Now sets checks (silly MSoft bug!!)
				for (i=0; i<ListSize; i++)
				{
					LayerList.m_List.SetCheck(i,Layer[i]->IsVisible());
				}
				LayerList.m_List.SetCurSel(ActiveLayer);
			}
}

/*****************************************************************************/
void		CCore::SetLayer(int NewLayer,bool Force)
{
int		LayerCount=Layer.size()-1;
int		LastLayer=ActiveLayer;

		if (NewLayer<0) NewLayer=0;

// If toggling layer, dont change the layer
		if ((int)LayerList.m_List.GetCheck(NewLayer)!=(int)Layer[NewLayer]->IsVisible())
		{
			Layer[NewLayer]->SetVisible(LayerList.m_List.GetCheck(NewLayer)!=0);
			LayerList.m_List.SetCurSel(ActiveLayer);
		}
		else
		{
			ActiveLayer=NewLayer;
		}

		if (LastLayer!=ActiveLayer || Force)
		{
			if (LastLayer<=LayerCount) Layer[LastLayer]->GUIKill(this);
			CurrentLayer=Layer[ActiveLayer];
			CurrentLayer->GUIInit(this);
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
		
		if (NewLayer->GetType()==LAYER_TYPE_TILE && NewLayer->GetSubType()==LAYER_SUBTYPE_ACTION) ActionLayer=(CLayerTile*)NewLayer;
		return(Idx);
}


/*****************************************************************************/
int		CCore::AddLayer(int Type, int SubType, int Width, int Height)
{
int		Idx;
CLayer	*Lyr;
sLayerDef	ThisDef;

		ThisDef.Type=Type;
		ThisDef.SubType=SubType;
		ThisDef.Width=Width;
		ThisDef.Height=Height;

		Lyr=CLayer::NewLayer(ThisDef);
		Idx=AddLayer(Lyr);
		if (ActionLayer) Layer[Idx]->InitSubView(this);
		return(Idx);
}

/*****************************************************************************/
void		CCore::AddLayer(int CurrentLayer)
{
std::vector<int>	List;
CGUIAddLayer		Dlg;
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
void		CCore::DeleteLayer(int ThisLayer)
{

		if (Layer[ThisLayer]->CanDelete())
		{
			Layer[ThisLayer]->GUIKill(this);
			delete Layer[ThisLayer];
			Layer.erase(Layer.begin() + ThisLayer);
			UpdateLayerGUI();

			if (ActiveLayer==ThisLayer)
			{
				ActiveLayer--;
				if (ActiveLayer<0) ActiveLayer=0;
				CurrentLayer=Layer[ActiveLayer];
				CurrentLayer->GUIInit(this);
				GUIUpdate();
			}

			TRACE1("Deleted Layer %i\n",ThisLayer);
		}
		else
		{
			TRACE1("Cant Delete Layer %i\n",ThisLayer);
		}
}

/*****************************************************************************/
/*** Grid ********************************************************************/
/*****************************************************************************/
void	CCore::ToggleGrid()
{
			GridFlag=!GridFlag;
			RedrawView();
}

/*****************************************************************************/
/*** SubView *****************************************************************/
/*****************************************************************************/
void	CCore::ToggleSubView()
{
CLayer	*LastLayer=CurrentLayer;

		CurrentLayer=CurrentLayer->GetSubView();	// <-- Funky toggle code of what!!
		if (!CurrentLayer) CurrentLayer=Layer[ActiveLayer];
		if (LastLayer!=CurrentLayer)
		{
			LastLayer->GUIKill(this);
			CurrentLayer->GUIInit(this);
			GUIUpdate();
			UpdateView();
		}
}

/*****************************************************************************/
/*** Misc ********************************************************************/
/*****************************************************************************/
Vector3	&CCore::GetCam()
{
		if (IsSubView() || CurrentLayer->IsUnique())
		{
			return(CurrentLayer->GetCam());
		}

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
void	CCore::SetCamPos(Vector3 Pos)
{
Vector3	&ThisCam=GetCam();

		ThisCam=Pos;
		UpdateView();
}

/*****************************************************************************/
void	CCore::SetCamPos(CPoint &Pos)
{
Vector3	&ThisCam=GetCam();

		ThisCam.x=Pos.x;
		ThisCam.y=Pos.y;
		UpdateView();
}

/*****************************************************************************/
/*** GUI *********************************************************************/
/*****************************************************************************/
void	CCore::GUIAdd(CDialog &Dlg,int ID,bool Visible,bool Lock)
{
CMainFrame		*Frm=(CMainFrame*)AfxGetMainWnd();
CGUIMultiBar	*ParamBar=Frm->GetParamBar();

		ParamBar->Add(Dlg,ID,Visible,Lock);
}

/*****************************************************************************/
void	CCore::GUIRemove(CDialog &Dlg,int ID,bool Force)
{
CMainFrame		*Frm=(CMainFrame*)AfxGetMainWnd();
CGUIMultiBar	*ParamBar=Frm->GetParamBar();

			ParamBar->Remove(Dlg,Force);
}

/*****************************************************************************/
void	CCore::GUIRemoveAll(bool Force)
{
CMainFrame		*Frm=(CMainFrame*)AfxGetMainWnd();
CGUIMultiBar	*ParamBar=Frm->GetParamBar();

			ParamBar->RemoveAll(Force);
}

/*****************************************************************************/
void	CCore::GUIUpdate()
{
CMainFrame		*Frm=(CMainFrame*)AfxGetMainWnd();
CGUIMultiBar	*ParamBar=Frm->GetParamBar();

		UpdateLayerGUI();
		CurrentLayer->GUIUpdate(this);
		ParamBar->Update();
}

/*****************************************************************************/
void	CCore::GUIChanged()
{
		CurrentLayer->GUIChanged(this);
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
		if (!CurrentLayer) return;
		if (Ofs)
		{
			Vector3	&ThisCam=GetCam();

			ThisCam.x+=Ofs->x;
			ThisCam.y+=Ofs->y;
			ThisCam.z-=Ofs->z;
			if (!IsSubView())
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
void	CCore::Export(const char *Filename)
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

/*****************************************************************************/
void	CCore::RenderToTga(char *Filename)
{
		TGAFilename=Filename;
		RenderTGAFlag=true;
		RedrawView();
}

/*****************************************************************************/
void	CCore::RenderToTga()
{
		RenderTGAFlag=false;
		ActionLayer->Render4TGA(TGAFilename);
}


/*******************/
/*** Layer Thing ***/
/*******************/


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
#include	"Layer.h"
#include	"LayerThing.h"
#include	"Utils.h"
#include	"Export.h"

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
CLayerThing::CLayerThing(sLayerDef &Def)
{
		InitLayer(Def);
}

/*****************************************************************************/
CLayerThing::~CLayerThing()
{
		ThingBank->CleanUp();
		delete ThingBank;
}

/*****************************************************************************/
void	CLayerThing::InitLayer(sLayerDef &Def)
{
		CLayer::InitLayer(Def);
		Mode=MouseModeNormal;
		Cursor.XY.resize(1);
		Cursor.ElemID=-1;
		CurrentDefThing=-1;
		CurrentThing=-1;
		CurrentThingPoint=0;
		DrawPoints=true;
}

/*****************************************************************************/
void	CLayerThing::Load(CFile *File,int Version)
{
int		i,ListSize;

		File->Read(&Mode,sizeof(MouseMode));
		File->Read(&CurrentDefThing,sizeof(int));
		File->Read(&CurrentThing,sizeof(int));
		File->Read(&CurrentThingPoint,sizeof(int));

		File->Read(&ListSize,sizeof(int));
		ThingList.resize(ListSize);

// Load Data
		for (i=0;i<ListSize;i++)
		{
			sLayerThing	&ThisThing=ThingList[i];
			LoadThing(File,Version,ThisThing);
			if (Version<9)
			{
				LoadOldThing(File,ThisThing);
			}
			else
			{
				if (Version==9)
				{
					sLayerThingDataV9	V9Data;
					File->Read(&V9Data,sizeof(sLayerThingDataV9));
					ThisThing.Data.WaypointCount=V9Data.WaypointCount;
					ThisThing.Data.Flip=0;
					ThisThing.Data.Angle=0;
					ThisThing.Data.RESERVE=V9Data.RESERVE;
				}
				else
				{
					File->Read(&ThisThing.Data,sizeof(sLayerThingData));
				}
				
			}
		}
		LoadThingNames(File,Version);

}

/*****************************************************************************/
void	CLayerThing::LoadThing(CFile *File,int Version,sLayerThing &ThisThing)
{
int		i,ListSize;
		File->Read(&ListSize,sizeof(int));
		ThisThing.XY.resize(ListSize);
		for (i=0 ;i<ListSize; i++)
		{
			CPoint	&ThisPoint=ThisThing.XY[i];
			File->Read(&ThisPoint,sizeof(CPoint));
		}
}

/*****************************************************************************/
void	CLayerThing::LoadThingNames(CFile *File,int Version)
{
int		i,ListSize=ThingList.size();

		for (i=0;i<ListSize;i++)
		{
			char	c=1;
			sLayerThing	&ThisThing=ThingList[i];
			GString	&ThisName=ThisThing.Name;

			while (c)
			{
				File->Read(&c,1);
				ThisName.Append(c);
			}
			// Update Gfx ID
			int	Idx=FindDefThing(ThisName);
			ThisThing.ElemID=DefList[Idx].ElemID;
		}
}

/*****************************************************************************/
void	CLayerThing::Save(CFile *File)
{
int		i,ListSize=ThingList.size();
// Always Save current version
		File->Write(&Mode,sizeof(MouseMode));
		File->Write(&CurrentDefThing,sizeof(int));
		File->Write(&CurrentThing,sizeof(int));
		File->Write(&CurrentThingPoint,sizeof(int));

		File->Write(&ListSize,sizeof(int));
		for (i=0;i<ListSize; i++)
		{
			sLayerThing	&ThisThing=ThingList[i];
			SaveThing(File,ThisThing);
			File->Write(&ThisThing.Data,sizeof(sLayerThingData));
		}
		SaveThingNames(File);
}

/*****************************************************************************/
void	CLayerThing::SaveThing(CFile *File,sLayerThing &ThisThing)
{
int		i,ListSize=ThisThing.XY.size();
		File->Write(&ListSize,sizeof(int));
		for (i=0 ;i<ListSize; i++)
		{	
			CPoint	&ThisPoint=ThisThing.XY[i];
			File->Write(&ThisPoint,sizeof(CPoint));
		}
}

/*****************************************************************************/
void	CLayerThing::SaveThingNames(CFile *File)
{
int		i,ListSize=ThingList.size();

		for (i=0; i<ListSize; i++)
		{
			char		Txt[256];
			sprintf(Txt,ThingList[i].Name);
			File->Write(Txt,strlen(Txt)+1);		
		}
}

/*****************************************************************************/
void	CLayerThing::InitSubView(CCore *Core)
{
}

/*****************************************************************************/
void	CLayerThing::LoadThingScript(const char *Filename)
{
GString	ExecPath;
GString	ScriptName;
		GetExecPath(ExecPath);
		ScriptName=ExecPath+Filename;
		ThingScript.LoadAndImport(ScriptName);

int		i,ListSize=ThingScript.GetGroupCount();
		DefList.resize(ListSize);

		for (i=0; i<ListSize; i++)
		{
			sLayerThing	&ThisDef=DefList[i];

			char	*Name=ThingScript.GetGroupName(i);
			char	*Gfx=ThingScript.GetStr(Name,"gfx");

			memset(&ThisDef.Data,0,sizeof(sLayerThingData));
			ThisDef.Name=Name;
			ThisDef.Data.WaypointCount=ThingScript.GetInt(Name,"WayPoints");
			ThisDef.Data.Flip=0;
			ThisDef.Data.Angle=0;

			LoadDefThing(Name,ThisDef);
			ThisDef.XY.resize(1);
			TRACE2("%s\t\t%s\n",Name,Gfx);
			if (Gfx) 
			{
				char	Filename[512];
				GFName::makeabsolute(ExecPath,Gfx,Filename);
				ThisDef.ElemID=ThingBank->AddSet(Filename);
			}

		}

}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
void	CLayerThing::Render(CCore *Core,Vector3 &CamPos,bool Is3d)
{
Vector3		ThisCam=Core->OffsetCam(CamPos,GetScaleFactor());
int			i,ListSize=ThingList.size();
		
		for (i=0; i<ListSize; i++)
		{
			RenderThing(Core,ThisCam,ThingList[i],Is3d,i==CurrentThing);
		}
}

/*****************************************************************************/
void	CLayerThing::RenderThing(CCore *Core,Vector3 &ThisCam,sLayerThing &ThisThing,bool Render3d,bool Selected)
{
float		ZoomW=Core->GetZoomW();
float		ZoomH=Core->GetZoomH();
Vector3		&Scale=Core->GetScaleVector();
Vector3		ScrOfs(ZoomW/2,ZoomH/2,0);

			if (Render3d)
			{
				ScrOfs.z=-4.0f;
			}


			glMatrixMode(GL_MODELVIEW);
			glPushMatrix();

			glLoadIdentity();

			glScalef(Scale.x,Scale.y,Scale.z);
			glTranslatef(-ThisCam.x,ThisCam.y,0);					// Set scroll offset
			glTranslatef(-ScrOfs.x,ScrOfs.y,0);						// Bring to top left corner

int			ListSize=ThisThing.XY.size();
			for (int i=0;i<ListSize; i++)
			{
// Render Thing			
				glPushMatrix();
				glTranslatef(ThisThing.XY[i].x,-ThisThing.XY[i].y,0);	// Set Pos
				if (i==0)
				{
					glColor4f(1,1,1,1);									// Set default Color
					glEnable(GL_DEPTH_TEST);
					glTranslatef(0,0,ScrOfs.z);
					glPushMatrix();
					glRotatef(ThisThing.Data.Angle,0,0,1);
					ThingBank->RenderElem(ThisThing.ElemID,0,ThisThing.Data.Flip,Render3d);
					glPopMatrix();
					glDisable(GL_DEPTH_TEST);
					glTranslatef(0,0,-ScrOfs.z);
					if (Selected)
						glColor4f(1,1,0,0.8f);
					else
						glColor4f(1,1,0,0.5f);
					if (DrawPoints) Core->RenderNumber(i);

				}
				else
				{
				if (Selected)
					glColor4f(1,1,1,1);									// Set default Color
				else
					glColor4f(1,1,1,0.5);									
				if (DrawPoints) Core->RenderNumber(i);
				}
				glPopMatrix();
			}

			glPopMatrix();
}

/*****************************************************************************/
/*** Gui *********************************************************************/
/*****************************************************************************/
void	CLayerThing::GUIInit(CCore *Core)
{
}

/*****************************************************************************/
void	CLayerThing::GUIKill(CCore *Core)
{
}

/*****************************************************************************/
void	CLayerThing::GUIUpdate(CCore *Core)
{
}

/*****************************************************************************/
void	CLayerThing::GUIChanged(CCore *Core)
{
}

/*****************************************************************************/
void	CLayerThing::GUIThingUpdateList(CComboBox &List,bool OnlySel)
{
int			i,ListSize;

		if (!OnlySel)
		{
// Setup ThingList
			ListSize=ThingList.size();
			List.ResetContent();
			for (i=0; i<ListSize; i++)
			{
				List.AddString(ThingList[i].Name);
			}
		}
		List.SetCurSel(CurrentThing);

}

/*****************************************************************************/
void	CLayerThing::GUIThingPointUpdateList(CListBox &List,bool OnlySel)
{
int			i,ListSize;

		if (!OnlySel)
		{
// Setup ThingPointList
			List.ResetContent();
			if (CurrentThing==-1) return;
			sLayerThing	&ThisThing=ThingList[CurrentThing];
			ListSize=ThisThing.XY.size();
				for (i=0; i<ListSize; i++)
				{
					CPoint	&ThisPoint=ThisThing.XY[i];
					CString	Str;
					Str.Format("%i: %i, %i",i,ThisPoint.x,ThisPoint.y);
					List.AddString(Str);
				}
		}
		List.SetCurSel(CurrentThingPoint);

}

/*****************************************************************************/
/*** Functions ***************************************************************/
/*****************************************************************************/
bool	CLayerThing::LButtonControl(CCore *Core,UINT nFlags, CPoint &CursorPos,bool DownFlag)
{
bool	Ret=false;

		if (!DownFlag) return(false);
		switch(Mode)
		{
		case MouseModeNormal:
			SelectThing(CursorPos);
			break;
		case MouseModeNew:
			GUIChanged(Core);
			AddThing(CursorPos);
			break;
		case MouseModePoints:
			AddThingPoint(CursorPos);
			break;
		default:
			break;
		}

		return(true);
}

/*****************************************************************************/
bool	CLayerThing::RButtonControl(CCore *Core,UINT nFlags, CPoint &CursorPos,bool DownFlag)
{
		Cancel();
		return(true);
}

/*****************************************************************************/
bool	CLayerThing::MouseMove(CCore *Core,UINT nFlags, CPoint &CursorPos)
{
bool		Ret=false;

		if (CurrentThing!=-1)
		{
			if (nFlags & MK_LBUTTON)	// Drag
			{
				UpdatePos(CursorPos,CurrentThing,CurrentThingPoint,(nFlags & MK_CONTROL)!=0);
				Ret=true;
			}
			else
			if (nFlags & MK_RBUTTON)	// Cancel
			{
				Cancel();
				Ret=true;
			}
		}
		return(Ret);
}

/*****************************************************************************/
void	CLayerThing::Cancel()
{
		switch(Mode)
		{
		case MouseModeNormal:
			CurrentThing=-1;
		case MouseModeNew:
		case MouseModePoints:
			Mode=MouseModeNormal;
			CurrentThingPoint=0;
			GUIThingDefClear();
			GUIThingUpdate();
			GUIThingPointUpdate();
			break;
		default:
			break;
		}
}

/*****************************************************************************/
bool	CLayerThing::Command(int CmdMsg,CCore *Core,int Param0,int Param1)
{
bool	Ret=false;

		switch(CmdMsg)
		{
		case CmdMsg_MirrorX:
			if (CanMirror())
			if (CurrentThing!=-1)
			{
				ThingList[CurrentThing].Data.Flip^=PC_TILE_FLAG_MIRROR_X;
			}
			break;
		case CmdMsg_MirrorY:
			if (CanMirror())
			if (CurrentThing!=-1)
			{
				ThingList[CurrentThing].Data.Flip^=PC_TILE_FLAG_MIRROR_Y;
			}
			break;
		case CmdMsg_Rotate:
			if (CanRotate())
			if (CurrentThing!=-1)
			{
				ThingList[CurrentThing].Data.Angle+=90;
				if (ThingList[CurrentThing].Data.Angle>359)
				{
					ThingList[CurrentThing].Data.Angle=0;
				}
			}
			break;
		case CmdMsg_ThingListDelete:
			DeleteThing();
			break;
		case CmdMsg_ThingListGoto:
			GotoThing(Core);
			break;
		case CmdMsg_ThingListSelect:
			CurrentDefThing=Param0;
			SetCursor(DefList[CurrentDefThing].Name);
			Mode=MouseModeNew;
			break;
		case CmdMsg_ThingLevelSelect:
			SelectThing(Param0);
			break;
		case CmdMsg_ThingPosSelect:
			CurrentThingPoint=Param0;
			GUIThingPointUpdate();
			break;
		case CmdMsg_ThingPosUp:
			MovePoint(-1);
			break;
		case CmdMsg_ThingPosDown:
			MovePoint(+1);
			break;
		case CmdMsg_ThingPosDelete:
			DeletePoint();
			break;
		default:
			break;
		}

		return(Ret);
}

/*****************************************************************************/
int		CLayerThing::FindDefThing(const char *Name)
{
int		i,ListSize=DefList.size();

		for (i=0; i<ListSize; i++)
		{
			if (DefList[i]==Name) return(i);
		}
		return(-1);
}

/*****************************************************************************/
void	CLayerThing::SetCursor(const char *Name)
{
int		Idx=FindDefThing(Name);
		if (Idx==-1) return;

		Cursor=DefList[Idx];
}

/*****************************************************************************/
void	CLayerThing::RenderCursor(CCore *Core,Vector3 &CamPos,bool Is3d)
{
Vector3		ThisCam=Core->OffsetCam(CamPos,GetScaleFactor());
CPoint		&CursPos=Core->GetCursorPos();

		Cursor.XY[0]=CursPos;

		if (CursPos.x<0 || CursPos.y<0) return;
		if (Mode!=MouseModeNew) return;
		if (Cursor.ElemID==-1) return;

		RenderThing(Core,ThisCam,Cursor,Is3d,true);
}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
int		CLayerThing::CheckThing(CPoint &Pos)
{
CList<int>	List;
int			Idx=-1,i,ListSize=ThingList.size();
int			StartIdx=0;

// Build List Of XY Matches
		for (i=0; i<ListSize; i++)
		{
			sLayerThing	&ThisThing=ThingList[i];
			if (ThisThing.XY[0]==Pos) 
			{
				if (i==CurrentThing) StartIdx=List.size();
				List.push_back(i);
			}
		}

		ListSize=List.size();
		if (ListSize)
		{
			StartIdx=(StartIdx+1)%ListSize;
			Idx=List[StartIdx];
		}
		return(Idx);
}

/*****************************************************************************/
void	CLayerThing::AddThing(CPoint &Pos)
{
		if (Pos.x==-1 || Pos.y==-1) return;	// Off Map?
		CurrentThing=SelectThing(Pos);
		if (CurrentThing!=-1) return;


		CurrentThing=ThingList.size();
		ThingList.resize(CurrentThing+1);

sLayerThing	&ThisThing=ThingList[CurrentThing];
		ThisThing=Cursor;
		SetThingParams(ThisThing);
		SelectThing(CurrentThing);
		GUIThingDefClear();
}

/*****************************************************************************/
int		CLayerThing::SelectThing(CPoint &Pos)
{
		if (Pos.x==-1 || Pos.y==-1) return(-1);	// Off Map?
		SelectThing(CheckThing(Pos));
		return(CurrentThing);
}

/*****************************************************************************/
int		CLayerThing::SelectThing(int Idx)
{
		CurrentThing=Idx;
		CurrentThingPoint=0;
		if (CurrentThing!=-1)
		{
			Mode=MouseModePoints;
			GUIThingUpdate();
			GUIThingPointUpdate();
		}
		return(CurrentThing);
}

/*****************************************************************************/
void	CLayerThing::DeleteThing()
{
		if (!ThingList.size()) return;

		ThingList.erase(CurrentThing);
		CurrentThing--;
		GUIThingUpdate();
}

/*****************************************************************************/
void	CLayerThing::GotoThing(CCore *Core)
{
		if (CurrentThing==-1) return;
sLayerThing	&ThisThing=ThingList[CurrentThing];

		Core->SetCamPos(ThisThing.XY[0]);
}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
int		CLayerThing::CheckThingPoint(CPoint &Pos)
{
CList<int>	List;
sLayerThing	&ThisThing=ThingList[CurrentThing];
int			Idx=-1,i,ListSize=ThisThing.XY.size();
int			StartIdx=0;

// Build List Of XY Matches
		for (i=0; i<ListSize; i++)
		{
			if (ThisThing.XY[i]==Pos) 
			{
				if (i==CurrentThing) StartIdx=List.size();
				List.push_back(i);
			}
		}

		ListSize=List.size();
		if (ListSize)
		{
			StartIdx=(StartIdx+1)%ListSize;
			Idx=List[StartIdx];
		}
		return(Idx);
}

/*****************************************************************************/
void	CLayerThing::AddThingPoint(CPoint &Pos)
{
		TRACE1("ADDTHINGPOINT %i\n",CurrentThingPoint);
		if (Pos.x==-1 || Pos.y==-1) return;	// Off Map?

		CurrentThingPoint=SelectThingPoint(Pos);
		
		if (CurrentThingPoint!=-1) 
		{
			GUIThingPointUpdate(true);
			return;
		}
sLayerThing	&ThisThing=ThingList[CurrentThing];

		int	PntCount=ThisThing.XY.size();
		if (PntCount<ThisThing.Data.WaypointCount+1)
		{
			CurrentThingPoint=ThisThing.XY.size();
			ThisThing.XY.resize(CurrentThingPoint+1);
			ThisThing.XY[CurrentThingPoint]=Pos;
			TRACE0("Add Point\n");
			GUIThingPointUpdate();
		}

}

/*****************************************************************************/
int		CLayerThing::SelectThingPoint(CPoint &Pos)
{
		if (Pos.x==-1 || Pos.y==-1) return(-1);	// Off Map?
		CurrentThingPoint=CheckThingPoint(Pos);
		return(CurrentThingPoint);
}

/*****************************************************************************/
void	CLayerThing::UpdatePos(CPoint &Pos,int Thing,int PosIdx,bool Recurs)
{
		if (Pos.x==-1 || Pos.y==-1) return;	// Off Map?

sLayerThing	&ThisThing=ThingList[Thing];
CPoint		dPos=Pos-ThisThing.XY[PosIdx];
int			StartIdx=PosIdx,EndIdx=ThisThing.XY.size();

		if (!dPos.x && !dPos.y) return;

		if (!Recurs)
		{
			StartIdx=PosIdx;
			EndIdx=StartIdx+1;
		}

		for (int i=StartIdx; i<EndIdx; i++)
		{
			ThisThing.XY[i]+=dPos;
		}
		GUIThingPointUpdate();

}

/*****************************************************************************/
void	CLayerThing::MovePoint(int Dir)
{
		if (CurrentThing==-1) return;

int		NewPos=CurrentThingPoint+Dir;
sLayerThing	&ThisThing=ThingList[CurrentThing];

		if (NewPos<0 || NewPos>=ThisThing.XY.size()) return;

CPoint	Tmp=ThisThing.XY[CurrentThingPoint];
		ThisThing.XY[CurrentThingPoint]=ThisThing.XY[NewPos];
		ThisThing.XY[NewPos]=Tmp;
		CurrentThingPoint=NewPos;
		GUIThingPointUpdate();
}

/*****************************************************************************/
void	CLayerThing::DeletePoint()
{
		if (!CurrentThingPoint) return;
		if (CurrentThing==-1) return;

sLayerThing	&ThisThing=ThingList[CurrentThing];

		ThisThing.XY.erase(CurrentThingPoint);
		CurrentThingPoint--;
		if (CurrentThingPoint<0) CurrentThingPoint=0;
		GUIThingPointUpdate();
}

/*****************************************************************************/
int		CLayerThing::GetWidth()
{
		return(theApp.GetCurrent()->GetMapWidth());
}

/*****************************************************************************/
int		CLayerThing::GetHeight()
{
		return(theApp.GetCurrent()->GetMapHeight());
}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
void	CLayerThing::Export(CCore *Core,CExport &Exp)
{
int		i,ListSize=ThingList.size();
		Exp.ExportLayerHeader(LayerDef);

		Exp.Write(&ListSize,sizeof(int));
		for (i=0;i<ListSize; i++)
		{
			ExportThing(Core,Exp,ThingList[i]);
		}
		ExportThingNames(Exp);
}

/*****************************************************************************/
void	CLayerThing::ExportThing(CCore *Core,CExport &Exp,sLayerThing &ThisThing)
{
int				i,ListSize=ThisThing.XY.size();
sLayerThingData	OutThing=ThisThing.Data;

		OutThing.WaypointCount=ListSize;
		ExportThingData(Core,Exp,ThisThing,OutThing);
		Exp.Write(&OutThing,sizeof(sLayerThingData));

// Point List
		for (i=0 ;i<ListSize; i++)
		{
			Exp.Write(&ThisThing.XY[i],sizeof(CPoint));
		}
}

/*****************************************************************************/
void	CLayerThing::ExportThingNames(CExport &Exp)
{
int		i,ListSize=ThingList.size();

		for (i=0; i<ListSize; i++)
		{
			char		Txt[256];
			sprintf(Txt,ThingList[i].Name);
			Exp.Write(Txt,strlen(Txt)+1);		
		}
}


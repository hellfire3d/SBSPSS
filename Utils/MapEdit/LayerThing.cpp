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
// New Layer
CLayerThing::CLayerThing(int _SubType,int _Width,int _Height)
{
		SetDefaultParams();

		Mode=MouseModeNormal;
		Width=_Width;
		Height=_Height;
		ThingBank=new CElemBank(-1,-1,false,true);
		CurrentThing=-1;
		CurrentPoint=0;

		ThingBank->AddSet("\\spongebob\\graphics\\Babyoctopus.bmp");
		ThingBank->AddSet("\\spongebob\\graphics\\barnicleboy.bmp");
		ThingBank->AddSet("\\spongebob\\graphics\\Caterpillar.bmp");
		ThingBank->AddSet("\\spongebob\\graphics\\clam.bmp");
		ThingBank->AddSet("\\spongebob\\graphics\\Eyeball.bmp");
		ThingBank->AddSet("\\spongebob\\graphics\\FlyingDutchman.bmp");
		ThingBank->AddSet("\\spongebob\\graphics\\gary.bmp");
		ThingBank->AddSet("\\spongebob\\graphics\\GiantWorm.bmp");
		ThingBank->AddSet("\\spongebob\\graphics\\HermitCrab.bmp");
		ThingBank->AddSet("\\spongebob\\graphics\\IronDogFish.bmp");
		ThingBank->AddSet("\\spongebob\\graphics\\Krusty.bmp");
		ThingBank->AddSet("\\spongebob\\graphics\\MermaidMan.bmp");
		ThingBank->AddSet("\\spongebob\\graphics\\Neptune.bmp");
		ThingBank->AddSet("\\spongebob\\graphics\\Patrick.bmp");
		ThingBank->AddSet("\\spongebob\\graphics\\plankton.bmp");
		ThingBank->AddSet("\\spongebob\\graphics\\PuffaFish.bmp");
		ThingBank->AddSet("\\spongebob\\graphics\\Sandy.bmp");
		ThingBank->AddSet("\\spongebob\\graphics\\SeaSnake.bmp");
		ThingBank->AddSet("\\spongebob\\graphics\\SharkSub.bmp");
		ThingBank->AddSet("\\spongebob\\graphics\\SpiderCrab.bmp");
		ThingBank->AddSet("\\spongebob\\graphics\\spongebob.bmp");
		ThingBank->AddSet("\\spongebob\\graphics\\Squidward.bmp");

}

/*****************************************************************************/
// Load Layer
CLayerThing::CLayerThing(CFile *File,int Version)
{
		Load(File,Version);
}

/*****************************************************************************/
CLayerThing::~CLayerThing()
{
		ThingBank->CleanUp();
		delete ThingBank;
}

/*****************************************************************************/
void	CLayerThing::Load(CFile *File,int Version)
{
		File->Read(&VisibleFlag,sizeof(BOOL));
		File->Read(&Mode,sizeof(MouseMode));

		TRACE1("%s\t",GetName());
}

/*****************************************************************************/
void	CLayerThing::Save(CFile *File)
{
// Always Save current version
		File->Write(&VisibleFlag,sizeof(BOOL));
		File->Write(&Mode,sizeof(MouseMode));
}

/*****************************************************************************/
void	CLayerThing::InitSubView(CCore *Core)
{
}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
void	CLayerThing::Render(CCore *Core,Vector3 &CamPos,bool Is3d)
{
Vector3		ThisCam=Core->OffsetCam(CamPos,GetScaleFactor());
int			i,ListSize=ThingList.size();

//		Is3d&=Render3dFlag;
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
float		ScrOfsX=(ZoomW/2);
float		ScrOfsY=(ZoomH/2);
Vector3		&Scale=Core->GetScaleVector();
CElemBank	*IconBank=Core->GetIconBank();

			if (ThingBank->NeedLoad()) ThingBank->LoadAllSets(Core);

			glMatrixMode(GL_MODELVIEW);
			glPushMatrix();

			glLoadIdentity();

			glScalef(Scale.x,Scale.y,Scale.z);
			glTranslatef(-ThisCam.x,ThisCam.y,0);					// Set scroll offset
			glTranslatef(-ScrOfsX,ScrOfsY,0);						// Bring to top left corner

int			ListSize=ThisThing.XY.size();
			TRACE1("%i pts\n",ListSize);
			for (int i=0;i<ListSize; i++)
			{
// Render Thing			
				glPushMatrix();
				glTranslatef(ThisThing.XY[i].x,-ThisThing.XY[i].y,0);	// Set Pos
				if (Selected)
					glColor4f(1,1,1,1);									// Set default Color
				else
					glColor4f(1,1,1,0.5);									
				
				IconBank->RenderElem(0,i,0,Render3d);
				if (i==0)
				{
					glColor4f(1,1,1,1);									// Set default Color
					ThingBank->RenderElem(ThisThing.Type,0,0,Render3d);
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
//		Core->GUIAdd(GUIToolBar,IDD_TOOLBAR);
}

/*****************************************************************************/
void	CLayerThing::GUIKill(CCore *Core)
{
//		Core->GUIRemove(GUIToolBar,IDD_TOOLBAR);
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
/*** Functions ***************************************************************/
/*****************************************************************************/
bool	CLayerThing::LButtonControl(CCore *Core,UINT nFlags, CPoint &CursorPos,bool DownFlag)
{
bool	Ret=false;

		switch(Mode)
		{
		case MouseModeNormal:
			if (DownFlag)
			{
				if (CurrentThing==-1)
					AddThing(CursorPos);
				else
					AddThingPoint(CursorPos);

			}
			break;
		case MouseModePoints:
			break;
		default:
			break;
		}

		return(true);
}

/*****************************************************************************/
bool	CLayerThing::RButtonControl(CCore *Core,UINT nFlags, CPoint &CursorPos,bool DownFlag)
{
bool		Ret=FALSE;

		switch(Mode)
		{
		case MouseModeNormal:
			if (DownFlag)
			{
				SelectThing(CursorPos);
			}
			break;
		case MouseModePoints:
			break;
		}

		return(Ret);
}

/*****************************************************************************/
bool	CLayerThing::MouseMove(CCore *Core,UINT nFlags, CPoint &CursorPos)
{
bool		Ret=false;

		if (CurrentThing!=-1)
		{
			if (nFlags & MK_LBUTTON)	// Drag
			{
				UpdatePos(CursorPos,CurrentThing,CurrentPoint,true);
				Ret=true;
			}
			else
			if (nFlags & MK_RBUTTON)	// Cancel
			{
				CurrentThing=-1;
				Ret=true;
			}
		}
		return(Ret);
}

/*****************************************************************************/
bool	CLayerThing::Command(int CmdMsg,CCore *Core,int Param0,int Param1)
{
bool	Ret=false;
/*
		switch(CmdMsg)
		{
		case CmdMsg_SetMode:
//			Mode=(MouseMode)Param0;
//			Core->GUIUpdate();
//			break;
//		case CmdMsg_SubViewSet:
//			Ret=ThingBank->Command(CmdMsg,Core,Param0,Param1);
			break;
		default:
			break;
		}
*/
		return(Ret);
}

/*****************************************************************************/
void	CLayerThing::RenderCursor(CCore *Core,Vector3 &CamPos,bool Is3d)
{
Vector3		ThisCam=Core->OffsetCam(CamPos,GetScaleFactor());
CPoint		&CursPos=Core->GetCursorPos();
Vector3		Ofs;

		if (CursPos.x<0 || CursPos.y<0) return;

		Ofs.x=-(CursPos.x-(int)ThisCam.x);
		Ofs.y=-(CursPos.y-(int)ThisCam.y);
		ThisCam.x-=(int)ThisCam.x;
		ThisCam.y-=(int)ThisCam.y;

		if (Is3d && Render3dFlag)
		{
			glEnable(GL_DEPTH_TEST);
//			Render(Core,ThisCam,,TRUE,0.5,&Ofs);
			glDisable(GL_DEPTH_TEST);
		}
			else
		{
//			Render(Core,ThisCam,Brush,FALSE,0.5,&Ofs);
		}
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
		CurrentThing=CheckThing(Pos);
		CurrentPoint=0;
		if (CurrentThing!=-1) return;

		CurrentThing=ThingList.size();
		ThingList.resize(CurrentThing+1);

sLayerThing	&ThisThing=ThingList[CurrentThing];
		
		ThisThing.XY.push_back(Pos);
		ThisThing.Type=ThingList.size()%22;
		ThisThing.SubType=0;
}

/*****************************************************************************/
void	CLayerThing::SelectThing(CPoint &Pos)
{
		if (Pos.x==-1 || Pos.y==-1) return;	// Off Map?
		CurrentThing=CheckThing(Pos);
		CurrentPoint=0;
}


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
		if (Pos.x==-1 || Pos.y==-1) return;	// Off Map?
		CurrentPoint=CheckThingPoint(Pos);

		if (CurrentPoint!=-1) return;
sLayerThing	&ThisThing=ThingList[CurrentThing];

		CurrentPoint=ThisThing.XY.size();
		ThisThing.XY.resize(CurrentPoint+1);
		ThisThing.XY[CurrentPoint]=Pos;
}

/*****************************************************************************/
void	CLayerThing::SelectThingPoint(CPoint &Pos)
{
		if (Pos.x==-1 || Pos.y==-1) return;	// Off Map?
		CurrentPoint=CheckThing(Pos);
}

/*****************************************************************************/
void	CLayerThing::UpdatePos(CPoint &Pos,int Thing,int PosIdx,bool Recurs)
{
		if (Pos.x==-1 || Pos.y==-1) return;	// Off Map?

sLayerThing	&ThisThing=ThingList[Thing];
CPoint		dPos=Pos-ThisThing.XY[PosIdx];
int			StartIdx=PosIdx,EndIdx=ThisThing.XY.size();

		if (!Recurs)
		{
			StartIdx=PosIdx;
			EndIdx=StartIdx++;
		}

		for (int i=StartIdx; i<EndIdx; i++)
		{
			ThisThing.XY[i]+=dPos;
		}
}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
void	CLayerThing::Export(CCore *Core,CExport &Exp)
{
/*
int				Width=Map.GetWidth();
int				Height=Map.GetHeight();

		Exp.ExportLayerHeader(LAYER_TYPE_Elem,SubType,Width,Height);

		for (int Y=0; Y<Height; Y++)
		{
			for (int X=0; X<Width; X++)
			{
				sMapElem		&MapElem=Map.Get(X,Y);
				sExpLayerThing	OutElem;

				if (MapElem.Set==0 && MapElem.Elem==0)
				{ // Blank
					OutElem.Elem=0;
					OutElem.Flags=0;
				}
				else
				{
					sExpElem		OutElem;
					CElem			&ThisElem=ElemBank->GetElem(MapElem.Set,MapElem.Elem);

					OutElem.Set=MapElem.Set;
					OutElem.Elem=MapElem.Elem;
					OutElem.TriStart=0;
					OutElem.TriCount=0;
					OutElem.XOfs=ThisElem.GetTexXOfs();
					OutElem.YOfs=ThisElem.GetTexYOfs();
					OutElem.Elem=Exp.AddElem(OutElem);
					OutElem.Flags=MapElem.Flags;
				}

				Exp.Write(&OutElem,sizeof(sExpLayerThing));
			}
		}
*/
}

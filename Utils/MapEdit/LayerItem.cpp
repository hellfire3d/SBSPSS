/******************/
/*** Layer Item ***/
/******************/


#include	"stdafx.h"
#include	<Vector3.h>
#include	<gl\gl.h>
#include	<gl\glu.h>
#include	"GLEnabledView.h"

#include	"MapEdit.h"
#include	"MapEditDoc.h"
#include	"MapEditView.h"
#include	"MainFrm.h"

//#include	"ItemSet.h"

#include	"Core.h"
#include	"Layer.h"
#include	"LayerItem.h"
#include	"Utils.h"
//#include	"Select.h"
#include	"Export.h"

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
// New Layer
CLayerItem::CLayerItem(int _SubType,int _Width,int _Height)
{
		SetDefaultParams();

		Mode=MouseModePaint;
}

/*****************************************************************************/
// Load Layer
CLayerItem::CLayerItem(CFile *File,int Version)
{
		Load(File,Version);
}

/*****************************************************************************/
CLayerItem::~CLayerItem()
{
		ElemBank->CleanUp();
		delete ElemBank;
}

/*****************************************************************************/
void	CLayerItem::Load(CFile *File,int Version)
{
		File->Read(&VisibleFlag,sizeof(BOOL));
		File->Read(&Mode,sizeof(MouseMode));

		TRACE1("%s\t",GetName());
}

/*****************************************************************************/
void	CLayerItem::Save(CFile *File)
{
// Always Save current version
		File->Write(&VisibleFlag,sizeof(BOOL));
		File->Write(&Mode,sizeof(MouseMode));
}

/*****************************************************************************/
void	CLayerItem::InitSubView(CCore *Core)
{
		ElemBank=new CElemStore;
		SubView=ElemBank;
}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
void	CLayerItem::Render(CCore *Core,Vector3 &CamPos,bool Is3d)
{
/*
Vector3		ThisCam=Core->OffsetCam(CamPos,GetScaleFactor());

		Is3d&=Render3dFlag;
		Render(Core,ThisCam,Map,Is3d);
*/
}

/*****************************************************************************/
void	CLayerItem::RenderCursorPaint(CCore *Core,Vector3 &CamPos,bool Is3d)
{
/*
Vector3		ThisCam=Core->OffsetCam(CamPos,GetScaleFactor());
CPoint		&CursPos=Core->GetCursorPos();
CMap		&Brush=ElemBank->GetActiveBrush();
Vector3		Ofs;

		if (!Brush.IsValid()) return;

		if (CursPos.x<0 || CursPos.y<0) return;

		Ofs.x=-(CursPos.x-(int)ThisCam.x);
		Ofs.y=-(CursPos.y-(int)ThisCam.y);
		ThisCam.x-=(int)ThisCam.x;
		ThisCam.y-=(int)ThisCam.y;

		if (Is3d && Render3dFlag)
		{
			glEnable(GL_DEPTH_TEST);
			Render(Core,ThisCam,Brush,TRUE,0.5,&Ofs);
			glDisable(GL_DEPTH_TEST);
		}
			else
		{
			Render(Core,ThisCam,Brush,FALSE,0.5,&Ofs);
		}
*/
}

/*****************************************************************************/
void	CLayerItem::Render(CCore *Core,Vector3 &ThisCam,CMap &ThisMap,bool Render3d,float Alpha,Vector3 *Ofs)
{
/*
int			MapWidth=ThisMap.GetWidth();
int			MapHeight=ThisMap.GetHeight();
float		ZoomW=Core->GetZoomW();
float		ZoomH=Core->GetZoomH();
float		ScrOfsX=(ZoomW/2);
float		ScrOfsY=(ZoomH/2);
Vector3		&Scale=Core->GetScaleVector();
bool		WrapMap=SubType==LAYER_SUBTYPE_BACK;
int			StartX=(int)ThisCam.x;
int			StartY=(int)ThisCam.y;
float		ShiftX=ThisCam.x - (int)ThisCam.x;
float		ShiftY=ThisCam.y - (int)ThisCam.y;

		if (ElemBank->NeedLoad()) ElemBank->LoadAllSets(Core);

		if (StartX<0) StartX=0;
		if (StartY<0) StartY=0;

int		DrawW=ZoomW+8;
int		DrawH=ZoomH+8;

		if (StartX+DrawW>MapWidth)	DrawW=MapWidth-StartX;
		if (StartY+DrawH>MapHeight)	DrawH=MapHeight-StartY;

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();
		glScalef(Scale.x,Scale.y,Scale.z);
		glTranslatef(-ShiftX,ShiftY,0);		// Set scroll offset
		glTranslatef(-ScrOfsX,ScrOfsY,0);	// Bring to top left corner


		if (Ofs)
		{
		glTranslatef(-Ofs->x,Ofs->y,0);		// Set scroll offset
		}

		if (WrapMap)
		{
			DrawW=MapWidth;
			DrawH=MapHeight;
		}
		for (int YLoop=0; YLoop<DrawH; YLoop++)
		{
			for (int XLoop=0; XLoop<DrawW; XLoop++)
			{
				int	XPos=StartX+XLoop;
				int	YPos=StartY+YLoop;
				if (WrapMap)
				{
					XPos%=MapWidth;
					YPos%=MapHeight;
				}
				
				sMapElem	&ThisElem=ThisMap.Get(XPos,YPos);
				if (ThisElem.Item>0)
				{ // Render Non Zero and Valid Items
					glColor4f(1,1,1,Alpha);	// Set default Color
					ElemBank->RenderItem(ThisElem.Set,ThisElem.Item,ThisElem.Flags,Render3d);
				}
				glTranslatef(1.0f,0,0);	// Next X
			}
			glTranslatef(-DrawW,-1,0); // Next y, rewind to start X
		}
		glPopMatrix();
*/
}

/*****************************************************************************/
/*** Gui *********************************************************************/
/*****************************************************************************/
void	CLayerItem::GUIInit(CCore *Core)
{
		ElemBank->GUIInit(Core);
		Core->GUIAdd(GUIToolBar,IDD_LAYERTILE_TOOLBAR);
}

/*****************************************************************************/
void	CLayerItem::GUIKill(CCore *Core)
{
		ElemBank->GUIKill(Core);
		Core->GUIRemove(GUIToolBar,IDD_LAYERTILE_TOOLBAR);
}

/*****************************************************************************/
void	CLayerItem::GUIUpdate(CCore *Core)
{
		GUIToolBar.ResetButtons();
		switch(Mode)
		{
		case MouseModePaint:
			GUIToolBar.SetButtonState(CGUIToolBar::PAINT,true);
			break;
		case MouseModeSelect:
			GUIToolBar.SetButtonState(CGUIToolBar::SELECT,true);
			break;
		default:
			break;
		}
		ElemBank->GUIUpdate(Core);
}

/*****************************************************************************/
void	CLayerItem::GUIChanged(CCore *Core)
{
}

/*****************************************************************************/
/*** Functions ***************************************************************/
/*****************************************************************************/
bool	CLayerItem::LButtonControl(CCore *Core,UINT nFlags, CPoint &CursorPos,bool DownFlag)
{
bool	Ret=false;
/*
		switch(Mode)
		{
		case MouseModePaint:
			if (DownFlag)
				Ret=Paint(ElemBank->GetLBrush(),CursorPos);
			break;
		case MouseModeSelect:
				Ret=Selection.Handle(CursorPos,nFlags);
				if (Selection.HasSelection())
				{
					TRACE0("LMB Selection\n");
				}
			break;
		default:
			break;
		}
*/
		return(Ret);
}

/*****************************************************************************/
bool	CLayerItem::RButtonControl(CCore *Core,UINT nFlags, CPoint &CursorPos,bool DownFlag)
{
bool		Ret=FALSE;
/*
		switch(Mode)
		{
		case MouseModePaint:
			if (DownFlag)
				Ret=Paint(ElemBank->GetRBrush(),CursorPos);
			break;
		case MouseModeSelect:
				Ret=Selection.Handle(CursorPos,nFlags);
				if (Selection.HasSelection())
				{
					TRACE0("RMB Selection\n");
				}
			break;
		default:
			break;
		}
*/
		return(Ret);
}

/*****************************************************************************/
bool	CLayerItem::MouseMove(CCore *Core,UINT nFlags, CPoint &CursorPos)
{
bool		Ret=FALSE;
/*
		switch(Mode)
		{
		case MouseModePaint:
			if (nFlags & MK_LBUTTON)
				Ret=Paint(ElemBank->GetLBrush(),CursorPos);
			else
			if (nFlags & MK_RBUTTON)
				Ret=Paint(ElemBank->GetRBrush(),CursorPos);
			break;
		case MouseModeSelect:
			Ret=Selection.Handle(CursorPos,nFlags);
			break;
		default:
			break;
		}
*/
		return(Ret);
}

/*****************************************************************************/
bool	CLayerItem::Command(int CmdMsg,CCore *Core,int Param0,int Param1)
{
bool	Ret=false;

		switch(CmdMsg)
		{
		case CmdMsg_SetMode:
			Mode=(MouseMode)Param0;
			Core->GUIUpdate();
			break;
		case CmdMsg_SubViewSet:
			Ret=ElemBank->Command(CmdMsg,Core,Param0,Param1);
			break;
		default:
			TRACE3("LayerItem-Unhandled Command %i (%i,%i)\n",CmdMsg,Param0,Param1);
		}

		return(Ret);
}

/*****************************************************************************/
void	CLayerItem::RenderCursor(CCore *Core,Vector3 &CamPos,bool Is3d)
{
		RenderCursorPaint(Core,CamPos,Is3d);
}

/*****************************************************************************/
bool	CLayerItem::Paint(CMap &Blk,CPoint &CursorPos)
{

		if (CursorPos.x==-1 || CursorPos.y==-1) return(false);	// Off Map?
		if (!Blk.IsValid()) return(false);	// Invalid Item?
/*
		Map.Set(CursorPos.x,CursorPos.y,Blk);
*/
		return(true);
}

/*****************************************************************************/
void	CLayerItem::Export(CCore *Core,CExport &Exp)
{
/*
int				Width=Map.GetWidth();
int				Height=Map.GetHeight();

		Exp.ExportLayerHeader(LAYER_TYPE_Item,SubType,Width,Height);

		for (int Y=0; Y<Height; Y++)
		{
			for (int X=0; X<Width; X++)
			{
				sMapElem		&MapElem=Map.Get(X,Y);
				sExpLayerItem	OutElem;

				if (MapElem.Set==0 && MapElem.Item==0)
				{ // Blank
					OutElem.Item=0;
					OutElem.Flags=0;
				}
				else
				{
					sExpItem		OutItem;
					CElem			&ThisItem=ElemBank->GetItem(MapElem.Set,MapElem.Item);

					OutItem.Set=MapElem.Set;
					OutItem.Item=MapElem.Item;
					OutItem.TriStart=0;
					OutItem.TriCount=0;
					OutItem.XOfs=ThisItem.GetTexXOfs();
					OutItem.YOfs=ThisItem.GetTexYOfs();
					OutElem.Item=Exp.AddItem(OutItem);
					OutElem.Flags=MapElem.Flags;
				}

				Exp.Write(&OutElem,sizeof(sExpLayerItem));
			}
		}
*/
}

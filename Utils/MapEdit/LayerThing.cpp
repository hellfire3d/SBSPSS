/******************/
/*** Layer Elem ***/
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

//#include	"ElemSet.h"

#include	"Core.h"
#include	"Layer.h"
#include	"LayerElem.h"
#include	"Utils.h"
//#include	"Select.h"
#include	"Export.h"

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
// New Layer
CLayerElem::CLayerElem(int _SubType,int _Width,int _Height)
{
		SetDefaultParams();

		Mode=MouseModePaint;
//		ElemBank=new CElemBank;

}

/*****************************************************************************/
// Load Layer
CLayerElem::CLayerElem(CFile *File,int Version)
{
		Load(File,Version);
}

/*****************************************************************************/
CLayerElem::~CLayerElem()
{
//		ElemBank->CleanUp();
//		delete ElemBank;
}

/*****************************************************************************/
void	CLayerElem::Load(CFile *File,int Version)
{
		File->Read(&VisibleFlag,sizeof(BOOL));
		File->Read(&Mode,sizeof(MouseMode));

		TRACE1("%s\t",GetName());
}

/*****************************************************************************/
void	CLayerElem::Save(CFile *File)
{
// Always Save current version
		File->Write(&VisibleFlag,sizeof(BOOL));
		File->Write(&Mode,sizeof(MouseMode));
}

/*****************************************************************************/
void	CLayerElem::InitSubView(CCore *Core)
{
		ElemBank=&Core->GetIconz();
}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
void	CLayerElem::Render(CCore *Core,Vector3 &CamPos,bool Is3d)
{
Vector3		ThisCam=Core->OffsetCam(CamPos,GetScaleFactor());

		Is3d&=Render3dFlag;
		for (int x=0; x<20; x++)
		{
			CPoint	XY;
			XY.x=x;
			XY.y=0;

			RenderElem(Core,ThisCam,XY,x&7,Is3d);
		}
}

/*****************************************************************************/
void	CLayerElem::RenderElem(CCore *Core,Vector3 &ThisCam,CPoint &Pos,int Elem,bool Render3d,float Alpha)
{
float		ZoomW=Core->GetZoomW();
float		ZoomH=Core->GetZoomH();
float		ScrOfsX=(ZoomW/2);
float		ScrOfsY=(ZoomH/2);
Vector3		&Scale=Core->GetScaleVector();

			if (ElemBank->NeedLoad()) ElemBank->LoadAllSets(Core);

			glMatrixMode(GL_MODELVIEW);
			glPushMatrix();

			glLoadIdentity();

			glScalef(Scale.x,Scale.y,Scale.z);
			glTranslatef(-ThisCam.x,ThisCam.y,0);		// Set scroll offset
			glTranslatef(-ScrOfsX,ScrOfsY,0);			// Bring to top left corner
			glTranslatef(Pos.x,Pos.y,0);				// Set Elem Pos
			glColor4f(1,1,1,Alpha);						// Set default Color
			ElemBank->RenderElem(0,Elem+1,0,Render3d);
			glPopMatrix();
}

/*****************************************************************************/
/*** Gui *********************************************************************/
/*****************************************************************************/
void	CLayerElem::GUIInit(CCore *Core)
{
//		Core->GUIAdd(GUIToolBar,IDD_TOOLBAR);
}

/*****************************************************************************/
void	CLayerElem::GUIKill(CCore *Core)
{
//		Core->GUIRemove(GUIToolBar,IDD_TOOLBAR);
}

/*****************************************************************************/
void	CLayerElem::GUIUpdate(CCore *Core)
{
}

/*****************************************************************************/
void	CLayerElem::GUIChanged(CCore *Core)
{
}

/*****************************************************************************/
/*** Functions ***************************************************************/
/*****************************************************************************/
bool	CLayerElem::LButtonControl(CCore *Core,UINT nFlags, CPoint &CursorPos,bool DownFlag)
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
bool	CLayerElem::RButtonControl(CCore *Core,UINT nFlags, CPoint &CursorPos,bool DownFlag)
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
bool	CLayerElem::MouseMove(CCore *Core,UINT nFlags, CPoint &CursorPos)
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
bool	CLayerElem::Command(int CmdMsg,CCore *Core,int Param0,int Param1)
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
			TRACE3("LayerElem-Unhandled Command %i (%i,%i)\n",CmdMsg,Param0,Param1);
		}

		return(Ret);
}

/*****************************************************************************/
void	CLayerElem::RenderCursor(CCore *Core,Vector3 &CamPos,bool Is3d)
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
bool	CLayerElem::Paint(CMap &Blk,CPoint &CursorPos)
{

		if (CursorPos.x==-1 || CursorPos.y==-1) return(false);	// Off Map?
		if (!Blk.IsValid()) return(false);	// Invalid Elem?
/*
		Map.Set(CursorPos.x,CursorPos.y,Blk);
*/
		return(true);
}

/*****************************************************************************/
void	CLayerElem::Export(CCore *Core,CExport &Exp)
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
				sExpLayerElem	OutElem;

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

				Exp.Write(&OutElem,sizeof(sExpLayerElem));
			}
		}
*/
}

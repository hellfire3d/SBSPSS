/******************/
/*** Layer Tile ***/
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

#include	"TileSet.h"

#include	"Core.h"
#include	"Layer.h"
#include	"LayerTile.h"
#include	"Utils.h"
#include	"Select.h"
#include	"Export.h"

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
// New Layer
CLayerTile::CLayerTile(int _SubType,int Width,int Height)
{
		SubType=_SubType;
		TileBank=0;
		if (SubType==LAYER_SUBTYPE_ACTION)
			TileBank=new CTileBank;
		else
			TileBank=0;
		SubView=TileBank;

		SetDefaultParams();

		Mode=MouseModePaint;

		if (ResizeFlag)
		{
			Width=TileLayerMinWidth+(Width-TileLayerMinWidth)/ScaleFactor;
			Height=TileLayerMinHeight+(Height-TileLayerMinHeight)/ScaleFactor;
		}
		else
		{
			Width=32;
			Height=32;
		}

		if (Width<TileLayerMinWidth) Width=TileLayerMinWidth;
		if (Height<TileLayerMinHeight) Height=TileLayerMinHeight;

		Map.SetSize(Width,Height,TRUE);

}

/*****************************************************************************/
// Load Layer
CLayerTile::CLayerTile(CFile *File,int Version)
{
		Load(File,Version);
}

/*****************************************************************************/
CLayerTile::~CLayerTile()
{
		if (SubType==LAYER_SUBTYPE_ACTION) delete TileBank;
}

/*****************************************************************************/
void	CLayerTile::Load(CFile *File,int Version)
{
		File->Read(&Render3dFlag,sizeof(BOOL));
		File->Read(&ScaleFactor,sizeof(float));
		File->Read(&ResizeFlag,sizeof(BOOL));
		File->Read(&VisibleFlag,sizeof(BOOL));
		File->Read(&Mode,sizeof(MouseMode));
		File->Read(&SubType,sizeof(int));
		Map.Load(File,Version);

		if (SubType==LAYER_SUBTYPE_ACTION) 
			TileBank=new CTileBank;
		else
			TileBank=0;
		SubView=TileBank;
		
		TRACE1("%s\t",GetName());
}

/*****************************************************************************/
void	CLayerTile::Save(CFile *File)
{
// Always Save current version

		File->Write(&Render3dFlag,sizeof(BOOL));
		File->Write(&ScaleFactor,sizeof(float));
		File->Write(&ResizeFlag,sizeof(BOOL));
		File->Write(&VisibleFlag,sizeof(BOOL));
		File->Write(&Mode,sizeof(MouseMode));
		File->Write(&SubType,sizeof(SubType));
		Map.Save(File);
}

/*****************************************************************************/
void	CLayerTile::InitSubView(CCore *Core)
{
// Fix up shared layers
		if (!SubView)
		{
			SubView=Core->FindSubView(LAYER_SUBVIEW_TILEBANK);
		}
		TileBank=(CTileBank*)SubView;
}

/*****************************************************************************/
/*****************************************************************************/
void	CLayerTile::CheckLayerSize(int Width,int Height)
{
		if (Resize(Width,Height))
		{
			CString mexstr;
			mexstr.Format("%s Layer Resized to Correct Size\nPlease re-save\n", GetName());
			AfxMessageBox(mexstr,MB_OK | MB_ICONEXCLAMATION);
		}
}

/*****************************************************************************/
bool	CLayerTile::Resize(int Width,int Height)
{
		if (!ResizeFlag) return(FALSE);	// Its a fixed size, so DONT DO IT!

int		ThisWidth=Map.GetWidth();
int		ThisHeight=Map.GetHeight();
		Width=TileLayerMinWidth+(Width-TileLayerMinWidth)/ScaleFactor;
		Height=TileLayerMinHeight+(Height-TileLayerMinHeight)/ScaleFactor;

		if (ThisWidth!=Width || ThisHeight!=Height)
		{
			Map.Resize(Width,Height);				
			return(TRUE);
		}
		return(FALSE);
}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
void	CLayerTile::Render(CCore *Core,Vector3 &CamPos,bool Is3d)
{
Vector3		ThisCam=Core->OffsetCam(CamPos,GetScaleFactor());

		if (Is3d && Render3dFlag)
		{
			glEnable(GL_DEPTH_TEST);
			Render(Core,ThisCam,Map,TRUE);
			glDisable(GL_DEPTH_TEST);
		}
			else
		{
			Render(Core,ThisCam,Map,FALSE);
		}
}

/*****************************************************************************/
void	CLayerTile::RenderCursorPaint(CCore *Core,Vector3 &CamPos,bool Is3d)
{
Vector3		ThisCam=Core->OffsetCam(CamPos,GetScaleFactor());
CPoint		&CursPos=Core->GetCursorPos();
CMap		&Brush=TileBank->GetActiveBrush();
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
}

/*****************************************************************************/
void	CLayerTile::Render(CCore *Core,Vector3 &ThisCam,CMap &ThisMap,bool Render3d,float Alpha,Vector3 *Ofs)
{
int			MapWidth=ThisMap.GetWidth();
int			MapHeight=ThisMap.GetHeight();
float		ZoomW=Core->GetZoomW();
float		ZoomH=Core->GetZoomH();
float		ScrOfsX=(ZoomW/2);
float		ScrOfsY=(ZoomH/2);
Vector3		&Scale=Core->GetScaleVector();
bool		WrapMap=SubType==LAYER_SUBTYPE_BACK;
int		StartX=(int)ThisCam.x;
int		StartY=(int)ThisCam.y;
float	ShiftX=ThisCam.x - (int)ThisCam.x;
float	ShiftY=ThisCam.y - (int)ThisCam.y;

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
				if (ThisElem.Tile && TileBank->IsTileValid(ThisElem.Set,ThisElem.Tile))
				{ // Render Non Zero Tiles
					CTile		&ThisTile=TileBank->GetTile(ThisElem.Set,ThisElem.Tile);

					glColor4f(1,1,1,Alpha);	// Set default Color
					ThisTile.Render(ThisElem.Flags,Render3d);
				}
				glTranslatef(1.0f,0,0);	// Next X
			}
			glTranslatef(-DrawW,-1,0); // Next y, rewind to start X
		}
		glPopMatrix();
}

/*****************************************************************************/
void	CLayerTile::RenderSelection(CCore *Core,Vector3 &CamPos)
{
CRect		Rect=Selection.GetRect();
Vector3		ThisCam=Core->OffsetCam(CamPos,GetScaleFactor());
float		ZoomW=Core->GetZoomW();
float		ZoomH=Core->GetZoomH();
float		ScrOfsX=(ZoomW/2);
float		ScrOfsY=(ZoomH/2);
Vector3		&Scale=Core->GetScaleVector();

		if (!Selection.IsValid()) return;
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();
		glScalef(Scale.x,Scale.y,Scale.z);
		glTranslatef(-ThisCam.x,ThisCam.y,0);
		glTranslatef(-ScrOfsX,ScrOfsY,0);	// Bring to top left corner

		glColor4f(1,0,1,0.5f);
		glBegin (GL_QUADS); 
float		X0=Rect.left;
float		X1=Rect.right;
float		Y0=Rect.top-1;
float		Y1=Rect.bottom-1;
			glVertex3f( X0, -Y0, 0);
			glVertex3f( X1, -Y0, 0);
			glVertex3f( X1, -Y1, 0);
			glVertex3f( X0, -Y1, 0);
		glEnd();

		glPopMatrix();
}

/*****************************************************************************/
void	CLayerTile::RenderGrid(CCore *Core,Vector3 &CamPos,bool Active)
{
Vector3		ThisCam=Core->OffsetCam(CamPos,GetScaleFactor());
int			MapWidth=Map.GetWidth();
int			MapHeight=Map.GetHeight();
float		ZoomW=Core->GetZoomW();
float		ZoomH=Core->GetZoomH();
float		ScrOfsX=(ZoomW/2);
float		ScrOfsY=(ZoomH/2);
Vector3		&Scale=Core->GetScaleVector();
float		Col;
const float	OverVal=0.1f;

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();
		glScalef(Scale.x,Scale.y,Scale.z);
		glTranslatef(-ThisCam.x,ThisCam.y,0);
		glTranslatef(-ScrOfsX,ScrOfsY,0);	// Bring to top left corner

		if (Active) Col=1; else Col=0.5f;

		glBegin(GL_LINES); 
		
		glColor3f(Col,Col,Col);
			
		for (int YLoop=0; YLoop<MapHeight+1; YLoop++)
			{
			glVertex3f( 0-OverVal,			-YLoop+1, 0);
			glVertex3f( MapWidth+OverVal,	-YLoop+1, 0);
			}

		for (int XLoop=0; XLoop<MapWidth+1; XLoop++)
			{
			glVertex3f( XLoop, 0+1+OverVal,		0);
			glVertex3f( XLoop, -MapHeight+1-OverVal, 0);
			}
		glEnd();

		glPopMatrix();
}

/*****************************************************************************/
void	CLayerTile::FindCursorPos(CCore *Core,Vector3 &CamPos,CPoint &MousePos)
{
Vector3		ThisCam=Core->OffsetCam(CamPos,GetScaleFactor());
int			MapWidth=Map.GetWidth();
int			MapHeight=Map.GetHeight();
float		ZoomW=Core->GetZoomW();
float		ZoomH=Core->GetZoomH();
float		ScrOfsX=(ZoomW/2);
float		ScrOfsY=(ZoomH/2);
Vector3		&Scale=Core->GetScaleVector();

GLint		Viewport[4];
GLuint		SelectBuffer[SELECT_BUFFER_SIZE];
int			TileID=0;
CPoint		&CursorPos=Core->GetCursorPos();

int		StartX=(int)ThisCam.x;
int		StartY=(int)ThisCam.y;
float	ShiftX=ThisCam.x - (int)ThisCam.x;
float	ShiftY=ThisCam.y - (int)ThisCam.y;

		if (StartX<0) StartX=0;
		if (StartY<0) StartY=0;

int		DrawW=ZoomW+8;
int		DrawH=ZoomH+8;

		if (StartX+DrawW>MapWidth)	DrawW=MapWidth-StartX;
		if (StartY+DrawH>MapHeight)	DrawH=MapHeight-StartY;
		
		glGetIntegerv(GL_VIEWPORT, Viewport);
		glSelectBuffer (SELECT_BUFFER_SIZE, SelectBuffer );
		glRenderMode (GL_SELECT);

	    glInitNames();
		glPushName(-1);

		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		gluPickMatrix( MousePos.x ,(Viewport[3]-MousePos.y),5.0,5.0,Viewport);
		Core->GetView()->SetupPersMatrix();

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();
		glScalef(Scale.x,Scale.y,Scale.z);
//		glTranslatef(-ThisCam.x,ThisCam.y,0);
		glTranslatef(-ShiftX,ShiftY,0);
		glTranslatef(-ScrOfsX,ScrOfsY,0);	// Bring to top left corner

bool		WrapMap=SubType==LAYER_SUBTYPE_BACK;
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

				TileID=(XPos)+(((YPos)*MapWidth));
				glLoadName (TileID);
				glBegin (GL_QUADS); 
					BuildGLQuad(XLoop,XLoop+1,-YLoop,-YLoop+1,0);
				glEnd();
				TileID++;
			}
		}

		TileID= glRenderMode (GL_RENDER);
		glPopMatrix();
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		CursorPos.x=CursorPos.y=-1;

// Process hits

GLuint	*HitPtr=SelectBuffer;

		if (TileID)	// Just take 1st		
		{
			int	HitID=HitPtr[3];
			CursorPos=IDToPoint(HitID,MapWidth);
		}
		glMatrixMode(GL_MODELVIEW);	// <-- Prevent arse GL assert
}

/*****************************************************************************/
/*** Gui *********************************************************************/
/*****************************************************************************/
void	CLayerTile::GUIInit(CCore *Core)
{
		TileBank->GUIInit(Core);
		Core->GUIAdd(ToolBarGUI,IDD_LAYERTILE_TOOLBAR);
}

/*****************************************************************************/
void	CLayerTile::GUIKill(CCore *Core)
{
		TileBank->GUIKill(Core);
		Core->GUIRemove(ToolBarGUI,IDD_LAYERTILE_TOOLBAR);
}

/*****************************************************************************/
void	CLayerTile::GUIUpdate(CCore *Core)
{
		ToolBarGUI.ResetButtons();
		switch(Mode)
		{
		case MouseModePaint:
			ToolBarGUI.SetButtonState(CLayerTileToolbar::PAINT,TRUE);
			break;
		case MouseModeSelect:
			ToolBarGUI.SetButtonState(CLayerTileToolbar::SELECT,TRUE);
			break;
		default:
			break;
		}
		TileBank->GUIUpdate(Core);
}

/*****************************************************************************/
void	CLayerTile::GUIChanged(CCore *Core)
{
}

/*****************************************************************************/
/*** Functions ***************************************************************/
/*****************************************************************************/
bool	CLayerTile::LButtonControl(CCore *Core,UINT nFlags, CPoint &CursorPos,bool DownFlag)
{
bool	Ret=false;

		switch(Mode)
		{
		case MouseModePaint:
			if (DownFlag)
				Ret=Paint(TileBank->GetLBrush(),CursorPos);
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
		return(Ret);
}

/*****************************************************************************/
bool	CLayerTile::RButtonControl(CCore *Core,UINT nFlags, CPoint &CursorPos,bool DownFlag)
{
bool		Ret=FALSE;

		switch(Mode)
		{
		case MouseModePaint:
			if (DownFlag)
				Ret=Paint(TileBank->GetRBrush(),CursorPos);
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
		return(Ret);
}

/*****************************************************************************/
bool	CLayerTile::MouseMove(CCore *Core,UINT nFlags, CPoint &CursorPos)
{
bool		Ret=FALSE;

		switch(Mode)
		{
		case MouseModePaint:
			if (nFlags & MK_LBUTTON)
				Ret=Paint(TileBank->GetLBrush(),CursorPos);
			else
			if (nFlags & MK_RBUTTON)
				Ret=Paint(TileBank->GetRBrush(),CursorPos);
			break;
		case MouseModeSelect:
			Ret=Selection.Handle(CursorPos,nFlags);
			break;
		default:
			break;
		}
		return(Ret);
}

/*****************************************************************************/
bool	CLayerTile::Command(int CmdMsg,CCore *Core,int Param0,int Param1)
{
bool	Ret=false;
		switch(CmdMsg)
		{
		case CmdMsg_SetMode:
			Mode=(MouseMode)Param0;
			Core->GUIUpdate();
			break;
		case CmdMsg_Copy:
			CopySelection(Core);
			break;
		case CmdMsg_Paste:
			PasteSelection(Core);
			break;
		case CmdMsg_MirrorX:
			Ret=MirrorX(Core);
			break;
		case CmdMsg_MirrorY:
			Ret=MirrorY(Core);
			break;
		case CmdMsg_SetColFlag:
			Ret=SetColFlags(Core,Param0);
			break;
		case CmdMsg_SubViewLoad:
		case CmdMsg_SubViewDelete:
		case CmdMsg_SubViewUpdate:
		case CmdMsg_SubViewSet:
		case CmdMsg_ActiveBrushLeft:
		case CmdMsg_ActiveBrushRight:
			Ret=TileBank->Command(CmdMsg,Core,Param0,Param1);
		default:
			TRACE3("LayerTile-Unhandled Command %i (%i,%i)\n",CmdMsg,Param0,Param1);
		}
		return(Ret);
}

/*****************************************************************************/
void	CLayerTile::RenderCursor(CCore *Core,Vector3 &CamPos,bool Is3d)
{
		switch(Mode)
		{
		case MouseModePaint:
			RenderCursorPaint(Core,CamPos,Is3d);
			break;
		case MouseModeSelect:
			RenderSelection(Core,CamPos);
			break;
		default:
			break;
		}
}

/*****************************************************************************/
bool	CLayerTile::MirrorX(CCore *Core)
{
		switch(Mode)
		{
		case MouseModePaint:
			{
			TileBank->GetLBrush().MirrorX(PC_TILE_FLAG_MIRROR_X);
			TileBank->GetRBrush().MirrorX(PC_TILE_FLAG_MIRROR_X);
			}
			break;
		case MouseModeSelect:
			{
				if (!Selection.IsValid()) return(false);	// No Selection
				CRect	R=Selection.GetRect();
				Map.MirrorX(PC_TILE_FLAG_MIRROR_X,&R);
			}
			break;
		default:
			break;
		}
		Core->UpdateView();
		return(TRUE);
}

/*****************************************************************************/
bool	CLayerTile::MirrorY(CCore *Core)
{
//		if (GetType()==LAYER_TYPE_COLLISION) return(false);
		switch(Mode)
		{
		case MouseModePaint:
			{
			TileBank->GetLBrush().MirrorY(PC_TILE_FLAG_MIRROR_Y);
			TileBank->GetRBrush().MirrorY(PC_TILE_FLAG_MIRROR_Y);
			}
			break;
		case MouseModeSelect:
			{
				if (!Selection.IsValid()) return(false);	// No Selection
				CRect	R=Selection.GetRect();
				Map.MirrorY(PC_TILE_FLAG_MIRROR_Y,&R);
			}
			break;
		default:
			break;
		}
		Core->UpdateView();
		return(TRUE);
}

/*****************************************************************************/
bool	CLayerTile::SetColFlags(CCore *Core,int Flags)
{
		switch(Mode)
		{
		case MouseModePaint:
			{
			TileBank->GetLBrush().SetFlags(Flags<<PC_TILE_FLAG_COLLISION_SHIFT,PC_TILE_FLAG_MIRROR_XY);
			TileBank->GetRBrush().SetFlags(Flags<<PC_TILE_FLAG_COLLISION_SHIFT,PC_TILE_FLAG_MIRROR_XY);
			}
			break;
		case MouseModeSelect:
			{
				if (!Selection.IsValid()) return(false);	// No Selection
				CRect	R=Selection.GetRect();
				Map.SetFlags(Flags<<PC_TILE_FLAG_COLLISION_SHIFT,PC_TILE_FLAG_MIRROR_XY,&R);
			}
			break;
		default:
			break;
		}

		return(TRUE);
}

/*****************************************************************************/
bool	CLayerTile::CopySelection(CCore *Core)
{
		if (Mode!=MouseModeSelect) return(false);	// Not in select mode
		if (!Selection.IsValid()) return(false);	// No Selection

CRect		Rect=Selection.GetRect();

		TileBank->GetActiveBrush().Set(Map,Rect.left,Rect.top,Rect.Width(),Rect.Height());

		return(true);

}

/*****************************************************************************/
bool	CLayerTile::PasteSelection(CCore *Core)
{
		if (Mode!=MouseModeSelect) return(false);	// Not in select mode
		if (!Selection.IsValid()) return(false);	// No Selection

CRect		Rect=Selection.GetRect();

		Map.Paste(TileBank->GetActiveBrush(),&Rect);
		return(true);
}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/

bool	CLayerTile::Paint(CMap &Blk,CPoint &CursorPos)
{
		if (CursorPos.y==-1 || CursorPos.y==-1) return(FALSE);	// Off Map?
		if (!Blk.IsValid()) return(FALSE);	// Invalid tile?

		Map.Set(CursorPos.x,CursorPos.y,Blk);

		return(TRUE);
}

/*****************************************************************************/
void	CLayerTile::Export(CCore *Core,CExport &Exp)
{
int				Width=Map.GetWidth();
int				Height=Map.GetHeight();
sExpTile		BlankElem={0,0,0,0};

		Exp.ExportLayerHeader(LAYER_TYPE_TILE,SubType,Width,Height);

		Exp.AddTile(BlankElem);	// Ensure blank tile is present
		
		for (int Y=0; Y<Height; Y++)
		{
			for (int X=0; X<Width; X++)
			{
				sMapElem		&MapElem=Map.Get(X,Y);
				CTile			&ThisTile=TileBank->GetTile(MapElem.Set,MapElem.Tile);
				sExpLayerTile	OutElem;
				sExpTile		OutTile;

				OutTile.Set=MapElem.Set;
				OutTile.Tile=MapElem.Tile;
				OutTile.TriCount=0;
				OutTile.XOfs=ThisTile.GetTexXOfs();
				OutTile.YOfs=ThisTile.GetTexYOfs();
				
				OutElem.Tile=Exp.AddTile(OutTile);
				OutElem.Flags=MapElem.Flags;
				Exp.Write(&OutElem,sizeof(sExpLayerTile));
			}
		}
}

/*****************************************************************************/
void	CLayerTile::DeleteSet(int Set)
{
		Map.DeleteSet(Set);
}

/*****************************************************************************/
void	CLayerTile::RemapSet(int OrigSet,int NewSet)
{
		Map.RemapSet(OrigSet,NewSet);

}

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
CLayerTile::CLayerTile(sLayerDef &Def)
{
		InitLayer(Def);
}

/*****************************************************************************/
CLayerTile::~CLayerTile()
{
		if (LayerDef.SubType==LAYER_SUBTYPE_ACTION) 
		{
			TileBank->CleanUp();
			delete TileBank;
		}
}

/*****************************************************************************/
void	CLayerTile::InitLayer(sLayerDef &Def)
{
		CLayer::InitLayer(Def);
		Mode=MouseModePaint;

		if (LayerDef.SubType==LAYER_SUBTYPE_ACTION)
			TileBank=new CTileBank;
		else
			TileBank=0;
		SubView=TileBank;

		if (!GetResizeFlag())
		{
			LayerDef.Width=32;
			LayerDef.Height=32;
		}

		Resize(LayerDef.Width,LayerDef.Height);
}

/*****************************************************************************/
void	CLayerTile::Load(CFile *File,int Version) 
{
		if (Version<=5)
		{
			LayerDef.Type=LAYER_TYPE_TILE;
			BOOL	DB;
			float	DF;
			File->Read(&DB,sizeof(BOOL));
			File->Read(&DF,sizeof(float));
			File->Read(&DB,sizeof(BOOL));
			File->Read(&LayerDef.VisibleFlag,sizeof(BOOL));
			File->Read(&Mode,sizeof(MouseMode));
			File->Read(&LayerDef.SubType,sizeof(int));
		}
		else
		{
			File->Read(&Mode,sizeof(MouseMode));
		}
		InitLayer(LayerDef);
		Map.Load(File,Version);

//		if (LayerDef.SubType==LAYER_SUBTYPE_ACTION) 
//			TileBank=new CTileBank;
//		else
//			TileBank=0;
//		SubView=TileBank;
		
}

/*****************************************************************************/
void	CLayerTile::Save(CFile *File)
{
// Always Save current version
		File->Write(&Mode,sizeof(MouseMode));
		Map.Save(File);
}

/*****************************************************************************/
void	CLayerTile::InitSubView(CCore *Core)
{
// Fix up shared layers
		if (LayerDef.SubType!=LAYER_SUBTYPE_ACTION)
		{
			TileBank=Core->GetTileBank();
			SubView=(CLayer*)TileBank;
		}
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
void	CLayerTile::Validate(CCore *Core)
{
int		Width=Map.GetWidth();
int		Height=Map.GetHeight();
int		Invalid=0;
int		X,Y;
bool	Ret=false;

		for (Y=0; Y<Height; Y++)
		{
			for (X=0; X<Width; X++)
			{
				sMapElem		&MapElem=Map.Get(X,Y);
				if (!TileBank->IsValid(MapElem.Set,MapElem.Tile)) 
				{
					Invalid++;
				}
			}
		}
		if (Invalid)
		{
			char	Txt[256];
			sprintf(Txt,"Map contains %i invalid tiles\n Do you want them removed?",Invalid);
			Ret=Core->Question(Txt);
		
		}
		if (Ret)
		{
			for (Y=0; Y<Height; Y++)
			{
				for (X=0; X<Width; X++)
				{
					sMapElem		&MapElem=Map.Get(X,Y);
					if (!TileBank->IsValid(MapElem.Set,MapElem.Tile))
					{
						MapElem.Set=0;
						MapElem.Tile=0;
					}
				}
			}
		}
	
}

/*****************************************************************************/
bool	CLayerTile::Resize(int Width,int Height)
{
int		ThisWidth=Map.GetWidth();
int		ThisHeight=Map.GetHeight();
bool	Flag=GetResizeFlag();

		Width=TileLayerMinWidth+(Width-TileLayerMinWidth)/GetScaleFactor();
		Height=TileLayerMinHeight+(Height-TileLayerMinHeight)/GetScaleFactor();

		if (Width<TileLayerMinWidth) Width=TileLayerMinWidth;
		if (Height<TileLayerMinHeight) Height=TileLayerMinHeight;

		if (!ThisWidth || !ThisHeight) Flag=true;

		if (Flag) 
		{
			if (ThisWidth!=Width || ThisHeight!=Height)
			{
				Map.Resize(Width,Height);				
				return(true);
			}
		}
		return(false);
}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
void	CLayerTile::Render(CCore *Core,Vector3 &CamPos,bool Is3d)
{
Vector3		ThisCam=Core->OffsetCam(CamPos,GetScaleFactor());

		Is3d&=GetRender3dFlag();
		Render(Core,ThisCam,Map,Is3d);
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

		Is3d&=GetRender3dFlag();
		if (Is3d)
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
bool		WrapMap=LayerDef.SubType==LAYER_SUBTYPE_BACK;
int			StartX=(int)ThisCam.x;
int			StartY=(int)ThisCam.y;
float		ShiftX=ThisCam.x - (int)ThisCam.x;
float		ShiftY=ThisCam.y - (int)ThisCam.y;

		if (TileBank->NeedLoad()) 
		{
			TileBank->LoadAllSets(Core);
			Core->Validate(GetType());
		}

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
				if (ThisElem.Tile>0)
				{ // Render Non Zero and Valid Tiles
					glColor4f(1,1,1,Alpha);	// Set default Color
					TileBank->RenderElem(ThisElem.Set,ThisElem.Tile,ThisElem.Flags,Render3d);
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
/*** Gui *********************************************************************/
/*****************************************************************************/
void	CLayerTile::GUIInit(CCore *Core)
{
		TileBank->GUIInit(Core);
		Core->GUIAdd(GUIToolBar,IDD_TOOLBAR);
}

/*****************************************************************************/
void	CLayerTile::GUIKill(CCore *Core)
{
		TileBank->GUIKill(Core);
		Core->GUIRemove(GUIToolBar,IDD_TOOLBAR);
}

/*****************************************************************************/
void	CLayerTile::GUIUpdate(CCore *Core)
{
		GUIToolBar.ResetButtons();
		switch(Mode)
		{
		case MouseModePaint:
			GUIToolBar.SetButtonState(CGUIToolBar::PAINT,TRUE);
			break;
		case MouseModeSelect:
			GUIToolBar.SetButtonState(CGUIToolBar::SELECT,TRUE);
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
			break;
		default:
			TRACE3("LayerTile-Unhandled Command %i (%i,%i)\n",CmdMsg,Param0,Param1);
			break;
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
		if (CursorPos.x==-1 || CursorPos.y==-1) return(FALSE);	// Off Map?
		if (!Blk.IsValid()) return(FALSE);	// Invalid tile?

		Map.Set(CursorPos.x,CursorPos.y,Blk);

		return(TRUE);
}

/*****************************************************************************/
void	CLayerTile::Export(CCore *Core,CExport &Exp)
{
int		Width=Map.GetWidth();
int		Height=Map.GetHeight();

		LayerDef.Width=Width;
		LayerDef.Height=Height;
		Exp.ExportLayerHeader(LayerDef);

		for (int Y=0; Y<Height; Y++)
		{
			for (int X=0; X<Width; X++)
			{
				sMapElem		&MapElem=Map.Get(X,Y);
				sExpLayerTile	OutElem;

				if (MapElem.Set==0 && MapElem.Tile==0)
				{ // Blank
					OutElem.Tile=0;
					OutElem.Flags=0;
				}
				else
				{
					sExpTile		OutTile;
					CElem			&ThisTile=TileBank->GetElem(MapElem.Set,MapElem.Tile);

					OutTile.Set=MapElem.Set;
					OutTile.Tile=MapElem.Tile;
					OutTile.TriStart=0;
					OutTile.TriCount=0;
					OutTile.XOfs=ThisTile.GetTexXOfs();
					OutTile.YOfs=ThisTile.GetTexYOfs();
					OutElem.Tile=Exp.AddTile(OutTile);
					OutElem.Flags=MapElem.Flags;
				}

				Exp.Write(&OutElem,sizeof(sExpLayerTile));
			}
		}
}

/*****************************************************************************/
void	CLayerTile::RemoveSet(int Set)
{
		Map.RemoveSet(Set);
}

/*****************************************************************************/
void	CLayerTile::RemapSet(int OrigSet,int NewSet)
{
		Map.RemapSet(OrigSet,NewSet);

}

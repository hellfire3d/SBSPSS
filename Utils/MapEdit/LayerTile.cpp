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

#include	"Core.h"
#include	"Layer.h"
#include	"LayerTile.h"
#include	"Utils.h"
#include	"Select.h"
#include	"Export.h"

/*****************************************************************************/
char	*CLayerTile::LayerName[]=
{
	"Back",
	"Mid",
	"Action",
	"Fore",
};

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
// New Layer
CLayerTile::CLayerTile(int _SubType,int Width,int Height,float Scale,BOOL Is3d,BOOL Resizable)
{
		SubType=_SubType;
		ScaleFactor=Scale;
		ResizeFlag=Resizable;
		Render3dFlag=Is3d;
		VisibleFlag=TRUE;
		Mode=MouseModePaint;

		if (ResizeFlag)
		{
			Width=TileLayerMinWidth+(Width-TileLayerMinWidth)/ScaleFactor;
			Height=TileLayerMinHeight+(Height-TileLayerMinHeight)/ScaleFactor;
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
}

/*****************************************************************************/
void	CLayerTile::Load(CFile *File,float Version)
{
// Version 1
		if (Version>=1.0)
		{
			File->Read(&Render3dFlag,sizeof(BOOL));
			File->Read(&ScaleFactor,sizeof(float));
			File->Read(&ResizeFlag,sizeof(BOOL));
			File->Read(&VisibleFlag,sizeof(BOOL));
			File->Read(&Mode,sizeof(MouseMode));
			File->Read(&SubType,sizeof(int));
			Map.Load(File,Version);
		}

		TRACE1("%s\t",GetName());
		TRACE1("Scl:%g\t",ScaleFactor);
		TRACE1("%i\n",VisibleFlag);
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
BOOL	CLayerTile::Resize(int Width,int Height)
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
void	CLayerTile::Render(CCore *Core,Vector3 &CamPos,BOOL Is3d)
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
void	CLayerTile::RenderCursorPaint(CCore *Core,Vector3 &CamPos,BOOL Is3d)
{
CTileBank	&TileBank=Core->GetTileBank();
Vector3		ThisCam=Core->OffsetCam(CamPos,GetScaleFactor());
CPoint		&CursPos=Core->GetCursorPos();
CMap		&Brush=TileBank.GetActiveBrush();

		if (!Brush.IsValid()) return;

		if (CursPos.x<0 || CursPos.y<0) return;
		ThisCam.x-=CursPos.x;
		ThisCam.y-=CursPos.y;

		if (Is3d && Render3dFlag)
		{
			glEnable(GL_DEPTH_TEST);
			Render(Core,ThisCam,Brush,TRUE,0.5);
			glDisable(GL_DEPTH_TEST);
		}
			else
		{
			Render(Core,ThisCam,Brush,FALSE,0.5);
		}
}

/*****************************************************************************/
void	CLayerTile::Render(CCore *Core,Vector3 &ThisCam,CMap &ThisMap,BOOL Render3d,float Alpha)
{
int			MapWidth=ThisMap.GetWidth();
int			MapHeight=ThisMap.GetHeight();
float		ZoomW=Core->GetZoomW();
float		ZoomH=Core->GetZoomH();
float		ScrOfsX=(ZoomW/2);
float		ScrOfsY=(ZoomH/2);
Vector3		&Scale=Core->GetScaleVector();

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();
		glScalef(Scale.x,Scale.y,Scale.z);
		glTranslatef(-ThisCam.x,ThisCam.y,0);
		glTranslatef(-ScrOfsX,ScrOfsY,0);	// Bring to top left corner

		glColor4f(1,1,1,Alpha);
		for (int YLoop=0; YLoop<MapHeight; YLoop++)
		{
			for (int XLoop=0; XLoop<MapWidth; XLoop++)
			{
				sMapElem	&ThisElem=ThisMap.Get(XLoop,YLoop);
				if (ThisElem.Tile && Core->IsTileValid(ThisElem.Set,ThisElem.Tile))
				{ // Render Non Zero Tiles
					CTile		&ThisTile=Core->GetTile(ThisElem.Set,ThisElem.Tile);

					ThisTile.Render(ThisElem.Flags,Render3d);
				}
				glTranslatef(1.0f,0,0);	// Next X
			}
			glTranslatef(-MapWidth,-1,0); // Next y, rewind to start X
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
void	CLayerTile::RenderGrid(CCore *Core,Vector3 &CamPos,BOOL Active)
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
void	CLayerTile::FindCursorPos(CCore *Core,CMapEditView *View,Vector3 &CamPos,CPoint &MousePos)
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
		
		glGetIntegerv(GL_VIEWPORT, Viewport);
		glSelectBuffer (SELECT_BUFFER_SIZE, SelectBuffer );
		glRenderMode (GL_SELECT);

	    glInitNames();
		glPushName(-1);

		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		gluPickMatrix( MousePos.x ,(Viewport[3]-MousePos.y),5.0,5.0,Viewport);
		View->SetupPersMatrix();

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();
		glScalef(Scale.x,Scale.y,Scale.z);
		glTranslatef(-ThisCam.x,ThisCam.y,0);
		glTranslatef(-ScrOfsX,ScrOfsY,0);	// Bring to top left corner

		for (int YLoop=0; YLoop<MapHeight; YLoop++)
		{
			for (int XLoop=0; XLoop<MapWidth; XLoop++)
			{
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
void	CLayerTile::InitGUI(CCore *Core)
{
CMainFrame	*Frm=(CMainFrame*)AfxGetApp()->GetMainWnd();
CMultiBar	*ParamBar=Frm->GetParamBar();

			ParamBar->Add(Frm->GetLayerTileGUI(),IDD_LAYERTILE_GUI,TRUE);
}

/*****************************************************************************/
void	CLayerTile::UpdateGUI(CCore *Core)
{
CMainFrame		*Frm=(CMainFrame*)AfxGetApp()->GetMainWnd();
CLayerTileGUI	*Dlg=(CLayerTileGUI *)Frm->GetDialog(IDD_LAYERTILE_GUI);

		if (Dlg)
		{
			Dlg->ResetButtons();
			switch(Mode)
			{
			case MouseModePaint:
				Dlg->SetButtonState(CLayerTileGUI::PAINT,TRUE);
				break;
			case MouseModeSelect:
				Dlg->SetButtonState(CLayerTileGUI::SELECT,TRUE);
				break;
			default:
				break;
			}
		}
		Core->UpdateTileViewGUI();
}

/*****************************************************************************/
/*** Functions ***************************************************************/
/*****************************************************************************/
BOOL	CLayerTile::SetMode(int NewMode)
{
BOOL	Ret=FALSE;
			
// Clean up last mode
			Ret|=ExitMode();
			Mode=(MouseMode)NewMode;
			Ret|=InitMode();
		return(Ret);
}

/*****************************************************************************/
BOOL	CLayerTile::InitMode()
{
		switch(Mode)
		{
		case MouseModePaint:
			break;
		case MouseModeSelect:
			break;
		default:
			break;
		}
		return(FALSE);
}

/*****************************************************************************/
BOOL	CLayerTile::ExitMode()
{
		switch(Mode)
		{
		case MouseModePaint:
			break;
		case MouseModeSelect:
			break;
		default:
			break;
		}
		return(FALSE);
}

/*****************************************************************************/
BOOL	CLayerTile::LButtonControl(CCore *Core,CMapEditView *View,UINT nFlags, CPoint &CursorPos,BOOL DownFlag)
{
BOOL		Ret=FALSE;
CTileBank	&TileBank=Core->GetTileBank();

		switch(Mode)
		{
		case MouseModePaint:
			if (DownFlag)
				Ret=Paint(TileBank.GetLBrush(),CursorPos);
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
BOOL	CLayerTile::RButtonControl(CCore *Core,CMapEditView *View,UINT nFlags, CPoint &CursorPos,BOOL DownFlag)
{
BOOL		Ret=FALSE;
CTileBank	&TileBank=Core->GetTileBank();

		switch(Mode)
		{
		case MouseModePaint:
			if (DownFlag)
				Ret=Paint(TileBank.GetRBrush(),CursorPos);
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
BOOL	CLayerTile::MouseMove(CCore *Core,CMapEditView *View,UINT nFlags, CPoint &CursorPos)
{
BOOL		Ret=FALSE;
CTileBank	&TileBank=Core->GetTileBank();

		switch(Mode)
		{
		case MouseModePaint:
			if (nFlags & MK_LBUTTON)
				Ret=Paint(TileBank.GetLBrush(),CursorPos);
			else
			if (nFlags & MK_RBUTTON)
				Ret=Paint(TileBank.GetRBrush(),CursorPos);
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
void	CLayerTile::RenderCursor(CCore *Core,Vector3 &CamPos,BOOL Is3d)
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
BOOL	CLayerTile::MirrorX(CCore *Core)
{
		switch(Mode)
		{
		case MouseModePaint:
			{
			CTileBank	&TileBank=Core->GetTileBank();

			TileBank.GetLBrush().MirrorX(TILE_FLAG_MIRROR_X);
			TileBank.GetRBrush().MirrorX(TILE_FLAG_MIRROR_X);
			}
			break;
		case MouseModeSelect:
			{
				if (!Selection.IsValid()) return(false);	// No Selection
				CRect	R=Selection.GetRect();
				Map.MirrorX(TILE_FLAG_MIRROR_X,&R);
			}
			break;
		default:
			break;
		}

		return(TRUE);
}

/*****************************************************************************/
BOOL	CLayerTile::MirrorY(CCore *Core)
{
		switch(Mode)
		{
		case MouseModePaint:
			{
			CTileBank	&TileBank=Core->GetTileBank();

			TileBank.GetLBrush().MirrorY(TILE_FLAG_MIRROR_Y);
			TileBank.GetRBrush().MirrorY(TILE_FLAG_MIRROR_Y);
			}
			break;
		case MouseModeSelect:
			{
				if (!Selection.IsValid()) return(false);	// No Selection
				CRect	R=Selection.GetRect();
				Map.MirrorY(TILE_FLAG_MIRROR_Y,&R);
			}
			break;
		default:
			break;
		}


		return(TRUE);
}

/*****************************************************************************/
BOOL	CLayerTile::CopySelection(CCore *Core)
{
		if (Mode!=MouseModeSelect) return(false);	// Not in select mode
		if (!Selection.IsValid()) return(false);	// No Selection

CTileBank	&TileBank=Core->GetTileBank();
CRect		Rect=Selection.GetRect();

		TileBank.GetLBrush().Set(Map,Rect.left,Rect.top,Rect.Width(),Rect.Height());

		return(true);

}

/*****************************************************************************/
BOOL	CLayerTile::PasteSelection(CCore *Core)
{
		if (Mode!=MouseModeSelect) return(false);	// Not in select mode
		if (!Selection.IsValid()) return(false);	// No Selection

		TRACE0("Paste\n");
		return(true);
}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/

BOOL	CLayerTile::Paint(CMap &Blk,CPoint &CursorPos)
{
		if (CursorPos.y==-1 || CursorPos.y==-1) return(FALSE);	// Off Map?
		if (!Blk.IsValid()) return(FALSE);	// Invalid tile?

		Map.Set(CursorPos.x,CursorPos.y,Blk);

		return(TRUE);
}

/*****************************************************************************/
void	CLayerTile::Export(CCore *Core,CExport &Exp)
{
		Exp.ExportLayerTile(Core,GetName(),SubType,Map);
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

/******************/
/*** Layer Tile ***/
/******************/


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
#include	"Utils.h"


/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
// New Layer
CLayerTile::CLayerTile(char *_Name,int Width,int Height,float MapDiv,float ZDiv,BOOL Is3d,BOOL Resizable)
{
		SetName(_Name);
		ZPosDiv=ZDiv;
		MapSizeDiv=MapDiv;
		ResizeFlag=Resizable;
		Render3dFlag=Is3d;
		VisibleFlag=TRUE;
		Mode=MouseModePaint;

		Map.SetSize(Width/MapDiv,Height/MapDiv,TRUE);
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
		File->Read(Name,256);
		File->Read(&Render3dFlag,sizeof(BOOL));
		File->Read(&ZPosDiv,sizeof(float));
		File->Read(&MapSizeDiv,sizeof(float));
		File->Read(&ResizeFlag,sizeof(BOOL));
		File->Read(&VisibleFlag,sizeof(BOOL));
		File->Read(&Mode,sizeof(MouseMode));
		Map.Load(File,Version);
}

/*****************************************************************************/
void	CLayerTile::Save(CFile *File)
{
// Always Save current version

		File->Write(Name,256);
		File->Write(&Render3dFlag,sizeof(BOOL));
		File->Write(&ZPosDiv,sizeof(float));
		File->Write(&MapSizeDiv,sizeof(float));
		File->Write(&ResizeFlag,sizeof(BOOL));
		File->Write(&VisibleFlag,sizeof(BOOL));
		File->Write(&Mode,sizeof(MouseMode));
		Map.Save(File);
}

/*****************************************************************************/
void	CLayerTile::Resize(int Width,int Height)
{
		if (!ResizeFlag) return;	// Its a fixed size, so DONT DO IT!

		Map.Resize(Width/MapSizeDiv,Height/MapSizeDiv);
}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
void	CLayerTile::Render(CCore *Core,Vec &CamPos,BOOL Is3d)
{
float	XYDiv=GetLayerZPosDiv();
Vec		ThisCam=CamPos/XYDiv;

		if (Is3d && Render3dFlag)
		{
			glEnable(GL_DEPTH_TEST);
			Render3d(Core,ThisCam,Map);
			glDisable(GL_DEPTH_TEST);
		}
			else
		{
			Render2d(Core,ThisCam,Map);
		}
}

/*****************************************************************************/
void	CLayerTile::RenderCursorPaint(CCore *Core,Vec &CamPos,BOOL Is3d)
{
CTileBank	&TileBank=Core->GetTileBank();
Vec			ThisCam=CamPos;
CPoint		&CursPos=Core->GetCursorPos();
CMap		&Brush=TileBank.GetActiveBrush();

		if (!Brush.IsValid()) return;

		if (CursPos.x==-1 || CursPos.y==-1) return;
		ThisCam.x+=CursPos.x;
		ThisCam.y-=CursPos.y;

		if (Is3d && Render3dFlag)
		{
			glEnable(GL_DEPTH_TEST);
			Render3d(Core,ThisCam,Brush,0.5);
			glDisable(GL_DEPTH_TEST);
		}
			else
		{
			Render2d(Core,ThisCam,Brush,0.5);
		}
}

/*****************************************************************************/
void	CLayerTile::Render2d(CCore *Core,Vec &CamPos,CMap &ThisMap,float Alpha)
{
	return;
int			Width=ThisMap.GetWidth();
int			Height=ThisMap.GetHeight();
		
		if (Alpha<1)
		{
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	// Alpha Blend Style
			glColor4f(0.5,0.5,0.5,Alpha);
		}
		else
		{
			glColor3f(0.5,0.5,0.5);
		}
		glMatrixMode(GL_MODELVIEW);

		for (int YLoop=0; YLoop<Height; YLoop++)
		{
			for (int XLoop=0; XLoop<Width; XLoop++)
			{
				sMapElem	&ThisElem=ThisMap.Get(XLoop,YLoop);
				CTile		&ThisTile=Core->GetTile(ThisElem.Set,ThisElem.Tile);

				glLoadIdentity();	// Slow way, but good to go for the mo
				glTranslatef(CamPos.x+XLoop,CamPos.y-YLoop,CamPos.z);
//				ThisTile.Render();
int	c=(XLoop+YLoop)&1;
				glColor3f(c,1,1);
				glBegin(GL_QUADS);
					BuildGLQuad(0,1,0,1,0);
				glEnd();

			}
		}
		glDisable(GL_BLEND);
}

/*****************************************************************************/
void	CLayerTile::Render3d(CCore *Core,Vec &CamPos,CMap &ThisMap,float Alpha)
{
int			Width=ThisMap.GetWidth();
int			Height=ThisMap.GetHeight();
		
		if (Alpha<1)
		{
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	// Alpha Blend Style
			glColor4f(0.5,0.5,0.5,Alpha);
		}
		else
		{
			glColor3f(0.5,0.5,0.5);
		}

		glMatrixMode(GL_MODELVIEW);

		for (int YLoop=0; YLoop<Height; YLoop++)
		{
			for (int XLoop=0; XLoop<Width; XLoop++)
			{
				sMapElem	&ThisElem=ThisMap.Get(XLoop,YLoop);
				if (ThisElem.Tile)
				{ // Render Non Zero Tiles
					CTile		&ThisTile=Core->GetTile(ThisElem.Set,ThisElem.Tile);

					glLoadIdentity();	// Slow way, but good to go for the mo
					glTranslatef(CamPos.x+XLoop,CamPos.y-YLoop,CamPos.z);
					ThisTile.Render(ThisElem.Flags);
				}
			}
		}
		glDisable(GL_BLEND);
	
}

/*****************************************************************************/
void	CLayerTile::RenderGrid(CCore *Core,Vec &CamPos)
{
float	XYDiv=GetLayerZPosDiv();
int		Width=Map.GetWidth();
int		Height=Map.GetHeight();
float	StartX=CamPos.x/XYDiv;
float	StartY=CamPos.y/XYDiv;
float	OverVal=0.5;

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glTranslatef(StartX,StartY,CamPos.z);
		glDisable(GL_TEXTURE_2D);

		glBegin(GL_LINES); 
			glNormal3f( 1,1,1);
			glColor3ub(255,255,255);
			
			for (int YLoop=0; YLoop<Height+1; YLoop++)
			{
			glVertex3f( 0-OverVal,    -YLoop+1, 0);
			glVertex3f( Width+OverVal, -YLoop+1, 0);
			}

			for (int XLoop=0; XLoop<Width+1; XLoop++)
			{
			glVertex3f( XLoop, 0+1+OverVal,		0);
			glVertex3f( XLoop, -Height+1-OverVal, 0);
			}
		glEnd();
		glEnable(GL_TEXTURE_2D);

}

/*****************************************************************************/
void	CLayerTile::FindCursorPos(CCore *Core,CMapEditView *View,Vec &CamPos,CPoint &MousePos)
{
GLint	Viewport[4];
GLuint	SelectBuffer[SELECT_BUFFER_SIZE];
int		HitCount;
int		TileID=0;
CPoint	&CursorPos=Core->GetCursorPos();

float	XYDiv=GetLayerZPosDiv();
int		Width=Map.GetWidth();
int		Height=Map.GetHeight();
float	StartX=CamPos.x/XYDiv;
float	StartY=CamPos.y/XYDiv;

		
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
		glLoadIdentity();
		glTranslatef(StartX,StartY,CamPos.z);

		for (int YLoop=0; YLoop<Height; YLoop++)
		{
			for (int XLoop=0; XLoop<Width; XLoop++)
			{
				glLoadName (TileID);
				glBegin (GL_QUADS); 
					BuildGLQuad(XLoop,XLoop+1,-YLoop,-YLoop+1,0);
				glEnd();
				TileID++;
			}
		}

		HitCount= glRenderMode (GL_RENDER);
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		CursorPos.x=CursorPos.y=-1;

// Process hits

GLuint	*HitPtr=SelectBuffer;

		if (HitCount)	// Just take 1st		

		{
			int	HitID=HitPtr[3];
			CursorPos=IDToPoint(HitID,Width);
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

			ParamBar->Add(Frm->GetGfxToolBar(),IDD_GFXTOOLBAR,TRUE);
}

/*****************************************************************************/
void	CLayerTile::UpdateGUI(CCore *Core)
{
CMainFrame	*Frm=(CMainFrame*)AfxGetApp()->GetMainWnd();
CGfxToolBar	*GfxDlg=(CGfxToolBar *)Frm->GetDialog(IDD_GFXTOOLBAR);

		if (GfxDlg)
		{
			GfxDlg->ResetButtons();
			switch(Mode)
			{
			case MouseModePaint:
				GfxDlg->SetButtonState(CGfxToolBar::PAINT,TRUE);
				break;
			case MouseModeSelect:
				GfxDlg->SetButtonState(CGfxToolBar::SELECT,TRUE);
				break;
			case MouseModePicker:
				GfxDlg->SetButtonState(CGfxToolBar::PICKER,TRUE);
				break;
			default:
				break;
			}
		}

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
		case MouseModePicker:
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
		case MouseModePicker:
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
			break;
		case MouseModePicker:
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
			break;
		case MouseModePicker:
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
			break;
		case MouseModePicker:
			break;
		default:
			break;
		}
		return(Ret);
}

/*****************************************************************************/
void	CLayerTile::RenderCursor(CCore *Core,Vec &CamPos,BOOL Is3d)
{
		switch(Mode)
		{
		case MouseModePaint:
			RenderCursorPaint(Core,CamPos,Is3d);
			break;
		case MouseModeSelect:
			break;
		case MouseModePicker:
			break;
		default:
			break;
		}
}

/*****************************************************************************/
BOOL	CLayerTile::MirrorX(CCore *Core)
{
CTileBank	&TileBank=Core->GetTileBank();
CMap		&LBrush=TileBank.GetLBrush();
CMap		&RBrush=TileBank.GetRBrush();

			LBrush.MirrorX(MouseFlagMirrorX);
			RBrush.MirrorX(MouseFlagMirrorX);

		return(TRUE);
}

/*****************************************************************************/
BOOL	CLayerTile::MirrorY(CCore *Core)
{
CTileBank	&TileBank=Core->GetTileBank();
CMap		&LBrush=TileBank.GetLBrush();
CMap		&RBrush=TileBank.GetRBrush();

			LBrush.MirrorY(MouseFlagMirrorY);
			RBrush.MirrorY(MouseFlagMirrorY);

		return(TRUE);
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


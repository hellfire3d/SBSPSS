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
CLayerTile::CLayerTile(char *_Name,int Width,int Height,float ZDiv,BOOL Is3d)
{
		SetName(_Name);
		Map.SetSize(Width,Height,TRUE);
		ZPosDiv=ZDiv;
		Render3dFlag=Is3d;
		Mode=MouseModePaint;
		Flag=0;
}

/*****************************************************************************/
// Load Layer
CLayerTile::CLayerTile(char *_Name)
{
	ASSERT(1);
}

/*****************************************************************************/
CLayerTile::~CLayerTile()
{
}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
void	CLayerTile::Render(CCore *Core,Vec &CamPos,BOOL Is3d)
{
	if (Is3d && Render3dFlag)
	{
		glEnable(GL_DEPTH_TEST);
		Render3d(Core,CamPos);
		glDisable(GL_DEPTH_TEST);
	}
		else
	{
		Render2d(Core,CamPos);
	}
}

/*****************************************************************************/
void	CLayerTile::Render2d(CCore *Core,Vec &CamPos)
{
	return;
float		XYDiv=GetLayerZPosDiv();
int			MapW=Map.GetWidth();
int			MapH=Map.GetHeight();
float		StartX=CamPos.x/XYDiv;
float		StartY=CamPos.y/XYDiv;
CTexCache	&TexCache=Core->GetTexCache();
		
		glColor3f(0.5,0.5,0.5);
		glMatrixMode(GL_MODELVIEW);

		for (int YLoop=0; YLoop<MapH; YLoop++)
		{
			for (int XLoop=0; XLoop<MapW; XLoop++)
			{
				sMapElem	&ThisElem=Map.GetTile(XLoop,YLoop);
				CTile		&ThisTile=Core->GetTile(ThisElem.Set,ThisElem.Tile);

				glLoadIdentity();	// Slow way, but good to go for the mo
				glTranslatef(StartX+XLoop,StartY-YLoop,CamPos.z);
//				ThisTile.Render();
int	c=(XLoop+YLoop)&1;
				glColor3f(c,1,1);
				glBegin(GL_QUADS);
					BuildGLQuad(0,1,0,1,0);
				glEnd();

			}
		}
}

/*****************************************************************************/
void	CLayerTile::Render3d(CCore *Core,Vec &CamPos)
{
float		XYDiv=GetLayerZPosDiv();
int			MapW=Map.GetWidth();
int			MapH=Map.GetHeight();
float		StartX=CamPos.x/XYDiv;
float		StartY=CamPos.y/XYDiv;
CTexCache	&TexCache=Core->GetTexCache();

		glColor3f(0.5,0.5,0.5);
		glMatrixMode(GL_MODELVIEW);

		for (int YLoop=0; YLoop<MapH; YLoop++)
		{
			for (int XLoop=0; XLoop<MapW; XLoop++)
			{
				sMapElem	&ThisElem=Map.GetTile(XLoop,YLoop);
				if (ThisElem.Tile)
				{ // Render Non Zero Tiles
					CTile		&ThisTile=Core->GetTile(ThisElem.Set,ThisElem.Tile);

					glLoadIdentity();	// Slow way, but good to go for the mo
					glTranslatef(StartX+XLoop,StartY-YLoop,CamPos.z);
					ThisTile.Render(ThisElem.Flags);
				}
			}
		}
	
}

/*****************************************************************************/
void	CLayerTile::RenderGrid(CCore *Core,Vec &CamPos)
{
float	XYDiv=GetLayerZPosDiv();
int		MapW=Map.GetWidth();
int		MapH=Map.GetHeight();
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
			
			for (int YLoop=0; YLoop<MapH+1; YLoop++)
			{
			glVertex3f( 0-OverVal,    -YLoop+1, 0);
			glVertex3f( MapW+OverVal, -YLoop+1, 0);
			}

			for (int XLoop=0; XLoop<MapW+1; XLoop++)
			{
			glVertex3f( XLoop, 0+1+OverVal,		0);
			glVertex3f( XLoop, -MapH+1-OverVal, 0);
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
int		MapW=Map.GetWidth();
int		MapH=Map.GetHeight();
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

		for (int YLoop=0; YLoop<MapH; YLoop++)
		{
			for (int XLoop=0; XLoop<MapW; XLoop++)
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
			CursorPos.x=HitID%MapW;
			CursorPos.y=HitID/MapW;
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
			case MouseModeNone:
				break;
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
			GfxDlg->SetButtonState(CGfxToolBar::MIRRORX,Flag & MouseFlagMirrorX);
			GfxDlg->SetButtonState(CGfxToolBar::MIRRORY,Flag & MouseFlagMirrorY);
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
		case MouseModeNone:
			break;
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
		case MouseModeNone:
			break;
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
BOOL	CLayerTile::SetFlag(int NewFlag)
{
		Flag^=NewFlag;
		return(TRUE);
}

/*****************************************************************************/
BOOL	CLayerTile::LButtonControl(CCore *Core,CMapEditView *View,UINT nFlags, CPoint &CursorPos,BOOL DownFlag)
{
BOOL		Ret=FALSE;
CTileBank	&TileBank=Core->GetTileBank();

		switch(Mode)
		{
		case MouseModeNone:
			break;
		case MouseModePaint:
			if (DownFlag)
				Ret=Paint(TileBank.GetLTile(),CursorPos);
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
		case MouseModeNone:
			break;
		case MouseModePaint:
			if (DownFlag)
				Ret=Paint(TileBank.GetRTile(),CursorPos);
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
		case MouseModeNone:
			break;
		case MouseModePaint:
			if (nFlags & MK_LBUTTON)
				Ret=Paint(TileBank.GetLTile(),CursorPos);
			else
			if (nFlags & MK_RBUTTON)
				Ret=Paint(TileBank.GetRTile(),CursorPos);
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
BOOL	CLayerTile::MirrorX()
{
		SetFlag(MouseFlagMirrorX);
		return(TRUE);
}

/*****************************************************************************/
BOOL	CLayerTile::MirrorY()
{
		SetFlag(MouseFlagMirrorY);
		return(TRUE);
}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/

BOOL	CLayerTile::Paint(sMapElem &Tile,CPoint &CursorPos)
{
		if (CursorPos.y==-1 || CursorPos.y==-1) return(FALSE);	// Off Map?
		if (Tile.Set==-1 || Tile.Tile==-1) return(FALSE);	// Invalid tile?

		Tile.Flags=Flag;
		Map.SetTile(CursorPos.x,CursorPos.y,Tile);

		return(TRUE);


}


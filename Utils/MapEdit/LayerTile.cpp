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
//		SetName(_Name);
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
void	CLayerTile::Render(CCore *Core,Vec &CamPos,BOOL Is3d)
{
Vec		ThisCam=Core->OffsetCam(CamPos,GetScaleFactor());

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
void	CLayerTile::RenderCursorPaint(CCore *Core,Vec &CamPos,BOOL Is3d)
{
CTileBank	&TileBank=Core->GetTileBank();
//Vec			ThisCam=CamPos;
Vec			ThisCam=Core->OffsetCam(CamPos,GetScaleFactor());
CPoint		&CursPos=Core->GetCursorPos();
CMap		&Brush=TileBank.GetActiveBrush();

		if (!Brush.IsValid()) return;

		if (CursPos.x==-1 || CursPos.y==-1) return;
		ThisCam.x+=CursPos.x;
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
void	CLayerTile::Render(CCore *Core,Vec &CamPos,CMap &ThisMap,BOOL Render3d,float Alpha)
{
int			Width=ThisMap.GetWidth();
int			Height=ThisMap.GetHeight();
		
		if (Alpha<1)
		{
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
				if (ThisElem.Tile && Core->IsTileValid(ThisElem.Set,ThisElem.Tile))
				{ // Render Non Zero Tiles
					CTile		&ThisTile=Core->GetTile(ThisElem.Set,ThisElem.Tile);

					glLoadIdentity();	// Slow way, but good to go for the mo
					glTranslatef(CamPos.x+XLoop,CamPos.y-YLoop,CamPos.z);
					ThisTile.Render(ThisElem.Flags,Render3d);
				}
			}
		}
	
}

/*****************************************************************************/
void	CLayerTile::RenderGrid(CCore *Core,Vec &CamPos,BOOL Active)
{
int		Width=Map.GetWidth();
int		Height=Map.GetHeight();
Vec		ThisCam=Core->OffsetCam(CamPos,GetScaleFactor());
float	OverVal=0.5;

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glTranslatef(ThisCam.x,ThisCam.y,ThisCam.z);

		glBegin(GL_LINES); 
			if (Active)
				glColor3ub(255,255,255);
			else
				glColor3ub(127,127,127);
			
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

}

/*****************************************************************************/
void	CLayerTile::FindCursorPos(CCore *Core,CMapEditView *View,Vec &CamPos,CPoint &MousePos)
{
GLint	Viewport[4];
GLuint	SelectBuffer[SELECT_BUFFER_SIZE];
int		HitCount;
int		TileID=0;
CPoint	&CursorPos=Core->GetCursorPos();

int		Width=Map.GetWidth();
int		Height=Map.GetHeight();
Vec		ThisCam=Core->OffsetCam(CamPos,GetScaleFactor());
		
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
		glTranslatef(ThisCam.x,ThisCam.y,ThisCam.z);

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

/*****************************************************************************/
void	CLayerTile::Export(CCore *Core,CExport &Exp)
{
		Exp.ExportLayerTile(Core,GetName(),SubType,Map);
}

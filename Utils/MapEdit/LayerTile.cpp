/******************/
/*** Layer Tile ***/
/******************/


#include	"stdafx.h"
#include	"gl3d.h"
#include	<gl\gl.h>
#include	<gl\glu.h>
#include	<gl\glut.h>
#include	"GLEnabledView.h"

#include	"MapEditDoc.h"
#include	"MapEditView.h"

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
		Map.SetSize(Width,Height);
		ZPosDiv=ZDiv;
		Render3dFlag=Is3d;
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
void	CLayerTile::Render(CCore *Core,Vec &MapPos,BOOL Is3d)
{
	if (Is3d && Render3dFlag)
	{
		glEnable(GL_DEPTH_TEST);
		Render3d(Core,MapPos);
		glDisable(GL_DEPTH_TEST);
	}
		else
	{
		Render2d(Core,MapPos);
	}
}

/*****************************************************************************/
void	CLayerTile::Render2d(CCore *Core,Vec &MapPos)
{
float		XYDiv=GetLayerZPosDiv();
int			MapW=Map.GetWidth();
int			MapH=Map.GetHeight();
float		StartX=MapPos.x/XYDiv;
float		StartY=MapPos.y/XYDiv;
CTexCache	&TexCache=Core->GetTexCache();
		
		glColor3f(0.5,0.5,0.5);
		glMatrixMode(GL_MODELVIEW);

		for (int YLoop=0; YLoop<MapH; YLoop++)
		{
			for (int XLoop=0; XLoop<MapW; XLoop++)
			{
				sMapElem	&ThisElem=Map.GetTile(XLoop,YLoop);
				CTile		&ThisTile=Core->GetTile(ThisElem.Bank,ThisElem.Tile);

				glLoadIdentity();	// Slow way, but good to go for the mo
				glTranslatef(StartX+XLoop,StartY-YLoop,MapPos.z);
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
void	CLayerTile::Render3d(CCore *Core,Vec &MapPos)
{
float		XYDiv=GetLayerZPosDiv();
int			MapW=Map.GetWidth();
int			MapH=Map.GetHeight();
float		StartX=MapPos.x/XYDiv;
float		StartY=MapPos.y/XYDiv;
CTexCache	&TexCache=Core->GetTexCache();
		
		glColor3f(0.5,0.5,0.5);
		glMatrixMode(GL_MODELVIEW);

		for (int YLoop=0; YLoop<MapH; YLoop++)
		{
			for (int XLoop=0; XLoop<MapW; XLoop++)
			{
				sMapElem	&ThisElem=Map.GetTile(XLoop,YLoop);
				CTile		&ThisTile=Core->GetTile(ThisElem.Bank,ThisElem.Tile);

				glLoadIdentity();	// Slow way, but good to go for the mo
				glTranslatef(StartX+XLoop,StartY-YLoop,MapPos.z);
				ThisTile.Render();
			}
		}
	
}

/*****************************************************************************/
void	CLayerTile::RenderGrid(CCore *Core,Vec &MapPos)
{
float	XYDiv=GetLayerZPosDiv();
int		MapW=Map.GetWidth();
int		MapH=Map.GetHeight();
float	StartX=MapPos.x/XYDiv;
float	StartY=MapPos.y/XYDiv;
float	OverVal=0.5;

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glTranslatef(StartX,StartY,MapPos.z);
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
void	CLayerTile::FindCursorPos(CCore *Core,CMapEditView *View,Vec &MapPos,CPoint &MousePos)
{
GLint	Viewport[4];
GLuint	SelectBuffer[SELECT_BUFFER_SIZE];
int		HitCount;
int		TileID=0;
CPoint	&CursorPos=Core->GetCursorPos();

float	XYDiv=GetLayerZPosDiv();
int		MapW=Map.GetWidth();
int		MapH=Map.GetHeight();
float	StartX=MapPos.x/XYDiv;
float	StartY=MapPos.y/XYDiv;

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
		glTranslatef(StartX,StartY,MapPos.z);

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

/******************/
/*** Layer Core ***/
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
#include	"Utils.h"


/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
CLayer::CLayer(CCore *_Core)
{
}

/*****************************************************************************/
CLayer::~CLayer()
{
}

/*****************************************************************************/
void	CLayer::InitLayer(CCore *_Core)
{
int		Width=Map.GetWidth();
int		Height=Map.GetHeight();

	Core=_Core;
	TRACE3("%i x %i = %i\t",Width,Height,Width*Height);
}

/*****************************************************************************/
void	CLayer::Render(Vec &MapPos,BOOL Is3d)
{
		Render2d(MapPos);
}

/*****************************************************************************/
void	CLayer::Render2d(Vec &MapPos)
{
float	XYDiv=GetLayerZPosDiv();
return;
int		Width=Map.GetWidth();
int		Height=Map.GetHeight();

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glTranslatef(MapPos.x/XYDiv,MapPos.y/XYDiv,MapPos.z);

		glBegin(GL_QUADS);
			BuildGLQuad(-1,Width+1,-1,0,0);						// Bottom
			BuildGLQuad(-1,Width+1,Height+1,Height,0);	// Top
			BuildGLQuad(-1,0,Height,0,0);							// Left
			BuildGLQuad(Width,Width+1,Height,0,0);		// Right
		glEnd();


}

/*****************************************************************************/
void	CLayer::Render3d(Vec &MapPos)
{
float	XYDiv=GetLayerZPosDiv();
int		MapW=Map.GetWidth();
int		MapH=Map.GetHeight();
float	StartX=MapPos.x/XYDiv;
float	StartY=MapPos.y/XYDiv;

		glMatrixMode(GL_MODELVIEW);

		for (int YLoop=0; YLoop<MapH; YLoop++)
		{
			for (int XLoop=0; XLoop<MapW; XLoop++)
			{
				sMapElem	&ThisTile=Map.GetTile(XLoop,YLoop);

				glLoadIdentity();	// Slow way, but good to go for the mo
				glTranslatef(StartX+XLoop,StartY-YLoop,MapPos.z);
				glCallList(Core->GetTile(ThisTile.Bank,ThisTile.Tile));
			}
		}
	
}

/*****************************************************************************/
void	CLayer::RenderGrid(Vec &MapPos)
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
void	CLayer::FindCursorPos(Vec &MapPos,CPoint &MousePos)
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
		Core->GetParentWindow()->SetupPersMatrix();

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
		glMatrixMode(GL_MODELVIEW);
		CursorPos.x=CursorPos.y=-1;
//		TRACE1("Hits %i\n",HitCount);

// Process hits

GLuint	*Ptr=SelectBuffer,*BestHit=0;

		for (int H=0;H<HitCount;H++)
			{
			BestHit=Ptr;	// Fakeness for the mo
			}
		
		if (BestHit)
		{
			int	HitID=BestHit[3];
			CursorPos.x=HitID%MapH;
			CursorPos.y=HitID/MapW;
		}

}

/*****************************************************************************/

/******************/
/*** Layer Core ***/
/******************/


#include	"stdafx.h"
#include	"gl3d.h"
#include	<gl\gl.h>
#include	<gl\glu.h>
#include	<gl\glut.h>
//#include	"GLEnabledView.h"

//#include	"MapEditDoc.h"
//#include	"MapEditView.h"

#include	"Layer.h"
#include	"Utils.h"

/*
Core Functionality

Layers
	Gfx Layers
		Background Layer
		Mid Layer
		Action Layer
		Fore Layer
Tile Bank
	Tile Set
	Core
	GUI

Map Data
	Core

Project
	Load Project
	Save Project

Output
	PSX Data
		Map Data
			Level Data
		Tile Data
			Tile Blocks
			Textures
	AGB Data
		Map Data
			Level Data
		Tile Data
			Textures

Edit Functions
	Paint
	Tile Mirror
*/

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
CLayer::CLayer()
{
}

/*****************************************************************************/
CLayer::~CLayer()
{
}

/*****************************************************************************/
void	CLayer::Render(Vec &MapPos,BOOL Is3d)
{

/*
	if (Is3d && CanRender3d())
		Render3d(MapPos);
		else
		Render2d(MapPos);
*/
Vec		ZeroPos(0,0,0);
	if (Is3d && CanRender3d())
		Render3d(ZeroPos);
		else
		Render2d(ZeroPos);

}

/*****************************************************************************/
extern GLint TestTile;

void	CLayer::Render2d(Vec &MapPos)
{
float	XYDiv=GetLayerZPosDiv();
return;
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glTranslatef(MapPos.x/XYDiv,MapPos.y/XYDiv,MapPos.z);

		glBegin(GL_QUADS);
		SetTestColor();
			BuildGLQuad(-1,LayerWidth+1,-1,0,0);						// Bottom
			BuildGLQuad(-1,LayerWidth+1,LayerHeight+1,LayerHeight,0);	// Top
			BuildGLQuad(-1,0,LayerHeight,0,0);							// Left
			BuildGLQuad(LayerWidth,LayerWidth+1,LayerHeight,0,0);		// Right
		glEnd();


}

/*****************************************************************************/
float	asd=0;
void	CLayer::Render3d(Vec &MapPos)
{
float	XYDiv=GetLayerZPosDiv();

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glTranslatef(MapPos.x/XYDiv,MapPos.y/XYDiv,MapPos.z);
		glRotatef(asd,0,1,0);
		asd+=0.5;

		glCallList(TestTile);

}


/*****************************************************************************/
/*****************************************************************************/

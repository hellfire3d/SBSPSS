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
void	CLayer::Render(Vec &MapPos)
{
		TRACE1("%s\n",GetName());

float	ZOfs=GetLayerZPos();
float	XYDiv=GetLayerZPosDiv();

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glTranslatef(MapPos.x/XYDiv,MapPos.y/XYDiv,MapPos.z-ZOfs);

		glBegin(GL_QUADS);
		SetTestColor();
			BuildGLBox(-1,LayerWidth+1,-1,0,0,0+1);						// Bottom
			BuildGLBox(-1,LayerWidth+1,LayerHeight+1,LayerHeight,0,0+1);	// Top
			BuildGLBox(-1,0,LayerHeight,0,0,0+1);							// Left
			BuildGLBox(LayerWidth,LayerWidth+1,LayerHeight,0,0,0+1);		// Right
		glEnd();
}

/*****************************************************************************/
/*****************************************************************************/

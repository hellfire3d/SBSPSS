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
	if (Is3d && CanRender3d())
		Render3d(MapPos);
		else
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
		SetTestColor();
			BuildGLQuad(-1,Width+1,-1,0,0);						// Bottom
			BuildGLQuad(-1,Width+1,Height+1,Height,0);	// Top
			BuildGLQuad(-1,0,Height,0,0);							// Left
			BuildGLQuad(Width,Width+1,Height,0,0);		// Right
		glEnd();


}

/*****************************************************************************/
float	asd=0;
void	CLayer::Render3d(Vec &MapPos)
{
float	XYDiv=GetLayerZPosDiv();
float	X,Y;
int		XX=0;
int		YY=0;

		glMatrixMode(GL_MODELVIEW);

		Y=MapPos.y;

		for (YY=0; YY<3; YY++)
		{
		X=MapPos.x;
			for (XX=0; XX<3; XX++)
			{
				glLoadIdentity();
				glTranslatef(X/XYDiv,Y/XYDiv,MapPos.z);
//				glRotatef(asd,0,1,0);
//				asd+=0.5;
//				glCallList(Core->GetTile(0,2));
				glCallList(Core->GetTile(0,XX+(YY*3)));
			
				X+=1.0f;
			}
		Y+=1.0f;
		}
	
}


/*****************************************************************************/
/*****************************************************************************/

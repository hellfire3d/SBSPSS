/*******************/
/*** Layer Shade ***/
/*******************/


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
#include	"LayerShade.h"
#include	"Utils.h"
#include	"Select.h"
#include	"Export.h"
#include	"LayerShadeGUI.h"


/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
// New Layer
CLayerShade::CLayerShade(int _SubType,int _Width,int _Height)
{
		SubType=_SubType;

		SetDefaultParams();

		Width=TileLayerMinWidth+(_Width-TileLayerMinWidth)/ScaleFactor;
		Height=TileLayerMinHeight+(_Height-TileLayerMinHeight)/ScaleFactor;

		RGB[0].rgbRed=255; RGB[0].rgbGreen=255; RGB[0].rgbBlue=255;
		RGB[1].rgbRed=255; RGB[1].rgbGreen=0; RGB[1].rgbBlue=0;

}

/*****************************************************************************/
// Load Layer
CLayerShade::CLayerShade(CFile *File,int Version)
{
		Load(File,Version);
}

/*****************************************************************************/
CLayerShade::~CLayerShade()
{
}

/*****************************************************************************/
void	CLayerShade::Load(CFile *File,int Version)
{
		File->Read(&Render3dFlag,sizeof(BOOL));
		File->Read(&ScaleFactor,sizeof(float));
		File->Read(&ResizeFlag,sizeof(BOOL));
		File->Read(&VisibleFlag,sizeof(BOOL));
		File->Read(&SubType,sizeof(int));
		File->Read(&Width,sizeof(int));
		File->Read(&Height,sizeof(int));

		File->Read(&RGB[0],sizeof(RGBQUAD));
		File->Read(&RGB[1],sizeof(RGBQUAD));

		TRACE1("%s\t",GetName());
		TRACE1("Scl:%g\t",ScaleFactor);
		TRACE1("%i\n",VisibleFlag);
}

/*****************************************************************************/
void	CLayerShade::Save(CFile *File)
{
// Always Save current version

		File->Write(&Render3dFlag,sizeof(BOOL));
		File->Write(&ScaleFactor,sizeof(float));
		File->Write(&ResizeFlag,sizeof(BOOL));
		File->Write(&VisibleFlag,sizeof(BOOL));
		File->Write(&SubType,sizeof(SubType));
		File->Write(&Width,sizeof(int));
		File->Write(&Height,sizeof(int));

		File->Write(&RGB[0],sizeof(RGBQUAD));
		File->Write(&RGB[1],sizeof(RGBQUAD));
}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
void	CLayerShade::Render(CCore *Core,Vector3 &CamPos,BOOL Is3d)
{
Vector3		ThisCam=Core->OffsetCam(CamPos,GetScaleFactor());
float		ZoomW=Core->GetZoomW();
float		ZoomH=Core->GetZoomH();
float		ScrOfsX=(ZoomW/2);
float		ScrOfsY=(ZoomH/2);
Vector3		&Scale=Core->GetScaleVector();

float		x0=0;
float		x1=x0+Width;
float		y0=(0+1);
float		y1=y0-Height;

			glMatrixMode(GL_MODELVIEW);
			glPushMatrix();
			glLoadIdentity();
			glScalef(Scale.x,Scale.y,Scale.z);
			glTranslatef(-ThisCam.x,ThisCam.y,0);		// Set scroll offset
			glTranslatef(-ScrOfsX,ScrOfsY,0);	// Bring to top left corner

			glBegin (GL_QUADS);
				glColor3ub(RGB[0].rgbRed,RGB[0].rgbGreen,RGB[0].rgbBlue);
				glVertex3f( x0,y0,0.0f);
				glVertex3f( x1,y0,0.0f);

				glColor3ub(RGB[1].rgbRed,RGB[1].rgbGreen,RGB[1].rgbBlue);
				glVertex3f( x1,y1,0.0f);
				glVertex3f( x0,y1,0.0f);

			glEnd();
		glPopMatrix();
}


/*****************************************************************************/
BOOL	CLayerShade::Resize(int _Width,int _Height)
{
		Width=TileLayerMinWidth+(_Width-TileLayerMinWidth)/ScaleFactor;
		Height=TileLayerMinHeight+(_Height-TileLayerMinHeight)/ScaleFactor;

		return(true);
}

/*****************************************************************************/
/*** Gui *********************************************************************/
/*****************************************************************************/
void	CLayerShade::GUIInit(CCore *Core)
{
		Core->GUIAdd(ShadeDlg,IDD_LAYERSHADE_GUI);
		ShadeDlg.SetRGB0(RGB[0]);
		ShadeDlg.SetRGB1(RGB[1]);
}

/*****************************************************************************/
void	CLayerShade::GUIKill(CCore *Core)
{
		Core->GUIRemove(ShadeDlg,IDD_LAYERSHADE_GUI);
}

/*****************************************************************************/
void	CLayerShade::GUIUpdate(CCore *Core)
{
}

/*****************************************************************************/
void	CLayerShade::GUIChanged(CCore *Core)
{
		ShadeDlg.GetRGB0(RGB[0]);
		ShadeDlg.GetRGB1(RGB[1]);
}

/*****************************************************************************/
/*** Functions ***************************************************************/
/*****************************************************************************/
void	CLayerShade::Export(CCore *Core,CExport &Exp)
{
		Exp.ExportLayerHeader(LAYER_TYPE_SHADE,SubType,Width,Height);
		for (int i=0; i<LAYER_SHADE_RGB_MAX; i++)
		{
			Exp.Write(&RGB[i],sizeof(RGBQUAD));
		}
}


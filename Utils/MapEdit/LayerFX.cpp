/****************/
/*** Layer FX ***/
/****************/

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
#include	"LayerThing.h"
#include	"LayerFX.h"
#include	"Utils.h"
#include	"Export.h"

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
CLayerFX::CLayerFX(sLayerDef &Def)
{
		InitLayer(Def);
}

/*****************************************************************************/
void	CLayerFX::InitLayer(sLayerDef &Def)
{
		ThingBank=new CElemBank(-1,-1,false,CElem::CentreModeLR | CElem::CentreModeB);
		CLayerThing::InitLayer(Def);
		LoadThingScript(theApp.GetConfigStr("LayerScript","FXScript"));
}

/*****************************************************************************/
void	CLayerFX::InitSubView(CCore *Core)
{
}

/*****************************************************************************/
void	CLayerFX::LoadDefThing(const char *Name,sLayerThing &ThisDef)
{
}

/*****************************************************************************/
void	CLayerFX::LoadOldThing(CFile *File,sLayerThing &ThisThing)
{
sLayerThingDataOLD	OldThing;

		File->Read(&OldThing,sizeof(sLayerThingDataOLD));
		ThisThing.Data.FX.FXSpeed=OldThing.Speed;
		ThisThing.Data.FX.FXWidth=OldThing.Width;
		ThisThing.Data.FX.FXHeight=OldThing.Height;
}

/*****************************************************************************/
void	CLayerFX::RenderThing(CCore *Core,Vector3 &ThisCam,sLayerThing &ThisThing,bool Render3d,bool Selected)
{
float		ZoomW=Core->GetZoomW();
float		ZoomH=Core->GetZoomH();
Vector3		&Scale=Core->GetScaleVector();
Vector3		ScrOfs(ZoomW/2,ZoomH/2,0);
float		Col=0.8f,A=0.8f;

			glColor4f(1,1,1,0.8f);
			if (Selected) 
			{
				Col=1.0f;
				A=0.8f;
				glColor4f(1,1,1,1.0f); // For number
			}
			
			glMatrixMode(GL_MODELVIEW);
			glPushMatrix();

			glLoadIdentity();

			glScalef(Scale.x,Scale.y,Scale.z);
			glTranslatef(-ThisCam.x,ThisCam.y,0);					// Set scroll offset
			glTranslatef(-ScrOfs.x,ScrOfs.y,0);						// Bring to top left corner

			glTranslatef(ThisThing.XY[0].x,-ThisThing.XY[0].y,0);	// Set Pos

			Core->RenderNumber(0);
			glEnable(GL_DEPTH_TEST);

float	W=(ThisThing.Data.FX.FXWidth);
float	H=-(ThisThing.Data.FX.FXHeight);
// Draw Box
			glBegin (GL_QUADS);
				glColor4f(0,0,Col-0.25f,A);
				glVertex3f(0,0+1,0);
				glVertex3f(W,0+1,0);
				glVertex3f(W,H+1,0);
				glVertex3f(0,H+1,0);
			glEnd();
// Draw OutLine
			glBegin(GL_LINES); 
				glColor4f(Col,Col,Col,A);
			
				glVertex3f( 0,0+1,0);
				glVertex3f( W,0+1,0);

				glVertex3f( W,0+1,0);
				glVertex3f( W,H+1,0);

				glVertex3f( W,H+1,0);
				glVertex3f( 0,H+1,0);

				glVertex3f( 0,H+1,0);
				glVertex3f( 0,0+1,0);

			glEnd();

			glDisable(GL_DEPTH_TEST);

			glPopMatrix();
}

/*****************************************************************************/
/*** Gui *********************************************************************/
/*****************************************************************************/
void	CLayerFX::GUIInit(CCore *Core)
{
		GUIFX.DisableCallback(true);
		Core->GUIAdd(GUIThing,IDD_LAYER_THING);
		Core->GUIAdd(GUIFX,IDD_LAYER_FX);
		GUIFX.DisableCallback(false);

		GUIFX.m_WidthSpin.SetRange(1,255);
		GUIFX.m_HeightSpin.SetRange(1,255);
		GUIFX.m_SpeedSpin.SetRange(0,255);

}

/*****************************************************************************/
void	CLayerFX::GUIKill(CCore *Core)
{
		Core->GUIRemove(GUIThing,IDD_LAYER_THING);
		Core->GUIRemove(GUIFX,IDD_LAYER_FX);
}

/*****************************************************************************/
void	CLayerFX::GUIUpdate(CCore *Core)
{
int			i,ListSize;
CComboBox	&List=GUIThing.m_DefList;

// Setup Def FX List
		ListSize=DefList.size();
		List.ResetContent();
		for (i=0; i<ListSize; i++)
		{
			List.AddString(DefList[i].Name);
		}
		List.SetCurSel(CurrentDefThing);

		GUIThingUpdate();
}

/*****************************************************************************/
void	CLayerFX::GUIThingDefClear()
{
CComboBox	&List=GUIThing.m_DefList;
		CurrentDefThing=-1;
		List.SetCurSel(CurrentDefThing);
}

/*****************************************************************************/
void	CLayerFX::GUIThingUpdate(bool OnlySel)
{
		GUIThingUpdateList(GUIThing.m_List,false);
// Params
		GUIFX.DisableCallback(true);
		if (CurrentThing!=-1)
		{
			sLayerThing	&ThisThing=ThingList[CurrentThing];
			GUIFX.SetVal(GUIFX.m_Speed,ThisThing.Data.FX.FXSpeed);
			GUIFX.SetVal(GUIFX.m_Width,ThisThing.Data.FX.FXWidth);
			GUIFX.SetVal(GUIFX.m_Height,ThisThing.Data.FX.FXHeight);
		}
		else
		{
			GUIFX.m_Speed.SetWindowText("");
			GUIFX.m_Width.SetWindowText("");
			GUIFX.m_Height.SetWindowText("");
		}
		GUIFX.DisableCallback(false);
}

/*****************************************************************************/
void	CLayerFX::GUIThingPointUpdate(bool OnlySel)
{
}

/*****************************************************************************/
void	CLayerFX::GUIChanged(CCore *Core)
{
		if (CurrentThing!=-1)
		{
			sLayerThing	&ThisThing=ThingList[CurrentThing];
			ThisThing.Data.FX.FXSpeed=GUIFX.GetVal(GUIFX.m_Speed);
			ThisThing.Data.FX.FXWidth=GUIFX.GetVal(GUIFX.m_Width);
			ThisThing.Data.FX.FXHeight=GUIFX.GetVal(GUIFX.m_Height);
		}
}

/*****************************************************************************/
void	CLayerFX::SetThingParams(sLayerThing &Thing)
{
		Thing.Data.WaypointCount=1;
		if (Thing.Data.FX.FXWidth<1) Thing.Data.FX.FXWidth=1;
		if (Thing.Data.FX.FXHeight<1) Thing.Data.FX.FXHeight=1;
}
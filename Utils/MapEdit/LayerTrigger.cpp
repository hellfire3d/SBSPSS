/*********************/
/*** Layer Trigger ***/
/*********************/

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
#include	"LayerTrigger.h"
#include	"Utils.h"
#include	"Export.h"

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
CLayerTrigger::CLayerTrigger(sLayerDef &Def)
{
		InitLayer(Def);
}

/*****************************************************************************/
void	CLayerTrigger::InitLayer(sLayerDef &Def)
{
		ThingBank=new CElemBank(-1,-1,false,CElem::CentreModeLR | CElem::CentreModeB);
		CLayerThing::InitLayer(Def);
		LoadThingScript(theApp.GetConfigStr("LayerScript","TriggerScript"));
}

/*****************************************************************************/
void	CLayerTrigger::InitSubView(CCore *Core)
{
}

/*****************************************************************************/
void	CLayerTrigger::LoadDefThing(const char *Name,sLayerThing &ThisDef)
{
}

/*****************************************************************************/
void	CLayerTrigger::LoadOldThing(CFile *File,sLayerThing &ThisThing)
{
sLayerThingDataOLD	OldThing;

		File->Read(&OldThing,sizeof(sLayerThingDataOLD));
		ThisThing.Data.Trigger.TriggerWidth=OldThing.Width;
		ThisThing.Data.Trigger.TriggerHeight=OldThing.Height;
		ThisThing.Data.Trigger.TriggerTargetX=OldThing.TargetX;
		ThisThing.Data.Trigger.TriggerTargetY=OldThing.TargetY;
}


/*****************************************************************************/
void	CLayerTrigger::RenderThing(CCore *Core,Vector3 &ThisCam,sLayerThing &ThisThing,bool Render3d,bool Selected)
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

float	W=(ThisThing.Data.Trigger.TriggerWidth);
float	H=-(ThisThing.Data.Trigger.TriggerHeight);
// Draw Box
			glBegin (GL_QUADS);
				glColor4f(0,Col-0.25f,0,A);
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

			if (Selected && ThingScript.GetInt(ThisThing.Name,"HasTarget"))
			{ // Draw Target
				float	ox,oy;
				float	tx,ty;

				ox=W/2.0f;
				oy=H/2.0f;
				tx=ThisThing.Data.Trigger.TriggerTargetX-ThisThing.XY[0].x;
				ty=ThisThing.XY[0].y-ThisThing.Data.Trigger.TriggerTargetY;
				glBegin(GL_LINES); 
					glColor4f(Col,Col,Col,A);
					glVertex3f( ox,oy+1.0f,0);
					glVertex3f( tx+0.5f,ty+0.5f,0);
				glEnd();
				glBegin (GL_QUADS);
					glColor4f(Col,0,Col,A);
					glVertex3f(tx+0,ty+0,0);
					glVertex3f(tx+1,ty+0,0);
					glVertex3f(tx+1,ty+1,0);
					glVertex3f(tx+0,ty+1,0);
				glEnd();

				
			}


			glDisable(GL_DEPTH_TEST);

			glPopMatrix();
}

/*****************************************************************************/
/*** Gui *********************************************************************/
/*****************************************************************************/
void	CLayerTrigger::GUIInit(CCore *Core)
{
		
		GUITrigger.DisableCallback(true);
		Core->GUIAdd(GUIThing,IDD_LAYER_THING);
		Core->GUIAdd(GUITrigger,IDD_LAYER_TRIGGER);
		GUITrigger.DisableCallback(false);
		GUITrigger.m_WidthSpin.SetRange(1,255);
		GUITrigger.m_HeightSpin.SetRange(1,255);
		GUITrigger.m_TargetXSpin.SetRange(0,32000);
		GUITrigger.m_TargetYSpin.SetRange(0,32000);
}

/*****************************************************************************/
void	CLayerTrigger::GUIKill(CCore *Core)
{
		Core->GUIRemove(GUIThing,IDD_LAYER_THING);
		Core->GUIRemove(GUITrigger,IDD_LAYER_TRIGGER);
}

/*****************************************************************************/
void	CLayerTrigger::GUIUpdate(CCore *Core)
{
int			i,ListSize;
CComboBox	&List=GUIThing.m_DefList;

// Setup Def Trigger List
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
void	CLayerTrigger::GUIThingDefClear()
{
CComboBox	&List=GUIThing.m_DefList;
		CurrentDefThing=-1;
		List.SetCurSel(CurrentDefThing);
}

/*****************************************************************************/
void	CLayerTrigger::GUIThingUpdate(bool OnlySel)
{
int		TargetMode=SW_SHOW;

		GUIThingUpdateList(GUIThing.m_List,false);
// Params
		GUITrigger.DisableCallback(true);
		if (CurrentThing!=-1)
		{
			sLayerThing	&ThisThing=ThingList[CurrentThing];
			GUITrigger.SetVal(GUITrigger.m_Width,ThisThing.Data.Trigger.TriggerWidth);
			GUITrigger.SetVal(GUITrigger.m_Height,ThisThing.Data.Trigger.TriggerHeight);
			GUITrigger.SetVal(GUITrigger.m_TargetX,ThisThing.Data.Trigger.TriggerTargetX);
			GUITrigger.SetVal(GUITrigger.m_TargetY,ThisThing.Data.Trigger.TriggerTargetY);
			if (ThingScript.GetInt(ThisThing.Name,"HasTarget")==0)
			{
				TargetMode=SW_HIDE;
			}
		}
		else
		{
			GUITrigger.m_Width.SetWindowText("");
			GUITrigger.m_Height.SetWindowText("");
			TargetMode=SW_HIDE;
		}

		GUITrigger.m_TargetTxt.ShowWindow(TargetMode);
		GUITrigger.m_TargetX.ShowWindow(TargetMode);
		GUITrigger.m_TargetY.ShowWindow(TargetMode);
		GUITrigger.m_TargetXSpin.ShowWindow(TargetMode);
		GUITrigger.m_TargetYSpin.ShowWindow(TargetMode);

		GUITrigger.DisableCallback(false);
}

/*****************************************************************************/
void	CLayerTrigger::GUIThingPointUpdate(bool OnlySel)
{
//		GUIThingPointUpdateList(GUIThingPos.m_List,OnlySel);
}

/*****************************************************************************/
void	CLayerTrigger::GUIChanged(CCore *Core)
{
		if (CurrentThing!=-1)
		{
			sLayerThing	&ThisThing=ThingList[CurrentThing];
			ThisThing.Data.Trigger.TriggerWidth=GUITrigger.GetVal(GUITrigger.m_Width);
			ThisThing.Data.Trigger.TriggerHeight=GUITrigger.GetVal(GUITrigger.m_Height);
			ThisThing.Data.Trigger.TriggerTargetX=GUITrigger.GetVal(GUITrigger.m_TargetX);
			ThisThing.Data.Trigger.TriggerTargetY=GUITrigger.GetVal(GUITrigger.m_TargetY);
		}
}

/*****************************************************************************/
void	CLayerTrigger::SetThingParams(sLayerThing &Thing)
{
		Thing.Data.WaypointCount=1;
		if (Thing.Data.Trigger.TriggerWidth<1) Thing.Data.Trigger.TriggerWidth=1;
		if (Thing.Data.Trigger.TriggerHeight<1) Thing.Data.Trigger.TriggerHeight=1;
}
/******************/
/*** Layer Core ***/
/******************/


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
#include	"LayerTile.h"
#include	"LayerCollision.h"
#include	"LayerShade.h"
#include	"LayerThing.h"
#include	"LayerActor.h"
#include	"LayerItem.h"
#include	"LayerPlatform.h"
#include	"LayerTrigger.h"
#include	"LayerFX.h"
#include	"LayerHazard.h"
#include	"LayerRGB.h"

#include	"LayerDef.h"
#include	"Utils.h"

/*****************************************************************************/
sLayerInfoTable	CLayer::InfoTable[]=
{
	//Type					SubType					Name			delete?	Scale	3d		Resizable	Export	SubView?
	{LAYER_TYPE_TILE,		LAYER_SUBTYPE_SCRATCH,	"WorkPad",		true,	1.0f,	true,	false,		false,	LAYER_SUBVIEW_TILEBANK,},
	{LAYER_TYPE_SHADE,		LAYER_SUBTYPE_BACK,		"Back Shade",	true,	4.0f,	false,	true,		true,	LAYER_SUBVIEW_NONE,},
	{LAYER_TYPE_TILE,		LAYER_SUBTYPE_BACK,		"Back",			true,	4.0f,	false,	false,		true,	LAYER_SUBVIEW_TILEBANK,},
	{LAYER_TYPE_TILE,		LAYER_SUBTYPE_MID,		"Mid",			true,	2.0f,	false,	true,		true,	LAYER_SUBVIEW_TILEBANK,},
	{LAYER_TYPE_TILE,		LAYER_SUBTYPE_ACTION,	"Action",		false,	1.0f,	true,	true,		true,	LAYER_SUBVIEW_TILEBANK,},
	{LAYER_TYPE_COLLISION,	LAYER_SUBTYPE_NONE,		"Collision",	true,	1.0f,	false,	true,		true,	LAYER_SUBVIEW_TILEBANK,},
	{LAYER_TYPE_ACTOR,		LAYER_SUBTYPE_NONE,		"Actor",		true,	1.0f,	false,	true,		true,	LAYER_SUBVIEW_NONE,},
	{LAYER_TYPE_ITEM,		LAYER_SUBTYPE_NONE,		"Item",			true,	1.0f,	false,	true,		true,	LAYER_SUBVIEW_NONE,},
	{LAYER_TYPE_PLATFORM,	LAYER_SUBTYPE_NONE,		"Platform",		true,	1.0f,	false,	true,		true,	LAYER_SUBVIEW_NONE,},
	{LAYER_TYPE_TRIGGER,	LAYER_SUBTYPE_NONE,		"Trigger",		true,	1.0f,	false,	true,		true,	LAYER_SUBVIEW_NONE,},
	{LAYER_TYPE_FX,			LAYER_SUBTYPE_NONE,		"FX",			true,	1.0f,	false,	true,		true,	LAYER_SUBVIEW_NONE,},
	{LAYER_TYPE_HAZARD,		LAYER_SUBTYPE_NONE,		"Hazard",		true,	1.0f,	false,	true,		true,	LAYER_SUBVIEW_NONE,},
	{LAYER_TYPE_RGB,		LAYER_SUBTYPE_NONE,		"RGB",			true,	1.0f,	false,	true,		true,	LAYER_SUBVIEW_NONE,},
};

int		CLayer::InfoTableSize=sizeof(InfoTable)/sizeof(sLayerInfoTable);

/*****************************************************************************/
CLayer::CLayer()
{
		SubView=0;
		LayerCam=DefaultCamPos;
		LayerDef.VisibleFlag=TRUE;
		LayerDef.Width=-1;
		LayerDef.Height=-1;
		TableIdx=-1;
}

/*****************************************************************************/
void	CLayer::InitLayer(sLayerDef &_LayerDef)
{
		LayerDef=_LayerDef;
		TableIdx=CLayer::GetLayerIdx(LayerDef.Type,LayerDef.SubType);
}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
CLayer	*CLayer::NewLayer(sLayerDef &Def)
{
CLayer	*New;

		switch (Def.Type)
		{
		case LAYER_TYPE_TILE:
			New=new CLayerTile(Def);
			break;
		case LAYER_TYPE_COLLISION:
			New=new CLayerCollision(Def);
			break;
		case LAYER_TYPE_SHADE:
			New=new CLayerShade(Def);
			break;
		case LAYER_TYPE_ACTOR:
			New=new CLayerActor(Def);
			break;
		case LAYER_TYPE_ITEM:
			New=new CLayerItem(Def);
			break;
		case LAYER_TYPE_PLATFORM:
			New=new CLayerPlatform(Def);
			break;
		case LAYER_TYPE_TRIGGER:
			New=new CLayerTrigger(Def);
			break;
		case LAYER_TYPE_FX:
			New=new CLayerFX(Def);
			break;
		case LAYER_TYPE_HAZARD:
			New=new CLayerHazard(Def);
			break;
		case LAYER_TYPE_RGB:
			New=new CLayerRGB(Def);
			break;
		default:
			ASSERT(!"Unknown Layer");
		}
		return(New);
}

/*****************************************************************************/
CLayer	*CLayer::LoadLayer(CFile *File,int Version)
{
CLayer	*New;
		if (Version<=5)
		{
			int	Type;
			File->Read(&Type,sizeof(int));
			switch (Type)
			{
			case LAYER_TYPE_TILE:
				New=new CLayerTile(File,Version);
				break;
			case LAYER_TYPE_COLLISION:
				New=new CLayerCollision(File,Version);
				break;
			case LAYER_TYPE_SHADE:
				New=new CLayerShade(File,Version);
				break;
			case LAYER_TYPE_ACTOR:
				New=new CLayerActor(File,Version);
				break;
			case LAYER_TYPE_ITEM:
				New=new CLayerItem(File,Version);
				break;
			case LAYER_TYPE_PLATFORM:
				New=new CLayerPlatform(File,Version);
				break;
			case LAYER_TYPE_TRIGGER:
				New=new CLayerTrigger(File,Version);
				break;
			case LAYER_TYPE_FX:
				New=new CLayerFX(File,Version);
				break;
			case LAYER_TYPE_HAZARD:
				New=new CLayerHazard(File,Version);
				break;
			case LAYER_TYPE_RGB:
				New=new CLayerRGB(File,Version);
				break;
			default:
				ASSERT(!"Unknown Layer");
			}
		}
		else
		{
			sLayerDef	ThisDef;

			File->Read(&ThisDef,sizeof(sLayerDef));
			New=NewLayer(ThisDef);//Type,SubType,0,0);
			New->Load(File,Version);
		}
		return(New);		
}

/*****************************************************************************/
void	CLayer::Load(CFile *File,int Version)
{
		File->Read(&LayerDef,sizeof(sLayerDef));
}

/*****************************************************************************/
void	CLayer::Save(CFile *File)
{
		File->Write(&LayerDef,sizeof(sLayerDef));
}

/*****************************************************************************/
int		CLayer::GetLayerIdx(int Type,int SubType)
{
		for (int i=0; i<InfoTableSize; i++)
		{
			if (InfoTable[i].Type==Type && InfoTable[i].SubType==SubType)
			{
				return(i);
			}
		}

		return(-1);
}

/*****************************************************************************/
void	CLayer::RenderGrid(CCore *Core,Vector3 &CamPos,bool Active)
{
Vector3		ThisCam=Core->OffsetCam(CamPos,GetScaleFactor());
int			MapWidth=GetWidth();
int			MapHeight=GetHeight();
float		ZoomW=Core->GetZoomW();
float		ZoomH=Core->GetZoomH();
float		ScrOfsX=(ZoomW/2);
float		ScrOfsY=(ZoomH/2);
Vector3		&Scale=Core->GetScaleVector();
float		Col;
const float	OverVal=0.1f;

		if (GetType()>LAYER_TYPE_COLLISION) return;
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();
		glScalef(Scale.x,Scale.y,Scale.z);
		glTranslatef(-ThisCam.x,ThisCam.y,0);
		glTranslatef(-ScrOfsX,ScrOfsY,0);	// Bring to top left corner

		if (Active) Col=1; else Col=0.5f;

		glBegin(GL_LINES); 
		
		glColor3f(Col,Col,Col);
			
		for (int YLoop=0; YLoop<MapHeight+1; YLoop++)
			{
			glVertex3f( 0-OverVal,			-YLoop+1, 0);
			glVertex3f( MapWidth+OverVal,	-YLoop+1, 0);
			}

		for (int XLoop=0; XLoop<MapWidth+1; XLoop++)
			{
			glVertex3f( XLoop, 0+1+OverVal,		0);
			glVertex3f( XLoop, -MapHeight+1-OverVal, 0);
			}
		glEnd();

		glPopMatrix();
}

/*****************************************************************************/
void	CLayer::FindCursorPos(CCore *Core,Vector3 &CamPos,CPoint &MousePos)
{
Vector3		ThisCam=Core->OffsetCam(CamPos,GetScaleFactor());
int			MapWidth=GetWidth();
int			MapHeight=GetHeight();
float		ZoomW=Core->GetZoomW();
float		ZoomH=Core->GetZoomH();
float		ScrOfsX=(ZoomW/2);
float		ScrOfsY=(ZoomH/2);
Vector3		&Scale=Core->GetScaleVector();

GLint		Viewport[4];
GLuint		SelectBuffer[SELECT_BUFFER_SIZE];
int			TileID=0;
CPoint		&CursorPos=Core->GetCursorPos();

int		StartX=(int)ThisCam.x;
int		StartY=(int)ThisCam.y;
float	ShiftX=ThisCam.x - (int)ThisCam.x;
float	ShiftY=ThisCam.y - (int)ThisCam.y;

		if (StartX<0) StartX=0;
		if (StartY<0) StartY=0;
		
int		DrawW=ZoomW+8;
int		DrawH=ZoomH+8;

		if (StartX+DrawW>MapWidth)	DrawW=MapWidth-StartX;
		if (StartY+DrawH>MapHeight)	DrawH=MapHeight-StartY;
		
		glGetIntegerv(GL_VIEWPORT, Viewport);
		glSelectBuffer (SELECT_BUFFER_SIZE, SelectBuffer );
		glRenderMode (GL_SELECT);

	    glInitNames();
		glPushName(-1);

		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		gluPickMatrix( MousePos.x ,(Viewport[3]-MousePos.y),5.0,5.0,Viewport);
		Core->GetView()->SetupPersMatrix();

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();
		glScalef(Scale.x,Scale.y,Scale.z);
//		glTranslatef(-ThisCam.x,ThisCam.y,0);
		glTranslatef(-ShiftX,ShiftY,0);
		glTranslatef(-ScrOfsX,ScrOfsY,0);	// Bring to top left corner

bool		WrapMap=GetSubType()==LAYER_SUBTYPE_BACK;
		if (WrapMap)
		{
			DrawW=MapWidth;
			DrawH=MapHeight;
		}
				
		for (int YLoop=0; YLoop<DrawH; YLoop++)
		{
			for (int XLoop=0; XLoop<DrawW; XLoop++)
			{
				int	XPos=StartX+XLoop;
				int	YPos=StartY+YLoop;
				if (WrapMap)
				{
					XPos%=MapWidth;
					YPos%=MapHeight;
				}

				TileID=(XPos)+(((YPos)*MapWidth));
				glLoadName (TileID);
				glBegin (GL_QUADS); 
					BuildGLQuad(XLoop,XLoop+1,-YLoop,-YLoop+1,0);
				glEnd();
				TileID++;
			}
		}

		TileID= glRenderMode (GL_RENDER);
		glPopMatrix();
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		CursorPos.x=CursorPos.y=-1;

// Process hits

GLuint	*HitPtr=SelectBuffer;

		if (TileID)	// Just take 1st		
		{
			int	HitID=HitPtr[3];
			CursorPos=IDToPoint(HitID,MapWidth);
		}
		glMatrixMode(GL_MODELVIEW);	// <-- Prevent arse GL assert
}

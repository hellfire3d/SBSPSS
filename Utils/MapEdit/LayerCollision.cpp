/******************/
/*** Layer Tile ***/
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
#include	"Utils.h"
#include	"Select.h"
#include	"Export.h"

GString		ColFName="Collision.bmp";

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
// New Layer
CLayerCollision::CLayerCollision(int _SubType,int Width,int Height)
{
		SubType=LAYER_SUBTYPE_NONE;
		SetDefaultParams();
		Mode=MouseModePaint;
		Map.SetSize(Width,Height,TRUE);
		VisibleFlag=true;
}

/*****************************************************************************/
// Load Layer
CLayerCollision::CLayerCollision(CFile *File,int Version)
{
		Load(File,Version);
}

/*****************************************************************************/
CLayerCollision::~CLayerCollision()
{
		TileBank->CleanUp();
		delete TileBank;
}

/*****************************************************************************/
void	CLayerCollision::InitSubView(CCore *Core)
{
GString	Filename;
		TileBank=new CTileBank;
		SubView=TileBank;

		Core->GetExecPath(Filename);
		Filename+=ColFName;
		TileBank->AddSet(Filename);
}

/*****************************************************************************/
void	CLayerCollision::Load(CFile *File,int Version)
{
// Version 2
		File->Read(&Render3dFlag,sizeof(BOOL));
		File->Read(&ScaleFactor,sizeof(float));
		File->Read(&ResizeFlag,sizeof(BOOL));
		File->Read(&VisibleFlag,sizeof(BOOL));
		File->Read(&Mode,sizeof(MouseMode));
		File->Read(&SubType,sizeof(int));
		Map.Load(File,Version);

		TRACE1("%s\t",GetName());
		TRACE1("Scl:%g\t",ScaleFactor);
		TRACE1("%i\n",VisibleFlag);
}

/*****************************************************************************/
void	CLayerCollision::Save(CFile *File)
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
void	CLayerCollision::Render(CCore *Core,Vector3 &CamPos,bool Is3d)
{
Vector3		ThisCam=Core->OffsetCam(CamPos,GetScaleFactor());

			CLayerTile::Render(Core,ThisCam,Map,false,0.5f);
}

/*****************************************************************************/
/*****************************************************************************/
bool	CLayerCollision::Command(int CmdMsg,CCore *Core,int Param0,int Param1)
{
bool	Ret=false;
		switch(CmdMsg)
		{
		case CmdMsg_MirrorY:
//			Ret=MirrorY(Core);	No longer mirror Y collision tiles
			break;
		default:
			Ret=CLayerTile::Command(CmdMsg,Core,Param0,Param1);
			break;
		}
		return(Ret);
}
/*****************************************************************************/
/*** Gui *********************************************************************/
/*****************************************************************************/
void	CLayerCollision::GUIInit(CCore *Core) 
{
		Core->GUIAdd(GUIToolBar,IDD_TOOLBAR);
		Core->GUIAdd(GUI,IDD_LAYER_COLLISION);
}

/*****************************************************************************/
void	CLayerCollision::GUIKill(CCore *Core)
{
		Core->GUIRemove(GUIToolBar,IDD_TOOLBAR);
		Core->GUIRemove(GUI,IDD_LAYER_COLLISION);
}

/*****************************************************************************/
void	CLayerCollision::GUIUpdate(CCore *Core)
{
		CLayerTile::GUIUpdate(Core);
}

/*****************************************************************************/
void	CLayerCollision::GUIChanged(CCore *Core)
{
		CLayerTile::GUIChanged(Core);
}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
void	CLayerCollision::Export(CCore *Core,CExport &Exp)
{
int		Width=Map.GetWidth();
int		Height=Map.GetHeight();
	
		Exp.ExportLayerHeader(LAYER_TYPE_COLLISION,SubType,Width,Height);

		for (int Y=0; Y<Height; Y++)
		{
			for (int X=0; X<Width; X++)
			{
				sMapElem		&MapElem=Map.Get(X,Y);
				sExpColTile		OutElem;
				
				if (MapElem.Tile)
				{
					OutElem.Tile=((MapElem.Tile-1)*4)+1;
					OutElem.Tile+=MapElem.Flags & PC_TILE_FLAG_MIRROR_XY;
					OutElem.Flags=MapElem.Flags>>PC_TILE_FLAG_COLLISION_SHIFT;
				}
				else
				{
					OutElem.Tile=0;
					OutElem.Flags=0;
				}
				
				Exp.Write(&OutElem,sizeof(sExpColTile));
			}
		}

}

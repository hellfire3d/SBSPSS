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

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
// New Layer
//CLayerCollision::CLayerCollision(int Type,int SubType,int Width,int Height) :CLayerTile(LAYER_TYPE_COLLISION,SubType,Width,Height)
CLayerCollision::CLayerCollision(sLayerDef &Def)
{
		InitLayer(Def);
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

		GetExecPath(Filename);
		Filename+=theApp.GetConfigStr("FileLocation","Collision");
		TileBank->AddSet(Filename);
}

/*****************************************************************************/
void	CLayerCollision::Load(CFile *File,int Version)
{
		if (Version<=5)
		{
			BOOL	DB;
			float	DF;
			LayerDef.Type=LAYER_TYPE_COLLISION;
			File->Read(&DB,sizeof(BOOL));
			File->Read(&DF,sizeof(float));
			File->Read(&DB,sizeof(BOOL));
			File->Read(&LayerDef.VisibleFlag,sizeof(BOOL));
			File->Read(&Mode,sizeof(MouseMode));
			File->Read(&LayerDef.SubType,sizeof(int));
		}
		else
		{
			File->Read(&Mode,sizeof(MouseMode));
		}
		InitLayer(LayerDef);
		Map.Load(File,Version);

}

/*****************************************************************************/
void	CLayerCollision::Save(CFile *File)
{
// Always Save current version
		File->Write(&Mode,sizeof(MouseMode));
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
	
		LayerDef.Width=Width;
		LayerDef.Height=Height;

		Exp.ExportLayerHeader(LayerDef);//LAYER_TYPE_COLLISION,LayerDef.SubType,Width,Height);

		for (int Y=0; Y<Height; Y++)
		{
			for (int X=0; X<Width; X++)
			{
				sMapElem		&MapElem=Map.Get(X,Y);
				sExpColTile		OutElem;
				
				if (MapElem.Tile)
				{
// new				OutElem.Tile=(MapElem.Tile-1)<<1;
//					OutElem.Tile+=MapElem.Flags & PC_TILE_FLAG_MIRROR_X;
//					OutElem.Flags=MapElem.Flags>>PC_TILE_FLAG_COLLISION_SHIFT;
// Old
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

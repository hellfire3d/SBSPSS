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
CLayerCollision::CLayerCollision(int _SubType,int Width,int Height)
{
		SubType=LAYER_SUBTYPE_NONE;
		SetDefaultParams();
		Mode=MouseModePaint;
		Map.SetSize(Width,Height,TRUE);
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
/*** Gui *********************************************************************/
/*****************************************************************************/
void	CLayerCollision::GUIInit(CCore *Core) 
{
//		Core->TileBankGUIInit();
		Core->GUIAdd(ToolBarGUI,IDD_LAYERTILE_TOOLBAR);
}

/*****************************************************************************/
void	CLayerCollision::GUIKill(CCore *Core)
{
//		Core->TileBankGUIKill();
		Core->GUIRemove(ToolBarGUI,IDD_LAYERTILE_TOOLBAR);
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
		Exp.ExportLayerCollision(Core,GetName(),SubType,Map);
}

/*****************************************************************************/
void	CLayerCollision::DeleteSet(int Set)
{
		Map.DeleteSet(Set);
}

/*****************************************************************************/
void	CLayerCollision::RemapSet(int OrigSet,int NewSet)
{
		Map.RemapSet(OrigSet,NewSet);

}

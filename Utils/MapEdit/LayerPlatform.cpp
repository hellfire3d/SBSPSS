/**********************/
/*** Layer Platform ***/
/**********************/

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
#include	"LayerPlatform.h"
#include	"Utils.h"
#include	"Export.h"

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
// New Layer
CLayerPlatform::CLayerPlatform(sLayerDef &Def) : CLayerThing(Def)
{
		LoadThingScript("c:\\temp\\character.txt");
}

/*****************************************************************************/
//CLayerPlatform::CLayerPlatform(CFile *File,int Version) :CLayerThing (File,Version)
//{
//}

/*****************************************************************************/
void	CLayerPlatform::Load(CFile *File,int Version) 
{
		CLayerThing::Load(File,Version);
}

/*****************************************************************************/
void	CLayerPlatform::Save(CFile *File) 
{
		CLayerThing::Save(File);
}

/*****************************************************************************/
void	CLayerPlatform::InitSubView(CCore *Core)
{
}

/*****************************************************************************/
void	CLayerPlatform::LoadThingScript(const char *Filename)
{
	CLayerThing::LoadThingScript(Filename);
//	CLayerThing::ThingScript.LoadAndImport(Filename);

int	Count=ThingScript.GetGroupCount();
		for (int i=0; i<Count; i++)
		{
			char	*Name=ThingScript.GetGroupName(i);
			char	*Gfx=ThingScript.GetData(Name,"gfx");
			TRACE2("%s\t\t%s\n",Name,Gfx);
			if (Gfx)
			ThingBank->AddSet(Gfx);
		}
}

/*****************************************************************************/
/*** Gui *********************************************************************/
/*****************************************************************************/
void	CLayerPlatform::GUIInit(CCore *Core)
{
//		Core->GUIAdd(GUIToolBar,IDD_TOOLBAR);
}

/*****************************************************************************/
void	CLayerPlatform::GUIKill(CCore *Core)
{
//		Core->GUIRemove(GUIToolBar,IDD_TOOLBAR);
}

/*****************************************************************************/
void	CLayerPlatform::GUIUpdate(CCore *Core)
{
}

/*****************************************************************************/
void	CLayerPlatform::GUIChanged(CCore *Core)
{
}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
void	CLayerPlatform::Export(CCore *Core,CExport &Exp)
{

}

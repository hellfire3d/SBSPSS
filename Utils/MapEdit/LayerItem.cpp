/******************/
/*** Layer Item ***/
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
#include	"LayerThing.h"
#include	"LayerItem.h"
#include	"Utils.h"
#include	"Export.h"

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
// New Layer
CLayerItem::CLayerItem(sLayerDef &Def) : CLayerThing (Def)
{
		LoadThingScript("c:\\temp\\character.txt");
}

/*****************************************************************************/
//CLayerItem::CLayerItem(CFile *File,int Version) :CLayerThing (File,Version)
//{
//
//}

/*****************************************************************************/
void	CLayerItem::Load(CFile *File,int Version) 
{
		CLayerThing::Load(File,Version);
}

/*****************************************************************************/
void	CLayerItem::Save(CFile *File) 
{
		CLayerThing::Save(File);
}

/*****************************************************************************/
void	CLayerItem::InitSubView(CCore *Core)
{
}

/*****************************************************************************/
void	CLayerItem::LoadThingScript(const char *Filename)
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
void	CLayerItem::GUIInit(CCore *Core)
{
//		Core->GUIAdd(GUIToolBar,IDD_TOOLBAR);
}

/*****************************************************************************/
void	CLayerItem::GUIKill(CCore *Core)
{
//		Core->GUIRemove(GUIToolBar,IDD_TOOLBAR);
}

/*****************************************************************************/
void	CLayerItem::GUIUpdate(CCore *Core)
{
}

/*****************************************************************************/
void	CLayerItem::GUIChanged(CCore *Core)
{
}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
void	CLayerItem::Export(CCore *Core,CExport &Exp)
{

}

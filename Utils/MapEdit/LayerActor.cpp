/*******************/
/*** Layer Actor ***/
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
#include	"LayerThing.h"
#include	"LayerActor.h"
#include	"Utils.h"
#include	"Export.h"

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
CLayerActor::CLayerActor(sLayerDef &Def) :CLayerThing(Def)
{
//	CLayerThing::CLayerThing(_SubType,_Width,_Height);
		LoadThingScript("c:\\temp\\character.txt");
}

/*****************************************************************************/
//CLayerActor::CLayerActor(CFile *File,int Version) 
//{
//	CLayerThing::CLayerThing(File,Version);
//
//}

/*****************************************************************************/
void	CLayerActor::Load(CFile *File,int Version) 
{
//		CLayerThing::Load(File,Version);
}

/*****************************************************************************/
void	CLayerActor::Save(CFile *File) 
{
		CLayerThing::Save(File);
}

/*****************************************************************************/
void	CLayerActor::InitSubView(CCore *Core)
{
}

/*****************************************************************************/
void	CLayerActor::LoadThingScript(const char *Filename)
{
	CLayerThing::LoadThingScript(Filename);
//	CLayerThing::ThingScript.LoadAndImport(Filename);

int	Count=ThingScript.GetGroupCount();
		for (int i=0; i<Count; i++)
		{
			char	*Name=ThingScript.GetGroupName(i);
			char	*Gfx=ThingScript.GetData(Name,"gfx");
			TRACE2("%s\t\t%s\n",Name,Gfx);
			if (Gfx) ThingBank->AddSet(Gfx);
		}
}

/*****************************************************************************/
/*** Gui *********************************************************************/
/*****************************************************************************/
void	CLayerActor::GUIInit(CCore *Core)
{
//		Core->GUIAdd(GUIToolBar,IDD_TOOLBAR);
}

/*****************************************************************************/
void	CLayerActor::GUIKill(CCore *Core)
{
//		Core->GUIRemove(GUIToolBar,IDD_TOOLBAR);
}

/*****************************************************************************/
void	CLayerActor::GUIUpdate(CCore *Core)
{
}

/*****************************************************************************/
void	CLayerActor::GUIChanged(CCore *Core)
{
}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
void	CLayerActor::Export(CCore *Core,CExport &Exp)
{

}

/**********************/
/*** Layer Item ***/
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
#include	"LayerItem.h"
#include	"Utils.h"
#include	"Export.h"

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
CLayerItem::CLayerItem(sLayerDef &Def)
{
		InitLayer(Def);
}

/*****************************************************************************/
void	CLayerItem::InitLayer(sLayerDef &Def)
{
		ThingBank=new CElemBank(-1,-1,false,CElem::CentreModeLR || CElem::CentreModeTB);
		CLayerThing::InitLayer(Def);
		LoadThingScript(theApp.GetConfigStr("LayerScript","ItemScript"));
		DrawPoints=false;

}

/*****************************************************************************/
void	CLayerItem::InitSubView(CCore *Core)
{
}

/*****************************************************************************/
void	CLayerItem::LoadDefThing(const char *Name,sLayerThing &ThisDef)
{
}

/*****************************************************************************/
void	CLayerItem::LoadOldThing(CFile *File,sLayerThing &ThisThing)
{
sLayerThingDataOLD	OldThing;

		File->Read(&OldThing,sizeof(sLayerThingDataOLD));
}


/*****************************************************************************/
/*** Gui *********************************************************************/
/*****************************************************************************/
void	CLayerItem::GUIInit(CCore *Core)
{
		Core->GUIAdd(GUIThing,IDD_LAYER_THING);
}

/*****************************************************************************/
void	CLayerItem::GUIKill(CCore *Core)
{
		Core->GUIRemove(GUIThing,IDD_LAYER_THING);
}

/*****************************************************************************/
void	CLayerItem::GUIUpdate(CCore *Core)
{
int			i,ListSize;
CComboBox	&List=GUIThing.m_DefList;

// Setup Def Item List
		ListSize=DefList.size();
		List.ResetContent();
		for (i=0; i<ListSize; i++)
		{
			List.AddString(DefList[i].Name);
		}
		List.SetCurSel(CurrentDefThing);

//		GUIThingUpdate();

}

/*****************************************************************************/
void	CLayerItem::GUIThingDefClear()
{

CComboBox	&List=GUIThing.m_DefList;
		CurrentDefThing=-1;
		List.SetCurSel(CurrentDefThing);

}

/*****************************************************************************/
void	CLayerItem::GUIThingUpdate(bool OnlySel)
{
int			i,ListSize;
CComboBox	&List=GUIThing.m_List;

		if (OnlySel)
		{
			List.SetCurSel(CurrentThing);
			return;
		}

// Setup ThingList
		ListSize=ThingList.size();
		TRACE1("%i\n",ListSize);
		List.ResetContent();
		for (i=0; i<ListSize; i++)
		{
			
			List.AddString(ThingList[i].Name);
		}
		List.SetCurSel(CurrentThing);
		GUIThingPointUpdate();
}

/*****************************************************************************/
void	CLayerItem::GUIChanged(CCore *Core)
{
}


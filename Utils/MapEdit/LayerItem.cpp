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
/*** Gui *********************************************************************/
/*****************************************************************************/
void	CLayerItem::GUIInit(CCore *Core)
{
//		GUI.DisableCallback(true);
		Core->GUIAdd(GUI,IDD_LAYER_ITEM);
//		GUI.DisableCallback(false);
}

/*****************************************************************************/
void	CLayerItem::GUIKill(CCore *Core)
{
		Core->GUIRemove(GUI,IDD_LAYER_ITEM);
}

/*****************************************************************************/
void	CLayerItem::GUIUpdate(CCore *Core)
{
int			i,ListSize;
CComboBox	&List=GUI.m_List;

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
CComboBox	&List=GUI.m_List;
		CurrentDefThing=-1;
		List.SetCurSel(CurrentDefThing);
}

/*****************************************************************************/
void	CLayerItem::GUIThingUpdate(bool OnlySel)
{
int			i,ListSize;
CComboBox	&List=GUI.m_LevelList;

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

/*****************************************************************************/
/*** Functions ***************************************************************/
/*****************************************************************************/
void	CLayerItem::ExportThing(CExport &Exp,sLayerThing &ThisThing)
{
/*
int				i,ListSize=ThisThing.XY.size();
sExpLayerItem	OutThing;
// Point List
		Exp.Write(&ListSize,sizeof(int));
		for (i=0 ;i<ListSize; i++)
		{
			Exp.Write(&ThisThing.XY[i],sizeof(CPoint));
		}
// Thing
		OutThing.Health=ThisThing.Data.Health;
		OutThing.AttackStrength=ThisThing.Data.AttackStrength;
		OutThing.Speed=ThisThing.Data.Speed;
		OutThing.TurnRate=ThisThing.Data.TurnRate;
		OutThing.CollisionFlag=ThisThing.Data.CollisionFlag;
		OutThing.PlayerFlag=ThisThing.Data.PlayerFlag;
		Exp.Write(&OutThing,sizeof(sExpLayerItem));
*/
}

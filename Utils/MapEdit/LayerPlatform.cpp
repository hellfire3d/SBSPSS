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
CLayerPlatform::CLayerPlatform(sLayerDef &Def)
{
		InitLayer(Def);
}

/*****************************************************************************/
void	CLayerPlatform::InitLayer(sLayerDef &Def)
{
		ThingBank=new CElemBank(-1,-1,false,0);
		CLayerThing::InitLayer(Def);
		LoadThingScript(theApp.GetConfigStr("LayerScript","PlatformScript"));
}

/*****************************************************************************/
void	CLayerPlatform::InitSubView(CCore *Core)
{
}

/*****************************************************************************/
/*** Gui *********************************************************************/
/*****************************************************************************/
void	CLayerPlatform::GUIInit(CCore *Core)
{
//		GUI.DisableCallback(true);
//		Core->GUIAdd(GUI,IDD_LAYER_Platform);
//		GUI.DisableCallback(false);
}

/*****************************************************************************/
void	CLayerPlatform::GUIKill(CCore *Core)
{
//		Core->GUIRemove(GUI,IDD_LAYER_Platform);
}

/*****************************************************************************/
void	CLayerPlatform::GUIUpdate(CCore *Core)
{
/*
int			i,ListSize;
CComboBox	&List=GUI.m_List;

// Setup Def Platform List
		ListSize=DefList.size();
		List.ResetContent();
		for (i=0; i<ListSize; i++)
		{
			List.AddString(DefList[i].Name);
		}
		List.SetCurSel(CurrentDefThing);

		GUIThingUpdate();
*/
}

/*****************************************************************************/
void	CLayerPlatform::GUIThingDefClear()
{
/*
CComboBox	&List=GUI.m_List;
		CurrentDefThing=-1;
		List.SetCurSel(CurrentDefThing);
*/
}

/*****************************************************************************/
void	CLayerPlatform::GUIThingUpdate(bool OnlySel)
{
//	CLayerThing::GUIThingUpdateList(GUI.
}

/*****************************************************************************/
void	CLayerPlatform::GUIThingPointUpdate(bool OnlySel)
{
/*
int			i,ListSize;
sLayerThing	&ThisThing=ThingList[CurrentThing];
CListBox	&List=GUI.m_PosList;

		List.ResetContent();
		if (CurrentThing==-1) 
		{
		}
		else
		{
// Setup ThingPointList
		ListSize=ThisThing.XY.size();
		for (i=0; i<ListSize; i++)
		{
			CString	Str;
			Str.Format("%i: %i, %i",i,ThisThing.XY[i].x,ThisThing.XY[i].y);
			List.AddString(Str);
		}
		List.SetCurSel(CurrentThingPoint);
		}
*/
}

/*****************************************************************************/
void	CLayerPlatform::GUIChanged(CCore *Core)
{
/*
		if (CurrentThing!=-1)
		{
			sLayerThing	&ThisThing=ThingList[CurrentThing];
			ThisThing.Data.Speed=GUI.GetVal(GUI.m_Speed);
			ThisThing.Data.TurnRate=GUI.GetVal(GUI.m_TurnRate);
			ThisThing.Data.Health=GUI.GetVal(GUI.m_Health);
			ThisThing.Data.AttackStrength=GUI.GetVal(GUI.m_Attack);
			ThisThing.Data.CollisionFlag=GUI.m_Collision.GetCheck()!=0;
			ThisThing.Data.PlayerFlag=GUI.m_Player.GetCheck()!=0;
		}
*/
}


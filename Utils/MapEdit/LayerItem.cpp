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
		CLayerThing::InitLayer(Def);
		LoadThingScript(theApp.GetConfigStr("LayerScript","ItemScript"));
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
//		Core->GUIAdd(GUI,IDD_LAYER_Item);
//		GUI.DisableCallback(false);
}

/*****************************************************************************/
void	CLayerItem::GUIKill(CCore *Core)
{
//		Core->GUIRemove(GUI,IDD_LAYER_Item);
}

/*****************************************************************************/
void	CLayerItem::GUIUpdate(CCore *Core)
{
/*
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

		GUIThingUpdate();
*/
}

/*****************************************************************************/
void	CLayerItem::GUIThingDefClear()
{
/*
CComboBox	&List=GUI.m_List;
		CurrentDefThing=-1;
		List.SetCurSel(CurrentDefThing);
*/
}

/*****************************************************************************/
void	CLayerItem::GUIThingUpdate()
{
/*
int			i,ListSize;
CComboBox	&List=GUI.m_LevelList;
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
// Params
		GUI.DisableCallback(true);
		if (CurrentThing!=-1)
		{
			sLayerThing	&ThisThing=ThingList[CurrentThing];
			GUI.SetVal(GUI.m_Speed,ThisThing.Data.Speed);
			GUI.SetVal(GUI.m_TurnRate,ThisThing.Data.TurnRate);
			GUI.SetVal(GUI.m_Health,ThisThing.Data.Health);
			GUI.SetVal(GUI.m_Attack,ThisThing.Data.AttackStrength);
			GUI.m_Collision.SetCheck(ThisThing.Data.CollisionFlag);
			GUI.m_Player.SetCheck(ThisThing.Data.PlayerFlag);
		}
		else
		{
			GUI.m_Speed.SetWindowText("");
			GUI.m_TurnRate.SetWindowText("");
			GUI.m_Health.SetWindowText("");
			GUI.m_Attack.SetWindowText("");
			GUI.m_Collision.SetCheck(false);
			GUI.m_Player.SetCheck(false);
		}
		GUI.DisableCallback(false);
*/
}

/*****************************************************************************/
void	CLayerItem::GUIThingPointUpdate()
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
void	CLayerItem::GUIChanged(CCore *Core)
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

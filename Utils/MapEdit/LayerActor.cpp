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
CLayerActor::CLayerActor(sLayerDef &Def)
{
		InitLayer(Def);
}

/*****************************************************************************/
void	CLayerActor::InitLayer(sLayerDef &Def)
{
		ThingBank=new CElemBank(-1,-1,false,CElem::CentreModeLR | CElem::CentreModeB);
		CLayerThing::InitLayer(Def);
		LoadThingScript(theApp.GetConfigStr("LayerScript","ActorScript"));
}

/*****************************************************************************/
void	CLayerActor::InitSubView(CCore *Core)
{
}

/*****************************************************************************/
/*** Gui *********************************************************************/
/*****************************************************************************/
void	CLayerActor::GUIInit(CCore *Core)
{
		GUIActor.DisableCallback(true);
		Core->GUIAdd(GUIThing,IDD_LAYER_THING);
		Core->GUIAdd(GUIThingPos,IDD_LAYER_THING_POS);
		Core->GUIAdd(GUIActor,IDD_LAYER_ACTOR);
		GUIActor.DisableCallback(false);
}

/*****************************************************************************/
void	CLayerActor::GUIKill(CCore *Core)
{
		Core->GUIRemove(GUIThing,IDD_LAYER_THING);
		Core->GUIRemove(GUIThingPos,IDD_LAYER_THING_POS);
		Core->GUIRemove(GUIActor,IDD_LAYER_ACTOR);
}

/*****************************************************************************/
void	CLayerActor::GUIUpdate(CCore *Core)
{
int			i,ListSize;
CComboBox	&List=GUIThing.m_DefList;

// Setup Def Actor List
		ListSize=DefList.size();
		List.ResetContent();
		for (i=0; i<ListSize; i++)
		{
			List.AddString(DefList[i].Name);
		}
		List.SetCurSel(CurrentDefThing);

		GUIThingUpdate();
}

/*****************************************************************************/
void	CLayerActor::GUIThingDefClear()
{
CComboBox	&List=GUIThing.m_DefList;
		CurrentDefThing=-1;
		List.SetCurSel(CurrentDefThing);
}

/*****************************************************************************/
void	CLayerActor::GUIThingUpdate(bool OnlySel)
{
		GUIThingUpdateList(GUIThing.m_List,false);
// Params
		GUIActor.DisableCallback(true);
		if (CurrentThing!=-1)
		{
			sLayerThing	&ThisThing=ThingList[CurrentThing];
			GUIActor.SetVal(GUIActor.m_Speed,ThisThing.Data.Speed);
			GUIActor.SetVal(GUIActor.m_TurnRate,ThisThing.Data.TurnRate);
			GUIActor.SetVal(GUIActor.m_Health,ThisThing.Data.Health);
			GUIActor.SetVal(GUIActor.m_Attack,ThisThing.Data.AttackStrength);
			GUIActor.m_Collision.SetCheck(ThisThing.Data.CollisionFlag);
			GUIActor.m_Player.SetCheck(ThisThing.Data.PlayerFlag);
		}
		else
		{
			GUIActor.m_Speed.SetWindowText("");
			GUIActor.m_TurnRate.SetWindowText("");
			GUIActor.m_Health.SetWindowText("");
			GUIActor.m_Attack.SetWindowText("");
			GUIActor.m_Collision.SetCheck(false);
			GUIActor.m_Player.SetCheck(false);
		}
		GUIActor.DisableCallback(false);
}

/*****************************************************************************/
void	CLayerActor::GUIThingPointUpdate(bool OnlySel)
{
		GUIThingPointUpdateList(GUIThingPos.m_List,OnlySel);
}

/*****************************************************************************/
void	CLayerActor::GUIChanged(CCore *Core)
{
		if (CurrentThing!=-1)
		{
			sLayerThing	&ThisThing=ThingList[CurrentThing];
			ThisThing.Data.Speed=GUIActor.GetVal(GUIActor.m_Speed);
			ThisThing.Data.TurnRate=GUIActor.GetVal(GUIActor.m_TurnRate);
			ThisThing.Data.Health=GUIActor.GetVal(GUIActor.m_Health);
			ThisThing.Data.AttackStrength=GUIActor.GetVal(GUIActor.m_Attack);
			ThisThing.Data.CollisionFlag=GUIActor.m_Collision.GetCheck()!=0;
			ThisThing.Data.PlayerFlag=GUIActor.m_Player.GetCheck()!=0;
		}

}

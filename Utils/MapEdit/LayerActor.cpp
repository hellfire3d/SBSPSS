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
		GUI.DisableCallback(true);
		Core->GUIAdd(GUI,IDD_LAYER_ACTOR);
		GUI.DisableCallback(false);
}

/*****************************************************************************/
void	CLayerActor::GUIKill(CCore *Core)
{
		Core->GUIRemove(GUI,IDD_LAYER_ACTOR);
}

/*****************************************************************************/
void	CLayerActor::GUIUpdate(CCore *Core)
{
int			i,ListSize;
CComboBox	&List=GUI.m_List;

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
CComboBox	&List=GUI.m_List;
		CurrentDefThing=-1;
		List.SetCurSel(CurrentDefThing);
}

/*****************************************************************************/
void	CLayerActor::GUIThingUpdate(bool OnlySel)
{
		GUIThingUpdateList(GUI.m_LevelList,false);
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
}

/*****************************************************************************/
void	CLayerActor::GUIThingPointUpdate(bool OnlySel)
{
		GUIThingPointUpdateList(GUI.m_PosList,OnlySel);
}

/*****************************************************************************/
void	CLayerActor::GUIChanged(CCore *Core)
{
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

}

/*****************************************************************************/
/*** Functions ***************************************************************/
/*****************************************************************************/
void	CLayerActor::ExportThing(CExport &Exp,sLayerThing &ThisThing)
{
int				i,ListSize=ThisThing.XY.size();
sExpLayerActor	OutThing;
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
		Exp.Write(&OutThing,sizeof(sExpLayerActor));
}

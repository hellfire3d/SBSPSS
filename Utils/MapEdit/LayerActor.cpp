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
void	CLayerActor::LoadDefThing(const char *Name,sLayerThing &ThisDef)
{
		ThisDef.Data.Actor.ActorSpeed=ThingScript.GetInt(Name,"Speed");
		ThisDef.Data.Actor.ActorTurnRate=ThingScript.GetInt(Name,"TurnRate");
		ThisDef.Data.Actor.ActorHealth=ThingScript.GetInt(Name,"Health");
		ThisDef.Data.Actor.ActorAttackStrength=ThingScript.GetInt(Name,"AttackStrength");
		ThisDef.Data.Actor.ActorCollisionFlag=ThingScript.GetInt(Name,"Collision")==1;
}

/*****************************************************************************/
void	CLayerActor::LoadOldThing(CFile *File,sLayerThing &ThisThing)
{
sLayerThingDataOLD	OldThing;

		File->Read(&OldThing,sizeof(sLayerThingDataOLD));

		ThisThing.Data.Actor.ActorSpeed=OldThing.Speed;
		ThisThing.Data.Actor.ActorTurnRate=OldThing.TurnRate;
		ThisThing.Data.Actor.ActorHealth=OldThing.Health;
		ThisThing.Data.Actor.ActorAttackStrength=OldThing.AttackStrength;
		ThisThing.Data.Actor.ActorCollisionFlag=OldThing.CollisionFlag;

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

		GUIActor.m_HealthSpin.SetRange(0,255);
		GUIActor.m_AttackSpin.SetRange(0,255);
		GUIActor.m_SpeedSpin.SetRange(0,255);
		GUIActor.m_TurnRateSpin.SetRange(0,255);

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
			GUIActor.SetVal(GUIActor.m_Speed,ThisThing.Data.Actor.ActorSpeed);
			GUIActor.SetVal(GUIActor.m_TurnRate,ThisThing.Data.Actor.ActorTurnRate);
			GUIActor.SetVal(GUIActor.m_Health,ThisThing.Data.Actor.ActorHealth);
			GUIActor.SetVal(GUIActor.m_Attack,ThisThing.Data.Actor.ActorAttackStrength);
			GUIActor.m_Collision.SetCheck(ThisThing.Data.Actor.ActorCollisionFlag);
			bool	IsPlayer=ThingScript.GetInt(ThisThing.Name,"Player")==1;
			GUIActor.m_Player.SetCheck(IsPlayer);
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
			ThisThing.Data.Actor.ActorSpeed=GUIActor.GetVal(GUIActor.m_Speed);
			ThisThing.Data.Actor.ActorTurnRate=GUIActor.GetVal(GUIActor.m_TurnRate);
			ThisThing.Data.Actor.ActorHealth=GUIActor.GetVal(GUIActor.m_Health);
			ThisThing.Data.Actor.ActorAttackStrength=GUIActor.GetVal(GUIActor.m_Attack);
			ThisThing.Data.Actor.ActorCollisionFlag=GUIActor.m_Collision.GetCheck()!=0;
		}

}

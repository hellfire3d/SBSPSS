/********************/
/*** Layer Hazard ***/
/********************/

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
#include	"LayerHazard.h"
#include	"Utils.h"
#include	"Export.h"

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
CLayerHazard::CLayerHazard(sLayerDef &Def)
{
		InitLayer(Def);
}

/*****************************************************************************/
void	CLayerHazard::InitLayer(sLayerDef &Def)
{
		ThingBank=new CElemBank(-1,-1,false,CElem::CentreModeLR | CElem::CentreModeB);
		CLayerThing::InitLayer(Def);
		LoadThingScript(theApp.GetConfigStr("LayerScript","HazardScript"));
}

/*****************************************************************************/
void	CLayerHazard::InitSubView(CCore *Core)
{
}

/*****************************************************************************/
void	CLayerHazard::LoadDefThing(const char *Name,sLayerThing &ThisDef)
{
		ThisDef.Data.Hazard.HazardSpeed=ThingScript.GetInt(Name,"Speed");
		ThisDef.Data.Hazard.HazardTurnRate=ThingScript.GetInt(Name,"TurnRate");
		ThisDef.Data.Hazard.HazardHealth=ThingScript.GetInt(Name,"Health");
		ThisDef.Data.Hazard.HazardAttackStrength=ThingScript.GetInt(Name,"AttackStrength");
		ThisDef.Data.Hazard.HazardRespawn=ThingScript.GetInt(Name,"Respawn");
		ThisDef.Data.Hazard.HazardCollisionFlag=ThingScript.GetInt(Name,"Collision")==1;

}

/*****************************************************************************/
void	CLayerHazard::LoadOldThing(CFile *File,sLayerThing &ThisThing)
{
sLayerThingDataOLD	OldThing;

		File->Read(&OldThing,sizeof(sLayerThingDataOLD));
		ThisThing.Data.Hazard.HazardSpeed=OldThing.Speed;
		ThisThing.Data.Hazard.HazardTurnRate=OldThing.TurnRate;
		ThisThing.Data.Hazard.HazardHealth=OldThing.Health;
		ThisThing.Data.Hazard.HazardAttackStrength=OldThing.AttackStrength;
		ThisThing.Data.Hazard.HazardRespawn=OldThing.Speed;
		ThisThing.Data.Hazard.HazardCollisionFlag=OldThing.CollisionFlag;
}


/*****************************************************************************/
/*** Gui *********************************************************************/
/*****************************************************************************/
void	CLayerHazard::GUIInit(CCore *Core)
{
		GUIHazard.DisableCallback(true);
		Core->GUIAdd(GUIThing,IDD_LAYER_THING);
		Core->GUIAdd(GUIThingPos,IDD_LAYER_THING_POS);
		Core->GUIAdd(GUIHazard,IDD_LAYER_HAZARD);
		GUIHazard.DisableCallback(false);

		GUIHazard.m_HealthSpin.SetRange(0,255);
		GUIHazard.m_AttackSpin.SetRange(0,255);
		GUIHazard.m_SpeedSpin.SetRange(0,255);
		GUIHazard.m_TurnRateSpin.SetRange(0,255);
		GUIHazard.m_RespawnSpin.SetRange(0,255);

}

/*****************************************************************************/
void	CLayerHazard::GUIKill(CCore *Core)
{
		Core->GUIRemove(GUIThing,IDD_LAYER_THING);
		Core->GUIRemove(GUIThingPos,IDD_LAYER_THING_POS);
		Core->GUIRemove(GUIHazard,IDD_LAYER_HAZARD);
}

/*****************************************************************************/
void	CLayerHazard::GUIUpdate(CCore *Core)
{
int			i,ListSize;
CComboBox	&List=GUIThing.m_DefList;

// Setup Def Hazard List
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
void	CLayerHazard::GUIThingDefClear()
{
CComboBox	&List=GUIThing.m_DefList;
		CurrentDefThing=-1;
		List.SetCurSel(CurrentDefThing);
}

/*****************************************************************************/
void	CLayerHazard::GUIThingUpdate(bool OnlySel)
{
		GUIThingUpdateList(GUIThing.m_List,false);
// Params
		GUIHazard.DisableCallback(true);
		if (CurrentThing!=-1)
		{
			sLayerThing	&ThisThing=ThingList[CurrentThing];
			GUIHazard.SetVal(GUIHazard.m_Speed,ThisThing.Data.Hazard.HazardSpeed);
			GUIHazard.SetVal(GUIHazard.m_TurnRate,ThisThing.Data.Hazard.HazardTurnRate);
			GUIHazard.SetVal(GUIHazard.m_Health,ThisThing.Data.Hazard.HazardHealth);
			GUIHazard.SetVal(GUIHazard.m_Attack,ThisThing.Data.Hazard.HazardAttackStrength);
			GUIHazard.SetVal(GUIHazard.m_Respawn,ThisThing.Data.Hazard.HazardRespawn);
			GUIHazard.m_Collision.SetCheck(ThisThing.Data.Hazard.HazardCollisionFlag);
		}
		else
		{
			GUIHazard.m_Speed.SetWindowText("");
			GUIHazard.m_TurnRate.SetWindowText("");
			GUIHazard.m_Health.SetWindowText("");
			GUIHazard.m_Attack.SetWindowText("");
			GUIHazard.m_Respawn.SetWindowText("");
			GUIHazard.m_Collision.SetCheck(false);
		}
		GUIHazard.DisableCallback(false);
}

/*****************************************************************************/
void	CLayerHazard::GUIThingPointUpdate(bool OnlySel)
{
		GUIThingPointUpdateList(GUIThingPos.m_List,OnlySel);
}

/*****************************************************************************/
void	CLayerHazard::GUIChanged(CCore *Core)
{
		if (CurrentThing!=-1)
		{
			sLayerThing	&ThisThing=ThingList[CurrentThing];
			ThisThing.Data.Hazard.HazardSpeed=GUIHazard.GetVal(GUIHazard.m_Speed);
			ThisThing.Data.Hazard.HazardTurnRate=GUIHazard.GetVal(GUIHazard.m_TurnRate);
			ThisThing.Data.Hazard.HazardHealth=GUIHazard.GetVal(GUIHazard.m_Health);
			ThisThing.Data.Hazard.HazardAttackStrength=GUIHazard.GetVal(GUIHazard.m_Attack);
			ThisThing.Data.Hazard.HazardRespawn=GUIHazard.GetVal(GUIHazard.m_Respawn);
			ThisThing.Data.Hazard.HazardCollisionFlag=GUIHazard.m_Collision.GetCheck()!=0;
		}

}

/*****************************************************************************/
void	CLayerHazard::Export(CCore *Core,CExport &Exp)
{
		CLayerThing::Export(Core,Exp);
}

/*****************************************************************************/
void	CLayerHazard::ExportThingData(CCore *Core,CExport &Exp,sLayerThing &ThisThing,sLayerThingData &OutThing)
{
CElem	&ThisElem=ThingBank->GetElem(ThisThing.ElemID,0);

		Exp.ExportElem3d(Core,ThisElem,OutThing.Hazard.HazardTriStart,OutThing.Hazard.HazardTriCount);
		
}

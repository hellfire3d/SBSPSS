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
		ThingBank=new CElemBank(-1,-1,false,CElem::CentreModeLR /*| CElem::CentreModeB*/);
		CLayerThing::InitLayer(Def);
		LoadThingScript(theApp.GetConfigStr("LayerScript","PlatformScript"));
}

/*****************************************************************************/
void	CLayerPlatform::InitSubView(CCore *Core)
{
}

/*****************************************************************************/
void	CLayerPlatform::LoadDefThing(const char *Name,sLayerThing &ThisDef)
{
}

/*****************************************************************************/
void	CLayerPlatform::LoadOldThing(CFile *File,sLayerThing &ThisThing)
{
sLayerThingDataOLD	OldThing;

		File->Read(&OldThing,sizeof(sLayerThingDataOLD));
		ThisThing.Data.Platform.PlatformSpeed=OldThing.Speed;
		ThisThing.Data.Platform.PlatformTurnRate=OldThing.TurnRate;
		ThisThing.Data.Platform.PlatformType=OldThing.PlatformType;
		ThisThing.Data.Platform.PlatformMoveType=OldThing.MoveType;
		ThisThing.Data.Platform.PlatformCollisionFlag=OldThing.CollisionFlag;
		ThisThing.Data.Platform.PlatformTriCount=OldThing.TriCount;	// Not needed but what the hell!!
		ThisThing.Data.Platform.PlatformTriStart=OldThing.TriStart;	// Not needed but what the hell!!
}


/*****************************************************************************/
/*** Gui *********************************************************************/
/*****************************************************************************/
void	CLayerPlatform::GUIInit(CCore *Core)
{
		GUIPlatform.DisableCallback(true);
		Core->GUIAdd(GUIThing,IDD_LAYER_THING);
		Core->GUIAdd(GUIThingPos,IDD_LAYER_THING_POS);
		Core->GUIAdd(GUIPlatform,IDD_LAYER_PLATFORM);
		GUIPlatform.DisableCallback(false);

		GUIPlatform.m_SpeedSpin.SetRange(0,255);
		GUIPlatform.m_TurnRateSpin.SetRange(0,255);
		GUIPlatform.m_Param0Spin.SetRange(0,255);

// Init type lists
		{
CComboBox	&List=GUIPlatform.m_MoveList;
			List.AddString("Linear");
			List.AddString("Circular");
		}

		{
CComboBox	&List=GUIPlatform.m_Type;
			List.AddString("Normal");
			List.AddString("Weighted");
			List.AddString("Rotating");
		}
		GUIPlatform.m_Param0Txt.ShowWindow(SW_HIDE);
		GUIPlatform.m_Param0.ShowWindow(SW_HIDE);
		GUIPlatform.m_Param0Spin.ShowWindow(SW_HIDE);

}

/*****************************************************************************/
void	CLayerPlatform::GUIKill(CCore *Core)
{
		Core->GUIRemove(GUIThing,IDD_LAYER_THING);
		Core->GUIRemove(GUIThingPos,IDD_LAYER_THING_POS);
		Core->GUIRemove(GUIPlatform,IDD_LAYER_PLATFORM);
}

/*****************************************************************************/
void	CLayerPlatform::GUIUpdate(CCore *Core)
{
int			i,ListSize;
CComboBox	&List=GUIThing.m_DefList;

// Setup Def Platform List
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
void	CLayerPlatform::GUIThingDefClear()
{
CComboBox	&List=GUIThing.m_DefList;
		CurrentDefThing=-1;
		List.SetCurSel(CurrentDefThing);
}

/*****************************************************************************/
void	CLayerPlatform::GUIThingUpdate(bool OnlySel)
{
		GUIThingUpdateList(GUIThing.m_List,false);
// Params
		GUIPlatform.DisableCallback(true);
		if (CurrentThing!=-1)
		{
			sLayerThing	&ThisThing=ThingList[CurrentThing];
			GUIPlatform.SetVal(GUIPlatform.m_Speed,ThisThing.Data.Platform.PlatformSpeed);
			GUIPlatform.SetVal(GUIPlatform.m_TurnRate,ThisThing.Data.Platform.PlatformTurnRate);
			GUIPlatform.m_Collision.SetCheck(ThisThing.Data.Platform.PlatformCollisionFlag);
			GUIPlatform.m_MoveList.SetCurSel(ThisThing.Data.Platform.PlatformMoveType);
			GUIPlatform.m_Type.SetCurSel(ThisThing.Data.Platform.PlatformType);
// Additional Params
			char	*Param0Txt=ThingScript.GetStr(ThisThing.Name,"Param0");
			if (Param0Txt)
			{
				GUIPlatform.m_Param0Txt.SetWindowText(Param0Txt);
				GUIPlatform.SetVal(GUIPlatform.m_Param0,ThisThing.Data.Platform.PlatformParam0);
				GUIPlatform.m_Param0Txt.ShowWindow(SW_SHOW);
				GUIPlatform.m_Param0.ShowWindow(SW_SHOW);
				GUIPlatform.m_Param0Spin.ShowWindow(SW_SHOW);
			}
			else
			{
				GUIPlatform.m_Param0Txt.ShowWindow(SW_HIDE);
				GUIPlatform.m_Param0.ShowWindow(SW_HIDE);
				GUIPlatform.m_Param0Spin.ShowWindow(SW_HIDE);
			}
		}
		else
		{
			GUIPlatform.m_Speed.SetWindowText("");
			GUIPlatform.m_TurnRate.SetWindowText("");
			GUIPlatform.m_Collision.SetCheck(false);
			GUIPlatform.m_MoveList.SetCurSel(-1);
			GUIPlatform.m_Type.SetCurSel(-1);

		}
		GUIPlatform.DisableCallback(false);
}

/*****************************************************************************/
void	CLayerPlatform::GUIThingPointUpdate(bool OnlySel)
{
		GUIThingPointUpdateList(GUIThingPos.m_List,OnlySel);
}

/*****************************************************************************/
void	CLayerPlatform::GUIChanged(CCore *Core)
{
		if (CurrentThing!=-1)
		{
			sLayerThing	&ThisThing=ThingList[CurrentThing];
			ThisThing.Data.Platform.PlatformSpeed=GUIPlatform.GetVal(GUIPlatform.m_Speed);
			ThisThing.Data.Platform.PlatformTurnRate=GUIPlatform.GetVal(GUIPlatform.m_TurnRate);
			ThisThing.Data.Platform.PlatformCollisionFlag=GUIPlatform.m_Collision.GetCheck()!=0;
			ThisThing.Data.Platform.PlatformMoveType=GUIPlatform.m_MoveList.GetCurSel();
			ThisThing.Data.Platform.PlatformType=GUIPlatform.m_Type.GetCurSel();
			ThisThing.Data.Platform.PlatformParam0=GUIPlatform.GetVal(GUIPlatform.m_Param0);
			SetThingParams(ThisThing);
		}
}

/*****************************************************************************/
void	CLayerPlatform::SetThingParams(sLayerThing &Thing)
{
		switch(Thing.Data.Platform.PlatformMoveType)
		{
			case MoveTypeLinear:
				Thing.Data.WaypointCount=16;
				break;
			case MoveTypeCirular:
				Thing.Data.WaypointCount=1;
				if (Thing.XY.size()>2)
				{
					Thing.XY.resize(2);
					GUIThingPointUpdate();
				}

				break;
		}
}

/*****************************************************************************/
void	CLayerPlatform::Export(CCore *Core,CExport &Exp)
{
		CLayerThing::Export(Core,Exp);
}

/*****************************************************************************/
void	CLayerPlatform::ExportThingData(CCore *Core,CExport &Exp,sLayerThing &ThisThing,sLayerThingData &OutThing)
{
CElem	&ThisElem=ThingBank->GetElem(ThisThing.ElemID,0);

		Exp.ExportElem3d(Core,ThisElem,OutThing.Platform.PlatformTriStart,OutThing.Platform.PlatformTriCount);
		
}

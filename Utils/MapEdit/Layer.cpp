/******************/
/*** Layer Core ***/
/******************/


#include	"stdafx.h"
#include	<Vector3.h>
#include	<gl\gl.h>
#include	<gl\glu.h>

#include	"Core.h"
#include	"Layer.h"
#include	"LayerDef.h"
#include	"Utils.h"

/*****************************************************************************/
sLayerInfoTable	CLayer::InfoTable[]=
{
	//Type					SubType				Name			delete?Scale	3d		Resizable	Export
	{LAYER_TYPE_TILE,		LAYERTILE_SCRATCH,	"WorkPad",		false,	1.0f,	true,	false,		false,},
	{LAYER_TYPE_TILE,		LAYERTILE_BACK,		"Back",			true,	4.0f,	false,	false,		true,},
	{LAYER_TYPE_TILE,		LAYERTILE_MID,		"Mid",			true,	2.0f,	false,	true,		true,},
	{LAYER_TYPE_TILE,		LAYERTILE_ACTION,	"Action",		false,	1.0f,	true,	true,		true,},
	{LAYER_TYPE_COLLISION,	LAYER_SUBTYPE_NONE,	"Collision",	true,	1.0f,	false,	true,		true,},
};

int		CLayer::InfoTableSize=sizeof(InfoTable)/sizeof(sLayerInfoTable);

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
void	CLayer::SetDefaultParams()
{
int		Idx=CLayer::GetLayerIdx(GetType(),GetSubType());

		ScaleFactor=InfoTable[Idx].ScaleFactor;
		ResizeFlag=InfoTable[Idx].ResizeFlag;
		Render3dFlag=InfoTable[Idx].Render3dFlag;
		VisibleFlag=TRUE;
}

/*****************************************************************************/
int		CLayer::GetLayerIdx(int Type,int SubType)
{

		for (int i=0; i<InfoTableSize; i++)
		{
			if (InfoTable[i].Type==Type && InfoTable[i].SubType==SubType)
			{
				return(i);
			}
		}

	return(-1);
}

/*****************************************************************************/
/*****************************************************************************/

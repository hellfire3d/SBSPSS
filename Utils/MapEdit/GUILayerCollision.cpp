// GUILayerCollision.cpp : implementation file
//

#include "stdafx.h"
#include "mapedit.h"
#include "GUILayerCollision.h"

#include "MapEditDoc.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// GUILayerCollision dialog


GUILayerCollision::GUILayerCollision(CWnd* pParent /*=NULL*/)
	: CDialog(GUILayerCollision::IDD, pParent)
{
	//{{AFX_DATA_INIT(GUILayerCollision)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void GUILayerCollision::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(GUILayerCollision)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(GUILayerCollision, CDialog)
	//{{AFX_MSG_MAP(GUILayerCollision)
	ON_BN_CLICKED(IDC_LAYERCOLLISION_NORMAL, OnLayercollisionNormal)
	ON_BN_CLICKED(IDC_LAYERCOLLISION_DAMAGE, OnLayercollisionDamage)
	ON_BN_CLICKED(IDC_LAYERCOLLISION_SLIPPERY, OnLayercollisionSlippery)
	ON_BN_CLICKED(IDC_LAYERCOLLISION_ELECTRIC, OnLayercollisionElectric)
	ON_BN_CLICKED(IDC_LAYERCOLLISION_STICKY, OnLayercollisionSticky)
	ON_BN_CLICKED(IDC_LAYERCOLLISION_WATER, OnLayercollisionWater)
	ON_BN_CLICKED(IDC_LAYERCOLLISION_SOLID, OnLayercollisionSolid)
	ON_BN_CLICKED(IDC_LAYERCOLLISION_DEATH, OnLayercollisionDeath)
	ON_BN_CLICKED(IDC_LAYERCOLLISION_DESTRUCTABLE, OnLayercollisionDestructable)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// GUILayerCollision message handlers

void GUILayerCollision::OnLayercollisionNormal()		{theApp.GetCurrent()->Command(CmdMsg_SetColFlag,PC_TILE_COLLISION_NORMAL);}
void GUILayerCollision::OnLayercollisionDamage()		{theApp.GetCurrent()->Command(CmdMsg_SetColFlag,PC_TILE_COLLISION_DAMAGE);}
void GUILayerCollision::OnLayercollisionSlippery()		{theApp.GetCurrent()->Command(CmdMsg_SetColFlag,PC_TILE_COLLISION_SLIPPERY);}
void GUILayerCollision::OnLayercollisionElectric()		{theApp.GetCurrent()->Command(CmdMsg_SetColFlag,PC_TILE_COLLISION_ELECTRIC);}
void GUILayerCollision::OnLayercollisionSticky()		{theApp.GetCurrent()->Command(CmdMsg_SetColFlag,PC_TILE_COLLISION_STICKY);}
void GUILayerCollision::OnLayercollisionWater()			{theApp.GetCurrent()->Command(CmdMsg_SetColFlag,PC_TILE_COLLISION_WATER);}
void GUILayerCollision::OnLayercollisionSolid()			{theApp.GetCurrent()->Command(CmdMsg_SetColFlag,PC_TILE_COLLISION_SOLID);}
void GUILayerCollision::OnLayercollisionDeath()			{theApp.GetCurrent()->Command(CmdMsg_SetColFlag,PC_TILE_COLLISION_DEATH);}
void GUILayerCollision::OnLayercollisionDestructable()	{theApp.GetCurrent()->Command(CmdMsg_SetColFlag,PC_TILE_COLLISION_DESTRUCTABLE);}


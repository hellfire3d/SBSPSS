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
	ON_BN_CLICKED(IDC_LAYERCOLLISION_DAMAGE, OnLayercollisionDamage)
	ON_BN_CLICKED(IDC_LAYERCOLLISION_DEATH_FALL, OnLayercollisionDeathFall)
	ON_BN_CLICKED(IDC_LAYERCOLLISION_DEATH_INSTANT, OnLayercollisionDeathInstant)
	ON_BN_CLICKED(IDC_LAYERCOLLISION_DEATH_LIQUID, OnLayercollisionDeathLiquid)
	ON_BN_CLICKED(IDC_LAYERCOLLISION_DESTRUCT_FLOOR, OnLayercollisionDestructFloor)
	ON_BN_CLICKED(IDC_LAYERCOLLISION_DESTRUCT_WALL, OnLayercollisionDestructWall)
	ON_BN_CLICKED(IDC_LAYERCOLLISION_ELECTRIC, OnLayercollisionElectric)
	ON_BN_CLICKED(IDC_LAYERCOLLISION_MOVE_LEFT, OnLayercollisionMoveLeft)
	ON_BN_CLICKED(IDC_LAYERCOLLISION_MOVE_RIGHT, OnLayercollisionMoveRight)
	ON_BN_CLICKED(IDC_LAYERCOLLISION_NORMAL, OnLayercollisionNormal)
	ON_BN_CLICKED(IDC_LAYERCOLLISION_SLIPPERY, OnLayercollisionSlippery)
	ON_BN_CLICKED(IDC_LAYERCOLLISION_SOLID, OnLayercollisionSolid)
	ON_BN_CLICKED(IDC_LAYERCOLLISION_SOAK_UP, OnLayercollisionSoakUp)
	ON_BN_CLICKED(IDC_LAYERCOLLISION_SB_NOMOVE, OnLayercollisionSbNomove)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// GUILayerCollision message handlers

void GUILayerCollision::OnLayercollisionNormal()		{theApp.GetCurrent()->Command(CmdMsg_SetColFlag,PC_TILE_COLLISION_NORMAL);}
void GUILayerCollision::OnLayercollisionSolid()			{theApp.GetCurrent()->Command(CmdMsg_SetColFlag,PC_TILE_COLLISION_SOLID);}
void GUILayerCollision::OnLayercollisionSlippery()		{theApp.GetCurrent()->Command(CmdMsg_SetColFlag,PC_TILE_COLLISION_SLIPPERY);}
void GUILayerCollision::OnLayercollisionMoveLeft()		{theApp.GetCurrent()->Command(CmdMsg_SetColFlag,PC_TILE_COLLISION_MOVE_LEFT);}
void GUILayerCollision::OnLayercollisionMoveRight()		{theApp.GetCurrent()->Command(CmdMsg_SetColFlag,PC_TILE_COLLISION_MOVE_RIGHT);}

void GUILayerCollision::OnLayercollisionDamage()		{theApp.GetCurrent()->Command(CmdMsg_SetColFlag,PC_TILE_COLLISION_DAMAGE);}
void GUILayerCollision::OnLayercollisionElectric()		{theApp.GetCurrent()->Command(CmdMsg_SetColFlag,PC_TILE_COLLISION_ELECTRIC);}
void GUILayerCollision::OnLayercollisionDeathLiquid()	{theApp.GetCurrent()->Command(CmdMsg_SetColFlag,PC_TILE_COLLISION_DEATH_LIQUID);}
void GUILayerCollision::OnLayercollisionDeathFall()		{theApp.GetCurrent()->Command(CmdMsg_SetColFlag,PC_TILE_COLLISION_DEATH_FALL);}
void GUILayerCollision::OnLayercollisionDeathInstant()	{theApp.GetCurrent()->Command(CmdMsg_SetColFlag,PC_TILE_COLLISION_DEATH_INSTANT);}

void GUILayerCollision::OnLayercollisionDestructWall()	{theApp.GetCurrent()->Command(CmdMsg_SetColFlag,PC_TILE_COLLISION_DESTRUCTABLE_WALL);}
void GUILayerCollision::OnLayercollisionDestructFloor() {theApp.GetCurrent()->Command(CmdMsg_SetColFlag,PC_TILE_COLLISION_DESTRUCTABLE_FLOOR);}
void GUILayerCollision::OnLayercollisionSoakUp()		{theApp.GetCurrent()->Command(CmdMsg_SetColFlag,PC_TILE_COLLISION_SOAKUP);}

void GUILayerCollision::OnLayercollisionSbNomove()		{theApp.GetCurrent()->Command(CmdMsg_SetColFlag,PC_TILE_COLLISION_SB_NOMOVE);}


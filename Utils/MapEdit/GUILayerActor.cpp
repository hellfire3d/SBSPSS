// GUILayerActor.cpp : implementation file
//

#include "stdafx.h"
#include "mapedit.h"
#include "GUILayerActor.h"

#include "MapEditDoc.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGUILayerActor dialog


CGUILayerActor::CGUILayerActor(CWnd* pParent /*=NULL*/)
	: CDialog(CGUILayerActor::IDD, pParent)
{
	DisableCallback(true);
	//{{AFX_DATA_INIT(CGUILayerActor)
	//}}AFX_DATA_INIT
	DisableCallback(false);
}


void CGUILayerActor::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGUILayerActor)
	DDX_Control(pDX, IDC_ACTOR_COLLISION, m_Collision);
	DDX_Control(pDX, IDC_ACTOR_PLAYER, m_Player);
	DDX_Control(pDX, IDC_ACTOR_TURNRATE, m_TurnRate);
	DDX_Control(pDX, IDC_ACTOR_SPEED, m_Speed);
	DDX_Control(pDX, IDC_ACTOR_ATTACK, m_Attack);
	DDX_Control(pDX, IDC_ACTOR_HEALTH, m_Health);
	DDX_Control(pDX, IDC_LEVEL_ACTOR_LIST, m_LevelList);
	DDX_Control(pDX, IDC_ACTOR_POS_LIST, m_PosList);
	DDX_Control(pDX, IDC_ACTOR_LIST, m_List);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGUILayerActor, CDialog)
	//{{AFX_MSG_MAP(CGUILayerActor)
	ON_BN_CLICKED(IDC_ACTOR_DELETE, OnActorDelete)
	ON_BN_CLICKED(IDC_ACTOR_POS_UP, OnActorPosUp)
	ON_BN_CLICKED(IDC_ACTOR_POS_DOWN, OnActorPosDown)
	ON_BN_CLICKED(IDC_ACTOR_POS_DELETE, OnActorPosDelete)
	ON_EN_CHANGE(IDC_ACTOR_HEALTH, OnChangeParam)
	ON_CBN_SELCHANGE(IDC_ACTOR_LIST, OnSelchangeActorList)
	ON_CBN_SELCHANGE(IDC_LEVEL_ACTOR_LIST, OnSelchangeLevelActorList)
	ON_LBN_SELCHANGE(IDC_ACTOR_POS_LIST, OnSelchangeActorPosList)
	ON_EN_CHANGE(IDC_ACTOR_ATTACK, OnChangeParam)
	ON_BN_CLICKED(IDC_ACTOR_COLLISION, OnChangeParam)
	ON_EN_CHANGE(IDC_ACTOR_SPEED, OnChangeParam)
	ON_EN_CHANGE(IDC_ACTOR_TURNRATE, OnChangeParam)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
void	CGUILayerActor::SetVal(CEdit &Dlg,int Val)
{
CString	Str;
		if (!Dlg) return;
		DisableCallback(true);
		Str.Format("%i",Val); 
		Dlg.SetWindowText(Str); 
		DisableCallback(false);
}

/////////////////////////////////////////////////////////////////////////////
int		CGUILayerActor::GetVal(CEdit &Dlg)
{
CString	Str;
int		Val=0;
		if (!Dlg) return(0);
		Dlg.GetWindowText(Str); 
		if (Str.GetLength()) 
			Val=atoi(Str);
		return(Val);
}

/////////////////////////////////////////////////////////////////////////////
// CGUILayerActor message handlers

void CGUILayerActor::OnSelchangeActorList()				{theApp.GetCurrent()->Command(CmdMsg_ThingListSelect,m_List.GetCurSel());}
void CGUILayerActor::OnActorDelete()					{theApp.GetCurrent()->Command(CmdMsg_ThingListDelete,m_List.GetCurSel());}

void CGUILayerActor::OnSelchangeLevelActorList()		{theApp.GetCurrent()->Command(CmdMsg_ThingLevelSelect,m_LevelList.GetCurSel());}

void CGUILayerActor::OnSelchangeActorPosList()			{theApp.GetCurrent()->Command(CmdMsg_ThingPosSelect,m_PosList.GetCurSel());}
void CGUILayerActor::OnActorPosUp()						{theApp.GetCurrent()->Command(CmdMsg_ThingPosUp,m_PosList.GetCurSel());}
void CGUILayerActor::OnActorPosDown()					{theApp.GetCurrent()->Command(CmdMsg_ThingPosDown,m_PosList.GetCurSel());}
void CGUILayerActor::OnActorPosDelete()					{theApp.GetCurrent()->Command(CmdMsg_ThingPosDelete,m_PosList.GetCurSel());}

void CGUILayerActor::OnChangeParam()					{if (!CallbackFlag) theApp.GetCurrent()->GUIChanged();}

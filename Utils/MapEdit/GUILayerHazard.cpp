// GUILayerHazard.cpp : implementation file
//

#include "stdafx.h"
#include "mapedit.h"
#include "GUILayerHazard.h"

#include "MapEditDoc.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGUILayerHazard dialog


CGUILayerHazard::CGUILayerHazard(CWnd* pParent /*=NULL*/)
	: CDialog(CGUILayerHazard::IDD, pParent)
{
	DisableCallback(true);
	//{{AFX_DATA_INIT(CGUILayerHazard)
	//}}AFX_DATA_INIT
	DisableCallback(false);
}


void CGUILayerHazard::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGUILayerHazard)
	DDX_Control(pDX, IDC_HAZARD_TURNRATE_SPIN, m_TurnRateSpin);
	DDX_Control(pDX, IDC_HAZARD_SPEED_SPIN, m_SpeedSpin);
	DDX_Control(pDX, IDC_HAZARD_HEALTH_SPIN, m_HealthSpin);
	DDX_Control(pDX, IDC_HAZARD_ATTACK_SPIN, m_AttackSpin);
	DDX_Control(pDX, IDC_HAZARD_RESPAWN_SPIN, m_RespawnSpin);
	DDX_Control(pDX, IDC_HAZARD_COLLISION, m_Collision);
	DDX_Control(pDX, IDC_HAZARD_TURNRATE, m_TurnRate);
	DDX_Control(pDX, IDC_HAZARD_SPEED, m_Speed);
	DDX_Control(pDX, IDC_HAZARD_ATTACK, m_Attack);
	DDX_Control(pDX, IDC_HAZARD_HEALTH, m_Health);
	DDX_Control(pDX, IDC_HAZARD_RESPAWN, m_Respawn);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGUILayerHazard, CDialog)
	//{{AFX_MSG_MAP(CGUILayerHazard)
	ON_EN_CHANGE(IDC_HAZARD_HEALTH, OnChangeParam)
	ON_EN_CHANGE(IDC_HAZARD_ATTACK, OnChangeParam)
	ON_BN_CLICKED(IDC_HAZARD_COLLISION, OnChangeParam)
	ON_EN_CHANGE(IDC_HAZARD_SPEED, OnChangeParam)
	ON_EN_CHANGE(IDC_HAZARD_TURNRATE, OnChangeParam)
	ON_EN_CHANGE(IDC_HAZARD_RESPAWN, OnChangeParam)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
void	CGUILayerHazard::SetVal(CEdit &Dlg,int Val)
{
CString	Str;
		if (!Dlg) return;
		DisableCallback(true);
		Str.Format("%i",Val); 
		Dlg.SetWindowText(Str); 
		DisableCallback(false);
}

/////////////////////////////////////////////////////////////////////////////
int		CGUILayerHazard::GetVal(CEdit &Dlg)
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
// CGUILayerHazard message handlers

void CGUILayerHazard::OnChangeParam()					{if (!CallbackFlag) theApp.GetCurrent()->GUIChanged();}

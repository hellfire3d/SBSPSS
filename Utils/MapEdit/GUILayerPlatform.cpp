// GUILayerPlatform.cpp : implementation file
//

#include "stdafx.h"
#include "mapedit.h"
#include "GUILayerPlatform.h"

#include "MapEditDoc.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGUILayerPlatform dialog


CGUILayerPlatform::CGUILayerPlatform(CWnd* pParent /*=NULL*/)
	: CDialog(CGUILayerPlatform::IDD, pParent)
{
	DisableCallback(true);
	//{{AFX_DATA_INIT(CGUILayerPlatform)
	//}}AFX_DATA_INIT
	DisableCallback(false);
}


void CGUILayerPlatform::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGUILayerPlatform)
	DDX_Control(pDX, IDC_PLATFORM_PARAM_SPIN, m_Param0Spin);
	DDX_Control(pDX, IDC_PLATFORM_PARAM0_TEXT, m_Param0Txt);
	DDX_Control(pDX, IDC_PLATFORM_PARAM0, m_Param0);
	DDX_Control(pDX, IDC_PLATFORM_TURNRATE_SPIN, m_TurnRateSpin);
	DDX_Control(pDX, IDC_PLATFORM_SPEED_SPIN, m_SpeedSpin);
	DDX_Control(pDX, IDC_PLATFORM_TYPE, m_Type);
	DDX_Control(pDX, IDC_PLATFORM_MOVE_TYPE, m_MoveList);
	DDX_Control(pDX, IDC_PLATFORM_COLLISION, m_Collision);
	DDX_Control(pDX, IDC_PLATFORM_TURNRATE, m_TurnRate);
	DDX_Control(pDX, IDC_PLATFORM_SPEED, m_Speed);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGUILayerPlatform, CDialog)
	//{{AFX_MSG_MAP(CGUILayerPlatform)
	ON_EN_CHANGE(IDC_PLATFORM_SPEED, OnChangeParam)
	ON_BN_CLICKED(IDC_PLATFORM_COLLISION, OnChangeParam)
	ON_EN_CHANGE(IDC_PLATFORM_TURNRATE, OnChangeParam)
	ON_CBN_SELCHANGE(IDC_PLATFORM_TYPE, OnChangeParam)
	ON_CBN_SELCHANGE(IDC_PLATFORM_MOVE_TYPE, OnChangeParam)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
void	CGUILayerPlatform::SetVal(CEdit &Dlg,int Val)
{
CString	Str;
		if (!Dlg) return;
		DisableCallback(true);
		Str.Format("%i",Val); 
		Dlg.SetWindowText(Str); 
		DisableCallback(false);
}

/////////////////////////////////////////////////////////////////////////////
int		CGUILayerPlatform::GetVal(CEdit &Dlg)
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
// CGUILayerPlatform message handlers

void CGUILayerPlatform::OnChangeParam()				{if (!CallbackFlag) theApp.GetCurrent()->GUIChanged();}

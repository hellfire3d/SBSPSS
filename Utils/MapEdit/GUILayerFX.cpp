// GUILayerFX.cpp : implementation file
//

#include "stdafx.h"
#include "mapedit.h"
#include "GUILayerFX.h"

#include "MapEditDoc.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGUILayerFX dialog


CGUILayerFX::CGUILayerFX(CWnd* pParent /*=NULL*/)
	: CDialog(CGUILayerFX::IDD, pParent)
{
	DisableCallback(true);
	//{{AFX_DATA_INIT(CGUILayerFX)
	//}}AFX_DATA_INIT
	DisableCallback(false);
}


void CGUILayerFX::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGUILayerFX)
	DDX_Control(pDX, IDC_FX_HEIGHT_SPIN, m_HeightSpin);
	DDX_Control(pDX, IDC_FX_SPEED_SPIN, m_SpeedSpin);
	DDX_Control(pDX, IDC_FX_WIDTH_SPIN, m_WidthSpin);
	DDX_Control(pDX, IDC_FX_WIDTH, m_Width);
	DDX_Control(pDX, IDC_FX_HEIGHT, m_Height);
	DDX_Control(pDX, IDC_FX_SPEED, m_Speed);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGUILayerFX, CDialog)
	//{{AFX_MSG_MAP(CGUILayerFX)
	ON_EN_CHANGE(IDC_FX_SPEED, OnParamChange)
	ON_EN_CHANGE(IDC_FX_HEIGHT, OnParamChange)
	ON_EN_CHANGE(IDC_FX_WIDTH, OnParamChange)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
void	CGUILayerFX::SetVal(CEdit &Dlg,int Val)
{
CString	Str;
		if (!Dlg) return;
		DisableCallback(true);
		Str.Format("%i",Val); 
		Dlg.SetWindowText(Str); 
		DisableCallback(false);
}

/////////////////////////////////////////////////////////////////////////////
int		CGUILayerFX::GetVal(CEdit &Dlg)
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
// CGUILayerFX message handlers

void CGUILayerFX::OnParamChange()				{if (!CallbackFlag) theApp.GetCurrent()->GUIChanged();}

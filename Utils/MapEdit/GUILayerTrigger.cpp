// GUILayerTrigger.cpp : implementation file
//

#include "stdafx.h"
#include "mapedit.h"
#include "GUILayerTrigger.h"

#include "MapEditDoc.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGUILayerTrigger dialog


CGUILayerTrigger::CGUILayerTrigger(CWnd* pParent /*=NULL*/)
	: CDialog(CGUILayerTrigger::IDD, pParent)
{
	DisableCallback(true);
	//{{AFX_DATA_INIT(CGUILayerTrigger)
	//}}AFX_DATA_INIT
	DisableCallback(false);
}


void CGUILayerTrigger::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGUILayerTrigger)
	DDX_Control(pDX, IDC_TRIGGER_WIDTH, m_Width);
	DDX_Control(pDX, IDC_TRIGGER_HEIGHT, m_Height);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGUILayerTrigger, CDialog)
	//{{AFX_MSG_MAP(CGUILayerTrigger)
	ON_EN_CHANGE(IDC_TRIGGER_HEIGHT, OnParamChange)
	ON_EN_CHANGE(IDC_TRIGGER_WIDTH, OnParamChange)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
void	CGUILayerTrigger::SetVal(CEdit &Dlg,int Val)
{
CString	Str;
		if (!Dlg) return;
		DisableCallback(true);
		Str.Format("%i",Val); 
		Dlg.SetWindowText(Str); 
		DisableCallback(false);
}

/////////////////////////////////////////////////////////////////////////////
int		CGUILayerTrigger::GetVal(CEdit &Dlg)
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
// CGUILayerTrigger message handlers

void CGUILayerTrigger::OnParamChange()				{if (!CallbackFlag) theApp.GetCurrent()->GUIChanged();}

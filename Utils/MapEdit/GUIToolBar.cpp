// GUIToolBar.cpp : implementation file
//

#include "stdafx.h"
#include "mapedit.h"
#include "GUIToolBar.h"

#include	"MapEditDoc.h"
#include	"MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// GUIToolBar dialog


CGUIToolBar::CGUIToolBar(CWnd* pParent /*=NULL*/)
	: CDialog(CGUIToolBar::IDD, pParent)
{
	//{{AFX_DATA_INIT(GUIToolBar)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CGUIToolBar::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(GUIToolBar)
	DDX_Control(pDX, IDD_LAYERTILE_BTN_SELECT, m_Select);
	DDX_Control(pDX, IDD_LAYERTILE_BTN_PAINT, m_Paint);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGUIToolBar, CDialog)
	//{{AFX_MSG_MAP(GUIToolBar)
	ON_BN_CLICKED(IDD_LAYERTILE_BTN_PAINT, OnLayertileBtnPaint)
	ON_BN_CLICKED(IDD_LAYERTILE_BTN_SELECT, OnLayertileBtnSelect)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// GUIToolBar message handlers

BOOL CGUIToolBar::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_Paint.SetIcon( (HICON) LoadImage (AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_PAINT),IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR) ); 
	m_Select.SetIcon( (HICON) LoadImage (AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_SELECT),IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR) ); 
	
	return TRUE;  
}
/////////////////////////////////////////////////////////////////////////////
void	CGUIToolBar::ResetButtons()
{
		m_Paint.SetState(FALSE);
		m_Select.SetState(FALSE);
}

/////////////////////////////////////////////////////////////////////////////
void	CGUIToolBar::SetButtonState(GUIToolBarButton ID,BOOL State)
{
		switch(ID)
		{
		case PAINT:
			m_Paint.SetState(State);
			break;
		case SELECT:
			m_Select.SetState(State);
			break;
		}
}

/////////////////////////////////////////////////////////////////////////////
BOOL	CGUIToolBar::GetButtonState(GUIToolBarButton ID)
{
		switch(ID)
		{
		case PAINT:		return(m_Paint.GetState());
		case SELECT:	return(m_Select.GetState());
		}
		return(FALSE);
}

/////////////////////////////////////////////////////////////////////////////
void	CGUIToolBar::OnLayertileBtnPaint()								{theApp.GetCurrent()->Command(CmdMsg_SetMode,CLayerTile::MouseModePaint);}
void	CGUIToolBar::OnLayertileBtnSelect()								{theApp.GetCurrent()->Command(CmdMsg_SetMode,CLayerTile::MouseModeSelect);}

// GuiLayerThingPos.cpp : implementation file
//

#include "stdafx.h"
#include "mapedit.h"
#include "GuiLayerThingPos.h"

#include "MapEditDoc.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGuiLayerThingPos dialog


CGuiLayerThingPos::CGuiLayerThingPos(CWnd* pParent /*=NULL*/)
	: CDialog(CGuiLayerThingPos::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGuiLayerThingPos)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CGuiLayerThingPos::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGuiLayerThingPos)
	DDX_Control(pDX, IDC_THING_POS_LIST, m_List);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGuiLayerThingPos, CDialog)
	//{{AFX_MSG_MAP(CGuiLayerThingPos)
	ON_LBN_SELCHANGE(IDC_THING_POS_LIST, OnSelchangeThingPosList)
	ON_BN_CLICKED(IDC_THING_POS_UP, OnThingPosUp)
	ON_BN_CLICKED(IDC_THING_POS_DOWN, OnThingPosDown)
	ON_BN_CLICKED(IDC_THING_POS_DELETE, OnThingPosDelete)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGuiLayerThingPos message handlers

void CGuiLayerThingPos::OnSelchangeThingPosList()		{theApp.GetCurrent()->Command(CmdMsg_ThingPosSelect,m_List.GetCurSel());}
void CGuiLayerThingPos::OnThingPosUp()					{theApp.GetCurrent()->Command(CmdMsg_ThingPosUp,m_List.GetCurSel());}
void CGuiLayerThingPos::OnThingPosDown()				{theApp.GetCurrent()->Command(CmdMsg_ThingPosDown,m_List.GetCurSel());}
void CGuiLayerThingPos::OnThingPosDelete()				{theApp.GetCurrent()->Command(CmdMsg_ThingPosDelete,m_List.GetCurSel());}

// GUILayerThing.cpp : implementation file
//

#include "stdafx.h"
#include "mapedit.h"
#include "GUILayerThing.h"

#include "MapEditDoc.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// CGUILayerThing dialog


CGUILayerThing::CGUILayerThing(CWnd* pParent /*=NULL*/)
	: CDialog(CGUILayerThing::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGUILayerThing)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CGUILayerThing::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGUILayerThing)
	DDX_Control(pDX, IDC_THING_LIST, m_List);
	DDX_Control(pDX, IDC_DEF_THING_LIST, m_DefList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGUILayerThing, CDialog)
	//{{AFX_MSG_MAP(CGUILayerThing)
	ON_BN_CLICKED(IDC_THING_DELETE, OnThingDelete)
	ON_BN_CLICKED(IDC_THING_GOTO, OnThingGoto)
	ON_CBN_SELCHANGE(IDC_DEF_THING_LIST, OnSelchangeDefThingList)
	ON_CBN_SELCHANGE(IDC_THING_LIST, OnSelchangeThingList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGUILayerThing message handlers

void CGUILayerThing::OnThingDelete()					{theApp.GetCurrent()->Command(CmdMsg_ThingListDelete,m_List.GetCurSel());}
void CGUILayerThing::OnThingGoto()						{theApp.GetCurrent()->Command(CmdMsg_ThingListGoto,m_List.GetCurSel());}
void CGUILayerThing::OnSelchangeDefThingList()			{theApp.GetCurrent()->Command(CmdMsg_ThingListSelect,m_DefList.GetCurSel());}
void CGUILayerThing::OnSelchangeThingList()				{theApp.GetCurrent()->Command(CmdMsg_ThingLevelSelect,m_List.GetCurSel());}



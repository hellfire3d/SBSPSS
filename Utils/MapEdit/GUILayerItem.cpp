// GUILayerItem.cpp : implementation file
//

#include "stdafx.h"
#include "mapedit.h"
#include "GUILayerItem.h"

#include "MapEditDoc.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGUILayerItem dialog


CGUILayerItem::CGUILayerItem(CWnd* pParent /*=NULL*/)
	: CDialog(CGUILayerItem::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGUILayerItem)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CGUILayerItem::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGUILayerItem)
	DDX_Control(pDX, IDC_LEVEL_ITEM_LIST, m_LevelList);
	DDX_Control(pDX, IDC_ITEM_LIST, m_List);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGUILayerItem, CDialog)
	//{{AFX_MSG_MAP(CGUILayerItem)
	ON_BN_CLICKED(IDC_ITEM_DELETE, OnItemDelete)
	ON_CBN_SELCHANGE(IDC_LEVEL_ITEM_LIST, OnSelchangeLevelItemList)
	ON_CBN_SELCHANGE(IDC_ITEM_LIST, OnSelchangeItemList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGUILayerItem message handlers


void CGUILayerItem::OnSelchangeItemList()				{theApp.GetCurrent()->Command(CmdMsg_ThingListSelect,m_List.GetCurSel());}
void CGUILayerItem::OnSelchangeLevelItemList()			{theApp.GetCurrent()->Command(CmdMsg_ThingLevelSelect,m_LevelList.GetCurSel());}
void CGUILayerItem::OnItemDelete()						{theApp.GetCurrent()->Command(CmdMsg_ThingListDelete,m_List.GetCurSel());}


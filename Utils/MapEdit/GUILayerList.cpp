// GUILayerList.cpp : implementation file
//

#include "stdafx.h"
#include "mapedit.h"
#include "GUILayerList.h"

#include "MapEditDoc.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGUILayerList dialog


CGUILayerList::CGUILayerList(CWnd* pParent /*=NULL*/)
	: CDialog(CGUILayerList::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGUILayerList)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CGUILayerList::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGUILayerList)
	DDX_Control(pDX, IDC_LAYERLIST_LIST, m_List);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGUILayerList, CDialog)
	//{{AFX_MSG_MAP(CGUILayerList)
	ON_BN_CLICKED(IDC_LAYERLIST_ADD, OnLayerlistAdd)
	ON_BN_CLICKED(IDC_LAYERLIST_DELETE, OnLayerlistDelete)
	ON_LBN_SELCHANGE(IDC_LAYERLIST_LIST, OnSelchangeLayerlistList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGUILayerList message handlers

void CGUILayerList::OnLayerlistAdd()				{theApp.GetCurrent()->Command(CmdMsg_AddLayer,m_List.GetCurSel());}
void CGUILayerList::OnLayerlistDelete()				{theApp.GetCurrent()->Command(CmdMsg_DeleteLayer,m_List.GetCurSel());}
void CGUILayerList::OnSelchangeLayerlistList()		{theApp.GetCurrent()->Command(CmdMsg_SetLayer,m_List.GetCurSel());}

// GUIElemList.cpp : implementation file
//

#include "stdafx.h"
#include "mapedit.h"
#include "GUIElemList.h"

#include "MapEditDoc.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGUIElemList dialog


CGUIElemList::CGUIElemList(CWnd* pParent /*=NULL*/)
	: CDialog(CGUIElemList::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGUIElemList)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CGUIElemList::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGUIElemList)
	DDX_Control(pDX, IDD_ELEM_LIST, m_List);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGUIElemList, CDialog)
	//{{AFX_MSG_MAP(CGUIElemList)
	ON_CBN_CLOSEUP(IDD_ELEM_LIST, OnCloseupElemList)
	ON_CBN_SELCHANGE(IDD_ELEM_LIST, OnSelchangeElemList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGUIElemList message handlers

void CGUIElemList::OnCloseupElemList()							{theApp.GetCurrent()->Command(CmdMsg_SubViewSet);}
void CGUIElemList::OnSelchangeElemList()						{theApp.GetCurrent()->Command(CmdMsg_SubViewSet);}

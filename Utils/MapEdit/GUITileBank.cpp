// GUITileBank.cpp : implementation file
//

#include "stdafx.h"
#include "mapedit.h"
#include "GUITileBank.h"

#include "MapEditDoc.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGUITileBank dialog


CGUITileBank::CGUITileBank(CWnd* pParent /*=NULL*/)
	: CDialog(CGUITileBank::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGUITileBank)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CGUITileBank::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGUITileBank)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGUITileBank, CDialog)
	//{{AFX_MSG_MAP(CGUITileBank)
	ON_BN_CLICKED(IDD_TILEBANK_LOAD, OnTilebankLoad)
	ON_BN_CLICKED(IDD_TILEBANK_DELETE, OnTilebankDelete)
	ON_BN_CLICKED(IDD_TILEBANK_UPDATE, OnTilebankUpdate)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGUITileBank message handlers
void CGUITileBank::OnTilebankLoad() 								{theApp.GetCurrent()->Command(CmdMsg_SubViewLoad);}
void CGUITileBank::OnTilebankDelete() 								{theApp.GetCurrent()->Command(CmdMsg_SubViewDelete);}
void CGUITileBank::OnTilebankUpdate() 								{theApp.GetCurrent()->Command(CmdMsg_SubViewUpdate);}

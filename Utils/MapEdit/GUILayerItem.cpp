// GUILayerItem.cpp : implementation file
//

#include "stdafx.h"
#include "mapedit.h"
#include "GUILayerItem.h"

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
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGUILayerItem, CDialog)
	//{{AFX_MSG_MAP(CGUILayerItem)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGUILayerItem message handlers

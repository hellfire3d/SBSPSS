// GUILayerTrigger.cpp : implementation file
//

#include "stdafx.h"
#include "mapedit.h"
#include "GUILayerTrigger.h"

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
	//{{AFX_DATA_INIT(CGUILayerTrigger)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CGUILayerTrigger::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGUILayerTrigger)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGUILayerTrigger, CDialog)
	//{{AFX_MSG_MAP(CGUILayerTrigger)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGUILayerTrigger message handlers

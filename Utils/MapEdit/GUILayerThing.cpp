// GUILayerThing.cpp : implementation file
//

#include "stdafx.h"
#include "mapedit.h"
#include "GUILayerThing.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

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
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGUILayerThing, CDialog)
	//{{AFX_MSG_MAP(CGUILayerThing)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGUILayerThing message handlers

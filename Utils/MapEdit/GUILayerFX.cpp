// GUILayerFX.cpp : implementation file
//

#include "stdafx.h"
#include "mapedit.h"
#include "GUILayerFX.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGUILayerFX dialog


CGUILayerFX::CGUILayerFX(CWnd* pParent /*=NULL*/)
	: CDialog(CGUILayerFX::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGUILayerFX)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CGUILayerFX::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGUILayerFX)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGUILayerFX, CDialog)
	//{{AFX_MSG_MAP(CGUILayerFX)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGUILayerFX message handlers

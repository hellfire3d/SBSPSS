// GUILayerRGB.cpp : implementation file
//

#include "stdafx.h"
#include "mapedit.h"
#include "GUILayerRGB.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGUILayerRGB dialog


CGUILayerRGB::CGUILayerRGB(CWnd* pParent /*=NULL*/)
	: CDialog(CGUILayerRGB::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGUILayerRGB)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CGUILayerRGB::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGUILayerRGB)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGUILayerRGB, CDialog)
	//{{AFX_MSG_MAP(CGUILayerRGB)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGUILayerRGB message handlers

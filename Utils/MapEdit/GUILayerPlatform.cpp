// GUILayerPlatform.cpp : implementation file
//

#include "stdafx.h"
#include "mapedit.h"
#include "GUILayerPlatform.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGUILayerPlatform dialog


CGUILayerPlatform::CGUILayerPlatform(CWnd* pParent /*=NULL*/)
	: CDialog(CGUILayerPlatform::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGUILayerPlatform)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CGUILayerPlatform::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGUILayerPlatform)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGUILayerPlatform, CDialog)
	//{{AFX_MSG_MAP(CGUILayerPlatform)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGUILayerPlatform message handlers

// GuiLayerThingPos.cpp : implementation file
//

#include "stdafx.h"
#include "mapedit.h"
#include "GuiLayerThingPos.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGuiLayerThingPos dialog


CGuiLayerThingPos::CGuiLayerThingPos(CWnd* pParent /*=NULL*/)
	: CDialog(CGuiLayerThingPos::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGuiLayerThingPos)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CGuiLayerThingPos::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGuiLayerThingPos)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGuiLayerThingPos, CDialog)
	//{{AFX_MSG_MAP(CGuiLayerThingPos)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGuiLayerThingPos message handlers

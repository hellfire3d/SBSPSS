// LayerTileCollision.cpp : implementation file
//

#include "stdafx.h"
#include "mapedit.h"
#include "LayerTileCollision.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLayerTileCollision dialog


CLayerTileCollision::CLayerTileCollision(CWnd* pParent /*=NULL*/)
	: CDialog(CLayerTileCollision::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLayerTileCollision)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CLayerTileCollision::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLayerTileCollision)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLayerTileCollision, CDialog)
	//{{AFX_MSG_MAP(CLayerTileCollision)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLayerTileCollision message handlers

// GUINewMap.cpp : implementation file
//

#include "stdafx.h"
#include "mapedit.h"
#include "GUINewMap.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGUINewMap dialog


CGUINewMap::CGUINewMap(CWnd* pParent /*=NULL*/)
	: CDialog(CGUINewMap::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGUINewMap)
	m_Height = 0;
	m_Width = 0;
	//}}AFX_DATA_INIT
}


void CGUINewMap::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGUINewMap)
	DDX_Text(pDX, IDC_MAPSIZE_HEIGHT, m_Height);
	DDV_MinMaxUInt(pDX, m_Height, 24, 32000);
	DDX_Text(pDX, IDC_MAPSIZE_WIDTH, m_Width);
	DDV_MinMaxUInt(pDX, m_Width, 34, 32000);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGUINewMap, CDialog)
	//{{AFX_MSG_MAP(CGUINewMap)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGUINewMap message handlers

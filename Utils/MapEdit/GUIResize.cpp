// GUIResize.cpp : implementation file
//

#include "stdafx.h"
#include "mapedit.h"
#include "GUIResize.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGUIResize dialog


CGUIResize::CGUIResize(CWnd* pParent /*=NULL*/)
	: CDialog(CGUIResize::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGUIResize)
	m_Height = 0;
	m_Width = 0;
	//}}AFX_DATA_INIT
}


void CGUIResize::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGUIResize)
	DDX_Text(pDX, IDC_MAPSIZE_HEIGHT, m_Height);
	DDV_MinMaxUInt(pDX, m_Height, 24, 32000);
	DDX_Text(pDX, IDC_MAPSIZE_WIDTH, m_Width);
	DDV_MinMaxUInt(pDX, m_Width, 34, 32000);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGUIResize, CDialog)
	//{{AFX_MSG_MAP(CGUIResize)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGUIResize message handlers

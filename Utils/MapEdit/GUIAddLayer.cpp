// AddLayerDlg.cpp : implementation file
//

#include	"stdafx.h"
#include	"mapedit.h"
#include	"GUIAddLayer.h"
#include	<vector>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGUIAddLayer dialog


CGUIAddLayer::CGUIAddLayer(CWnd* pParent /*=NULL*/)
	: CDialog(CGUIAddLayer::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGUIAddLayer)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CGUIAddLayer::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGUIAddLayer)
	DDX_Control(pDX, IDC_ADDLAYER_LIST, m_List);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGUIAddLayer, CDialog)
	//{{AFX_MSG_MAP(CGUIAddLayer)
	ON_LBN_SELCHANGE(IDC_ADDLAYER_LIST, OnSelchangeAddlayerList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGUIAddLayer message handlers

BOOL CGUIAddLayer::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_List.ResetContent();
	for (int i=0; i<StrList.size(); i++)
	{
		m_List.AddString(StrList[i]);	
	}
	m_List.SetCurSel(0);
	
	return TRUE;	
	              
}

void CGUIAddLayer::OnSelchangeAddlayerList() 
{
		*Sel=m_List.GetCurSel();
}

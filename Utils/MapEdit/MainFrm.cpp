// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "MapEdit.h"

#include	"MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame
static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CURSORXY,
};

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
		

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	
	// TODO: add member initialization code here
	
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1) return -1;
	EnableDocking(CBRS_ALIGN_ANY);

// Status Bar
	if (!m_wndStatusBar.Create(this) || !m_wndStatusBar.SetIndicators(indicators,sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

// ParamBar
	if (!m_wndParamBar.Create(this,IDD_PARAMBAR,(CBRS_LEFT | CBRS_SIZE_DYNAMIC),IDD_PARAMBAR))
	{
		TRACE0("Failed to create ParamBar\n");
		return -1;
	}
	
	m_wndParamBar.EnableDocking( CBRS_ALIGN_ANY);
	m_wndParamBar.SetWindowText("Param");
	DockControlBar(&m_wndParamBar);

//	Test1.Create(IDD_TEST1,NULL);
	m_wndParamBar.AddDialog(LayerList,IDD_LAYER_LIST_DIALOG,TRUE);
	m_wndParamBar.AddDialog(Test1,IDD_TEST1,TRUE);
	m_wndParamBar.AddDialog(Test2,IDD_TEST2,TRUE);


// ToolBar (plus extra code for extra controls!!)
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) || !m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
	m_wndToolBar.SetWindowText("ToolBar");
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);
	
#define SNAP_WIDTH 80 //the width of the combo box

    //set up the ComboBox control as a snap mode select box
    //
    //First get the index of the placeholder's position in the toolbar
int	index = 0;
RECT	rect;
	while (m_wndToolBar.GetItemID(index)!=ID_TOOLBAR_COMBO) index++;

	//next convert that button to a seperator and get its position
	m_wndToolBar.SetButtonInfo(index, ID_TOOLBAR_COMBO, TBBS_SEPARATOR, SNAP_WIDTH);
	m_wndToolBar.GetItemRect(index, &rect);

	//expand the rectangle to allow the combo box room to drop down
	rect.top+=2;
	rect.bottom += 200;

	// then .Create the combo box and show it

	if (!m_wndToolBar.m_Combo.Create(WS_CHILD|WS_VISIBLE | CBS_AUTOHSCROLL | CBS_DROPDOWNLIST | CBS_HASSTRINGS ,rect, &m_wndToolBar, IDC_TOOLBAR_COMBO))
	{
	   TRACE0("Failed to create combo-box\n");
	   return FALSE;
	}
	m_wndToolBar.m_Combo.SetFont(m_wndToolBar.GetFont());	// Set to nice font (not playskool font!!)
	m_wndToolBar.m_Combo.ShowWindow(SW_SHOW);

	//fill the combo box
	m_wndToolBar.m_Combo.AddString("SNAP OFF");
	m_wndToolBar.m_Combo.AddString("SNAP GRID");
	m_wndToolBar.m_Combo.AddString("SNAP RASTER");
	m_wndToolBar.m_Combo.AddString("SNAP VERTEX");
	m_wndToolBar.m_Combo.AddString("SNAP LINE");
	m_wndToolBar.m_Combo.SetCurSel(0);
	
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers





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
	if (!StatusBar.Create(this) || !StatusBar.SetIndicators(indicators,sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

// ParamBar
	if (!ParamBar.Create(this,IDD_PARAMBAR,(CBRS_LEFT | CBRS_SIZE_DYNAMIC),IDD_PARAMBAR))
	{
		TRACE0("Failed to create ParamBar\n");
		return -1;
	}
	
	ParamBar.EnableDocking( CBRS_ALIGN_ANY);
	ParamBar.SetWindowText("Param");
	DockControlBar(&ParamBar);

	ParamBar.Add(LayerList,IDD_LAYER_LIST_DIALOG,TRUE,TRUE);	// Add default parram bar item


// ToolBar (plus extra code for extra controls!!)
	if (!ToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) || !ToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
	ToolBar.SetWindowText("ToolBar");
	ToolBar.EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&ToolBar);
/*	
#define SNAP_WIDTH 80 //the width of the combo box

    //set up the ComboBox control as a snap mode select box
    //
    //First get the index of the placeholder's position in the toolbar
int	index = 0;
RECT	rect;
	while (ToolBar.GetItemID(index)!=ID_TOOLBAR_COMBO) index++;

	//next convert that button to a seperator and get its position
	ToolBar.SetButtonInfo(index, ID_TOOLBAR_COMBO, TBBS_SEPARATOR, SNAP_WIDTH);
	ToolBar.GetItemRect(index, &rect);

	//expand the rectangle to allow the combo box room to drop down
	rect.top+=2;
	rect.bottom += 200;

	// then .Create the combo box and show it

	if (!ToolBar.m_Combo.Create(WS_CHILD|WS_VISIBLE | CBS_AUTOHSCROLL | CBS_DROPDOWNLIST | CBS_HASSTRINGS ,rect, &ToolBar, IDC_TOOLBAR_COMBO))
	{
	   TRACE0("Failed to create combo-box\n");
	   return FALSE;
	}
	ToolBar.m_Combo.SetFont(ToolBar.GetFont());	// Set to nice font (not playskool font!!)
	ToolBar.m_Combo.ShowWindow(SW_SHOW);

	//fill the combo box
	ToolBar.m_Combo.AddString("SNAP OFF");
	ToolBar.m_Combo.AddString("SNAP GRID");
	ToolBar.m_Combo.AddString("SNAP RASTER");
	ToolBar.m_Combo.AddString("SNAP VERTEX");
	ToolBar.m_Combo.AddString("SNAP LINE");
	ToolBar.m_Combo.SetCurSel(0);
*/	
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


/////////////////////////////////////////////////////////////////////////////



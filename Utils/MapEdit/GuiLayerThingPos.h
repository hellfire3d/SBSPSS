#if !defined(AFX_GUILAYERTHINGPOS_H__FE3575F7_161F_4A9B_9D3C_CB9421D9EAB5__INCLUDED_)
#define AFX_GUILAYERTHINGPOS_H__FE3575F7_161F_4A9B_9D3C_CB9421D9EAB5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GuiLayerThingPos.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGuiLayerThingPos dialog

class CGuiLayerThingPos : public CDialog
{
// Construction
public:
	CGuiLayerThingPos(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CGuiLayerThingPos)
	enum { IDD = IDD_LAYER_THING_POS };
	CListBox	m_List;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGuiLayerThingPos)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CGuiLayerThingPos)
	afx_msg void OnSelchangeThingPosList();
	afx_msg void OnThingPosUp();
	afx_msg void OnThingPosDown();
	afx_msg void OnThingPosDelete();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GUILAYERTHINGPOS_H__FE3575F7_161F_4A9B_9D3C_CB9421D9EAB5__INCLUDED_)

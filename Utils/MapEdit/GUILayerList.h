#if !defined(AFX_GUILAYERLIST_H__5D89A904_B1E5_4DF0_88E4_12560972B6DB__INCLUDED_)
#define AFX_GUILAYERLIST_H__5D89A904_B1E5_4DF0_88E4_12560972B6DB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GUILayerList.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGUILayerList dialog

class CGUILayerList : public CDialog
{
// Construction
public:
	CGUILayerList(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CGUILayerList)
	enum { IDD = IDD_LAYER_LIST };
	CCheckListBox	m_List;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGUILayerList)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CGUILayerList)
	afx_msg void OnLayerlistAdd();
	afx_msg void OnLayerlistDelete();
	afx_msg void OnSelchangeLayerlistList();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GUILAYERLIST_H__5D89A904_B1E5_4DF0_88E4_12560972B6DB__INCLUDED_)

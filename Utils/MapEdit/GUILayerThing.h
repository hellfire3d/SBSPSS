#if !defined(AFX_GUILAYERTHING_H__BD4306D6_8C84_4589_B8A6_FE1C95C27018__INCLUDED_)
#define AFX_GUILAYERTHING_H__BD4306D6_8C84_4589_B8A6_FE1C95C27018__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GUILayerThing.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGUILayerThing dialog

class CGUILayerThing : public CDialog
{
// Construction
public:
	CGUILayerThing(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CGUILayerThing)
	enum { IDD = IDD_LAYER_THING };
	CComboBox	m_List;
	CComboBox	m_DefList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGUILayerThing)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CGUILayerThing)
	afx_msg void OnThingDelete();
	afx_msg void OnThingGoto();
	afx_msg void OnSelchangeDefThingList();
	afx_msg void OnSelchangeThingList();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.



#endif // !defined(AFX_GUILAYERTHING_H__BD4306D6_8C84_4589_B8A6_FE1C95C27018__INCLUDED_)

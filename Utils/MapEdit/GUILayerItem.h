#if !defined(AFX_GUILAYERITEM_H__5F713D8B_F890_4284_81D1_F1D6BB0AF3C0__INCLUDED_)
#define AFX_GUILAYERITEM_H__5F713D8B_F890_4284_81D1_F1D6BB0AF3C0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GUILayerItem.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGUILayerItem dialog

class CGUILayerItem : public CDialog
{
// Construction
public:
	CGUILayerItem(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CGUILayerItem)
	enum { IDD = IDD_LAYER_ITEM };
	CComboBox	m_LevelList;
	CComboBox	m_List;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGUILayerItem)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CGUILayerItem)
	afx_msg void OnItemDelete();
	afx_msg void OnSelchangeLevelItemList();
	afx_msg void OnSelchangeItemList();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GUILAYERITEM_H__5F713D8B_F890_4284_81D1_F1D6BB0AF3C0__INCLUDED_)

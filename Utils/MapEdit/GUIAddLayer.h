#if !defined(AFX_ADDLAYERDLG_H__5364908E_729B_40DA_A43C_7056FA88D6CC__INCLUDED_)
#define AFX_ADDLAYERDLG_H__5364908E_729B_40DA_A43C_7056FA88D6CC__INCLUDED_

#include	<vector>
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AddLayerDlg.h : header file
//


/////////////////////////////////////////////////////////////////////////////
// CGUIAddLayer dialog

class CGUIAddLayer : public CDialog
{
// Construction
public:
	CGUIAddLayer(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CGUIAddLayer)
	enum { IDD = IDD_ADDLAYER };
	CListBox	m_List;
	//}}AFX_DATA
	std::vector<char *>	StrList;
	int		*Sel;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGUIAddLayer)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CGUIAddLayer)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeAddlayerList();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADDLAYERDLG_H__5364908E_729B_40DA_A43C_7056FA88D6CC__INCLUDED_)

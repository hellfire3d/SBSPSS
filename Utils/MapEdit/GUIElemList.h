#if !defined(AFX_GUIELEMLIST_H__869021D6_2A35_4EF0_A0A1_B2DD59F4534D__INCLUDED_)
#define AFX_GUIELEMLIST_H__869021D6_2A35_4EF0_A0A1_B2DD59F4534D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GUIElemList.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGUIElemList dialog

class CGUIElemList : public CDialog
{
// Construction
public:
	CGUIElemList(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CGUIElemList)
	enum { IDD = IDD_ELEMLIST };
	CComboBox	m_List;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGUIElemList)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CGUIElemList)
	afx_msg void OnCloseupElemList();
	afx_msg void OnSelchangeElemList();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GUIELEMLIST_H__869021D6_2A35_4EF0_A0A1_B2DD59F4534D__INCLUDED_)

#if !defined(AFX_GUINEWMAP_H__2C6F68D1_14CE_43D5_B35A_11F9E5948CF4__INCLUDED_)
#define AFX_GUINEWMAP_H__2C6F68D1_14CE_43D5_B35A_11F9E5948CF4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GUINewMap.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGUINewMap dialog

class CGUINewMap : public CDialog
{
// Construction
public:
	CGUINewMap(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CGUINewMap)
	enum { IDD = IDD_NEWMAP };
	UINT	m_Height;
	UINT	m_Width;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGUINewMap)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CGUINewMap)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GUINEWMAP_H__2C6F68D1_14CE_43D5_B35A_11F9E5948CF4__INCLUDED_)

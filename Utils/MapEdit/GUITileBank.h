#if !defined(AFX_GUITILEBANK_H__85A70565_C33B_4B2D_88AE_D10C8C9FC455__INCLUDED_)
#define AFX_GUITILEBANK_H__85A70565_C33B_4B2D_88AE_D10C8C9FC455__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GUITileBank.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGUITileBank dialog

class CGUITileBank : public CDialog
{
// Construction
public:
	CGUITileBank(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CGUITileBank)
	enum { IDD = IDD_TILEBANK };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGUITileBank)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CGUITileBank)
	afx_msg void OnTilebankLoad();
	afx_msg void OnTilebankDelete();
	afx_msg void OnTilebankUpdate();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GUITILEBANK_H__85A70565_C33B_4B2D_88AE_D10C8C9FC455__INCLUDED_)

#if !defined(AFX_GUILAYERTRIGGER_H__AD953411_95E5_4401_965F_EF988C489530__INCLUDED_)
#define AFX_GUILAYERTRIGGER_H__AD953411_95E5_4401_965F_EF988C489530__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GUILayerTrigger.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGUILayerTrigger dialog

class CGUILayerTrigger : public CDialog
{
// Construction
public:
	CGUILayerTrigger(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CGUILayerTrigger)
	enum { IDD = IDD_LAYER_TRIGGER };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGUILayerTrigger)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CGUILayerTrigger)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GUILAYERTRIGGER_H__AD953411_95E5_4401_965F_EF988C489530__INCLUDED_)

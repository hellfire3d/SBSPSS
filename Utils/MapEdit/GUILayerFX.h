#if !defined(AFX_GUILAYERFX_H__06B9ED8F_0F1A_4D43_8DE9_788996BCAB52__INCLUDED_)
#define AFX_GUILAYERFX_H__06B9ED8F_0F1A_4D43_8DE9_788996BCAB52__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GUILayerFX.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGUILayerFX dialog

class CGUILayerFX : public CDialog
{
// Construction
public:
	CGUILayerFX(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CGUILayerFX)
	enum { IDD = IDD_LAYER_FX };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGUILayerFX)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CGUILayerFX)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GUILAYERFX_H__06B9ED8F_0F1A_4D43_8DE9_788996BCAB52__INCLUDED_)

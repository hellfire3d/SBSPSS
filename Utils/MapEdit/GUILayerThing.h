#if !defined(AFX_GUILAYERTHING_H__2ADF47A2_1F6C_4AA5_8E36_528C50968C89__INCLUDED_)
#define AFX_GUILAYERTHING_H__2ADF47A2_1F6C_4AA5_8E36_528C50968C89__INCLUDED_

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
		// NOTE: the ClassWizard will add data members here
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
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GUILAYERTHING_H__2ADF47A2_1F6C_4AA5_8E36_528C50968C89__INCLUDED_)

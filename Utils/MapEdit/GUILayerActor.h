#if !defined(AFX_GUILAYERACTOR_H__D2268E7C_1D7E_4C2F_AF3C_49BB374ED65B__INCLUDED_)
#define AFX_GUILAYERACTOR_H__D2268E7C_1D7E_4C2F_AF3C_49BB374ED65B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GUILayerActor.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGUILayerActor dialog

class CGUILayerActor : public CDialog
{
// Construction
public:
	CGUILayerActor(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CGUILayerActor)
	enum { IDD = IDD_LAYER_ACTOR };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGUILayerActor)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CGUILayerActor)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GUILAYERACTOR_H__D2268E7C_1D7E_4C2F_AF3C_49BB374ED65B__INCLUDED_)

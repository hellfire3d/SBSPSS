#if !defined(AFX_GUILAYERRGB_H__2B103B4E_E1B3_4FD4_B791_A12204424DEE__INCLUDED_)
#define AFX_GUILAYERRGB_H__2B103B4E_E1B3_4FD4_B791_A12204424DEE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GUILayerRGB.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGUILayerRGB dialog

class CGUILayerRGB : public CDialog
{
// Construction
public:
	CGUILayerRGB(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CGUILayerRGB)
	enum { IDD = IDD_LAYER_RGB };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGUILayerRGB)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CGUILayerRGB)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GUILAYERRGB_H__2B103B4E_E1B3_4FD4_B791_A12204424DEE__INCLUDED_)

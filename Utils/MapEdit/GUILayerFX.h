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
	CSpinButtonCtrl	m_HeightSpin;
	CSpinButtonCtrl	m_SpeedSpin;
	CSpinButtonCtrl	m_WidthSpin;
	CEdit	m_Width;
	CEdit	m_Height;
	CEdit	m_Speed;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGUILayerFX)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation

public:
	void	DisableCallback(bool f)	{CallbackFlag=f;}
	void	SetVal(CEdit &Dlg,int Number);
	int		GetVal(CEdit &Dlg);
protected:
	bool	CallbackFlag;

	// Generated message map functions
	//{{AFX_MSG(CGUILayerFX)
	afx_msg void OnParamChange();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GUILAYERFX_H__06B9ED8F_0F1A_4D43_8DE9_788996BCAB52__INCLUDED_)

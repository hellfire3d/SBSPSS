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
	CEdit	m_Val0;
	CStatic	m_ValText;
	CSpinButtonCtrl	m_ValSpin;
	CSpinButtonCtrl	m_TargetWSpin;
	CSpinButtonCtrl	m_TargetHSpin;
	CStatic	m_TargetWHTxt;
	CEdit	m_TargetW;
	CEdit	m_TargetH;
	CStatic	m_TargetTxt;
	CSpinButtonCtrl	m_TargetYSpin;
	CSpinButtonCtrl	m_TargetXSpin;
	CEdit	m_TargetY;
	CEdit	m_TargetX;
	CSpinButtonCtrl	m_WidthSpin;
	CSpinButtonCtrl	m_HeightSpin;
	CEdit	m_Width;
	CEdit	m_Height;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGUILayerTrigger)
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
	//{{AFX_MSG(CGUILayerTrigger)
	afx_msg void OnSelchangeTriggerType();
	afx_msg void OnParamChange();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GUILAYERTRIGGER_H__AD953411_95E5_4401_965F_EF988C489530__INCLUDED_)

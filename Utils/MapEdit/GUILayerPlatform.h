#if !defined(AFX_GUILAYERPLATFORM_H__18E94AB1_AC24_4A0D_8C9B_90B9A543EEA7__INCLUDED_)
#define AFX_GUILAYERPLATFORM_H__18E94AB1_AC24_4A0D_8C9B_90B9A543EEA7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GUILayerPlatform.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGUILayerPlatform dialog

class CGUILayerPlatform : public CDialog
{
// Construction
public:
	CGUILayerPlatform(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CGUILayerPlatform)
	enum { IDD = IDD_LAYER_PLATFORM };
	CSpinButtonCtrl	m_Param0Spin;
	CStatic	m_Param0Txt;
	CEdit	m_Param0;
	CSpinButtonCtrl	m_TurnRateSpin;
	CSpinButtonCtrl	m_SpeedSpin;
	CComboBox	m_Type;
	CComboBox	m_MoveList;
	CButton	m_Collision;
	CEdit	m_TurnRate;
	CEdit	m_Speed;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGUILayerPlatform)
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
	//{{AFX_MSG(CGUILayerPlatform)
	afx_msg void OnChangeParam();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GUILAYERPLATFORM_H__18E94AB1_AC24_4A0D_8C9B_90B9A543EEA7__INCLUDED_)

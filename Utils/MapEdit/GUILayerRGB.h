#if !defined(AFX_GUILAYERRGB_H__2B103B4E_E1B3_4FD4_B791_A12204424DEE__INCLUDED_)
#define AFX_GUILAYERRGB_H__2B103B4E_E1B3_4FD4_B791_A12204424DEE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GUILayerRGB.h : header file
//
#ifndef	u8
typedef	unsigned char	u8;
#endif

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
	CSpinButtonCtrl	m_BSpin;
	CEdit	m_B;
	CSpinButtonCtrl	m_GSpin;
	CEdit	m_G;
	CSpinButtonCtrl	m_RSpin;
	CEdit	m_R;
	CComboBox	m_RateList;
	CComboBox	m_BrushList;
	CComboBox	m_ModeList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGUILayerRGB)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	void	EnableCallback()		{CallbackFlag=true;}
	void	DisableCallback()		{CallbackFlag=false;}

	void	SetVal(CEdit &Dlg,int &Val,int Min=-1,int Max=-1);
	void	GetVal(CEdit &Dlg,int &Val,int Min=-1,int Max=-1);
	void	SetRGB(u8 &R,u8 &G,u8 &B);
	void	GetRGB(u8 &R,u8 &G,u8 &B);

protected:
	bool	CallbackFlag;

	// Generated message map functions
	//{{AFX_MSG(CGUILayerRGB)
	afx_msg void OnParamChange();
	afx_msg void OnLayershadeTrifilter();
	afx_msg void OnLayershadeStrifilter();
	afx_msg void OnLayershadeBifilter();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GUILAYERRGB_H__2B103B4E_E1B3_4FD4_B791_A12204424DEE__INCLUDED_)

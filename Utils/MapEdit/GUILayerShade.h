#if !defined(AFX_LAYERSHADEGUI_H__28B4764C_E86F_481B_9822_CFAE6CD32C9D__INCLUDED_)
#define AFX_LAYERSHADEGUI_H__28B4764C_E86F_481B_9822_CFAE6CD32C9D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LayerShadeGUI.h : header file
//

#ifndef	u8
typedef	unsigned char	u8;
#endif
/////////////////////////////////////////////////////////////////////////////
// CGUILayerShade dialog

class CGUILayerShade : public CDialog
{
// Construction
public:
	CGUILayerShade(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CGUILayerShade)
	enum { IDD = IDD_LAYER_SHADE };
	CSpinButtonCtrl	m_GfxPosYSpin;
	CSpinButtonCtrl	m_GfxPosXSpin;
	CComboBox	m_DefList;
	CSpinButtonCtrl	m_ShadeCountSpin;
	CEdit			m_ShadeCount;
	CEdit			m_ShadeR0;
	CEdit			m_ShadeG0;
	CEdit			m_ShadeB0;
	CEdit			m_ShadeR1;
	CEdit			m_ShadeG1;
	CEdit			m_ShadeB1;
	CEdit			m_ShadeR2;
	CEdit			m_ShadeG2;
	CEdit			m_ShadeB2;
	CEdit			m_ShadeR3;
	CEdit			m_ShadeG3;
	CEdit			m_ShadeB3;
	CSpinButtonCtrl	m_GfxCurrentSpin;
	CEdit			m_GfxCurrent;
	CComboBox		m_GfxBankList;
	CEdit			m_GfxPosX;
	CEdit			m_GfxPosY;
	CSpinButtonCtrl	m_GfxSpinx0;
	CSpinButtonCtrl	m_GfxSpiny0;
	CEdit			m_Gfxx0;
	CEdit			m_Gfxy0;
	CEdit			m_GfxR0;
	CEdit			m_GfxG0;
	CEdit			m_GfxB0;
	CSpinButtonCtrl	m_GfxSpinx1;
	CSpinButtonCtrl	m_GfxSpiny1;
	CEdit			m_Gfxx1;
	CEdit			m_Gfxy1;
	CEdit			m_GfxR1;
	CEdit			m_GfxG1;
	CEdit			m_GfxB1;
	CSpinButtonCtrl	m_GfxSpinx2;
	CSpinButtonCtrl	m_GfxSpiny2;
	CEdit			m_Gfxx2;
	CEdit			m_Gfxy2;
	CEdit			m_GfxR2;
	CEdit			m_GfxG2;
	CEdit			m_GfxB2;
	CSpinButtonCtrl	m_GfxSpinx3;
	CSpinButtonCtrl	m_GfxSpiny3;
	CEdit			m_Gfxx3;
	CEdit			m_Gfxy3;
	CEdit			m_GfxR3;
	CEdit			m_GfxG3;
	CEdit			m_GfxB3;
	CComboBox		m_GfxTransList;
	//}}AFX_DATA

	void OnOK();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGUILayerShade)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	void	EnableCallback()		{CallbackFlag=true;}
	void	DisableCallback()		{CallbackFlag=false;}
	void	SetVal(CEdit &Dlg,int &Val,int Min=-1,int Max=-1);
	void	GetVal(CEdit &Dlg,int &Val,int Min=-1,int Max=-1);
	void	SetRGB(CEdit &RDlg,CEdit &GDlg,CEdit &BDlg,u8 &R,u8 &G,u8 &B);
	void	GetRGB(CEdit &RDlg,CEdit &GDlg,CEdit &BDlg,u8 &R,u8 &G,u8 &B);

	void	SetShadeRGB(int idx,u8 &R,u8 &G,u8 &B);
	void	GetShadeRGB(int idx,u8 &R,u8 &G,u8 &B);
	void	SetGfxRGB(int idx,u8 &R,u8 &G,u8 &B);
	void	GetGfxRGB(int idx,u8 &R,u8 &G,u8 &B);

protected:
	bool	CallbackFlag;

	// Generated message map functions
	//{{AFX_MSG(CGUILayerShade)
	afx_msg void OnChangeParam();
	afx_msg void OnGoto();
	afx_msg void OnDelete();
	afx_msg void OnChangeCurrent();
	afx_msg void OnNew();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LAYERSHADEGUI_H__28B4764C_E86F_481B_9822_CFAE6CD32C9D__INCLUDED_)

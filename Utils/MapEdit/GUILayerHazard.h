#if !defined(AFX_GUILAYERHAZARD_H__D2268E7C_1D7E_4C2F_AF3C_49BB374ED65B__INCLUDED_)
#define AFX_GUILAYERHAZARD_H__D2268E7C_1D7E_4C2F_AF3C_49BB374ED65B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GUILayerHazard.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGUILayerHazard dialog

class CGUILayerHazard : public CDialog 
{
// Construction
public:
	CGUILayerHazard(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CGUILayerHazard)
	enum { IDD = IDD_LAYER_HAZARD };
	CSpinButtonCtrl	m_TurnRateSpin;
	CSpinButtonCtrl	m_SpeedSpin;
	CSpinButtonCtrl	m_HealthSpin;
	CSpinButtonCtrl	m_AttackSpin;
	CSpinButtonCtrl	m_RespawnSpin;
	CButton	m_Collision;
	CEdit	m_TurnRate;
	CEdit	m_Speed;
	CEdit	m_Attack;
	CEdit	m_Health;
	CEdit	m_Respawn;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGUILayerHazard)
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
	//{{AFX_MSG(CGUILayerHazard)
	afx_msg void OnChangeParam();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GUILAYERHAZARD_H__D2268E7C_1D7E_4C2F_AF3C_49BB374ED65B__INCLUDED_)

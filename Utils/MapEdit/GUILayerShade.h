#if !defined(AFX_LAYERSHADEGUI_H__28B4764C_E86F_481B_9822_CFAE6CD32C9D__INCLUDED_)
#define AFX_LAYERSHADEGUI_H__28B4764C_E86F_481B_9822_CFAE6CD32C9D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LayerShadeGUI.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGUILayerShade dialog

class CGUILayerShade : public CDialog
{
// Construction
public:
	CGUILayerShade(CWnd* pParent = NULL);   // standard constructor

	void	SetVal(CEdit &Dlg,int &Val);
	void	GetVal(CEdit &Dlg,int &Val);
	void	LimitVal(int Min,int Max,CEdit &Dlg);

	void	SetRGB(RGBQUAD &RGB,int Set);
	void	GetRGB(RGBQUAD &RGB,int Set);

	void	SetCount(int &Count)	{SetVal(m_Count,Count);}
	void	GetCount(int &Count)	{GetVal(m_Count,Count);}

	void	CheckData();
	void	CheckRGB(int Set);

	void	SetStatus(int Set,bool f);

// Dialog Data
	//{{AFX_DATA(CGUILayerShade)
	enum { IDD = IDD_LAYERSHADE_GUI };
	CEdit	m_R0;
	CEdit	m_G0;
	CEdit	m_B0;
	CEdit	m_R1;
	CEdit	m_G1;
	CEdit	m_B1;
	CEdit	m_R2;
	CEdit	m_G2;
	CEdit	m_B2;
	CEdit	m_R3;
	CEdit	m_G3;
	CEdit	m_B3;
	CEdit	m_Count;
	//}}AFX_DATA

	void OnOK();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGUILayerShade)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CGUILayerShade)
	afx_msg void OnChangeLayershadeEdit();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	bool	SetFlag;

	CEdit	*Map[4][4];
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LAYERSHADEGUI_H__28B4764C_E86F_481B_9822_CFAE6CD32C9D__INCLUDED_)

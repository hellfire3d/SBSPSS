#if !defined(AFX_GUITOOLBAR_H__5622CD93_4D01_455A_85DC_12A630CA8D9D__INCLUDED_)
#define AFX_GUITOOLBAR_H__5622CD93_4D01_455A_85DC_12A630CA8D9D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GUIToolBar.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// GUIToolBar dialog

class CGUIToolBar : public CDialog
{
// Construction
public:
	enum GUIToolBarButton
	{
		PAINT=0,
		SELECT,
	};

	CGUIToolBar(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(GUIToolBar)
	enum { IDD = IDD_TOOLBAR };
	CButton	m_Select;
	CButton	m_Paint;
	//}}AFX_DATA

	void	ResetButtons();
	void	SetButtonState(GUIToolBarButton ID,BOOL Down);
	BOOL	GetButtonState(GUIToolBarButton ID);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(GUIToolBar)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(GUIToolBar)
	afx_msg void OnLayertileBtnPaint();
	afx_msg void OnLayertileBtnSelect();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GUITOOLBAR_H__5622CD93_4D01_455A_85DC_12A630CA8D9D__INCLUDED_)

#if !defined(AFX_GUILAYERCOLLISION_H__19A02D59_C518_4A7E_8F76_93EF28087F77__INCLUDED_)
#define AFX_GUILAYERCOLLISION_H__19A02D59_C518_4A7E_8F76_93EF28087F77__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GUILayerCollision.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// GUILayerCollision dialog

class GUILayerCollision : public CDialog
{
// Construction
public:
	GUILayerCollision(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(GUILayerCollision)
	enum { IDD = IDD_LAYER_COLLISION };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(GUILayerCollision)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(GUILayerCollision)
	afx_msg void OnLayercollisionDamage();
	afx_msg void OnLayercollisionDeathFall();
	afx_msg void OnLayercollisionDeathInstant();
	afx_msg void OnLayercollisionDeathLiquid();
	afx_msg void OnLayercollisionDestructFloor();
	afx_msg void OnLayercollisionDestructWall();
	afx_msg void OnLayercollisionElectric();
	afx_msg void OnLayercollisionMoveLeft();
	afx_msg void OnLayercollisionMoveRight();
	afx_msg void OnLayercollisionNormal();
	afx_msg void OnLayercollisionSlippery();
	afx_msg void OnLayercollisionSolid();
	afx_msg void OnLayercollisionSoakUp();
	afx_msg void OnLayercollisionSbNomove();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GUILAYERCOLLISION_H__19A02D59_C518_4A7E_8F76_93EF28087F77__INCLUDED_)

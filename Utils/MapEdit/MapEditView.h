// MapEditView.h : interface of the CMapEditView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAPEDITVIEW_H__DBE61BE7_547C_43E9_BC46_E55636495066__INCLUDED_)
#define AFX_MAPEDITVIEW_H__DBE61BE7_547C_43E9_BC46_E55636495066__INCLUDED_


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMapEditView : public CGLEnabledView
{
protected: // create from serialization only
	CMapEditView();
	DECLARE_DYNCREATE(CMapEditView)

// Attributes
public:
	CMapEditDoc* GetDocument();
	void		OnCreateGL();
	void		OnDrawGL();
	void		OnSizeGL(int cx, int cy);
	void		SetupPersMatrix();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMapEditView)
	public:
	virtual void OnInitialUpdate();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMapEditView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	void		VideoMode(ColorsNumber &c,ZAccuracy &z,BOOL &dbuf);

// Generated message map functions
protected:
	//{{AFX_MSG(CMapEditView)
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in MapEditView.cpp
inline CMapEditDoc* CMapEditView::GetDocument()
   { return (CMapEditDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAPEDITVIEW_H__DBE61BE7_547C_43E9_BC46_E55636495066__INCLUDED_)

// MapEditView.h : interface of the CMapEditView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAPEDITVIEW_H__DBE61BE7_547C_43E9_BC46_E55636495066__INCLUDED_)
#define AFX_MAPEDITVIEW_H__DBE61BE7_547C_43E9_BC46_E55636495066__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CMapEditView : public CView
{
protected: // create from serialization only
	CMapEditView();
	DECLARE_DYNCREATE(CMapEditView)

// Attributes
public:
	CMapEditDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMapEditView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMapEditView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CMapEditView)
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

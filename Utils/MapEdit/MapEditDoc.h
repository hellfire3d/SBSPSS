// MapEditDoc.h : interface of the CMapEditDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAPEDITDOC_H__C235E087_8243_4400_BA8E_2937FBC1F9F2__INCLUDED_)
#define AFX_MAPEDITDOC_H__C235E087_8243_4400_BA8E_2937FBC1F9F2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include	"Core.h"

class	CMapEditDoc : public CDocument
{
protected: // create from serialization only
	CMapEditDoc();
	DECLARE_DYNCREATE(CMapEditDoc)

// Attributes
public:
	void			UpdateView(CMapEditView *View);
	void			Render(CMapEditView *View);
	void			UpdateAll(CMapEditView *View);

// Windows Messages Thru Point
	void			LButtonControl(CMapEditView *View,UINT nFlags, CPoint &point,BOOL DownFlag);
	void			MButtonControl(CMapEditView *View,UINT nFlags, CPoint &point,BOOL DownFlag);
	void			RButtonControl(CMapEditView *View,UINT nFlags, CPoint &point,BOOL DownFlag);
	void			MouseWheel(CMapEditView *View,UINT nFlags, short zDelta, CPoint &pt);
	void			MouseMove(CMapEditView *View,UINT nFlags, CPoint &point);
	void			ToggleTileView(CMapEditView *View);
	void			ToggleGrid(CMapEditView *View);
	void			MirrorX(CMapEditView *View);
	void			MirrorY(CMapEditView *View);
	void			ActiveBrushLeft(CMapEditView *View);
	void			ActiveBrushRight(CMapEditView *View);
	void			MapSetSize(CMapEditView *View);

	void			TileBankLoad();
	void			TileBankReload();
	void			TileBankSet();

	void			SetMode(int NewMode);

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMapEditDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMapEditDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	CCore		Core;

// Generated message map functions
protected:
	//{{AFX_MSG(CMapEditDoc)
	afx_msg void OnStatusCursorXY(CCmdUI *pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAPEDITDOC_H__C235E087_8243_4400_BA8E_2937FBC1F9F2__INCLUDED_)

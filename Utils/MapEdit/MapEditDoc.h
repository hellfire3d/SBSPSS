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
	void			SetView(CMapEditView *View);
	CMapEditView	*GetView();
	void			UpdateView();
	void			Render();
	void			UpdateAll();
	void			FocusView();
	void			GUIUpdate();
	void			GUIChanged();

	int				GetMapWidth()	{return(Core.GetMapWidth());}
	int				GetMapHeight()	{return(Core.GetMapHeight());}

// Windows Messages Thru Point

	void			LButtonControl(UINT nFlags, CPoint &point,BOOL DownFlag);
	void			MButtonControl(UINT nFlags, CPoint &point,BOOL DownFlag);
	void			RButtonControl(UINT nFlags, CPoint &point,BOOL DownFlag);
	void			MouseWheel(UINT nFlags, short zDelta, CPoint &pt);
	void			MouseMove(UINT nFlags, CPoint &point);

	void			Command(int CmdMsg,int Param0=0,int Param1=0);

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMapEditDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual void Serialize(CArchive& ar);
	virtual void OnCloseDocument();
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
	afx_msg void OnExport();
	afx_msg void OnZoomIn();
	afx_msg void OnZoomOut();
	afx_msg void OnToggleSubView();
	afx_msg void OnToggleGrid();
	afx_msg void OnMirrorx();
	afx_msg void OnMirrory();
	afx_msg void OnActivebrushLeft();
	afx_msg void OnActivebrushRight();
	afx_msg void OnMapSetSize();
	afx_msg void OnEditCopy();
	afx_msg void OnEditPaste();
	afx_msg void On2d3dToggle();
	afx_msg void OnResetView();
	afx_msg void OnRenderToTga();
	afx_msg void OnRotate();
	afx_msg void OnReport();
	afx_msg void OnEditUndo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAPEDITDOC_H__C235E087_8243_4400_BA8E_2937FBC1F9F2__INCLUDED_)

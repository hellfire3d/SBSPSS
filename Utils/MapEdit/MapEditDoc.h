// MapEditDoc.h : interface of the CMapEditDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAPEDITDOC_H__C235E087_8243_4400_BA8E_2937FBC1F9F2__INCLUDED_)
#define AFX_MAPEDITDOC_H__C235E087_8243_4400_BA8E_2937FBC1F9F2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include	"Core.h"

class	CLayerBar;
class	CMapEditDoc : public CDocument
{
protected: // create from serialization only
	CMapEditDoc();
	DECLARE_DYNCREATE(CMapEditDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMapEditDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMapEditDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	void		UpdateAll();

protected:
	void		UpdateLayerBar();
	int			GetLayerCurSel();
	int			GetLayerCount();

	CCore		Core;
	CLayerBar	*LayerBar;
	
// Generated message map functions
protected:
	//{{AFX_MSG(CMapEditDoc)
	afx_msg void	OnLayerbarNew();
	afx_msg void	OnLayerbarDelete();
	afx_msg void	OnLayerbarUp();
	afx_msg void	OnLayerbarDown();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAPEDITDOC_H__C235E087_8243_4400_BA8E_2937FBC1F9F2__INCLUDED_)

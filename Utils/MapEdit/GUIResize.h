#if !defined(AFX_GUIRESIZE_H__C20CE7DA_F313_4A28_9923_794954C4DF9F__INCLUDED_)
#define AFX_GUIRESIZE_H__C20CE7DA_F313_4A28_9923_794954C4DF9F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GUIResize.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGUIResize dialog

class CGUIResize : public CDialog
{
// Construction
public:
	CGUIResize(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CGUIResize)
	enum { IDD = IDD_RESIZE };
	UINT	m_Height;
	UINT	m_Width;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGUIResize)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CGUIResize)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GUIRESIZE_H__C20CE7DA_F313_4A28_9923_794954C4DF9F__INCLUDED_)

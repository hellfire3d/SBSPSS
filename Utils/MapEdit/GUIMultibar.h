/*****************************/
/*** MultiBar Dialog Class ***/
/*** Dave                  ***/
/*****************************/

#if !defined(AFX_MULTIBAR_H__806BA89D_41A5_4499_834F_613A21999A82__INCLUDED_)
#define AFX_MULTIBAR_H__806BA89D_41A5_4499_834F_613A21999A82__INCLUDED_

#include	<vector>

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/*********************************************************************************/
class CGUIMultiBar : public CDialogBar
{
// Construction
public:
	enum
	{
		MultiBarGap=4,
	};

	struct	sList
	{
//		int		ID;
		CDialog	*Dlg;
		BOOL	Visible;
		BOOL	Lock;
	};

	CGUIMultiBar(CWnd* pParent = NULL);   // standard constructor

	void	Add(CDialog &Dlg,int ID,BOOL Visible=FALSE,BOOL Lock=FALSE);
	void	Remove(CDialog &Dlg,BOOL Force=FALSE)	{RemoveDlg(GetDialogIdx(&Dlg),Force);}
//	void	Remove(int ID,BOOL Force=FALSE)			{RemoveDlg(GetDialogIdx(ID),Force);}
	void	RemoveAll(BOOL Force=FALSE);

	void	Show(CDialog &Dlg,BOOL Force=FALSE)		{Show(&Dlg);}
	void	Show(CDialog *Dlg,BOOL Force=FALSE)		{SetVisible(Dlg,TRUE,Force);}
	void	ShowAll(BOOL Force=FALSE)				{SetVisibleAll(TRUE,Force);}

	void	Hide(CDialog &Dlg,BOOL Force=FALSE)		{Hide(&Dlg);}
	void	Hide(CDialog *Dlg,BOOL Force=FALSE)		{SetVisible(Dlg,FALSE,Force);}
	void	HideAll(BOOL Force=FALSE)				{SetVisibleAll(FALSE,Force);}

	void	Lock(CDialog &Dlg)						{Lock(&Dlg);}
	void	Lock(CDialog *Dlg);
	void	LockAll();

	void	Unlock(CDialog &Dlg)					{Unlock(&Dlg);}
	void	Unlock(CDialog *Dlg);
	void	UnlockAll();
	
	CDialog	*GetDialog(int ID);

	void	Update();

virtual CSize CalcFixedLayout(BOOL bStretch, BOOL bHorz);
virtual	CSize CalcDynamicLayout( int nLength, DWORD dwMode );




// Dialog Data
	//{{AFX_DATA(CGUIMultiBar)
	enum { IDD = IDD_MULTIBAR };
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGUIMultiBar)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	sList	&GetDialog(CDialog *Dlg);
	int		GetDialogIdx(CDialog *Dlg);
//	int		GetDialogIdx(int ID);

	void	SetVisible(CDialog &Dlg,BOOL Visible,BOOL Force=FALSE)	{SetVisible(&Dlg,Visible,Force);}
	void	SetVisible(CDialog *Dlg,BOOL Visible,BOOL Force=FALSE);
	void	SetVisibleAll(BOOL Visible,BOOL Force);

	void	SetLock(CDialog &Dlg,BOOL Lock)							{SetLock(&Dlg,Lock);}
	void	SetLock(CDialog *Dlg,BOOL Lock);
	void	SetLockAll(BOOL Lock);

	void	RemoveDlg(int Idx,BOOL Force=FALSE);
	CSize	CalcBarSize();

	CSize	DefSize;

	std::vector<sList>	List;
		
	// Generated message map functions
	//{{AFX_MSG(CGUIMultiBar)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MULTIBAR_H__806BA89D_41A5_4499_834F_613A21999A82__INCLUDED_)

/*****************************/
/*** MultiBar Dialog Class ***/
/*** Dave                  ***/
/*****************************/

#include "stdafx.h"
#include "mapedit.h"
#include "GUIMultiBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/*********************************************************************************/

CGUIMultiBar::CGUIMultiBar(CWnd* pParent /*=NULL*/)
{
	//{{AFX_DATA_INIT(CGUIMultiBar)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

/*********************************************************************************/
void CGUIMultiBar::DoDataExchange(CDataExchange* pDX)
{
	CDialogBar::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGUIMultiBar)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGUIMultiBar, CDialogBar)
	//{{AFX_MSG_MAP(CGUIMultiBar)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/*********************************************************************************/
int CGUIMultiBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
		if (CDialogBar::OnCreate(lpCreateStruct) == -1)	return -1;

RECT	R;
		GetWindowRect(&R);
		DefSize.cx=(R.right-R.left)+4;
		DefSize.cy=(R.bottom-R.top)+4;

		return 0;
}

/*********************************************************************************/
/*********************************************************************************/
/*********************************************************************************/
void	CGUIMultiBar::Add(CDialog &Dlg,int ID,BOOL Visible,BOOL Lock)
{
//int		ListSize=List.size();

		if (GetDialogIdx(&Dlg)!=-1) return;	// Already exists

		if (!Dlg.Create(ID,this))
		{
			ASSERT(!"Failed to create MultiDialog");
		}
		
sList	NewDlg;

		NewDlg.Dlg=&Dlg;
//		NewDlg.ID=ID;
		NewDlg.Visible=Visible;
		NewDlg.Lock=Lock;

		List.push_back(NewDlg);

		SetVisible(Dlg,Visible,TRUE);
}

/*********************************************************************************/
void	CGUIMultiBar::RemoveDlg(int Idx,BOOL Force)
{
		if (Idx==-1) return;
sList	&ThisDlg=List[Idx];
int		ListSize=List.size();
		if (Force || !ThisDlg.Lock)
		{
			ThisDlg.Dlg->DestroyWindow();
			List.erase(List.begin()+Idx);
		}
}

/*********************************************************************************/
void	CGUIMultiBar::RemoveAll(BOOL Force)
{
int	ListSize=List.size();

		for (int i=ListSize; i; i--)
		{
			RemoveDlg(i-1,Force);
		}
}

/*********************************************************************************/
/*********************************************************************************/
/*********************************************************************************/
CGUIMultiBar::sList	&CGUIMultiBar::GetDialog(CDialog *Dlg)
{
int	Idx=GetDialogIdx(Dlg);

	return(List[Idx]);
}

/*********************************************************************************/
int		CGUIMultiBar::GetDialogIdx(CDialog *Dlg)
{
int	ListSize=List.size();

	for (int i=0; i<ListSize; i++)
	{
		sList	&ThisDlg=List[i];

		if (ThisDlg.Dlg==Dlg) return(i);
	}
	return(-1);
}

/*********************************************************************************/
/*********************************************************************************/
/*********************************************************************************/
void	CGUIMultiBar::SetVisible(CDialog *Dlg,BOOL Visible,BOOL Force)
{
sList	&ThisDlg=GetDialog(Dlg);

		if (Force || !ThisDlg.Lock)
		{
			ThisDlg.Visible=Visible;
			if (Visible)
				ThisDlg.Dlg->ShowWindow(SW_SHOW);
			else
				ThisDlg.Dlg->ShowWindow(SW_HIDE);
			ShowWindow(SW_SHOW);
		}
}

/*********************************************************************************/
void	CGUIMultiBar::SetVisibleAll(BOOL Visible,BOOL Force)
{
int	ListSize=List.size();

		for (int i=0; i<ListSize; i++)
		{
			SetVisible(List[i].Dlg,Visible,Force);
		}
}

/*********************************************************************************/
/*********************************************************************************/
/*********************************************************************************/
void	CGUIMultiBar::SetLock(CDialog *Dlg,BOOL Lock)
{
sList	&ThisDlg=GetDialog(Dlg);
		ThisDlg.Lock=Lock;
}

/*********************************************************************************/
void	CGUIMultiBar::SetLockAll(BOOL Lock)
{
int	ListSize=List.size();

		for (int i=0; i<ListSize; i++)
		{
			SetLock(List[i].Dlg,Lock);
		}
}

/*********************************************************************************/
/*********************************************************************************/
/*********************************************************************************/
CSize	CGUIMultiBar::CalcBarSize()
{
int		ListSize=List.size();
CSize	Size(0,MultiBarGap);;

		if (!ListSize)
		{
			Size=DefSize;
		}

		for (int i=0; i<ListSize; i++)
		{
			sList	&ThisDlg=List[i];

			if (ThisDlg.Visible)
			{
				CDialog	*Dlg=ThisDlg.Dlg;
				CRect	Rect;
				Dlg->GetWindowRect(&Rect);

				Dlg->SetWindowPos(&wndTop,MultiBarGap,Size.cy,0,0,SWP_NOSIZE);
				Size.cy+=Rect.Height()+MultiBarGap;
				Size.cx=max(Rect.Width(),Size.cx);
			}
		}

		SetWindowPos(&wndTop,0,0,Size.cx,Size.cy,SWP_NOMOVE);
		return(Size);
}

/*********************************************************************************/

CSize CGUIMultiBar::CalcFixedLayout(BOOL bStretch, BOOL bHorz)
{
	return(CalcBarSize());
} 

/*********************************************************************************/
CSize CGUIMultiBar::CalcDynamicLayout( int nLength, DWORD dwMode )
{
	return(CalcBarSize());
}

/*********************************************************************************/
void	CGUIMultiBar::Update()
{
		CalcBarSize();
}


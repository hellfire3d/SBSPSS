// LayerShadeGUI.cpp : implementation file
//

#include "stdafx.h"
#include "mapedit.h"
#include "GUILayerShade.h"

#include	"MapEditDoc.h"
#include	"MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGUILayerShade dialog


CGUILayerShade::CGUILayerShade(CWnd* pParent /*=NULL*/)
	: CDialog(CGUILayerShade::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGUILayerShade)
	//}}AFX_DATA_INIT
	SetFlag=true;
	Map[0][0]=&m_R0; Map[0][1]=&m_G0;	Map[0][2]=&m_B0;
	Map[1][0]=&m_R1; Map[1][1]=&m_G1;	Map[1][2]=&m_B1;
	Map[2][0]=&m_R2; Map[2][1]=&m_G2;	Map[2][2]=&m_B2;
	Map[3][0]=&m_R3; Map[3][1]=&m_G3;	Map[3][2]=&m_B3;

}


void CGUILayerShade::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGUILayerShade)
	DDX_Control(pDX, IDC_LAYERSHADE_EDITR0, m_R0);
	DDX_Control(pDX, IDC_LAYERSHADE_EDITG0, m_G0);
	DDX_Control(pDX, IDC_LAYERSHADE_EDITB0, m_B0);
	DDX_Control(pDX, IDC_LAYERSHADE_EDITR1, m_R1);
	DDX_Control(pDX, IDC_LAYERSHADE_EDITG1, m_G1);
	DDX_Control(pDX, IDC_LAYERSHADE_EDITB1, m_B1);
	DDX_Control(pDX, IDC_LAYERSHADE_EDITR2, m_R2);
	DDX_Control(pDX, IDC_LAYERSHADE_EDITG2, m_G2);
	DDX_Control(pDX, IDC_LAYERSHADE_EDITB2, m_B2);
	DDX_Control(pDX, IDC_LAYERSHADE_EDITR3, m_R3);
	DDX_Control(pDX, IDC_LAYERSHADE_EDITG3, m_G3);
	DDX_Control(pDX, IDC_LAYERSHADE_EDITB3, m_B3);
	DDX_Control(pDX, IDC_LAYERSHADE_COUNTEDIT, m_Count);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGUILayerShade, CDialog)
	//{{AFX_MSG_MAP(CGUILayerShade)
	ON_EN_CHANGE(IDC_LAYERSHADE_EDITB0, OnChangeLayershadeEdit)
	ON_EN_CHANGE(IDC_LAYERSHADE_EDITB1, OnChangeLayershadeEdit)
	ON_EN_CHANGE(IDC_LAYERSHADE_EDITG0, OnChangeLayershadeEdit)
	ON_EN_CHANGE(IDC_LAYERSHADE_EDITG1, OnChangeLayershadeEdit)
	ON_EN_CHANGE(IDC_LAYERSHADE_EDITR0, OnChangeLayershadeEdit)
	ON_EN_CHANGE(IDC_LAYERSHADE_EDITR1, OnChangeLayershadeEdit)
	ON_EN_CHANGE(IDC_LAYERSHADE_EDITB2, OnChangeLayershadeEdit)
	ON_EN_CHANGE(IDC_LAYERSHADE_EDITB3, OnChangeLayershadeEdit)
	ON_EN_CHANGE(IDC_LAYERSHADE_EDITG2, OnChangeLayershadeEdit)
	ON_EN_CHANGE(IDC_LAYERSHADE_EDITG3, OnChangeLayershadeEdit)
	ON_EN_CHANGE(IDC_LAYERSHADE_EDITR2, OnChangeLayershadeEdit)
	ON_EN_CHANGE(IDC_LAYERSHADE_EDITR3, OnChangeLayershadeEdit)
	ON_EN_CHANGE(IDC_LAYERSHADE_COUNTEDIT, OnChangeLayershadeEdit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGUILayerShade message handlers
void	CGUILayerShade::OnOK() 
{// Overide the OK close dialog (GRR)
		theApp.GetCurrent()->FocusView();
}	

/////////////////////////////////////////////////////////////////////////////
void	CGUILayerShade::SetVal(CEdit &Dlg,int &Val)
{
CString	Str;
		if (!Dlg) return;
		SetFlag=true;
		Str.Format("%i",Val); 
		Dlg.SetWindowText(Str); 
		SetFlag=false;
}

/////////////////////////////////////////////////////////////////////////////
void	CGUILayerShade::GetVal(CEdit &Dlg,int &Val)
{
CString	Str;
		if (!Dlg) return;
		
		Dlg.GetWindowText(Str); 
		if (Str.GetLength()) 
			Val=atoi(Str);
		else
			Val=0;
}

/////////////////////////////////////////////////////////////////////////////
void	CGUILayerShade::LimitVal(int Min,int Max,CEdit &Dlg)
{
int		Val;
		if (!Dlg) return;
DWORD	Sel=Dlg.GetSel();	// Retain cursor pos :o), Im getting good at this!

		GetVal(Dlg,Val);
		if (Val<Min) Val=Min;
		if (Val>Max) Val=Max;

		SetVal(Dlg,Val);

		Dlg.SetSel(Sel);

}
/////////////////////////////////////////////////////////////////////////////
void	CGUILayerShade::SetRGB(RGBQUAD &RGB,int Set)
{
int		Val;

		Val=RGB.rgbRed;		SetVal(*Map[Set][0],Val); 
		Val=RGB.rgbGreen;	SetVal(*Map[Set][1],Val);
		Val=RGB.rgbBlue;	SetVal(*Map[Set][2],Val);
}

/////////////////////////////////////////////////////////////////////////////
void	CGUILayerShade::GetRGB(RGBQUAD &RGB,int Set)
{
int		Val;
		GetVal(*Map[Set][0],Val);	RGB.rgbRed=Val;		
		GetVal(*Map[Set][1],Val);	RGB.rgbGreen=Val;	
		GetVal(*Map[Set][2],Val);	RGB.rgbBlue=Val;	
}

/////////////////////////////////////////////////////////////////////////////
void	CGUILayerShade::CheckRGB(int Set)
{
		LimitVal(0,255,*Map[Set][0]);
		LimitVal(0,255,*Map[Set][1]);
		LimitVal(0,255,*Map[Set][2]);
}

/////////////////////////////////////////////////////////////////////////////
void	CGUILayerShade::SetStatus(int Set,bool f)
{
		Map[Set][0]->SetReadOnly(f);
		Map[Set][1]->SetReadOnly(f);
		Map[Set][2]->SetReadOnly(f);
}

/////////////////////////////////////////////////////////////////////////////
void	CGUILayerShade::CheckData()
{
int		i;
int		Count;

		SetFlag=true;
		LimitVal(2,4,m_Count);
		GetCount(Count);
		for (i=0; i<4; i++) 
		{
			CheckRGB(i);
			SetStatus(i,i>=Count);
		}

		SetFlag=false;
}

/////////////////////////////////////////////////////////////////////////////

void CGUILayerShade::OnChangeLayershadeEdit() 
{
	if (SetFlag) return;

	if (theApp.GetCurrent() && m_R0)
	{
		CheckData();
		theApp.GetCurrent()->GUIChanged();
	}	
}

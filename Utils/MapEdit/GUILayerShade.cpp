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
}


void CGUILayerShade::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGUILayerShade)
	DDX_Control(pDX, IDC_LAYERSHADE_GFX_YSPIN, m_GfxPosYSpin);
	DDX_Control(pDX, IDC_LAYERSHADE_GFX_XSPIN, m_GfxPosXSpin);
	DDX_Control(pDX, IDC_LAYERSHADE_DEF_LIST, m_DefList);
	DDX_Control(pDX, IDC_LAYERSHADE_SHADECOUNT_SPIN, m_ShadeCountSpin);
	DDX_Control(pDX, IDC_LAYERSHADE_SHADECOUNT, m_ShadeCount);
	DDX_Control(pDX, IDC_LAYERSHADE_SHADER0, m_ShadeR0);
	DDX_Control(pDX, IDC_LAYERSHADE_SHADEG0, m_ShadeG0);
	DDX_Control(pDX, IDC_LAYERSHADE_SHADEB0, m_ShadeB0);
	DDX_Control(pDX, IDC_LAYERSHADE_SHADER1, m_ShadeR1);
	DDX_Control(pDX, IDC_LAYERSHADE_SHADEG1, m_ShadeG1);
	DDX_Control(pDX, IDC_LAYERSHADE_SHADEB1, m_ShadeB1);
	DDX_Control(pDX, IDC_LAYERSHADE_SHADER2, m_ShadeR2);
	DDX_Control(pDX, IDC_LAYERSHADE_SHADEG2, m_ShadeG2);
	DDX_Control(pDX, IDC_LAYERSHADE_SHADEB2, m_ShadeB2);
	DDX_Control(pDX, IDC_LAYERSHADE_SHADER3, m_ShadeR3);
	DDX_Control(pDX, IDC_LAYERSHADE_SHADEG3, m_ShadeG3);
	DDX_Control(pDX, IDC_LAYERSHADE_SHADEB3, m_ShadeB3);
	DDX_Control(pDX, IDC_LAYERSHADE_GFX_CURRENT_SPIN, m_GfxCurrentSpin);
	DDX_Control(pDX, IDC_LAYERSHADE_GFX_CURRENT, m_GfxCurrent);
	DDX_Control(pDX, IDC_LAYERSHADE_GFX_LIST, m_GfxBankList);
	DDX_Control(pDX, IDC_LAYERSHADE_GFX_X, m_GfxPosX);
	DDX_Control(pDX, IDC_LAYERSHADE_GFX_Y, m_GfxPosY);
	DDX_Control(pDX, IDC_LAYERSHADE_GFX_SPIN_X0, m_GfxSpinx0);
	DDX_Control(pDX, IDC_LAYERSHADE_GFX_SPIN_Y0, m_GfxSpiny0);
	DDX_Control(pDX, IDC_LAYERSHADE_GFX_X0, m_Gfxx0);
	DDX_Control(pDX, IDC_LAYERSHADE_GFX_Y0, m_Gfxy0);
	DDX_Control(pDX, IDC_LAYERSHADE_GFXR0, m_GfxR0);
	DDX_Control(pDX, IDC_LAYERSHADE_GFXG0, m_GfxG0);
	DDX_Control(pDX, IDC_LAYERSHADE_GFXB0, m_GfxB0);
	DDX_Control(pDX, IDC_LAYERSHADE_GFX_SPIN_X1, m_GfxSpinx1);
	DDX_Control(pDX, IDC_LAYERSHADE_GFX_SPIN_Y1, m_GfxSpiny1);
	DDX_Control(pDX, IDC_LAYERSHADE_GFX_X1, m_Gfxx1);
	DDX_Control(pDX, IDC_LAYERSHADE_GFX_Y1, m_Gfxy1);
	DDX_Control(pDX, IDC_LAYERSHADE_GFXR1, m_GfxR1);
	DDX_Control(pDX, IDC_LAYERSHADE_GFXG1, m_GfxG1);
	DDX_Control(pDX, IDC_LAYERSHADE_GFXB1, m_GfxB1);
	DDX_Control(pDX, IDC_LAYERSHADE_GFX_SPIN_X2, m_GfxSpinx2);
	DDX_Control(pDX, IDC_LAYERSHADE_GFX_SPIN_Y2, m_GfxSpiny2);
	DDX_Control(pDX, IDC_LAYERSHADE_GFX_X2, m_Gfxx2);
	DDX_Control(pDX, IDC_LAYERSHADE_GFX_Y2, m_Gfxy2);
	DDX_Control(pDX, IDC_LAYERSHADE_GFXR2, m_GfxR2);
	DDX_Control(pDX, IDC_LAYERSHADE_GFXG2, m_GfxG2);
	DDX_Control(pDX, IDC_LAYERSHADE_GFXB2, m_GfxB2);
	DDX_Control(pDX, IDC_LAYERSHADE_GFX_SPIN_X3, m_GfxSpinx3);
	DDX_Control(pDX, IDC_LAYERSHADE_GFX_SPIN_Y3, m_GfxSpiny3);
	DDX_Control(pDX, IDC_LAYERSHADE_GFX_X3, m_Gfxx3);
	DDX_Control(pDX, IDC_LAYERSHADE_GFX_Y3, m_Gfxy3);
	DDX_Control(pDX, IDC_LAYERSHADE_GFXR3, m_GfxR3);
	DDX_Control(pDX, IDC_LAYERSHADE_GFXG3, m_GfxG3);
	DDX_Control(pDX, IDC_LAYERSHADE_GFXB3, m_GfxB3);
	DDX_Control(pDX, IDC_LAYERSHADE_GFX_TRANSLIST, m_GfxTransList);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CGUILayerShade, CDialog)
	//{{AFX_MSG_MAP(CGUILayerShade)
	ON_EN_CHANGE(IDC_LAYERSHADE_SHADECOUNT, OnChangeParam)
	ON_BN_CLICKED(IDC_LAYERSHADE_GFX_GOTO, OnGoto)
	ON_BN_CLICKED(IDC_LAYERSHADE_GFX_DELETE, OnDelete)
	ON_EN_CHANGE(IDC_LAYERSHADE_GFX_CURRENT, OnChangeCurrent)
	ON_EN_CHANGE(IDC_LAYERSHADE_SHADER0, OnChangeParam)
	ON_EN_CHANGE(IDC_LAYERSHADE_SHADEG0, OnChangeParam)
	ON_EN_CHANGE(IDC_LAYERSHADE_SHADEB0, OnChangeParam)
	ON_EN_CHANGE(IDC_LAYERSHADE_SHADER1, OnChangeParam)
	ON_EN_CHANGE(IDC_LAYERSHADE_SHADEG1, OnChangeParam)
	ON_EN_CHANGE(IDC_LAYERSHADE_SHADEB1, OnChangeParam)
	ON_EN_CHANGE(IDC_LAYERSHADE_SHADER2, OnChangeParam)
	ON_EN_CHANGE(IDC_LAYERSHADE_SHADEG2, OnChangeParam)
	ON_EN_CHANGE(IDC_LAYERSHADE_SHADEB2, OnChangeParam)
	ON_EN_CHANGE(IDC_LAYERSHADE_SHADER3, OnChangeParam)
	ON_EN_CHANGE(IDC_LAYERSHADE_SHADEG3, OnChangeParam)
	ON_EN_CHANGE(IDC_LAYERSHADE_SHADEB3, OnChangeParam)
	ON_EN_CHANGE(IDC_LAYERSHADE_GFX_XSPIN, OnChangeParam)
	ON_EN_CHANGE(IDC_LAYERSHADE_GFX_X, OnChangeParam)
	ON_EN_CHANGE(IDC_LAYERSHADE_GFX_YSPIN, OnChangeParam)
	ON_EN_CHANGE(IDC_LAYERSHADE_GFX_Y, OnChangeParam)
	ON_EN_CHANGE(IDC_LAYERSHADE_GFX_X0, OnChangeParam)
	ON_EN_CHANGE(IDC_LAYERSHADE_GFX_Y0, OnChangeParam)
	ON_EN_CHANGE(IDC_LAYERSHADE_GFXR0, OnChangeParam)
	ON_EN_CHANGE(IDC_LAYERSHADE_GFXG0, OnChangeParam)
	ON_EN_CHANGE(IDC_LAYERSHADE_GFXB0, OnChangeParam)
	ON_EN_CHANGE(IDC_LAYERSHADE_GFX_X1, OnChangeParam)
	ON_EN_CHANGE(IDC_LAYERSHADE_GFX_Y1, OnChangeParam)
	ON_EN_CHANGE(IDC_LAYERSHADE_GFXR1, OnChangeParam)
	ON_EN_CHANGE(IDC_LAYERSHADE_GFXG1, OnChangeParam)
	ON_EN_CHANGE(IDC_LAYERSHADE_GFXB1, OnChangeParam)
	ON_EN_CHANGE(IDC_LAYERSHADE_GFX_X2, OnChangeParam)
	ON_EN_CHANGE(IDC_LAYERSHADE_GFX_Y2, OnChangeParam)
	ON_EN_CHANGE(IDC_LAYERSHADE_GFXR2, OnChangeParam)
	ON_EN_CHANGE(IDC_LAYERSHADE_GFXG2, OnChangeParam)
	ON_EN_CHANGE(IDC_LAYERSHADE_GFXB2, OnChangeParam)
	ON_EN_CHANGE(IDC_LAYERSHADE_GFX_X3, OnChangeParam)
	ON_EN_CHANGE(IDC_LAYERSHADE_GFX_Y3, OnChangeParam)
	ON_EN_CHANGE(IDC_LAYERSHADE_GFXR3, OnChangeParam)
	ON_EN_CHANGE(IDC_LAYERSHADE_GFXG3, OnChangeParam)
	ON_EN_CHANGE(IDC_LAYERSHADE_GFXB3, OnChangeParam)
	ON_CBN_SELCHANGE(IDC_LAYERSHADE_GFX_LIST, OnChangeParam)
	ON_CBN_SELCHANGE(IDC_LAYERSHADE_GFX_TRANSLIST, OnChangeParam)
	ON_CBN_SELCHANGE(IDC_LAYERSHADE_DEF_LIST, OnNew)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGUILayerShade message handlers
void	CGUILayerShade::OnOK() 
{// Overide the OK close dialog (GRR)
		theApp.GetCurrent()->FocusView();
}	

/////////////////////////////////////////////////////////////////////////////
void	CGUILayerShade::SetVal(CEdit &Dlg,int &Val,int Min,int Max)
{
CString	Str;
bool	CF=CallbackFlag;
		if (!Dlg) return;
		if (Min!=-1 && Val<Min) Val=Min;
		if (Max!=-1 && Val>Max) Val=Max;
		CallbackFlag=false;
		Str.Format("%i",Val); 
		Dlg.SetWindowText(Str); 
		CallbackFlag=CF;
}

/////////////////////////////////////////////////////////////////////////////
void	CGUILayerShade::GetVal(CEdit &Dlg,int &Val,int Min,int Max)
{
CString	Str;
		Val=0;
		if (!Dlg) return;
		Dlg.GetWindowText(Str); 
		if (Str.GetLength()) 
		{
			Val=atoi(Str);
			if (Min!=-1 && Val<Min) {Val=Min;SetVal(Dlg,Val,Min,Max);}
			if (Max!=-1 && Val>Max) {Val=Max;SetVal(Dlg,Val,Min,Max);}
		}
}

/////////////////////////////////////////////////////////////////////////////
void	CGUILayerShade::SetRGB(CEdit &DlgR,CEdit &DlgG,CEdit &DlgB,u8 &R,u8 &G,u8 &B)
{
int		iR,iG,iB;
		
		iR=R;
		iG=G;
		iB=B;

		SetVal(DlgR,iR,0,255);
		SetVal(DlgG,iG,0,255);
		SetVal(DlgB,iB,0,255);

		R=iR;
		G=iG;
		B=iB;

}

/////////////////////////////////////////////////////////////////////////////
void	CGUILayerShade::GetRGB(CEdit &DlgR,CEdit &DlgG,CEdit &DlgB,u8 &R,u8 &G,u8 &B)
{
int		iR,iG,iB;
		
		iR=R;
		iG=G;
		iB=B;
	
		GetVal(DlgR,iR,0,255);
		GetVal(DlgG,iG,0,255);
		GetVal(DlgB,iB,0,255);
	
		R=iR;
		G=iG;
		B=iB;

}

/////////////////////////////////////////////////////////////////////////////
void	CGUILayerShade::SetShadeRGB(int idx,u8 &R,u8 &G,u8 &B)
{
		switch(idx)
		{
		case 0: SetRGB(m_ShadeR0,m_ShadeG0,m_ShadeB0,R,G,B); break;
		case 1: SetRGB(m_ShadeR1,m_ShadeG1,m_ShadeB1,R,G,B); break;
		case 2: SetRGB(m_ShadeR2,m_ShadeG2,m_ShadeB2,R,G,B); break;
		case 3: SetRGB(m_ShadeR3,m_ShadeG3,m_ShadeB3,R,G,B); break;
		}
}

/////////////////////////////////////////////////////////////////////////////
void	CGUILayerShade::GetShadeRGB(int idx,u8 &R,u8 &G,u8 &B)
{
		switch(idx)
		{
		case 0: GetRGB(m_ShadeR0,m_ShadeG0,m_ShadeB0,R,G,B); break;
		case 1: GetRGB(m_ShadeR1,m_ShadeG1,m_ShadeB1,R,G,B); break;
		case 2: GetRGB(m_ShadeR2,m_ShadeG2,m_ShadeB2,R,G,B); break;
		case 3: GetRGB(m_ShadeR3,m_ShadeG3,m_ShadeB3,R,G,B); break;
		}
}

/////////////////////////////////////////////////////////////////////////////
void	CGUILayerShade::SetGfxRGB(int idx,u8 &R,u8 &G,u8 &B)
{
		switch(idx)
		{
		case 0: SetRGB(m_GfxR0,m_GfxG0,m_GfxB0,R,G,B); break;
		case 1: SetRGB(m_GfxR1,m_GfxG1,m_GfxB1,R,G,B); break;
		case 2: SetRGB(m_GfxR2,m_GfxG2,m_GfxB2,R,G,B); break;
		case 3: SetRGB(m_GfxR3,m_GfxG3,m_GfxB3,R,G,B); break;
		}
}

/////////////////////////////////////////////////////////////////////////////
void	CGUILayerShade::GetGfxRGB(int idx,u8 &R,u8 &G,u8 &B)
{
		switch(idx)
		{
		case 0: GetRGB(m_GfxR0,m_GfxG0,m_GfxB0,R,G,B); break;
		case 1: GetRGB(m_GfxR1,m_GfxG1,m_GfxB1,R,G,B); break;
		case 2: GetRGB(m_GfxR2,m_GfxG2,m_GfxB2,R,G,B); break;
		case 3: GetRGB(m_GfxR3,m_GfxG3,m_GfxB3,R,G,B); break;
		}
}

/////////////////////////////////////////////////////////////////////////////
void CGUILayerShade::OnChangeParam()				{if (CallbackFlag) theApp.GetCurrent()->GUIChanged();}
void CGUILayerShade::OnGoto()						{theApp.GetCurrent()->Command(CmdMsg_ShadeGfxGoto);}
void CGUILayerShade::OnDelete()						{theApp.GetCurrent()->Command(CmdMsg_ShadeGfxDelete);}
void CGUILayerShade::OnChangeCurrent()				{if (CallbackFlag) theApp.GetCurrent()->Command(CmdMsg_ShadeGfxSelect);}
void CGUILayerShade::OnNew()						{theApp.GetCurrent()->Command(CmdMsg_ShadeGfxNew,m_DefList.GetCurSel());}

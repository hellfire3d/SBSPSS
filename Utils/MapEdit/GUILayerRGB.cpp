// GUILayerRGB.cpp : implementation file
//

#include "stdafx.h"
#include "mapedit.h"
#include "GUILayerRGB.h"

#include	"MapEditDoc.h"
#include	"MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGUILayerRGB dialog


CGUILayerRGB::CGUILayerRGB(CWnd* pParent /*=NULL*/)
	: CDialog(CGUILayerRGB::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGUILayerRGB)
	//}}AFX_DATA_INIT
}


void CGUILayerRGB::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGUILayerRGB)
	DDX_Control(pDX, IDC_LAYERSHADE_SHADEBSPIN, m_BSpin);
	DDX_Control(pDX, IDC_LAYERSHADE_SHADEB, m_B);
	DDX_Control(pDX, IDC_LAYERSHADE_SHADEGSPIN, m_GSpin);
	DDX_Control(pDX, IDC_LAYERSHADE_SHADEG, m_G);
	DDX_Control(pDX, IDC_LAYERSHADE_SHADERSPIN, m_RSpin);
	DDX_Control(pDX, IDC_LAYERSHADE_SHADER, m_R);
	DDX_Control(pDX, IDC_LAYERSHADE_RATELIST, m_RateList);
	DDX_Control(pDX, IDC_LAYERSHADE_BRUSHLIST, m_BrushList);
	DDX_Control(pDX, IDC_LAYERSHADE_MODELIST, m_ModeList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGUILayerRGB, CDialog)
	//{{AFX_MSG_MAP(CGUILayerRGB)
	ON_CBN_SELCHANGE(IDC_LAYERSHADE_MODELIST, OnParamChange)
	ON_BN_CLICKED(IDC_LAYERSHADE_TRIFILTER, OnLayershadeTrifilter)
	ON_BN_CLICKED(IDC_LAYERSHADE_BIFILTER, OnLayershadeBifilter)
	ON_CBN_SELCHANGE(IDC_LAYERSHADE_BRUSHLIST, OnParamChange)
	ON_CBN_SELCHANGE(IDC_LAYERSHADE_RATELIST, OnParamChange)
	ON_EN_CHANGE(IDC_LAYERSHADE_SHADER, OnParamChange)
	ON_EN_CHANGE(IDC_LAYERSHADE_SHADEG, OnParamChange)
	ON_EN_CHANGE(IDC_LAYERSHADE_SHADEB, OnParamChange)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
void	CGUILayerRGB::SetVal(CEdit &Dlg,int &Val,int Min,int Max)
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
void	CGUILayerRGB::GetVal(CEdit &Dlg,int &Val,int Min,int Max)
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
void	CGUILayerRGB::SetRGB(u8 &R,u8 &G,u8 &B)
{

int		iR,iG,iB;
		
		iR=R;
		iG=G;
		iB=B;

		SetVal(m_R,iR,0,255);
		SetVal(m_G,iG,0,255);
		SetVal(m_B,iB,0,255);

		R=iR;
		G=iG;
		B=iB;

}

/////////////////////////////////////////////////////////////////////////////
void	CGUILayerRGB::GetRGB(u8 &R,u8 &G,u8 &B)
{

int		iR,iG,iB;
		
		iR=R;
		iG=G;
		iB=B;
	
		GetVal(m_R,iR,0,255);
		GetVal(m_G,iG,0,255);
		GetVal(m_B,iB,0,255);
	
		R=iR;
		G=iG;
		B=iB;

}

/////////////////////////////////////////////////////////////////////////////
void CGUILayerRGB::OnParamChange()				{if (CallbackFlag) theApp.GetCurrent()->GUIChanged();}
void CGUILayerRGB::OnLayershadeBifilter()		{theApp.GetCurrent()->Command(CmdMsg_Filter,0);}
void CGUILayerRGB::OnLayershadeTrifilter()		{theApp.GetCurrent()->Command(CmdMsg_Filter,1);}



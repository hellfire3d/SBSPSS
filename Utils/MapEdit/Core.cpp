/***********************/
/*** Map Editor Core ***/
/***********************/


#include	"stdafx.h"
#include	"gl3d.h"
#include	<gl\gl.h>
#include	<gl\glu.h>
#include	<gl\glut.h>
#include	"GLEnabledView.h"

#include	"MapEdit.h"
#include	"MapEditDoc.h"
#include	"MapEditView.h"
#include	"MainFrm.h"

#include	"Core.h"
#include	"Layer.h"
#include	"LayerBack.h"
#include	"LayerMid.h"
#include	"LayerAction.h"
#include	"LayerFore.h"


BOOL	Test3dFlag=TRUE;

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
CCore::CCore()
{
	Layers[LAYER_TYPE_BACK]=	new CLayerBack(this);
	Layers[LAYER_TYPE_MID]=		new CLayerMid(this);
	Layers[LAYER_TYPE_ACTION]=	new CLayerAction(this);
	Layers[LAYER_TYPE_FORE]=	new CLayerFore(this);

	TileViewFlag=0;
	LayerViewFlag=1;
}

/*****************************************************************************/
CCore::~CCore()
{
int	i;
	for (i=0; i<LAYER_TYPE_MAX; i++) delete Layers[i];
}

/*****************************************************************************/
void	CCore::Init(CMapEditView *Wnd)
{
	ParentWindow=Wnd;
	ActiveLayer=LAYER_TYPE_ACTION;
	MapCam=Vec(0,0,0);
	TileCam=Vec(0,0,0);
	UpdateView();
	RenderFlag=TRUE;

	TileSet.push_back(CTileSet("c:/temp/3/test.gin",this));
}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
void	CCore::Render()
{
Vec		&ThisCam=GetCam();

		if (RenderFlag)
		{
			RenderFlag=FALSE;
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			if (GetTileView())
			{
	
			}
			else
			{
				for (int i=0;i<LAYER_TYPE_MAX;i++)
				{
					Layers[i]->Render(ThisCam,Test3dFlag);
				}
			}
		Layers[ActiveLayer]->RenderGrid(ThisCam);
		}
// Calc CursorPos
		Layers[ActiveLayer]->FindCursorPos(ThisCam,CurrentMousePos);

}


/*****************************************************************************/
/*** Control *****************************************************************/
/*****************************************************************************/
void	CCore::LButtonControl(UINT nFlags, CPoint &point,BOOL DownFlag)
{
}

/*****************************************************************************/
void	CCore::MButtonControl(UINT nFlags, CPoint &point,BOOL DownFlag)
{
		LastMousePos=point;
}

/*****************************************************************************/
void	CCore::RButtonControl(UINT nFlags, CPoint &point,BOOL DownFlag)
{
}

/*****************************************************************************/
void	CCore::MouseWheel(UINT nFlags, short zDelta, CPoint &pt) 
{
		if (zDelta>0)
			UpdateView(Vec(0,0,1.0f));
		else
			UpdateView(Vec(0,0,-1.0f));
}

/*****************************************************************************/
void	CCore::MouseMove(UINT nFlags, CPoint &point) 
{
Vec		Ofs(0,0,0);

Vec		&ThisCam=GetCam();
// check if active doc
		if (theApp.GetCurrent()!=ParentWindow->GetDocument()) return;

		CurrentMousePos=point;

// Handle Drag Movement
		if (nFlags & MK_MBUTTON)
			{
			float	XS,YS;
			RECT	ThisRect;

			ParentWindow->GetWindowRect(&ThisRect);
			XS=ThisCam.z*4;//*Layers[ActiveLayer]->GetLayerZPos();
			YS=ThisCam.z*4;//*Layers[ActiveLayer]->GetLayerZPos();
			XS/=((ThisRect.right-ThisRect.left));
			YS/=((ThisRect.bottom-ThisRect.top));
	
			Ofs.x=LastMousePos.x-CurrentMousePos.x;
			Ofs.y=LastMousePos.y-CurrentMousePos.y;
			LastMousePos=CurrentMousePos;
	
			Ofs.x*=XS;
			Ofs.y*=YS;

			UpdateView(Ofs);
			}
// Mouse has moved, so need to redraw windows, to get CursorPos (And pos render)
		ParentWindow->Invalidate();

}

/*****************************************************************************/
/*** Layers ******************************************************************/
/*****************************************************************************/
void	CCore::UpdateLayerBar(BOOL ViewFlag)
{
CMainFrame	*Frm=(CMainFrame*)AfxGetApp()->GetMainWnd();
CToolBar	*ToolBar=Frm->GetToolBar();
CDialogBar	*LayerBar=Frm->GetLayerBar();
CListBox	*Dlg=(CListBox *)LayerBar->GetDlgItem(IDC_LAYERBAR_LIST);

		LayerViewFlag=ViewFlag;
		if (LayerViewFlag)
		{
			Dlg->ResetContent();

			for (int i=0;i<LAYER_TYPE_MAX;i++)
			{
				CLayer	*ThisLayer=GetLayer(i);
				Dlg->AddString(ThisLayer->GetName());
			}
			Dlg->SetCurSel(ActiveLayer);
		}

		ToolBar->GetToolBarCtrl().PressButton(ID_TOOLBAR_LAYERBAR,LayerViewFlag);
		Frm->ShowControlBar(LayerBar, LayerViewFlag, FALSE);	
}

/*****************************************************************************/
void	CCore::SetActiveLayer(int i)
{
	UpdateLayerBar(LayerViewFlag);
}


/*****************************************************************************/
/*** TileBank ****************************************************************/
/*****************************************************************************/
void	CCore::UpdateTileView(BOOL ViewFlag)
{
CMainFrame	*Frm=(CMainFrame*)AfxGetApp()->GetMainWnd();
CToolBar	*ToolBar=Frm->GetToolBar();

			TileViewFlag=ViewFlag;
			ToolBar->GetToolBarCtrl().PressButton(ID_TOOLBAR_TILEPALETTE,TileViewFlag);
			UpdateView();
}

/*****************************************************************************/
GLint	CCore::GetTile(int Bank,int TileNo)
{
	return(TileSet[Bank].GetTile(TileNo));
}
/*****************************************************************************/
/*** Misc ********************************************************************/
/*****************************************************************************/
Vec		&CCore::GetCam()
{
	if (GetTileView())
		return(TileCam);
	else
		return(MapCam);

}

/*****************************************************************************/
void	CCore::UpdateAll()
{
	UpdateView();
	UpdateLayerBar(LayerViewFlag);
	UpdateTileView(TileViewFlag);
}

/*****************************************************************************/
void	CCore::Redraw(BOOL f)
{
	RenderFlag=f;
	if (RenderFlag)
		ParentWindow->Invalidate();
}

/*****************************************************************************/
void	CCore::UpdateView(Vec Ofs)
{
Vec		&ThisCam=GetCam();

		Ofs.y=-Ofs.y;
		ThisCam+=Ofs;
		if (ThisCam.z>-1) ThisCam.z=-1;
		Redraw();
}		

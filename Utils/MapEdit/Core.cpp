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

#include	"Core.h"
#include	"Layer.h"
#include	"LayerBack.h"
#include	"LayerMid.h"
#include	"LayerAction.h"
#include	"LayerFore.h"


/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
CCore::CCore()
{
	MouseMode=MOUSE_MODE_NONE;
	Layers[LAYER_TYPE_BACK]=	new CLayerBack;
	Layers[LAYER_TYPE_MID]=		new CLayerMid;
	Layers[LAYER_TYPE_ACTION]=	new CLayerAction;
	Layers[LAYER_TYPE_FORE]=	new CLayerFore;
}

/*****************************************************************************/
CCore::~CCore()
{
	for (int i=0; i<LAYER_TYPE_MAX; i++) delete Layers[i];
}

/*****************************************************************************/
void	CCore::Init(CMapEditView *Wnd)
{
	ParentWindow=Wnd;

	ActiveLayer=0;
	MapPos.x=MapPos.y=MapPos.z=0;


}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
void	CCore::Render()
{
	
	for (int i=0;i<LAYER_TYPE_MAX;i++)
	{
		Layers[i]->Render(MapPos);
	}
}


/*****************************************************************************/
void	CCore::UpdateView(float XOfs,float YOfs,float ZOfs)
{
		MapPos=MapPos+Vec(XOfs,YOfs,ZOfs);
		if (MapPos.z>-1) MapPos.z=-1;

		ParentWindow->Invalidate();
}

/*****************************************************************************/
/*** Control *****************************************************************/
/*****************************************************************************/
void	CCore::LButtonControl(UINT nFlags, CPoint &point,BOOL DownFlag)
{
//		if (!(nFlags & (MK_MBUTTON | MK_RBUTTON))) Layers[ActiveLayer].MouseMsg(nFlags,point);
}

/*****************************************************************************/
void	CCore::MButtonControl(UINT nFlags, CPoint &point,BOOL DownFlag)
{
//		if (!(nFlags & (MK_LBUTTON | MK_RBUTTON))) Layers[ActiveLayer].MouseMsg(nFlags,point);
}

/*****************************************************************************/
void	CCore::RButtonControl(UINT nFlags, CPoint &point,BOOL DownFlag)
{
//		if (!(nFlags & (MK_LBUTTON | MK_MBUTTON))) Layers[ActiveLayer].MouseMsg(nFlags,point);
}

/*****************************************************************************/
void	CCore::MouseWheel(UINT nFlags, short zDelta, CPoint &pt) 
{
		if (zDelta>0)
			UpdateView(0,0,1.0f);
		else
			UpdateView(0,0,-1.0f);
}

/*****************************************************************************/
void	CCore::MouseMove(UINT nFlags, CPoint &point) 
{
float	XOfs=0;
float	YOfs=0;

// check if active doc
		if (theApp.GetCurrent()!=ParentWindow->GetDocument()) return;

//		Layers[ActiveLayer].MouseMsg(nFlags,point);
/*
// Handle Movement
		CurrentMousePos=point;
			{
			float	XS,YS;
			RECT	ThisRect;

			ParentWindow->GetWindowRect(&ThisRect);

			XS=MapPos.z/((ThisRect.right-ThisRect.left));
			YS=MapPos.z/((ThisRect.bottom-ThisRect.top));
	
			XOfs=LastMousePos.x-CurrentMousePos.x;
			YOfs=LastMousePos.y-CurrentMousePos.y;
			LastMousePos=CurrentMousePos;
	
			XOfs*=XS;
			YOfs*=YS;

//		TRACE2("Move %i %i,",ThisRect.left,ThisRect.top);
//		TRACE2("Move %i %i \n",ThisRect.right,ThisRect.bottom);
			}
		UpdateView(-XOfs,-YOfs,0);
//		if (nFlags & MK_LBUTTON) LButtonControl(nFlags,point,TRUE);
//		if (nFlags & MK_RBUTTON) RButtonControl(nFlags,point,TRUE);


		TRACE2("Move %i %i \n",point.x,point.y);
*/
		MapPos.x+=0.01f;
		UpdateView(0.01f,0,0);
//Render();
}

/*****************************************************************************/
/*** Layer Code **************************************************************/
/*****************************************************************************/
void	CCore::LayerSetActive(int i)
{
}

/*****************************************************************************/
int		CCore::LayerGetActive()
{
	return(ActiveLayer);
}

/*****************************************************************************/
CLayer	*CCore::LayerGet(int i)
{
	return(Layers[i]);
}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/



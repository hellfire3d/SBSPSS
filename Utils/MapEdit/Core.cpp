/***********************/
/*** Map Editor Core ***/
/***********************/


#include	"stdafx.h"
#include	"gl3d.h"
#include	<gl\gl.h>
#include	<gl\glu.h>
#include	<gl\glut.h>
#include	"GLEnabledView.h"

#include	"MapEditDoc.h"
#include	"MapEditView.h"

#include	"Core.h"
#include	"Layer.h"


/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
CCore::CCore()
{
	MouseMode=MOUSE_MODE_NONE;

}

/*****************************************************************************/
CCore::~CCore()
{
}

/*****************************************************************************/
void	CCore::Init(CMapEditView *Wnd)
{
	ParentWindow=Wnd;
//	ParentWindow->SetCapture();
//	ParentWindow->SetCapture();
	
//	TestLayer.Init();
//	UpdateView();

}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
void	CCore::Render()
{
	TRACE0("HERE");
	Layers[ActiveLayer].Render();

//	if (RenderMode & RENDER_MODE_GFX) TestLayer.Render();
//	if (RenderMode & RENDER_MODE_POS) TestLayer.UpdateCursor(this);
//	RenderMode=0;
}


/*****************************************************************************/
void	CCore::UpdateView(float XOfs,float YOfs,float ZOfs)
{
//		RenderMode|= RENDER_MODE_POS;
//		RenderMode|= RENDER_MODE_GFX;
//		ViewPos=ViewPos+Vec(XOfs,YOfs,ZOfs);
//		if (ViewPos.z>-1) ViewPos.z=-1;

//		ParentWindow->Redraw();
}

/*****************************************************************************/
/*** Control *****************************************************************/
/*****************************************************************************/
void	CCore::SetMouseMode(MOUSE_MODE CurrentMode,MOUSE_MODE NewMode)
{
	if (MouseMode==CurrentMode) 
		{
//		ReleaseCapture();
		MouseMode=NewMode;
//		if (MouseMode!=MOUSE_MODE_NONE) ParentWindow->SetCapture();	// Set new capture
		}
}

/*****************************************************************************/
void	CCore::LButtonControl(UINT nFlags, CPoint &point,BOOL DownFlag)
{
//	TestLayer.LButtonControl(nFlags,point,DownFlag);
	SetMouseMode(MOUSE_MODE_NONE,MOUSE_MODE_LMB);
	TRACE0("LMB\n");
}

/*****************************************************************************/
void	CCore::MButtonControl(UINT nFlags, CPoint &point,BOOL DownFlag)
{
	if (DownFlag) 
		SetMouseMode(MOUSE_MODE_NONE,MOUSE_MODE_MMB);
	else
		SetMouseMode(MOUSE_MODE_MMB,MOUSE_MODE_NONE);
	TRACE0("MMB\n");
/*
	if (DownFlag) 
	{
		LastMousePos=point;
		ParentWindow->SetCapture();
	}
	else
	{
		ReleaseCapture();
	}
*/
}

/*****************************************************************************/
void	CCore::RButtonControl(UINT nFlags, CPoint &point,BOOL DownFlag)
{
//	TestLayer.RButtonControl(nFlags,point,DownFlag);
	SetMouseMode(MOUSE_MODE_NONE,MOUSE_MODE_RMB);
	TRACE0("RMB\n");
}

/*****************************************************************************/
void	CCore::MouseWheel(UINT nFlags, short zDelta, CPoint &pt) 
{
	if (zDelta<0)
		{
		UpdateView(0,0,+1.0f);
		}
	if (zDelta>0)
		{
		UpdateView(0,0,-1.0f);
		}
}

/*****************************************************************************/
void	CCore::MouseMove(UINT nFlags, CPoint &point,BOOL CaptureFlag) 
{
float	XOfs=0;
float	YOfs=0;

		CurrentMousePos=point;

		if (CaptureFlag)
			{
			float	XS,YS;
			RECT	ThisRect;

			ParentWindow->GetWindowRect(&ThisRect);
	
			XS=ViewPos.z/((ThisRect.right-ThisRect.left));
			YS=ViewPos.z/((ThisRect.bottom-ThisRect.top));
	
			XOfs=LastMousePos.x-CurrentMousePos.x;
			YOfs=LastMousePos.y-CurrentMousePos.y;
			LastMousePos=CurrentMousePos;
	
			XOfs*=XS;
			YOfs*=YS;
			}
		UpdateView(-XOfs,-YOfs,0);
//		if (nFlags & MK_LBUTTON) LButtonControl(nFlags,point,TRUE);
//		if (nFlags & MK_RBUTTON) RButtonControl(nFlags,point,TRUE);

		TRACE3("Move %i %i %i \n",point.x,point.y,CaptureFlag);
}

/*****************************************************************************/
/*** Layer Code **************************************************************/
/*****************************************************************************/
void	CCore::LayerAdd(char *Name)
{
CLayer	NewLayer;

	if (!Name)
	{
		char	DynName[32];
		sprintf(DynName,"Layer%2d",Layers.size()+1);
		Name=DynName;
	}

	NewLayer.SetName(Name);
//	strcpy(NewLayer.Name,Name);
	TRACE1("New Layer [%s]\n",Name);
	Layers.push_back(NewLayer);

}

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
int		CCore::LayerGetCount()
{
	return(Layers.size());
}

/*****************************************************************************/
CLayer	&CCore::LayerGet(int i)
{
	return(Layers[i]);
}


/*****************************************************************************/
void	CCore::LayerDelete(int Layer)
{
	Layers.erase(Layers.begin() + Layer);		
	TRACE0("Delete Layer\n");
}

/*****************************************************************************/
void	CCore::LayerMoveUp(int Layer)
{
CLayer	Tmp;

	Tmp=Layers[Layer];
	Layers[Layer]=Layers[Layer-1];
	Layers[Layer-1]=Tmp;

	TRACE0("Layer Up \n");
}

/*****************************************************************************/
void	CCore::LayerMoveDown(int Layer)
{
CLayer	Tmp;

	Tmp=Layers[Layer];
	Layers[Layer]=Layers[Layer+1];
	Layers[Layer+1]=Tmp;

	TRACE0("Layer Down\n");

}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/



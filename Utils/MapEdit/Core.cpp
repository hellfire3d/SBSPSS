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


/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
CCore::CCore()
{

}
/*****************************************************************************/
CCore::~CCore()
{
}

/*****************************************************************************/
void	CCore::Init(CMapEditView *Wnd)
{
	ParentWindow=Wnd;
//	TestLayer.Init();
//	UpdateView();

}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
void	CCore::Render()
{
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
void	CCore::LButtonControl(UINT nFlags, CPoint &point,BOOL DownFlag)
{
//	TestLayer.LButtonControl(nFlags,point,DownFlag);
}

/*****************************************************************************/
void	CCore::MButtonControl(UINT nFlags, CPoint &point,BOOL DownFlag)
{
	if (DownFlag) 
	{
		LastMousePos=point;
		ParentWindow->SetCapture();
	}
	else
	{
		ReleaseCapture();
	}

}

/*****************************************************************************/
void	CCore::RButtonControl(UINT nFlags, CPoint &point,BOOL DownFlag)
{
//	TestLayer.RButtonControl(nFlags,point,DownFlag);
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
void	CCore::MouseMove(UINT nFlags, CPoint &Point,BOOL CaptureFlag) 
{
float	XOfs=0;
float	YOfs=0;

		CurrentMousePos=Point;

		if (CaptureFlag)
			{
			float	XS,YS;
			RECT	ThisRect;

			ParentWindow->GetWindowRect(&ThisRect);
	
			XS=ViewPos.z/((ThisRect.right-ThisRect.left));
			YS=ViewPos.z/((ThisRect.bottom-ThisRect.top));
	
			XOfs=LastMousePos.x-CurrentMousePos.x;
			YOfs=LastMousePos.y-CurrentMousePos.y;
			LastMousePos=Point;
	
			XOfs*=XS;
			YOfs*=YS;
			}
		UpdateView(-XOfs,-YOfs,0);
		if (nFlags & MK_LBUTTON) LButtonControl(nFlags,Point,TRUE);
		if (nFlags & MK_RBUTTON) RButtonControl(nFlags,Point,TRUE);

}

/*****************************************************************************/
/*** Layer Code **************************************************************/
/*****************************************************************************/
void	CCore::LayerAdd(char *Name)
{
sLayer	NewLayer;

	if (!Name)
	{
		char	DynName[32];
		sprintf(DynName,"Layer%2d",Layers.size()+1);
		Name=DynName;
	}

	strcpy(NewLayer.Name,Name);
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
sLayer	const	&CCore::LayerGet(int i)
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
sLayer	Tmp;

	Tmp=Layers[Layer];
	Layers[Layer]=Layers[Layer-1];
	Layers[Layer-1]=Tmp;

	TRACE0("Layer Up \n");
}

/*****************************************************************************/
void	CCore::LayerMoveDown(int Layer)
{
sLayer	Tmp;

	Tmp=Layers[Layer];
	Layers[Layer]=Layers[Layer+1];
	Layers[Layer+1]=Tmp;

	TRACE0("Layer Down\n");

}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/



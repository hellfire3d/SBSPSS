/***********************/
/*** Map Editor Core ***/
/***********************/

#ifndef	__CORE_HEADER__
#define	__CORE_HEADER__

#include	<Vector>
#include	"gl3d.h"
#include	"Layer.h"
#include	"LayerBack.h"


/*****************************************************************************/
enum MOUSE_MODE
{
	MOUSE_MODE_NONE=0,
	MOUSE_MODE_LMB_EDIT,
	MOUSE_MODE_MMB_EDIT,
	MOUSE_MODE_RMB_EDIT,
	
};


/*****************************************************************************/
class	CMapEditView;
class	CCore
{
public:
		CCore();
		~CCore();
// Control
		void			LButtonControl(UINT nFlags, CPoint &point,BOOL DownFlag);
		void			MButtonControl(UINT nFlags, CPoint &point,BOOL DownFlag);
		void			RButtonControl(UINT nFlags, CPoint &point,BOOL DownFlag);
		void			MouseWheel(UINT nFlags, short zDelta, CPoint &pt);
		void			MouseMove(UINT nFlags, CPoint &point);

// Blah
		void			Init(CMapEditView *Wnd);
		void			Render();
		void			UpdateView(float XOfs,float YOfs,float ZOfs);
		void			SetMouseMode(MOUSE_MODE CurrentMode,MOUSE_MODE NewMode);		

// Layers
		void			LayerSetActive(int Layer);
		int				LayerGetActive();
		CLayer			*LayerGet(int i);


private:
		CMapEditView	*ParentWindow;	
		MOUSE_MODE		MouseMode;
		CPoint			CurrentMousePos,LastMousePos;
		Vec				MapPos;

		CLayer				*Layers[LAYER_TYPE_MAX];
		int					ActiveLayer;


};

/*****************************************************************************/
#endif

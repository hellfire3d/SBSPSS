/***********************/
/*** Map Editor Core ***/
/***********************/

#ifndef	__CORE_HEADER__
#define	__CORE_HEADER__

#include	<Vector>
#include	"gl3d.h"
#include	"Layer.h"


/*****************************************************************************/
enum MOUSE_MODE
{
	MOUSE_MODE_NONE=0,
	MOUSE_MODE_LMB,
	MOUSE_MODE_MMB,
	MOUSE_MODE_RMB,
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
		void			MouseMove(UINT nFlags, CPoint &point, BOOL CaptureFlag);

// Blah
		void			Init(CMapEditView *Wnd);
		void			Render();
		void			UpdateView(float XOfs,float YOfs,float ZOfs);
		void			SetMouseMode(MOUSE_MODE CurrentMode,MOUSE_MODE NewMode);		

// Layers
		void			LayerAdd(char *Name=0);
		void			LayerSetActive(int Layer);
		int				LayerGetActive();
		int				LayerGetCount();
		CLayer			&LayerGet(int i);
		void			LayerDelete(int Layer);
		void			LayerMoveUp(int Layer);
		void			LayerMoveDown(int Layer);



private:
		CMapEditView	*ParentWindow;	
		MOUSE_MODE		MouseMode;
		CPoint			CurrentMousePos,LastMousePos;
		Vec				ViewPos;

		
		std::vector<CLayer>	Layers;
		int					ActiveLayer;


};

/*****************************************************************************/
#endif

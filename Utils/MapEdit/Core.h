/***********************/
/*** Map Editor Core ***/
/***********************/

#ifndef	__CORE_HEADER__
#define	__CORE_HEADER__

#include	<Vector>
#include	"gl3d.h"


/*****************************************************************************/
//struct	sLayer;
struct	sLayer
{
	char	Name[32];
//	sLayer	*Next,*Prev;
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
		

// Layers
		void			LayerAdd(char *Name=0);
		void			LayerSetActive(int Layer);
		int				LayerGetActive();
		int				LayerGetCount();
		sLayer	const	&LayerGet(int i);
		void			LayerDelete(int Layer);
		void			LayerMoveUp(int Layer);
		void			LayerMoveDown(int Layer);



private:
		CMapEditView	*ParentWindow;	
		CPoint			CurrentMousePos,LastMousePos;
		Vec				ViewPos;

		std::vector<sLayer>	Layers;
		int					ActiveLayer;


};

/*****************************************************************************/
#endif

/******************/
/*** Layer Core ***/
/******************/

#ifndef	__LAYER_HEADER__
#define	__LAYER_HEADER__

#include	<Vector>
#include	"gl3d.h"


/*****************************************************************************/
class	CLayer
{
public:
		CLayer();
		~CLayer();
// Blah
		void			Init();
		void			Render();

		void			SetName(char *Str);
		char			*GetName()			{return(Name);}
		
// Control
//		void			LButtonControl(UINT nFlags, CPoint &point,BOOL DownFlag);
//		void			MButtonControl(UINT nFlags, CPoint &point,BOOL DownFlag);
//		void			RButtonControl(UINT nFlags, CPoint &point,BOOL DownFlag);
//		void			MouseWheel(UINT nFlags, short zDelta, CPoint &pt);
//		void			MouseMove(UINT nFlags, CPoint &point, BOOL CaptureFlag);


private:
		char			Name[32];

		float			LayerWidth,LayerHeight,LayerDepth;
		float			LayerX,LayerY,LayerZ;


};

/*****************************************************************************/
#endif

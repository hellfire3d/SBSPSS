/***************/
/*** Fire FX ***/
/***************/

#ifndef	__FX_FX_FIRE_HEADER__
#define __FX_FX_FIRE_HEADER__

#include "fx/fxbaseanim.h"

/*****************************************************************************/
class CFXFire : public CFXBaseAnim
{
public:
		void		init(DVECTOR const &Pos);
		void		render();
		void		SetSize(int W,int H)	{SprW=W; SprH=H;}
protected:
		
		int			SprW,SprH;
};

#endif

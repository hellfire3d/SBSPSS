/******************/
/*** Energy Bar ***/
/******************/

#ifndef	__FX_FX_NRG_BAR_HEADER__
#define __FX_FX_NRG_BAR_HEADER__

#include "fx/fx.h"

/*****************************************************************************/
class CFXNRGBar : public CFX
{
public:

/*virtual*/ void		init(DVECTOR const &Pos);
/*virtual*/ void		think(int _frames);
/*virtual*/ void		render();

		void		SetMax(int Max);
		bool		alwaysThink()								{return(true);}

protected:
		int			MaxHealth;
		int			CurrentHealth;
};

#endif

/*****************/
/*** Thwack!!! ***/
/*****************/

#ifndef	__FX_FX_THWACK_HEADER__
#define __FX_FX_THWACK_HEADER__

#include "fx/fx.h"

/*****************************************************************************/
class CFXThwack : public CFX
{
public:
/*virtual*/ void		init(DVECTOR const &Pos);
/*virtual*/ void		render();

protected:
		u16			Angle,Scale;
};

#endif

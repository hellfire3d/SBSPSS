/***********************/
/*** Anim Base Class ***/
/***********************/

#ifndef	__FX_FX_SMOKE_PUFF_HEADER__
#define __FX_FX_SMOKE_PUFF_HEADER__

#include "fx/fx.h"

/*****************************************************************************/
class CFXSmokePuff : public CFX
{
public:

virtual void		init(DVECTOR const &Pos);
virtual void		think(int _frames);
virtual void		render();

protected:
		POLY_FT4	*Frame;

		s32			CurrentScale;
		s32			CurrentAngle;
		s16			RGBDec;
};

#endif

/*************/
/*** Smoke ***/
/*************/

#ifndef	__FX_FX_SMOKE_HEADER__
#define __FX_FX_SMOKE_HEADER__

#include "fx/fx.h"

/*****************************************************************************/
class CFXSmoke : public CFX
{
public:

/*virtual*/ void		init(DVECTOR const &Pos);
/*virtual*/ void		think(int _frames);

		void		setRate(int R)		{Rate=R;}

protected:
		int			Rate;
		int			CurrentRate;
};

class CFXSmokePuff : public CFX
{
public:

/*virtual*/ void		init(DVECTOR const &Pos);
/*virtual*/ void		think(int _frames);
/*virtual*/ void		render();

protected:
		POLY_FT4	*Frame;

		int			CurrentScale,ScaleInc;
		int			CurrentAngle,AngleInc;
};

#endif

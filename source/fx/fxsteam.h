/**********************/
/*** JellyFish Legs ***/
/**********************/

#ifndef	__FX_FX_CLOUD_HEADER__
#define __FX_FX_CLOUD_HEADER__

#include "fx/fx.h"

/*****************************************************************************/
class CFXCloud : public CFX
{
public:
		struct	sList
		{
			DVECTOR	Ofs;
		};
		enum
		{
			MAX_TRAIL = 16
		};

virtual void		init(DVECTOR const &Pos);
virtual void		shutdown();
virtual void		think(int _frames);
virtual void		render();

protected:
		sList		List[MAX_TRAIL];
		int			ListCount;
		int			HeadIdx;
		int			Angle;
};

#endif

/**********************/
/*** JellyFish Legs ***/
/**********************/

#ifndef	__FX_FX_JELLYFISH_LEGS_HEADER__
#define __FX_FX_JELLYFISH_LEGS_HEADER__

#include "fx/fx.h"

/*****************************************************************************/
class CFXJellyFishLegs : public CFX
{
public:

virtual void		init(DVECTOR const &Pos);
virtual void		shutdown();
virtual void		think(int _frames);
virtual void		render();

		void		Setup(int XOfs,int YOfs,bool XFlip);

protected:
		DVECTOR		Ofs;

		int			Angle,AngleInc;
		bool		XFlip;

};

#endif

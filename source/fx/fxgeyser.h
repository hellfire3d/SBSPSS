/***********************/
/*** Anim Base Class ***/
/***********************/

#ifndef	__FX_FX_GEYSER_HEADER__
#define __FX_FX_GEYSER_HEADER__

#include "fx/fx.h"
#include "fx/fxbaseAnim.h"

/*****************************************************************************/
class CFXGeyser : public CFX
{
public:
/*virtual */void		init(DVECTOR const &Pos);
/*virtual */void		think(int Frames);
/*virtual */void		render();
		void		shutdown();

		void		setHeight(int H)	{TargetHeight=H;}

protected:
		s16			TargetHeight,Height;
		u16			currentFrame;
		int			SoundId;
		DVECTOR		BasePos;
};

#endif

/***********************/
/*** Anim Base Class ***/
/***********************/

#ifndef	__FX_FX_ANIM_HEADER__
#define __FX_FX_ANIM_HEADER__

#include "fx/fx.h"

/*****************************************************************************/
class CFXAnim : public CFX
{
public:

virtual void		init(DVECTOR const &Pos);
virtual void		shutdown();
virtual void		think(int _frames);
virtual void		render();
virtual	void		SetScale(int S)		{Scale=S;}
virtual	void		SetFrame(int Base,int Count,int Shift=0);

protected:
		s16			BaseFrame,Frame;
		s16			MaxFrame;
		s16			FrameShift;

		s16			Scale;
		u8			R,G,B;
};

#endif

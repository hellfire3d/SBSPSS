/*****************/
/*** Bubble FX ***/
/*****************/

#ifndef	__FX_FX_BUBBLE_HEADER__
#define __FX_FX_BUBBLE_HEADER__

#include "fx/fxbaseanim.h"

/*****************************************************************************/
class CFXBubble : public CFXBaseAnim
{
public:
virtual void		init(DVECTOR const &Pos);
virtual void		think(int _frames);
virtual	void		killFX();

protected:
		s8			Die;
		u16			XIdx;
};

#endif

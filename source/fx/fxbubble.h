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
		void		init(DVECTOR const &Pos);
		void		think(int _frames);

protected:
		u16			XIdx;
};

#endif

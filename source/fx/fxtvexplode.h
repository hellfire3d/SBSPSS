/*************/
/*** Laser ***/
/*************/

#ifndef	__FX_FX_TV_EXPLODE_HEADER__
#define __FX_FX_TV_EXPLODE_HEADER__

#include "fx/fx.h"

/*****************************************************************************/
class CFXTVExplode : public CFX
{
public:
		void		init(DVECTOR const &Pos);
		void		think(int _frames);

};

#endif

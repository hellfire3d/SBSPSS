/*************/
/*** Steam ***/
/*************/

#ifndef	__FX_FX_STEAM_HEADER__
#define __FX_FX_STEAM_HEADER__

#include "fx/fx.h"
#include "fx/fxcloud.h"

/*****************************************************************************/
class CFXSteam : public CFXCloud
{
public:
virtual void		init(DVECTOR const &Pos);
virtual void		shutdown();
virtual void		think(int _frames);

		void		SetSize(int Size);
protected:
		int			Life;
		DVECTOR		Vel;
};

#endif

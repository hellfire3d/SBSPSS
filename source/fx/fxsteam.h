/*************/
/*** Steam ***/
/*************/

#ifndef	__FX_FX_STEAM_HEADER__
#define __FX_FX_STEAM_HEADER__

#include "fx/fx.h"
#include "fx/fxtrail.h"

/*****************************************************************************/
class CFXSteam : public CFXTrail
{
public:
virtual void		init(DVECTOR const &Pos);
virtual void		shutdown();
virtual void		think(int _frames);

		void		SetSize(int Size);
virtual	void		setDie();

protected:
		DVECTOR		BaseVel;
		s16			Angle;
		s16			ScaleInc;
		s16			ShadeDec;

};

#endif

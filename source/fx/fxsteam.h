/*************/
/*** Steam ***/
/*************/

#ifndef	__FX_FX_STEAM_HEADER__
#define __FX_FX_STEAM_HEADER__

#include "fx/fx.h"
#include "fx/fxbasetrail.h"

/*****************************************************************************/
class CFXSteam : public CFXBaseTrail
{
public:
virtual void		init(DVECTOR const &Pos);

virtual void		think(int _frames);
virtual	void		killFX();

		void		SetSize(int Size);
		void		SetVel(DVECTOR const &Velocity);
		void		SetHorizontal(bool newHorizontal)		{IsHorizontal=newHorizontal;}

protected:
		DVECTOR		BaseVel;
		s16			ScaleInc;
		s16			ShadeDec;
		bool		DieOut;
		bool		IsHorizontal;

};

#endif

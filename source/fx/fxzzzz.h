/*************/
/*** Laser ***/
/*************/

#ifndef	__FX_FX_LASER_HEADER__
#define __FX_FX_LASER_HEADER__

#include "fx/fx.h"

/*****************************************************************************/
class CFXLaser : public CFX
{
public:
		void		init(DVECTOR const &Pos);
		void		think(int _frames);
		void		render();
/*virtual */int			canCollide()					{return true;}
/*virtual */int			checkCollisionAgainst(CThing *_thisThing, int _frames);

		void		setOffset(DVECTOR &Pos);
		void		setTarget(DVECTOR &Pos);

		void		setRGB(u8 r,u8 g,u8 b)	{R=r; G=g; B=g;}

protected:
/*virtual */void		collidedWith(CThing *_thisThing);
		DVECTOR		Offset,Target;
		u8			R,G,B;
};

#endif

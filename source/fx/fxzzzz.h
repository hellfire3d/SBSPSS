/**********************/
/*** ZZZZ (for FMA) ***/
/**********************/

#ifndef	__FX_FX_ZZZZ_HEADER__
#define __FX_FX_ZZZZ_HEADER__

#include "fx/fx.h"

/*****************************************************************************/
class CFXZZZZ : public CFX
{
public:
		enum
		{
			Z_COUNT=8,
		};
		struct	sZ
		{
			DVECTOR	Ofs;
			s16		TablePos;
			s16		Life;
		};

		void		init(DVECTOR const &Pos);
		void		think(int _frames);
		void		render();

		void		killFX()	{DieFlag=1;}

protected:
		sZ			Z[Z_COUNT];
		s16			Count;
		u8			DieFlag;
};

#endif

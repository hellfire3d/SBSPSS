/**********************/
/*** JellyFish Legs ***/
/**********************/

#ifndef	__FX_FX_JELLYFISH_LEGS_HEADER__
#define __FX_FX_JELLYFISH_LEGS_HEADER__

#include "fx/fx.h"

/*****************************************************************************/
class CFXJellyFishLegs : public CFX
{
public:
		struct	sList
		{
			DVECTOR	Ofs;
		};

virtual void		init();
virtual void		shutdown();
virtual void		think(int _frames);
virtual void		render();

		void		SetUp(int _Width,int _Gap,int _Length,int _Count);
//virtual	CRECT const	&getCollisionArea()						{return (Parent->getCollisionArea());}

protected:

		int			XOfs;
		int			Width,Gap;
		int			Length,Count;

		sList		**FXList;
		int			ListIdx;
		s16			*WidthTable;
		s16			*HeightTable;
};

#endif

/***********************/
/*** Trail Base Class ***/
/***********************/

#ifndef	__FX_FX_BASE_TRAIL_HEADER__
#define __FX_FX_BASE_TRAIL_HEADER__

#include "fx/fx.h"

/*****************************************************************************/
class CFXBaseTrail : public CFX
{
public:
		struct sFXData
		{
			s16		Scale;
			s16		StartFrame,EndFrame;
			s16		FrameShift;
			u8		R,G,B;
		};

		struct	sList
		{
			u16		Frame;
			u16		Scale,Angle;
			DVECTOR	Ofs,Vel;
			s16		Shade;	// Acts as life
		};
		enum
		{
			LIST_SIZE = 16
		};

/*virtual */void		init(DVECTOR const &Pos);
/*virtual */void		think(int _frames);
/*virtual */void		render();

/*virtual	*/sList		&moveHead();

protected:
		sList		List[LIST_SIZE];
		s16			ListCount;
		s16			HeadIdx;
		u16			Trans;
};

#endif

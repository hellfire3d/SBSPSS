/***********************/
/*** Anim Base Class ***/
/***********************/

#ifndef	__FX_FX_BASE_ANIM_HEADER__
#define __FX_FX_BASE_ANIM_HEADER__

#include "fx/fx.h"

/*****************************************************************************/
class CFXBaseAnim : public CFX
{
public:
	struct sFXBaseData
	{
		s16				StartFrame,EndFrame,FrameShift;
		u16				Flags;
	};

virtual void		init(DVECTOR const &Pos);
virtual void		think(int _frames);
virtual void		render();

		void		setBaseData(void *Data);

virtual	void		SetScale(int S)			{CurrentScaleX=CurrentScaleY=S;}
virtual void		SetScaleX(int S)		{CurrentScaleX=S;}
virtual void		SetScaleY(int S)		{CurrentScaleY=S;}
virtual void		SetHeading(int H)		{CurrentHeading=H;}

protected:

		sFXBaseData	*BaseData;
		POLY_FT4	*Frame;

		s16			MaxFrame;
		s16			CurrentFrame;
		u16			renderFrame;
		s32			CurrentScaleX;
		s32			CurrentScaleY;
		s16			CurrentHeading;
};

#endif

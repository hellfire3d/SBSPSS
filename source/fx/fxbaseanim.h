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
	struct sFXData
	{
		s16		Scale;
		s16		StartFrame,EndFrame;
		s16		FrameShift;
		u8		R,G,B;
	};

virtual void		init(DVECTOR const &Pos);
virtual void		shutdown();
virtual void		think(int _frames);
virtual void		render();
		void		setData(void *Data)		{DataPtr=(sFXData*)Data;}

virtual	void		SetScale(int S)			{CurrentScale=S;}

protected:
		sFXData		*DataPtr;

		s16			CurrentFrame;
		s16			CurrentScale;
};

#endif

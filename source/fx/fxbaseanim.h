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
	struct sFXAnim
	{
		s16				Scale;
		s16				StartFrame,EndFrame;
		s16				FrameShift;
		u8				R,G,B;
		u8				Flags;
		DVECTOR			Velocity;
		u16				EndFX;
	};
	enum FXAnim_Flags
	{
		FXANIM_FLAG_LOOP			=1<<0,
		FXANIM_FLAG_COLLIDE_KILL	=1<<1,
		FXANIM_FLAG_HAS_GRAVITY		=1<<2,
		FXANIM_FLAG_INJURE_PLAYER	=1<<3,
		FXANIM_FLAG_KILL_PLAYER		=1<<4,
	};

virtual void		init(DVECTOR const &Pos);
virtual void		shutdown();
virtual void		think(int _frames);
virtual void		render();
virtual	void		killFX();
virtual int			canCollide()					{return true;}

		void		setData(void *Data)		{DataPtr=(sFXAnim*)Data;}

virtual	void		SetScale(int S)			{CurrentScaleX=CurrentScaleY=S;}
virtual void		SetScaleX(int S)		{CurrentScaleX=S;}
virtual void		SetScaleY(int S)		{CurrentScaleY=S;}
virtual void		SetHeading(int H)		{CurrentHeading=H;}

protected:
virtual void		collidedWith(CThing *_thisThing);

		sFXAnim		*DataPtr;
		s16			VelY;
		s16			MaxFrame;
		s16			CurrentFrame;
		u16			renderFrame;
		s32			CurrentScaleX;
		s32			CurrentScaleY;
		s16			CurrentHeading;
};

#endif

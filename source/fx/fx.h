/*********************/
/*** FX Base Class ***/
/*********************/

#ifndef	__FX_FX_HEADER__
#define __FX_FX_HEADER__

#include "thing/thing.h"

/*****************************************************************************/
enum FX_FLAG
{
	FX_FLAG_LOOP			=1<<0,
	FX_FLAG_COLLIDE_KILL	=1<<1,
	FX_FLAG_HAS_GRAVITY		=1<<2,
	FX_FLAG_INJURE_PLAYER	=1<<3,
	FX_FLAG_TRANS			=1<<4,
	FX_FLAG_COLLIDE_BOUNCE	=1<<5,
	FX_FLAG_HAS_LIFE		=1<<6,

	FX_FLAG_NO_THINK_KILL	=1<<13,
	FX_FLAG_HIDDEN			=1<<14,
	FX_FLAG_SCREEN_FX		=1<<15,
};

class CFX : public CFXThing
{
public:
		enum	FX_TYPE
		{
			FX_TYPE_NONE=0,

			FX_TYPE_BASE_ANIM,

			FX_TYPE_DROP_WATER,
			FX_TYPE_DROP_ACID,
			FX_TYPE_DROP_LAVA,
			FX_TYPE_DROP_OIL,

			FX_TYPE_SPLASH_WATER,
			FX_TYPE_SPLASH_ACID,
			FX_TYPE_SPLASH_LAVA,
			FX_TYPE_SPLASH_OIL,

			FX_TYPE_BUBBLE_WATER,
			FX_TYPE_BUBBLE_ACID,
			FX_TYPE_BUBBLE_LAVA,
			FX_TYPE_BUBBLE_OIL,

			FX_TYPE_GEYSER_WATER,
			FX_TYPE_GEYSER_ACID,
			FX_TYPE_GEYSER_LAVA,
			FX_TYPE_GEYSER_OIL,

			FX_TYPE_THWACK,
			FX_TYPE_LIGHTNING_BOLT,

			FX_TYPE_STEAM,
			FX_TYPE_GAS,
			FX_TYPE_FLAMES,

			FX_TYPE_SMOKE,
			FX_TYPE_SMOKE_PUFF,

			FX_TYPE_JELLYFISH_LEGS,
			FX_TYPE_FALLINGTILE,
			FX_TYPE_EXPLODE,
			FX_TYPE_NRG_BAR,
			FX_TYPE_LASER,
			FX_TYPE_TV_EXPLODE,

			FX_TYPE_MAX
		};
		enum
		{ // For Dynamic ThingCache
			MAX_SUBTYPE	=FX_TYPE_MAX,
		};

		struct	sFXRGB
		{
			u8	R,G,B;
		};
		enum	FX_RGB
		{
			FX_RGB_WATER,
			FX_RGB_ACID,
			FX_RGB_LAVA,
			FX_RGB_OIL,
			FX_RGB_MAX
		};

		CFX()
		{// Sorry, I need this :o(
			RGB.R=RGB.G=RGB.B=127;
			AfterEffect=FX_TYPE_NONE;
		};

static	CFX			*Create(const FX_TYPE Type);
static	CFX			*Create(const FX_TYPE Type,CThing *Parent);
static	CFX			*Create(const FX_TYPE Type,DVECTOR const &Pos);

virtual	bool		alwaysThink()								{return(Flags & FX_FLAG_SCREEN_FX);}
virtual	void		leftThinkZone(int _frames)					{if (Flags & FX_FLAG_NO_THINK_KILL) killFX();}

virtual void		init();
virtual void		init(DVECTOR const &Pos);
virtual void		shutdown();
virtual void		think(int _frames);
virtual void		render();

virtual int			canCollide()					{return(Flags & FX_FLAG_INJURE_PLAYER);}
virtual	void		SetOtPos(int Ot)				{OtPos=Ot;}
virtual	void		setLife(int L)					{Life=L;}

virtual	void		getFXRenderPos(DVECTOR &Pos);
virtual	bool		getFXParentPos(DVECTOR &Pos);

virtual	void		killFX();
virtual void		toggleVisible()					{Flags ^=FX_FLAG_HIDDEN;}

protected:
		void		collidedWith(CThing *_thisThing);

// Def Data
public:
		virtual void		setData(void *Data){}
		virtual	void		setBaseData(void *Data);

		void		setRGB(u8 R,u8 G,u8 B)			{RGB.R=R; RGB.G=G; RGB.B=B;}
		void		setRGB(sFXRGB &rgb)				{setRGB(rgb.R,rgb.G,rgb.B);}
		void		setRGB(FX_RGB T)				{setRGB(FXRGBTable[T]);}

		void		setAfterEffect(FX_TYPE Type)	{AfterEffect=Type;}
		void		setVelocity(int X,int Y)		{Velocity.vx=X; Velocity.vy=Y;}

//protected:
		s16			Life;
		u16			Flags;
		sFXRGB		RGB;
		FX_TYPE		AfterEffect;
		DVECTOR		Velocity;

		s32			OtPos;
static	sFXRGB		FXRGBTable[FX_RGB_MAX];

};

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
void	TestFX(DVECTOR Pos,CThing *Parent);

#endif

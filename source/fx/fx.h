/*********************/
/*** FX Base Class ***/
/*********************/

#ifndef	__FX_FX_HEADER__
#define __FX_FX_HEADER__

#include "thing/thing.h"

/*****************************************************************************/
class SpriteBank;
class CFX : public CFXThing
{
public:
		enum	FX_TYPE
		{
			FX_TYPE_JELLYFISH_LEGS,
			FX_TYPE_FALLINGTILE,
			FX_TYPE_BUBBLE,
				FX_TYPE_BUBBLE_WATER,
				FX_TYPE_BUBBLE_ACID,
				FX_TYPE_BUBBLE_LAVA,
				FX_TYPE_BUBBLE_OIL,
			FX_TYPE_LIGHTNING_BOLT,
			FX_TYPE_LIGHTNING_SHEET,
			FX_TYPE_LIGHTNING_BLAST,
			FX_TYPE_LIGHTNING_RADIAL,
			FX_TYPE_LIGHTNING_PROJECTILE,
			FX_TYPE_SHOCKWAVE,
			FX_TYPE_DAZE,
			FX_TYPE_DROP,			
				FX_TYPE_DROP_WATER,
				FX_TYPE_DROP_ACID,
				FX_TYPE_DROP_LAVA,
				FX_TYPE_DROP_OIL,
			FX_TYPE_SPLASH,			
				FX_TYPE_SPLASH_WATER,
				FX_TYPE_SPLASH_ACID,
				FX_TYPE_SPLASH_LAVA,
				FX_TYPE_SPLASH_OIL,
			FX_TYPE_CASCADE,		
			FX_TYPE_CASCADE_SPLASH,	
			FX_TYPE_FIREBALL,
			FX_TYPE_STEAM,
			FX_TYPE_SMOKE,
			FX_TYPE_GAS,
			FX_TYPE_FLAMES,
			FX_TYPE_EXPLODE,
			FX_TYPE_DEBRIS,
		};

static	CFX			*Create(const FX_TYPE Type);
static	CFX			*Create(const FX_TYPE Type,CThing *Parent);
static	CFX			*Create(const FX_TYPE Type,DVECTOR const &Pos);

virtual void		init();
virtual void		init(DVECTOR const &Pos){};
virtual void		shutdown();
virtual void		think(int _frames);
virtual void		render();

virtual int			canCollide()					{return false;}
virtual	void		SetOtPos(int Ot)				{OtPos=Ot;}
virtual	void		setDie()						{DieOut=true;}
virtual	void		setLife(int L)					{Life=L;}

protected:
		SpriteBank	*m_spriteBank;
		s32			OtPos;

		u8			DieOut;
		s16			Life;

};

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
void	TestFX(DVECTOR Pos);

#endif

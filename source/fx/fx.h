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
			FX_TYPE_CLOUD,
				FX_TYPE_CLOUD_STEAN,
				FX_TYPE_CLOUD_SMOKE,
				FX_TYPE_CLOUD_GAS,
			FX_TYPE_FLAMES,
			FX_TYPE_EXPLODE,
			FX_TYPE_DEBRIS,
		};

static	CFX			*Create(const FX_TYPE Type,CThing *Parent);

virtual void		init();
virtual void		shutdown();
virtual void		think(int _frames);
virtual void		render();

virtual int			canCollide()					{return false;}

protected:
	
	SpriteBank	*m_spriteBank;
};

#endif

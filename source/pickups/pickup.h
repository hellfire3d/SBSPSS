/*=========================================================================

	pickup.h

	Author:		PKG
	Created:	
	Project:	Spongebob
	Purpose:	

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef	__PICKUPS_PICKUP_H__
#define __PICKUPS_PICKUP_H__

/*----------------------------------------------------------------------
	Includes
	-------- */

#ifndef __GAME_THING_H__
#include "game/thing.h"
#endif


/*	Std Lib
	------- */

/*----------------------------------------------------------------------
	Tyepdefs && Defines
	------------------- */

typedef enum
{
	PICKUP__100_PERCENT_LIFE,
	PICKUP__50_PERCENT_LIFE,
	PICKUP__25_PERCENT_LIFE,
	PICKUP__LIFE,
	PICKUP__SPATULA,
}
PICKUP_TYPE;


/*----------------------------------------------------------------------
	Structure defintions
	-------------------- */

class CBasePickup : public CThing
{
public:
	virtual void		init();
	virtual void		shutdown();
	virtual void		think(int _frames);
	virtual void		render();

	void				setPos(const struct DVECTOR *_pos);
	virtual void		collect(class CPlayer *_player);

protected:
	class SpriteBank	*getSpriteBank()					{return m_spriteBank;}
	DVECTOR				getRenderOffset();

private:
	class SpriteBank	*m_spriteBank;

};


/*----------------------------------------------------------------------
	Globals
	------- */

/*----------------------------------------------------------------------
	Functions
	--------- */

CBasePickup	*createPickup(const PICKUP_TYPE _type,const DVECTOR *_pos);


/*---------------------------------------------------------------------- */

#endif	/* __PICKUPS_PICKUP_H__ */

/*===========================================================================
 end */

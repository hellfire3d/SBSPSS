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
	PICKUP__JELLY_LAUNCHER_AMMO,
	PICKUP__BUBBLE_MIXTURE,
	PICKUP__GLASSES,
	PICKUP__SQUEAKY_SHOES,
	PICKUP__BALLOON,
	PICKUP__HELMET,
	PICKUP__QUEST_ITEM__TEST,	// Needs to be one of these for each quest item ( I think )
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
	enum
	{
		PICKUPS_OT_POS=15,
		DEFAULT_VISIBILITY_RADIUS=32,
	};

	class SpriteBank	*getSpriteBank()					{return m_spriteBank;}
	virtual int			getVisibilityRadius()				{return DEFAULT_VISIBILITY_RADIUS;}

	virtual void		thinkPickup(int _Frames)			{;}
	virtual void		renderPickup(DVECTOR *_pos)=0;

private:
	class SpriteBank	*m_spriteBank;

};


class CBaseRespawningPickup : public CBasePickup
{
public:
	virtual void		init();
	virtual void		think(int _frames);
	virtual void		render();

	virtual void		collect(class CPlayer *_player);

protected:
	virtual int			getRespawnTime()=0;

private:
	int					m_respawnTime;

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

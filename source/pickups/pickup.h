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

#ifndef __THING_THING_H__
#include "thing/thing.h"
#endif


/*	Std Lib
	------- */

/*----------------------------------------------------------------------
	Tyepdefs && Defines
	------------------- */

typedef enum
{
	PICKUP__BIG_HEALTH,
	PICKUP__MEDIUM_HEALTH,
	PICKUP__SMALL_HEALTH,
	PICKUP__LIFE,
	PICKUP__SPATULA,
	PICKUP__JELLY_LAUNCHER_AMMO,
	PICKUP__BUBBLE_MIXTURE,
	PICKUP__BUBBLE_WAND,
	PICKUP__NET,
	PICKUP__GLASSES,
	PICKUP__SQUEAKY_SHOES,
	PICKUP__BALLOON,
	PICKUP__HELMET,
	PICKUP__CORAL_BLOWER,
	PICKUP__QUEST_ITEM__TEST,	// Needs to be one of these for each quest item ( I think )
}
PICKUP_TYPE;


/*----------------------------------------------------------------------
	Structure defintions
	-------------------- */

class CBasePickup : public CPickupThing
{
public:
	virtual void		init();
	virtual void		shutdown();
	virtual void		think(int _frames);
	virtual void		render();

	virtual DVECTOR		getSizeForPlacement()=0;
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

	virtual void		collidedWith(CThing *_thisThing);

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
	virtual int			canCollide()						{return m_respawnTime==0;}

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

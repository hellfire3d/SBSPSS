/*=========================================================================

	pbubmix.h

	Author:		PKG
	Created:	
	Project:	Spongebob
	Purpose:	

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef	__PICKUPS_PBUBMIX_H__
#define __PICKUPS_PBUBMIX_H__

/*----------------------------------------------------------------------
	Includes
	-------- */

#ifndef	__PICKUPS_PICKUP_H__
#include "pickups/pickup.h"
#endif


/*	Std Lib
	------- */

/*----------------------------------------------------------------------
	Tyepdefs && Defines
	------------------- */

/*----------------------------------------------------------------------
	Structure defintions
	-------------------- */

class CBubbleMixturePickup : public CBaseRespawningPickup
{
public:
	virtual void		init();

	virtual DVECTOR		getSizeForPlacement();
	virtual void		collect(class CPlayer *_player);

protected:
	virtual int			getRespawnTime();

	virtual void		thinkPickup(int _frames);
	virtual void		renderPickup(DVECTOR *_pos);

private:
	virtual CSoundMediator::SFXID	sfxToPlayWhenCollected(){return CSoundMediator::SFX_ITEM__JAMJAR_AMMO;}

	int					m_sin;

};


class CBubbleWandPickup : public CBaseWeaponSimplePickup
{
public:
	virtual void		init();
	virtual void		collect(class CPlayer *_player);

protected:
	virtual int			getWeaponSpriteFrame();

};


/*----------------------------------------------------------------------
	Globals
	------- */

/*----------------------------------------------------------------------
	Functions
	--------- */

/*---------------------------------------------------------------------- */

#endif	/* __PICKUPS_PBUBMIX_H__ */

/*===========================================================================
 end */

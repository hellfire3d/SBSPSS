/*=========================================================================

	pblower.h

	Author:		PKG
	Created:	
	Project:	Spongebob
	Purpose:	

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef	__PICKUPS_PBLOWER_H__
#define __PICKUPS_PBLOWER_H__

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

class CCoralBlowerPickup : public CBaseWeaponSimplePickup
{
public:
	virtual void		init();
	virtual void		collect(class CPlayer *_player);

protected:
	virtual int			getWeaponSpriteFrame();

private:
	virtual CSoundMediator::SFXID	sfxToPlayWhenCollected(){return CSoundMediator::SFX_ITEM__CORAL_BLOWER;}

};


/*----------------------------------------------------------------------
	Globals
	------- */

/*----------------------------------------------------------------------
	Functions
	--------- */

/*---------------------------------------------------------------------- */

#endif	/* __PICKUPS_PBLOWER_H__ */

/*===========================================================================
 end */

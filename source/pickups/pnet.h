/*=========================================================================

	pnet.h

	Author:		PKG
	Created:	
	Project:	Spongebob
	Purpose:	

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef	__PICKUPS_PNET_H__
#define __PICKUPS_PNET_H__

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

class CNetPickup : public CBaseWeaponSimplePickup
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

#endif	/* __PICKUPS_PNET_H__ */

/*===========================================================================
 end */

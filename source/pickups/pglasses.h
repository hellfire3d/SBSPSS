/*=========================================================================

	pglasses.h

	Author:		PKG
	Created:	
	Project:	Spongebob
	Purpose:	

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef	__PICKUPS_PGLASSES_H__
#define __PICKUPS_PGLASSES_H__

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

class CGlassesPickup : public CBasePickup
{
public:
	virtual void		render();

	virtual void		collect(class CPlayer *_player);

};


/*----------------------------------------------------------------------
	Globals
	------- */

/*----------------------------------------------------------------------
	Functions
	--------- */

/*---------------------------------------------------------------------- */

#endif	/* __PICKUPS_PGLASSES_H__ */

/*===========================================================================
 end */

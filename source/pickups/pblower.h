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

class CCoralBlowerPickup : public CBasePickup
{
public:
	virtual DVECTOR		getSizeForPlacement();
	virtual void		collect(class CPlayer *_player);

protected:
	virtual void		renderPickup(DVECTOR *_pos);

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

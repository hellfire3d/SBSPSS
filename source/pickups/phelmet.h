/*=========================================================================

	phelmet.h

	Author:		PKG
	Created:	
	Project:	Spongebob
	Purpose:	

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef	__PICKUPS_PHELMET_H__
#define __PICKUPS_PHELMET_H__

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

class CHelmetPickup : public CBasePickup
{
public:
	virtual void		init();
	virtual int			dontKillDuringLevelRespawn()							{return true;}
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

#endif	/* __PICKUPS_PHELMET_H__ */

/*===========================================================================
 end */

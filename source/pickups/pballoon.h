/*=========================================================================

	pballoon.h

	Author:		PKG
	Created:	
	Project:	Spongebob
	Purpose:	

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef	__PICKUPS_PBALLOON_H__
#define __PICKUPS_PBALLOON_H__

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

class CBalloonPickup : public CBaseRespawningPickup
{
public:
	virtual void		init();

	virtual void		collect(class CPlayer *_player);

protected:
	virtual int			getVisibilityRadius();
	virtual int			getRespawnTime();

	virtual void		thinkPickup(int _frames);
	virtual void		renderPickup(DVECTOR *_pos);

private:
	int					m_sin;

};


/*----------------------------------------------------------------------
	Globals
	------- */

/*----------------------------------------------------------------------
	Functions
	--------- */

/*---------------------------------------------------------------------- */

#endif	/* __PICKUPS_PBALLOON_H__ */

/*===========================================================================
 end */

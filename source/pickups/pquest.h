/*=========================================================================

	pquest.h

	Author:		PKG
	Created:	
	Project:	Spongebob
	Purpose:	

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef	__PICKUPS_PQUEST_H__
#define __PICKUPS_PQUEST_H__

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

class CBaseQuestItemPickup : public CBasePickup
{
public:
	virtual void		init();
	virtual void		shutdown();
	virtual void		think(int _frames);
	virtual void		render();

	virtual void		collect(class CPlayer *_player);

protected:
	virtual int			getFrameNumber()=0;

};

class CTestQuestItemPickup : public CBaseQuestItemPickup
{
protected:
	virtual int			getFrameNumber();

};


/*----------------------------------------------------------------------
	Globals
	------- */

/*----------------------------------------------------------------------
	Functions
	--------- */

/*---------------------------------------------------------------------- */

#endif	/* __PICKUPS_PQUEST_H__ */

/*===========================================================================
 end */

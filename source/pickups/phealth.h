/*=========================================================================

	phealth.h

	Author:		PKG
	Created:	
	Project:	Spongebob
	Purpose:	

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef	__PICKUPS_PHEALTH_H__
#define __PICKUPS_PHEALTH_H__

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

class CBaseHealthPickup : public CBasePickup
{
public:
	virtual void		init();
	virtual void		shutdown();

	virtual void		collect(class CPlayer *_player);

protected:
	virtual void		thinkPickup(int _frames);
	virtual void		renderPickup(DVECTOR *_pos);

	virtual int			getHealthPoints()=0;
	virtual int			getFrameNumber()=0;

private:
	int					m_sin;

};


class C100HealthPickup : public CBaseHealthPickup
{
protected:
	virtual int			getHealthPoints();
	virtual int			getFrameNumber();
};

class C50HealthPickup : public CBaseHealthPickup
{
protected:
	virtual int			getHealthPoints();
	virtual int			getFrameNumber();
};

class C25HealthPickup : public CBaseHealthPickup
{
protected:
	virtual int			getHealthPoints();
	virtual int			getFrameNumber();
};


/*----------------------------------------------------------------------
	Globals
	------- */

/*----------------------------------------------------------------------
	Functions
	--------- */

/*---------------------------------------------------------------------- */

#endif	/* __PICKUPS_PHEALTH_H__ */

/*===========================================================================
 end */

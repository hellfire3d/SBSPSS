/*=========================================================================

	phealth.cpp

	Author:		PKG
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

/*----------------------------------------------------------------------
	Includes
	-------- */

#ifndef __GFX_SPRBANK_H__
#include "gfx\sprbank.h"	// Damnit.. include order! :( (pkg)
#endif

#include "pickups\phealth.h"

#ifndef __MATHTABLE_HEADER__
#include "utils\mathtab.h"
#endif


/*	Std Lib
	------- */

/*	Data
	---- */

#ifndef __SPR_INGAMEFX_H__
#include <ingamefx.h>
#endif


/*----------------------------------------------------------------------
	Tyepdefs && Defines
	------------------- */

/*----------------------------------------------------------------------
	Structure defintions
	-------------------- */

/*----------------------------------------------------------------------
	Function Prototypes
	------------------- */

/*----------------------------------------------------------------------
	Vars
	---- */

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CBaseHealthPickup::init()
{
	CBasePickup::init();
	m_sin=0;
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CBaseHealthPickup::collect(class CPlayer *_player)
{
	CBasePickup::collect(_player);
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int health_rotatespeed=200;
int health_rotatescale=200;
void	CBaseHealthPickup::thinkPickup(int _frames)
{
	m_sin=(m_sin+(_frames*health_rotatespeed))&4095;
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CBaseHealthPickup::renderPickup(DVECTOR *_pos)
{
	SpriteBank	*sprites;
	sFrameHdr	*fh;
	int			angle;

	sprites=getSpriteBank();
	fh=sprites->getFrameHeader(getFrameNumber());
	angle=((msin(m_sin)*health_rotatescale)>>12)&4095;
	sprites->printRotatedScaledSprite(fh,_pos->vx,_pos->vy,4096,4096,angle,PICKUPS_OT_POS);
}




/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int	C100HealthPickup::getHealthPoints()
{
	return 100;
}
int	C100HealthPickup::getFrameNumber()
{
	return FRM__HEALTH100;
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int	C50HealthPickup::getHealthPoints()
{
	return 50;
}
int	C50HealthPickup::getFrameNumber()
{
	return FRM__HEALTH50;
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int	C25HealthPickup::getHealthPoints()
{
	return 25;
}
int	C25HealthPickup::getFrameNumber()
{
	return FRM__HEALTH25;
}

/*===========================================================================
end */

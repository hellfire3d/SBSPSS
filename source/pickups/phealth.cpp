dsasdasda



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

#ifndef	__PLAYER_PLAYER_H__
#include "player\player.h"
#endif

#ifndef	__GFX_OTPOS_H__
#include "gfx\otpos.h"
#endif

#include	"game/game.h"

/*	Std Lib
	------- */

/*	Data
	---- */

#ifndef __SPR_SPRITES_H__
#include <sprites.h>
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
	sFrameHdr	*fh;

	CBasePickup::init();
	m_sin=0;

	fh=CGameScene::getSpriteBank()->getFrameHeader(getFrameNumber());
	setCollisionSize(fh->W,fh->H);
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
DVECTOR	CBaseHealthPickup::getSizeForPlacement()
{
	DVECTOR		size;
	sFrameHdr	*fh;

	fh=CGameScene::getSpriteBank()->getFrameHeader(getFrameNumber());
	size.vx=fh->W;
	size.vy=fh->H;
	return size;
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CBaseHealthPickup::collect(class CPlayer *_player)
{
	_player->addHealth(getHealthPoints());
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

	sprites=CGameScene::getSpriteBank();
	fh=sprites->getFrameHeader(getFrameNumber());
	angle=((msin(m_sin)*health_rotatescale)>>12)&4095;
	sprites->printRotatedScaledSprite(fh,_pos->vx,_pos->vy,4096,4096,angle,OTPOS__PICKUP_POS);
}



/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int	CLargeHealthPickup::getHealthPoints()
{
	return 5;
}
int	CLargeHealthPickup::getFrameNumber()
{
	return FRM__HEALTH100;
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int	CMediumHealthPickup::getHealthPoints()
{
	return 3;
}
int	CMediumHealthPickup::getFrameNumber()
{
	return FRM__HEALTH50;
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int	CSmallHealthPickup::getHealthPoints()
{
	return 1;
}
int	CSmallHealthPickup::getFrameNumber()
{
	return FRM__HEALTH25;
}

/*===========================================================================
end */

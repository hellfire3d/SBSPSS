/*=========================================================================

	plife.cpp

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

#include "pickups\plife.h"

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
void	CLifePickup::init()
{
	sFrameHdr	*fh;

	CBasePickup::init();
	m_sin=0;

	fh=CGameScene::getSpriteBank()->getFrameHeader(FRM__PANTS);
	setCollisionSize(fh->W,fh->H);
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
DVECTOR	CLifePickup::getSizeForPlacement()
{
	DVECTOR		size;
	sFrameHdr	*fh;

	fh=CGameScene::getSpriteBank()->getFrameHeader(FRM__PANTS);
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
void	CLifePickup::collect(class CPlayer *_player)
{
	_player->addLife();
	CBasePickup::collect(_player);
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int life_pulsespeed=205;
int life_pulsescale=75;
void	CLifePickup::thinkPickup(int _frames)
{
	m_sin=(m_sin+(_frames*life_pulsespeed))&4095;
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CLifePickup::renderPickup(DVECTOR *_pos)
{
	SpriteBank	*sprites;
	sFrameHdr	*fh;
	int			x,y;
	int			size;

	sprites=CGameScene::getSpriteBank();
	fh=sprites->getFrameHeader(FRM__PANTS);
	x=_pos->vx-(fh->W/2);
	y=_pos->vy-(fh->H/2);
	size=256+((msin(m_sin)*life_pulsescale)>>12);
	sprites->printFT4Scaled(fh,x,y,0,0,OTPOS__PICKUP_POS,size);
}

/*===========================================================================
end */

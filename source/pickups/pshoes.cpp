/*=========================================================================

	pshoes.cpp

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

#include "pickups\pshoes.h"

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
void	CShoesPickup::init()
{
	sFrameHdr	*fh;

	CBaseRespawningPickup::init();
	m_sin=0;

	fh=CGameScene::getSpriteBank()->getFrameHeader(FRM__SHOE);
	setCollisionSize(fh->W,fh->H);
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
DVECTOR	CShoesPickup::getSizeForPlacement()
{
	DVECTOR		size;
	sFrameHdr	*fh;

	fh=CGameScene::getSpriteBank()->getFrameHeader(FRM__SHOE);
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
void	CShoesPickup::collect(class CPlayer *_player)
{
	_player->giveSqueakyBoots();
	CBaseRespawningPickup::collect(_player);
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int		CShoesPickup::getRespawnTime()
{
	return 60*30;
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int shoes_bobspeed=100;
int shoes_bobscale=2;
int shoes_seperation=4;
void	CShoesPickup::thinkPickup(int _frames)
{
	m_sin=(m_sin+(_frames*shoes_bobspeed))&4095;
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CShoesPickup::renderPickup(DVECTOR *_pos)
{
	SpriteBank	*sprites;
	sFrameHdr	*fh;
	int			x,y,yoff;

	sprites=CGameScene::getSpriteBank();
	fh=sprites->getFrameHeader(FRM__SHOE);
	x=_pos->vx-(fh->W/2);
	y=_pos->vy-(fh->H/2);
	yoff=((msin(m_sin)*shoes_bobscale)>>12);
	sprites->printFT4(fh,x+shoes_seperation,y+yoff,0,0,OTPOS__PICKUP_POS);
	sprites->printFT4(fh,x-shoes_seperation,y-yoff,0,0,OTPOS__PICKUP_POS);
}

/*===========================================================================
end */

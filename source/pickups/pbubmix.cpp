/*=========================================================================

	pbubmix.cpp

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

#include "pickups\pbubmix.h"

#ifndef __MATHTABLE_HEADER__
#include "utils\mathtab.h"
#endif

#ifndef	__PLAYER_PLAYER_H__
#include "player\player.h"
#endif

#ifndef __GAME_GAME_H__
#include "game\game.h"
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
void	CBubbleMixturePickup::init()
{
	sFrameHdr	*fh;

	CBaseRespawningPickup::init();
	m_sin=0;

	fh=CGameScene::getSpriteBank()->getFrameHeader(FRM__BUBBLEMIXTURE);
	setCollisionSize(fh->W,fh->H);
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
DVECTOR	CBubbleMixturePickup::getSizeForPlacement()
{
	DVECTOR		size;
	sFrameHdr	*fh;

	fh=CGameScene::getSpriteBank()->getFrameHeader(FRM__BUBBLEMIXTURE);
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
void	CBubbleMixturePickup::collect(class CPlayer *_player)
{
	_player->giveBubbleAmmo();
	CBaseRespawningPickup::collect(_player);
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int		CBubbleMixturePickup::getRespawnTime()
{
	return 60*10;
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int bubmix_bobspeed=10;
int bubmix_bobscale=3;
void	CBubbleMixturePickup::thinkPickup(int _frames)
{
	m_sin=(m_sin+(_frames*bubmix_bobspeed))&4095;
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CBubbleMixturePickup::renderPickup(DVECTOR *_pos)
{
	SpriteBank	*sprites;
	sFrameHdr	*fh;
	int			x,y;

	sprites=CGameScene::getSpriteBank();
	fh=sprites->getFrameHeader(FRM__BUBBLEMIXTURE);
	x=_pos->vx-(fh->W/2);
	y=_pos->vy-(fh->H/2)+((msin(m_sin)*bubmix_bobscale)>>12);
	sprites->printFT4(fh,x,y,0,0,OTPOS__PICKUP_POS);
}




/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CBubbleWandPickup::init()
{
	CBaseWeaponSimplePickup::init();

	GameScene.getPlayer()->registerAddon(PLAYER_ADDON_BUBBLEWAND);
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CBubbleWandPickup::collect(class CPlayer *_player)
{
	if(!getHasBeenCollected())
	{
		_player->giveBubbleAmmoFromWeapon();
	}
	_player->setMode(PLAYER_MODE_BUBBLE_MIXTURE);
	CBaseWeaponSimplePickup::collect(_player);
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int		CBubbleWandPickup::getWeaponSpriteFrame()
{
	return FRM__BUBBLEWAND;
}

/*===========================================================================
end */

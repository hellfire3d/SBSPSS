/*=========================================================================

	pnet.cpp

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

#include "pickups\pnet.h"

#ifndef	__PLAYER_PLAYER_H__
#include "player\player.h"
#endif

#ifndef __GAME_GAME_H__
#include "game\game.h"
#endif


/*	Std Lib
	------- */

/*	Data
	---- */

#ifndef __SPR_SPRITES_H__
#include <sprites.h>
#endif

#ifndef	__GFX_OTPOS_H__
#include "gfx\otpos.h"
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
void	CNetPickup::init()
{
	CBaseWeaponSimplePickup::init();

	GameScene.getPlayer()->registerAddon(PLAYER_ADDON_NET);
	GameScene.getPlayer()->registerAddon(PLAYER_ADDON_JELLYFISHINNET);
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CNetPickup::collect(class CPlayer *_player)
{
	_player->setMode(PLAYER_MODE_NET);
	CBaseWeaponSimplePickup::collect(_player);
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int		CNetPickup::getWeaponSpriteFrame()
{
	return FRM__NET;
}


/*===========================================================================
end */

/*=========================================================================

	pjlammo.cpp

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

#include "pickups\pjlammo.h"

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
void	CJellyLauncherAmmoPickup::init()
{
	sFrameHdr	*fh;

	CBasePickup::init();
	m_rattle=0;

	fh=CGameScene::getSpriteBank()->getFrameHeader(FRM__JELLYAMMO);
	setCollisionSize(fh->W,fh->H);
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
DVECTOR	CJellyLauncherAmmoPickup::getSizeForPlacement()
{
	DVECTOR		size;
	sFrameHdr	*fh;

	fh=CGameScene::getSpriteBank()->getFrameHeader(FRM__JELLYAMMO);
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
void	CJellyLauncherAmmoPickup::collect(class CPlayer *_player)
{
	_player->giveJellyAmmo();
	CBasePickup::collect(_player);
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int jlammo_rattlespeed=500;
int jlammo_rattlescale=90;
int jlammo_rattlecount=5;
int jlammo_waitcount=10;
void	CJellyLauncherAmmoPickup::thinkPickup(int _frames)
{
	m_rattle+=jlammo_rattlespeed*_frames;
	if(m_rattle>(jlammo_rattlecount+jlammo_waitcount)*4095)m_rattle=0;
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CJellyLauncherAmmoPickup::renderPickup(DVECTOR *_pos)
{
	SpriteBank	*sprites;
	sFrameHdr	*fh;
	int			angle;

	sprites=CGameScene::getSpriteBank();
	fh=sprites->getFrameHeader(FRM__JELLYAMMO);
	if(m_rattle<=jlammo_rattlecount*4095)
	{
		angle=((msin(m_rattle&4095)*jlammo_rattlescale)>>12)&4095;
	}
	else
	{
		angle=0;
	}
	sprites->printRotatedScaledSprite(fh,_pos->vx,_pos->vy,4096,4096,angle,OTPOS__PICKUP_POS);
}




/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CJellyLauncherPickup::init()
{
	CBaseWeaponSimplePickup::init();

	GameScene.getPlayer()->registerAddon(PLAYER_ADDON_JELLYLAUNCHER);
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CJellyLauncherPickup::collect(class CPlayer *_player)
{
	if(!getHasBeenCollected())
	{
		_player->giveJellyAmmoFromWeapon();
	}
	_player->setMode(PLAYER_MODE_JELLY_LAUNCHER);
	CBaseWeaponSimplePickup::collect(_player);
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int		CJellyLauncherPickup::getWeaponSpriteFrame()
{
	return FRM__LAUNCHER;
}


/*===========================================================================
end */

/*=========================================================================

	pballoon.cpp

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

#include "pickups\pballoon.h"

#ifndef __LEVEL_LEVEL_H__
#include "level\level.h"
#endif

#ifndef __LAYER_COLLISION_H__
#include "layer\collision.h"
#endif

#ifndef __PRIM_HEADER__
#include "gfx\prim.h"
#endif

#ifndef __MATHTABLE_HEADER__
#include "utils\mathtab.h"
#endif

#ifndef	__PLAYER_PLAYER_H__
#include "player\player.h"
#endif

#ifndef	__GFX_OTPOS_H__
#include "gfx\otpos.h"
#endif

#include	"game\game.h"

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
void	CBalloonPickup::init()
{
	sFrameHdr	*fh;

	CBaseWeaponPickup::init();
	m_sin=0;

	fh=CGameScene::getSpriteBank()->getFrameHeader(FRM__BALLOON);
	setCollisionSize(fh->W,fh->H);
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
DVECTOR	CBalloonPickup::getSizeForPlacement()
{
	DVECTOR		size;
	sFrameHdr	*fh;

	fh=CGameScene::getSpriteBank()->getFrameHeader(FRM__BALLOON);
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
void	CBalloonPickup::collect(class CPlayer *_player)
{
	_player->setMode(PLAYER_MODE_BALLOON);
	CBaseWeaponPickup::collect(_player);
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int balloon_height=30;
int balloon_r1=251;
int balloon_g1=207;
int balloon_b1=167;
int balloon_r2=147;
int balloon_g2=95;
int balloon_b2=75;
int balloon_speed=25;
int balloon_scale1=5;
int balloon_scale2=2;
int balloon_phase=1024;
int balloon_vissize=40;
int	balloon_stringx=-3;
int		CBalloonPickup::getVisibilityRadius()
{
	return balloon_vissize;
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int		CBalloonPickup::getRespawnTime()
{
	return 60*10;
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CBalloonPickup::thinkPickup(int _frames)
{
	m_sin=(m_sin+(_frames*balloon_speed))&4095;
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CBalloonPickup::renderPickup(DVECTOR *_pos)
{
	SpriteBank	*sprites;
	sFrameHdr	*fh;
	int			xo1,xo2;
	int			x,y;

	sprites=CGameScene::getSpriteBank();
	fh=sprites->getFrameHeader(FRM__BALLOON);

	xo1=((msin((m_sin+balloon_phase)&4095)*balloon_scale1)>>12);
	xo2=((msin(m_sin)*balloon_scale2)>>12);
	x=_pos->vx-(fh->W/2);
	y=_pos->vy-(fh->H/2);
	sprites->printFT4(fh,x+xo1,y,0,0,OTPOS__PICKUP_POS);
setCollisionCentreOffset(xo1,0);

	x=_pos->vx+balloon_stringx;
	y=_pos->vy+(fh->H/2);
	DrawLine(x+xo1,y,x+xo2,y+balloon_height,balloon_r1,balloon_g1,balloon_b1,OTPOS__PICKUP_POS);
	x++;
	DrawLine(x+xo1,y,x+xo2,y+balloon_height,balloon_r2,balloon_g2,balloon_b2,OTPOS__PICKUP_POS);
}

/*===========================================================================
end */

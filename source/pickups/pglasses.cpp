dsadsasdsad


/*=========================================================================

	pglasses.cpp

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

#include "pickups\pglasses.h"

#ifndef __GAME_GAME_H__
#include "game\game.h"
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
void	CGlassesPickup::init()
{
	sFrameHdr	*fh;

	CBasePickup::init();

	m_glint=0;
	m_glintRot=0;

	fh=CGameScene::getSpriteBank()->getFrameHeader(FRM__GLASSES);
	setCollisionSize(fh->W,fh->H);

	GameScene.getPlayer()->registerAddon(PLAYER_ADDON_GLASSES);
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
DVECTOR	CGlassesPickup::getSizeForPlacement()
{
	DVECTOR		size;
	sFrameHdr	*fh;

	fh=CGameScene::getSpriteBank()->getFrameHeader(FRM__GLASSES);
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
void	CGlassesPickup::collect(class CPlayer *_player)
{
	_player->giveGlasses();
	CBasePickup::collect(_player);
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int glasses_glintframes=16;
int glasses_glintwait=400;
int glasses_glintspeed=1;
int glasses_glintRotSpeed=200;
DVECTOR glasses_gxy={2,2};
static const int glasses_glintFrames[]={FRM__GLINT1,FRM__GLINT2,FRM__GLINT3,FRM__GLINT4,FRM__GLINT4,FRM__GLINT3,FRM__GLINT2,FRM__GLINT1};
void	CGlassesPickup::thinkPickup(int _frames)
{
	m_glint+=_frames;
	if(m_glint>glasses_glintframes+glasses_glintwait)m_glint=0;
	m_glintRot+=_frames*glasses_glintRotSpeed;
	m_glintRot&=4095;
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CGlassesPickup::renderPickup(DVECTOR *_pos)
{
	SpriteBank	*sprites;
	sFrameHdr	*fh;
	int			x,y;

	sprites=CGameScene::getSpriteBank();
	fh=sprites->getFrameHeader(FRM__GLASSES);
	x=_pos->vx-(fh->W/2);
	y=_pos->vy-(fh->H/2);
	sprites->printFT4(fh,x,y,0,0,OTPOS__PICKUP_POS);

	if(m_glint<=glasses_glintframes)
	{
		fh=sprites->getFrameHeader(glasses_glintFrames[(m_glint>>glasses_glintspeed)&0x07]);
		sprites->printRotatedScaledSprite(fh,x+glasses_gxy.vx,y+glasses_gxy.vy,4095,4095,m_glintRot,OTPOS__PICKUP_POS-1);
	}
}

/*===========================================================================
end */

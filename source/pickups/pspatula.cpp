/*=========================================================================

	pspatula.cpp

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

#include "pickups\pspatula.h"

#ifndef	__GFX_OTPOS_H__
#include "gfx\otpos.h"
#endif

#ifndef __MATHTABLE_HEADER__
#include "utils\mathtab.h"
#endif

#ifndef	__GAME_GAMESLOT_H__
#include "game\gameslot.h"
#endif

#ifndef __GAME_GAME_H__
#include "game\game.h"
#endif

#ifndef	__PLAYER_PLAYER_H__
#include "player\player.h"
#endif


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
void	CSpatulaPickup::init()
{
	sFrameHdr	*fh;

	CBasePickup::init();
	m_glint=0;
	m_glintRot=0;

	fh=CGameScene::getSpriteBank()->getFrameHeader(FRM__SPATULA);
	setCollisionSize(fh->W,fh->H);
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
DVECTOR	CSpatulaPickup::getSizeForPlacement()
{
	DVECTOR		size;
	sFrameHdr	*fh;

	fh=CGameScene::getSpriteBank()->getFrameHeader(FRM__SPATULA);
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
void	CSpatulaPickup::collect(class CPlayer *_player)
{
	_player->addSpatula();
	CBasePickup::collect(_player);
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int spat_maxglint=50;
int spat_glintgrowspeed=3;
int spat_glintrotspeed=90;
DVECTOR spat_gxy={20,0};
static const int spat_glintFrames[]={FRM__GLINT1,FRM__GLINT2,FRM__GLINT3,FRM__GLINT4,FRM__GLINT4,FRM__GLINT3,FRM__GLINT2,FRM__GLINT1};
void	CSpatulaPickup::thinkPickup(int _frames)
{
	m_glint+=_frames;
	m_glint&=0xff;
	m_glintRot+=_frames*spat_glintrotspeed;
	m_glintRot&=4095;
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CSpatulaPickup::renderPickup(DVECTOR *_pos)
{
	SpriteBank	*sprites;
	sFrameHdr	*fh;
	int			x,y;

	sprites=CGameScene::getSpriteBank();
	fh=sprites->getFrameHeader(FRM__SPATULA);
	x=_pos->vx-(fh->W/2);
	y=_pos->vy-(fh->H/2);
	sprites->printFT4(fh,x,y,0,0,OTPOS__PICKUP_POS);

	if(m_glint<=spat_maxglint)
	{
		fh=sprites->getFrameHeader(spat_glintFrames[(m_glint>>spat_glintgrowspeed)&0x07]);
		x=x+spat_gxy.vx;
		y=y+spat_gxy.vy;
		sprites->printRotatedScaledSprite(fh,x,y,4095,4095,m_glintRot,OTPOS__PICKUP_POS-1);
	}
}


/*===========================================================================
end */

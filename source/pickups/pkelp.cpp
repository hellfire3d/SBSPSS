/*=========================================================================

	pkelp.cpp

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

#include "pickups\pkelp.h"

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
void	CKelpTokenPickup::init()
{
	sFrameHdr	*fh;

	CBasePickup::init();

	fh=CGameScene::getSpriteBank()->getFrameHeader(FRM__TOKEN);
	setCollisionSize(fh->W,fh->H);

	m_collectedFlag=false;
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
DVECTOR	CKelpTokenPickup::getSizeForPlacement()
{
	DVECTOR		size;
	sFrameHdr	*fh;

	fh=CGameScene::getSpriteBank()->getFrameHeader(FRM__TOKEN);
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
void	CKelpTokenPickup::collect(class CPlayer *_player)
{
	CBasePickup::collect(_player);
	CGameSlotManager::getSlotData()->collectKelpToken(GameScene.getChapterNumber()-1,GameScene.getLevelNumber()-1,m_tokenNumber);
	m_collectedFlag=true;
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CKelpTokenPickup::renderPickup(DVECTOR *_pos)
{
	SpriteBank	*sprites;
	sFrameHdr	*fh;
	int			x,y;

	sprites=CGameScene::getSpriteBank();
	fh=sprites->getFrameHeader(FRM__TOKEN);
	x=_pos->vx-(fh->W/2);
	y=_pos->vy-(fh->H/2);
	sprites->printFT4(fh,x,y,0,0,OTPOS__PICKUP_POS);
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CKelpTokenPickup::collidedWith(CThing *_thisThing)
{
	if(!m_collectedFlag)
	{
		switch(_thisThing->getThingType())
		{
			case TYPE_PLAYER:
			case TYPE_NPC:
				collect((CPlayer*)_thisThing);
				CSoundMediator::playSfx(sfxToPlayWhenCollected());
				break;

			default:
				ASSERT(0);
				break;
		}
	}
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
bool	CKelpTokenPickup::alwaysThink()
{
	int level=GameScene.getLevelNumber();
	int chapter=GameScene.getChapterNumber();

	if ( chapter == 5 && level == 5 )			// snail race
	{
		return( true );
	}
	else
	{
		return( false );
	}
}


/*===========================================================================
end */

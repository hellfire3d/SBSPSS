
/*=========================================================================

	psbutt.cpp

	Author:		PKG
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/


/*----------------------------------------------------------------------
	Includes
	-------- */

#include "player\psbutt.h"

#ifndef __PLAYER_PLAYER_H__
#include "player\player.h"
#endif

#ifndef __PAD_PADS_H__
#include "pad\pads.h"
#endif

#ifndef __GAME_GAMEBUBS_H__
#include "game\gamebubs.h"
#endif


/*	Std Lib
	------- */

/*	Data
	---- */

#ifndef	__ANIM_PLAYER_ANIM_HEADER__
#include <player_anim.h>
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
void CPlayerStateButtBounce::enter(CPlayer *_player)
{
	setAnimNo(_player,ANIM_PLAYER_ANIM_BUTTBOUNCE);
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CPlayerStateButtBounce::think(CPlayer *_player)
{
	if(advanceAnimFrameAndCheckForEndOfAnim(_player))
	{
		setState(_player,STATE_BUTTFALL);
	}
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CPlayerStateButtBounceLand::enter(CPlayer *_player)
{
	DVECTOR	pos;

	pos=getPlayerPos(_player);
	CGameBubicleFactory::spawnBubicles(pos.vx,pos.vy,40,10,CGameBubicleFactory::TYPE_MEDIUM);
	setAnimNo(_player,ANIM_PLAYER_ANIM_BUTTBOUNCE);
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CPlayerStateButtBounceLand::think(CPlayer *_player)
{
	if(advanceAnimFrameAndCheckForEndOfAnim(_player))
	{
		setState(_player,STATE_IDLE);
	}
}


/*===========================================================================
 end */

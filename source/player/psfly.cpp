/*=========================================================================

	psfly.cpp

	Author:		PKG
	Created: 
	Project:	Spongebob
	Purpose:	Debugginf mode to allow SB to fly around the map

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/


/*----------------------------------------------------------------------
	Includes
	-------- */

#include "player\psfly.h"

#ifndef __PLAYER_PLAYER_H__
#include "player\player.h"
#endif


/*	Std Lib
	------- */

/*	Data
	---- */

#ifndef	__ANIM_SPONGEBOB_HEADER__
#include <ACTOR_SPONGEBOB_ANIM.h>
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
void CPlayerStateFly::enter(CPlayer *_player)
{
	DVECTOR	move;

	move.vx=0;
	move.vy=0;
	setMoveVelocity(_player,&move);
	setAnimNo(_player,ANIM_SPONGEBOB_IDLEHOOLA);
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CPlayerStateFly::think(CPlayer *_player)
{
	int		controlHeld;
	DVECTOR	pos;

	controlHeld=getPadInputHeld(_player);
	pos=getPlayerPos(_player);
	if(controlHeld&PI_LEFT)
	{
		pos.vx-=8;
	}
	else if(controlHeld&PI_RIGHT)
	{
		pos.vx+=8;
	}
	if(controlHeld&PI_UP)
	{
		pos.vy-=8;
	}
	else if(controlHeld&PI_DOWN)
	{
		pos.vy+=8;
	}
	this->setPlayerPos(_player,&pos);

	advanceAnimFrameAndCheckForEndOfAnim(_player);	
}


/*===========================================================================
 end */

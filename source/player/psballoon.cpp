/*=========================================================================

	psballoon.cpp

	Author:		PKG
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/


/*----------------------------------------------------------------------
	Includes
	-------- */

#include "player\psballoon.h"

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

#define MAX_BALLOON_VELOCITY	5


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
void CPlayerStateBalloon::enter(CPlayer *_player)
{
	setAnimNo(_player,ANIM_SPONGEBOB_HOVER);
	m_canDropBalloon=false;
}

/*----------------------------------------------------------------------
  Function:
  Purpose:
  Params:
  Returns:
---------------------------------------------------------------------- */
void CPlayerStateBalloon::think(CPlayer *_player)
{
	int	controlDown,controlHeld;
	controlDown=getPadInputDown(_player);
	controlHeld=getPadInputHeld(_player);

	if(m_canDropBalloon&&controlDown&PI_ACTION)
	{
		setMode(_player,PLAYER_MODE_FULLUNARMED);
	}

	if(controlHeld&PI_LEFT)
	{
		moveLeft(_player);
	}
	else if(controlHeld&PI_RIGHT)
	{
		moveRight(_player);
	}
	else
	{
		slowdown(_player);
	}

	DVECTOR moveVel=getMoveVelocity(_player);
	if(moveVel.vy>-MAX_BALLOON_VELOCITY)
	{
		moveVel.vy--;
	}
	if(moveVel.vy<-MAX_BALLOON_VELOCITY)
	{
		moveVel.vy++;
	}
	setMoveVelocity(_player,&moveVel);

	if(advanceAnimFrameAndCheckForEndOfAnim(_player))
	{
		m_canDropBalloon=true;
	}
}

/*===========================================================================
 end */

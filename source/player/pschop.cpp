
/*=========================================================================

	pschop.cpp

	Author:		PKG
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/


/*----------------------------------------------------------------------
	Includes
	-------- */

#include "player\pschop.h"

#ifndef __PLAYER_PLAYER_H__
#include "player\player.h"
#endif

#ifndef __PAD_PADS_H__
#include "pad\pads.h"
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
void CPlayerStateChop::enter(CPlayer *_player)
{
	setAnimNo(_player,ANIM_SPONGEBOB_KARATE);
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CPlayerStateChop::think(CPlayer *_player)
{
	if(advanceAnimFrameAndCheckForEndOfAnim(_player))
	{
		setState(_player,STATE_IDLE);
	}
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CPlayerStateRunChop::think(CPlayer *_player)
{
	int	controlHeld;
	controlHeld=getPadInputHeld(_player);

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
	
	if(advanceAnimFrameAndCheckForEndOfAnim(_player))
	{
		if(getMoveVelocity(_player).vx==0)
		{
			setState(_player,STATE_IDLE);
		}
		else
		{
			setState(_player,STATE_RUN);
		}
	}
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CPlayerStateAirChop::think(CPlayer *_player)
{
	int	controlHeld;
	controlHeld=getPadInputHeld(_player);

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
	
	if(advanceAnimFrameAndCheckForEndOfAnim(_player))
	{
		setState(_player,STATE_FALL);
	}
}


/*===========================================================================
 end */

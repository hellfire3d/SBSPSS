/*=========================================================================

	psfall.cpp

	Author:		PKG
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/


/*----------------------------------------------------------------------
	Includes
	-------- */

#include "player\psfall.h"

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
void CPlayerStateFall::enter(CPlayer *_player)
{
	setAnimNo(_player,ANIM_SPONGEBOB_HOVER);
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CPlayerStateFall::think(CPlayer *_player)
{
	const PlayerMetrics	*metrics;
	int					controlHeld,controlDown;
	DVECTOR				move;

	metrics=getPlayerMetrics(_player);
	controlHeld=getPadInputHeld(_player);
	controlDown=getPadInputDown(_player);

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

	fall(_player);

	if(controlDown&PI_ACTION)
	{
		setState(_player,STATE_AIRATTACK);
	}
	else if(controlHeld&PI_DOWN)
	{
		if(setState(_player,STATE_BUTTBOUNCE))
		{
			// Only do this if this mode allows us to go into butt bounce, otherwise
			// SB just floats in the air :)
			move.vx=0;
			move.vy=0;
			setMoveVelocity(_player,&move);
		}
	}
	advanceAnimFrameAndCheckForEndOfAnim(_player);	
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CPlayerStateFallFar::enter(CPlayer *_player)
{
	setAnimNo(_player,ANIM_SPONGEBOB_FALL);
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CPlayerStateFallFar::think(CPlayer *_player)
{
	int		controlHeld;
	DVECTOR	move;

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

	fall(_player);
}


/*===========================================================================
 end */

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
void CPlayerStateFall::enter(CPlayer *_player)
{
	setAnimNo(_player,ANIM_PLAYER_ANIM_HOVER);
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
	int					controlHeld;
	DVECTOR				move;

	metrics=getPlayerMetrics(_player);
	controlHeld=getPadInputHeld(_player);

	if(controlHeld&CPadConfig::getButton(CPadConfig::PAD_CFG_LEFT))
	{
		moveLeft(_player);
	}
	else if(controlHeld&CPadConfig::getButton(CPadConfig::PAD_CFG_RIGHT))
	{
		moveRight(_player);
	}
	else
	{
		slowdown(_player);
	}

	fall(_player);

	if(controlHeld&CPadConfig::getButton(CPadConfig::PAD_CFG_ACTION))
	{
		setState(_player,STATE_AIRATTACK);
	}
	else if(controlHeld&CPadConfig::getButton(CPadConfig::PAD_CFG_DOWN))
	{
		move.vx=0;
		move.vy=0;
		setMoveVelocity(_player,&move);
		setState(_player,STATE_BUTTBOUNCE);
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
	setAnimNo(_player,ANIM_PLAYER_ANIM_FALL);
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

	if(controlHeld&CPadConfig::getButton(CPadConfig::PAD_CFG_LEFT))
	{
		moveLeft(_player);
	}
	else if(controlHeld&CPadConfig::getButton(CPadConfig::PAD_CFG_RIGHT))
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

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

#ifndef __PLAYER_PMODES_H__
#include "player\pmodes.h"
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
void CPlayerStateFall::enter(CPlayerModeBasic *_playerMode)
{
	_playerMode->setAnimNo(ANIM_SPONGEBOB_HOVER);
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CPlayerStateFall::think(CPlayerModeBasic *_playerMode)
{
	const PlayerMetrics	*metrics;
	int					controlHeld,controlDown;
	DVECTOR				move;

	metrics=_playerMode->getPlayerMetrics();
	controlHeld=_playerMode->getPadInputHeld();
	controlDown=_playerMode->getPadInputDown();

	if(controlHeld&PI_LEFT)
	{
		_playerMode->moveLeft();
	}
	else if(controlHeld&PI_RIGHT)
	{
		_playerMode->moveRight();
	}
	else
	{
		_playerMode->slowdown();
	}
	_playerMode->fall();

	if(controlHeld&PI_DOWN)
	{
		_playerMode->setState(STATE_BUTTBOUNCE);
	}
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CPlayerStateFallFar::enter(CPlayerModeBasic *_playerMode)
{
	_playerMode->setAnimNo(ANIM_SPONGEBOB_FALL);
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CPlayerStateFallFar::think(CPlayerModeBasic *_playerMode)
{
	int		controlHeld;
	DVECTOR	move;

	controlHeld=_playerMode->getPadInputHeld();

	if(controlHeld&PI_LEFT)
	{
		_playerMode->moveLeft();
	}
	else if(controlHeld&PI_RIGHT)
	{
		_playerMode->moveRight();
	}
	else
	{
		_playerMode->slowdown();
	}
	_playerMode->fall();
}


/*===========================================================================
 end */

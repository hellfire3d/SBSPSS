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

CPlayerStateFall			s_stateFall;
CPlayerStateFallFar			s_stateFallFar;


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CPlayerStateFall::enter(CPlayerModeBase *_playerMode)
{
	// If already in this state then don't do the entry code
	if(_playerMode->getState()!=STATE_JUMP)
	{
		if(_playerMode->getAnimNo()!=ANIM_SPONGEBOB_JUMP)
		{
			_playerMode->setAnimNo(ANIM_SPONGEBOB_JUMP);
		}
	}
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CPlayerStateFall::think(CPlayerModeBase *_playerMode)
{
	const PlayerMetrics	*metrics;
	int					controlHeld,controlDown;
	DVECTOR				move;

	metrics=_playerMode->getPlayerMetrics();
	controlHeld=_playerMode->getPadInputHeld();
	controlDown=_playerMode->getPadInputDown();

	_playerMode->advanceAnimFrameAndCheckForEndOfAnim();

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

	if(controlDown&PI_JUMP)
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
void CPlayerStateFallFar::enter(CPlayerModeBase *_playerMode)
{
	_playerMode->setAnimNo(ANIM_SPONGEBOB_FALL);
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CPlayerStateFallFar::think(CPlayerModeBase *_playerMode)
{
	int		frame;
	int		controlHeld;
	DVECTOR	move;

	frame=_playerMode->getAnimFrame();
	{
		if(_playerMode->advanceAnimFrameAndCheckForEndOfAnim())
		{
			_playerMode->setAnimFrame(frame);
		}
	}
	
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

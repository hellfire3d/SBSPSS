
/*=========================================================================

	pscrouch.cpp

	Author:		PKG
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/


/*----------------------------------------------------------------------
	Includes
	-------- */

#include "player\pscrouch.h"

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

CPlayerStateCrouchDown		s_stateCrouchDown;
CPlayerStateCrouchUp		s_stateCrouchUp;


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CPlayerStateCrouchDown::enter(CPlayerModeBase *_playerMode)
{
	_playerMode->setAnimNo(ANIM_SPONGEBOB_CROUCHDOWN);
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CPlayerStateCrouchDown::think(CPlayerModeBase *_playerMode)
{
	int	maxFrame;
	int	controlDown,controlHeld;
	controlDown=_playerMode->getPadInputDown();
	controlHeld=_playerMode->getPadInputHeld();

	_playerMode->slowdown();

	maxFrame=_playerMode->getAnimFrameCount()-1;
	if(_playerMode->getAnimFrame()<maxFrame)
	{
		_playerMode->advanceAnimFrameAndCheckForEndOfAnim();
	}
	else
	{
		if(!(controlHeld&PI_DOWN))
		{
			_playerMode->setState(STATE_CROUCHUP);
		}
	}

	if(controlDown&PI_JUMP)
	{
		_playerMode->setState(STATE_JUMP);
	}
	else if(controlHeld&PI_LEFT)
	{
		if(_playerMode->canMoveLeft())
			_playerMode->setState(STATE_RUN);
	}
	else if(controlHeld&PI_RIGHT)
	{
		if(_playerMode->canMoveRight())
			_playerMode->setState(STATE_RUN);
	}
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CPlayerStateCrouchUp::enter(CPlayerModeBase *_playerMode)
{
	  _playerMode->setAnimNo(ANIM_SPONGEBOB_CROUCHUP);
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CPlayerStateCrouchUp::think(CPlayerModeBase *_playerMode)
{
	int	controlDown,controlHeld;
	controlDown=_playerMode->getPadInputDown();
	controlHeld=_playerMode->getPadInputHeld();

	if(_playerMode->advanceAnimFrameAndCheckForEndOfAnim())
	{
		_playerMode->setState(STATE_IDLE);
	}

	if(controlDown&PI_JUMP)
	{
		_playerMode->setState(STATE_JUMP);
	}
	else if(controlHeld&PI_LEFT)
	{
		if(_playerMode->canMoveLeft())
			_playerMode->setState(STATE_RUN);
	}
	else if(controlHeld&PI_RIGHT)
	{
		if(_playerMode->canMoveRight())
			_playerMode->setState(STATE_RUN);
	}
}


/*===========================================================================
 end */

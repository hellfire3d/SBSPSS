
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

	_playerMode->slowdown();

	maxFrame=_playerMode->getAnimFrameCount()-1;
	if(_playerMode->getAnimFrame()<maxFrame)
	{
		_playerMode->advanceAnimFrameAndCheckForEndOfAnim();
	}
	else
	{
		if(!(_playerMode->getPadInputHeld()&PI_DOWN))
		{
			_playerMode->setState(STATE_CROUCHUP);
		}
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
	_playerMode->zeroMoveVelocity();	
	_playerMode->setAnimNo(ANIM_SPONGEBOB_SOAKUP);
	_playerMode->setAnimFrame(_playerMode->getAnimFrameCount()-1);
	_playerMode->setPlayerCollisionSize(0,-10,60,20);
}


/*
void CPlayerStateSoakUp::think(CPlayerModeBase *_playerMode)
{
	int	controlHeld;

	controlHeld=_playerMode->getPadInputHeld();
	if(!(controlHeld&PI_DOWN))
	{
		_playerMode->setState(STATE_GETUP);
	}
	else
	{
		_playerMode->inSoakUpState();
	}
}
*/

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
	if(_playerMode->advanceAnimFrameAndCheckForEndOfAnim())
	{
		_playerMode->setState(STATE_IDLE);
	}
}


/*===========================================================================
 end */

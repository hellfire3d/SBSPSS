
/*=========================================================================

	pslook.cpp

	Author:		PKG
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/


/*----------------------------------------------------------------------
	Includes
	-------- */

#include "player\pslook.h"

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

CPlayerStateLookDown		s_stateLookDown;
CPlayerStateLookDownRelax	s_stateLookDownRelax;
CPlayerStateLookUp			s_stateLookUp;
CPlayerStateLookUpRelax		s_stateLookUpRelax;


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CPlayerStateLookDown::enter(CPlayerModeBase *_playerMode)
{
	_playerMode->setAnimNo(ANIM_SPONGEBOB_CROUCHDOWN);
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CPlayerStateLookDown::think(CPlayerModeBase *_playerMode)
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
		if(!(controlHeld&controlThatMustBeHeld()))
		{
			_playerMode->setState(getNextState());
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
int CPlayerStateLookDown::getNextState()
{
	return STATE_LOOKDOWNRELAX;
}
int CPlayerStateLookDown::controlThatMustBeHeld()
{
	return PI_DOWN;
}




/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CPlayerStateLookDownRelax::enter(CPlayerModeBase *_playerMode)
{
	  _playerMode->setAnimNo(ANIM_SPONGEBOB_CROUCHUP);
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CPlayerStateLookDownRelax::think(CPlayerModeBase *_playerMode)
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




/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CPlayerStateLookUp::enter(CPlayerModeBase *_playerMode)
{
	_playerMode->setAnimNo(ANIM_SPONGEBOB_LOOKUP);
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int CPlayerStateLookUp::getNextState()
{
	return STATE_LOOKUPRELAX;
}
int CPlayerStateLookUp::controlThatMustBeHeld()
{
	return PI_UP;
}




/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CPlayerStateLookUpRelax::enter(CPlayerModeBase *_playerMode)
{
//	  _playerMode->setAnimNo(ANIM_SPONGEBOB_CROUCHUP);
}




/*===========================================================================
 end */


/*=========================================================================

	psduck.cpp

	Author:		PKG
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/


/*----------------------------------------------------------------------
	Includes
	-------- */

#include "player\psduck.h"

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

CPlayerStateDuck			s_stateDuck;
CPlayerStateSoakUp			s_stateSoakUp;
CPlayerStateGetUp			s_stateGetUp;


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CPlayerStateDuck::enter(CPlayerModeBase *_playerMode)
{
	_playerMode->setAnimNo(ANIM_SPONGEBOB_SOAKUP);
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CPlayerStateDuck::think(CPlayerModeBase *_playerMode)
{
	_playerMode->slowdown();
	if(_playerMode->advanceAnimFrameAndCheckForEndOfAnim())
	{
		_playerMode->setState(STATE_SOAKUP);
	}
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CPlayerStateSoakUp::enter(CPlayerModeBase *_playerMode)
{
	_playerMode->zeroMoveVelocity();	
	_playerMode->setAnimNo(ANIM_SPONGEBOB_GETUP);
	_playerMode->setPlayerCollisionSize(0,-10,60,20);
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CPlayerStateSoakUp::think(CPlayerModeBase *_playerMode)
{
	int	controlHeld;

	controlHeld=_playerMode->getPadInputHeld();
	if(!(controlHeld&PI_DOWN))
	{
		_playerMode->setState(STATE_GETUP);
	}
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CPlayerStateGetUp::enter(CPlayerModeBase *_playerMode)
{
	  _playerMode->setAnimNo(ANIM_SPONGEBOB_GETUP);
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CPlayerStateGetUp::think(CPlayerModeBase *_playerMode)
{
	if(_playerMode->advanceAnimFrameAndCheckForEndOfAnim())
	{
		_playerMode->setState(STATE_IDLE);
	}
}


/*===========================================================================
 end */

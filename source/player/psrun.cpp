/*=========================================================================

	psrun.cpp

	Author:		PKG
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/


/*----------------------------------------------------------------------
	Includes
	-------- */

#include "player\psrun.h"

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
void CPlayerStateRun::enter(CPlayerModeBasic *_playerMode)
{
	int	controlHeld;
	controlHeld=_playerMode->getPadInputHeld();

	if(_playerMode->getMoveVelocity().vx)
	{
		_playerMode->setAnimNo(ANIM_SPONGEBOB_RUN);
	}
	else
	{
		_playerMode->setAnimNo(ANIM_SPONGEBOB_RUNSTART);
	}

	if(controlHeld&PI_LEFT)
	{
		_playerMode->setFacing(FACING_LEFT);
	}
	else if(controlHeld&PI_RIGHT)
	{
		_playerMode->setFacing(FACING_RIGHT);
	}

	m_numberOfTimeAnimHasLooped=0;
}


/*----------------------------------------------------------------------
  Function:
  Purpose:
  Params:
  Returns:
---------------------------------------------------------------------- */
void CPlayerStateRun::think(CPlayerModeBasic *_playerMode)
{
	int	controlDown,controlHeld;
	int switchedState=false;
	controlDown=_playerMode->getPadInputDown();
	controlHeld=_playerMode->getPadInputHeld();

	if(controlDown&PI_JUMP)
	{
		switchedState=_playerMode->setState(STATE_JUMP);
	}
	if(controlHeld&PI_DOWN)
	{
		switchedState=_playerMode->setState(STATE_DUCK);
	}

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
		if(!switchedState)
		{
			if(_playerMode->slowdown())
			{
				_playerMode->setState(STATE_IDLE);
				if(m_numberOfTimeAnimHasLooped>=4)
				{
					_playerMode->setAnimNo(ANIM_SPONGEBOB_RUNSTOP);
				}
				return;
			}
		}
	}

	if(_playerMode->advanceAnimFrameAndCheckForEndOfAnim())
	{
		_playerMode->setAnimNo(ANIM_SPONGEBOB_RUN);
		m_numberOfTimeAnimHasLooped++;
	}
}


/*===========================================================================
 end */

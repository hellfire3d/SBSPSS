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

CPlayerStateRun				s_stateRun;
CPlayerStateWalk			s_stateWalk;


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CPlayerStateRun::enter(CPlayerModeBase *_playerMode)
{
	int	controlHeld;
	controlHeld=_playerMode->getPadInputHeld();

	if(_playerMode->getMoveVelocity().vx)
	{
		_playerMode->setAnimNo(getLoopFrame());
	}
	else
	{
		int	frame;
		frame=getStartFrame();
		if(frame==-1)
		{
			frame=getLoopFrame();
		}
		_playerMode->setAnimNo(frame);
	}

	if(controlHeld&PI_LEFT)
	{
		_playerMode->setFacing(FACING_LEFT);
	}
	else if(controlHeld&PI_RIGHT)
	{
		_playerMode->setFacing(FACING_RIGHT);
	}

	m_numberOfTimesAnimHasLooped=0;
}


/*----------------------------------------------------------------------
  Function:
  Purpose:
  Params:
  Returns:
---------------------------------------------------------------------- */
void CPlayerStateRun::think(CPlayerModeBase *_playerMode)
{
	int	controlDown,controlHeld;
	int switchedState=false;
	controlDown=_playerMode->getPadInputDown();
	controlHeld=_playerMode->getPadInputHeld();

	if(_playerMode->advanceAnimFrameAndCheckForEndOfAnim())
	{
		_playerMode->setAnimNo(getLoopFrame());
		m_numberOfTimesAnimHasLooped++;
	}

	if(controlDown&PI_JUMP)
	{
		switchedState=_playerMode->setState(STATE_JUMP);
	}
	if(controlDown&PI_DOWN)
	{
		switchedState=_playerMode->setState(STATE_LOOKDOWN);
	}
	else if(controlDown&PI_UP)
	{
		switchedState=_playerMode->setState(STATE_LOOKUP);
	}

	if(controlHeld&PI_LEFT)
	{
		CPlayer *player = _playerMode->getPlayer();
		player->setMoveLeftRight( player->getMoveLeftRight() - 1 );

		_playerMode->moveLeft();
	}
	else if(controlHeld&PI_RIGHT)
	{
		CPlayer *player = _playerMode->getPlayer();
		player->setMoveLeftRight( player->getMoveLeftRight() + 1 );

		_playerMode->moveRight();
	}
	else
	{
		if(!switchedState)
		{
			if(_playerMode->slowdown())
			{
				_playerMode->setState(STATE_IDLE);
				if(m_numberOfTimesAnimHasLooped>=4)
				{
					int	frame;
					frame=getStartFrame();
					if(frame!=-1)
					{
						_playerMode->setAnimNo(getEndFrame());
					}
				}
				return;
			}
		}
	}
}


/*===========================================================================
 end */

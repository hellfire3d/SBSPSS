/*=========================================================================

	psspring.cpp

	Author:		PKG
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/


/*----------------------------------------------------------------------
	Includes
	-------- */

#include "player\psspring.h"

#ifndef __PLAYER_PLAYER_H__
#include "player\player.h"
#endif

#ifndef __PLAYER_PMODES_H__
#include "player\pmodes.h"
#endif

#ifndef __SOUND_SOUND_H__
#include "sound\sound.h"
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

CPlayerStateSpring			s_stateSpring;


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CPlayerStateSpring::enter(CPlayerModeBase *_playerMode)
{
	// If already in this state then don't do the entry code
	if(_playerMode->getState()!=STATE_SPRINGUP)
	{
		_playerMode->setAnimNo(ANIM_SPONGEBOB_JUMP);
		m_springFrames=0;
		_playerMode->spring();
	}
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CPlayerStateSpring::think(CPlayerModeBase *_playerMode)
{
	const PlayerMetrics	*metrics;
	int					controlHeld,controlDown;

	metrics=_playerMode->getPlayerMetrics();
	controlHeld=_playerMode->getPadInputHeld();
	controlDown=_playerMode->getPadInputDown();

	_playerMode->advanceAnimFrameAndCheckForEndOfAnim();

	if(m_springFrames<=_playerMode->getSpringHeight())
	{
		_playerMode->spring();
		m_springFrames++;
	}
	else
	{
		_playerMode->setState(STATE_FALL);
	}

	if(controlDown&PI_JUMP)
	{
		_playerMode->setState(STATE_BUTTBOUNCE);
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
		_playerMode->slowdown();
	}
}


/*===========================================================================
 end */

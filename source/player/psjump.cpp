/*=========================================================================

	psjump.cpp

	Author:		PKG
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/


/*----------------------------------------------------------------------
	Includes
	-------- */

#include "player\psjump.h"

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

CPlayerStateJump			s_stateJump;


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CPlayerStateJump::enter(CPlayerModeBase *_playerMode)
{
	// If already in this state then don't do the entry code
	if(_playerMode->getState()!=STATE_JUMP)
	{
		_playerMode->setAnimNo(ANIM_SPONGEBOB_JUMP);
		m_jumpFrames=0;

		_playerMode->jump();

		CSoundMediator::playSfx(CSoundMediator::SFX_SPONGEBOB_JUMP);
	}
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CPlayerStateJump::think(CPlayerModeBase *_playerMode)
{
	const PlayerMetrics	*metrics;
	int					controlHeld,controlDown;

	metrics=_playerMode->getPlayerMetrics();
	controlHeld=_playerMode->getPadInputHeld();
	controlDown=_playerMode->getPadInputDown();

	_playerMode->advanceAnimFrameAndCheckForEndOfAnim();

	if(m_jumpFrames<=metrics->m_metric[PM__MAX_JUMP_FRAMES]&&controlHeld&PI_JUMP)
	{
		m_jumpFrames++;
	}
	else
	{
		_playerMode->setState(STATE_FALL);
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

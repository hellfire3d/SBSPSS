/*=========================================================================

	psjmpbck.cpp

	Author:		PKG
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/


/*----------------------------------------------------------------------
	Includes
	-------- */

#include "player\psjmpbck.h"

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

CPlayerStateJumpBack		s_stateJumpBack;



/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CPlayerStateJumpBack::enter(CPlayerModeBase *_playerMode)
{
	_playerMode->setAnimNo(_playerMode->getPlayer()->wasLastHitElectrical()?ANIM_SPONGEBOB_ELECTROCUTED:ANIM_SPONGEBOB_GETHIT);

	if(_playerMode->getState()!=STATE_JUMPBACK)
	{
		m_reactFrames=0;
	}

	_playerMode->jumpback();

}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CPlayerStateJumpBack::think(CPlayerModeBase *_playerMode)
{
	const PlayerMetrics	*metrics;
	int					xvel;

	metrics=_playerMode->getPlayerMetrics();

	_playerMode->advanceAnimFrameAndCheckForEndOfAnim();

	if(m_reactFrames<=metrics->m_metric[PM__HITREACT_FRAMES])
	{
		m_reactFrames++;
	}
	else
	{
		_playerMode->setState(STATE_FALL);
	}
}


/*===========================================================================
 end */

/*=========================================================================

	psdance.cpp

	Author:		PKG
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/


/*----------------------------------------------------------------------
	Includes
	-------- */

#include "player\psdance.h"

#ifndef __PLAYER_PLAYER_H__
#include "player\player.h"
#endif

#ifndef __PLAYER_PMODES_H__
#include "player\pmodes.h"
#endif

#ifndef	__GAME_CONVO_H__
#include "game\convo.h"
#endif

#ifndef __GAME_GAME_H__
#include "game\game.h"
#endif


/*	Std Lib
	------- */

/*	Data
	---- */

#ifndef __FILE_EQUATES_H__
#include <biglump.h>
#endif

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

CPlayerStateDance			s_stateDance;



static const FileEquate	s_celebrationScripts[5][4]=
{
	{	SCRIPTS_C1L1_CELEBRATE_DAT,	SCRIPTS_C1L2_CELEBRATE_DAT,	SCRIPTS_C1L3_CELEBRATE_DAT,	SCRIPTS_C1L4_CELEBRATE_DAT	},
	{	SCRIPTS_C2L1_CELEBRATE_DAT,	SCRIPTS_C2L2_CELEBRATE_DAT,	SCRIPTS_C2L3_CELEBRATE_DAT,	SCRIPTS_C2L4_CELEBRATE_DAT	},
	{	SCRIPTS_C3L1_CELEBRATE_DAT,	SCRIPTS_C3L2_CELEBRATE_DAT,	SCRIPTS_C3L3_CELEBRATE_DAT,	SCRIPTS_C3L4_CELEBRATE_DAT	},
	{	SCRIPTS_C4L1_CELEBRATE_DAT,	SCRIPTS_C4L2_CELEBRATE_DAT,	SCRIPTS_C4L3_CELEBRATE_DAT,	SCRIPTS_C4L4_CELEBRATE_DAT	},
	{	SCRIPTS_C5L1_CELEBRATE_DAT,	SCRIPTS_C5L2_CELEBRATE_DAT,	SCRIPTS_C5L3_CELEBRATE_DAT,	SCRIPTS_C5L4_CELEBRATE_DAT	},
};



/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CPlayerStateDance::enter(CPlayerModeBase *_playerMode)
{
	_playerMode->setAnimNo(ANIM_SPONGEBOB_JUMP);
	m_hitGround=false;
	m_startedConversation=false;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CPlayerStateDance::think(CPlayerModeBase *_playerMode)
{
	_playerMode->slowdown();
	if(!m_hitGround)
	{
		if(_playerMode->getHeightFromGound()<=0)
		{
			_playerMode->setAnimNo(ANIM_SPONGEBOB_CELEBRATE);
			m_hitGround=true;
		}
		else
		{
			_playerMode->fallToDance();
		}
	}
	else
	{
		if(!m_startedConversation)
		{
			if(_playerMode->advanceAnimFrameAndCheckForEndOfAnim())
			{
				_playerMode->setAnimNo(ANIM_SPONGEBOB_IDLEBREATH);
				CConversation::trigger(s_celebrationScripts[GameScene.getChapterNumber()-1][GameScene.getLevelNumber()-1]);
				m_startedConversation=true;
			}
		}
		else
		{
			if(!CConversation::isActive())
			{
				GameScene.getPlayer()->setCanExitLevelNow();
				_playerMode->setState(STATE_IDLE);
			}
		}
	}
}


/*===========================================================================
 end */

/*=========================================================================

	psbutt.cpp

	Author:		PKG
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/


/*----------------------------------------------------------------------
	Includes
	-------- */

#include "player\psbutt.h"

#ifndef __PAD_VIBE_H__
#include "pad\vibe.h"
#endif

#ifndef __PLAYER_PLAYER_H__
#include "player\player.h"
#endif

#ifndef __PLAYER_PMODES_H__
#include "player\pmodes.h"
#endif

#ifndef __GAME_GAMEBUBS_H__
#include "game\gamebubs.h"
#endif

#ifndef __LEVEL_LEVEL_H__
#include "level\level.h"
#endif

#ifndef __GAME_GAME_H__
#include "game\game.h"
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

CPlayerStateButtBounce		s_stateButtBounce;
CPlayerStateButtBounceFall	s_stateButtBounceFall;
CPlayerStateButtBounceLand	s_stateButtBounceLand;
CPlayerStateButtBounceUp	s_stateButtBounceUp;


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CPlayerStateButtBounce::enter(CPlayerModeBase *_playerMode)
{
	DVECTOR	move;
	_playerMode->setAnimNo(ANIM_SPONGEBOB_BUTTBOUNCESTART);
	move=_playerMode->getMoveVelocity();
	move.vy=0;
	_playerMode->setMoveVelocity(&move);
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CPlayerStateButtBounce::think(CPlayerModeBase *_playerMode)
{
	int	controlHeld;
	controlHeld=_playerMode->getPadInputHeld();

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

	if(_playerMode->advanceAnimFrameAndCheckForEndOfAnim())
	{
		_playerMode->setState(STATE_BUTTFALL);
	}
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CPlayerStateButtBounceFall::enter(CPlayerModeBase *_playerMode)
{
	_playerMode->setAnimNo(ANIM_SPONGEBOB_BUTTBOUNCEEND);
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CPlayerStateButtBounceFall::think(CPlayerModeBase *_playerMode)
{
	int	controlHeld;
	controlHeld=_playerMode->getPadInputHeld();

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

	_playerMode->buttFall();
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CPlayerStateButtBounceLand::enter(CPlayerModeBase *_playerMode)
{
	m_bounceOffFloor=false;
	if(_playerMode->getIsInWater())
	{
		DVECTOR	const &pos=_playerMode->getPlayerPos();

		if((CGameScene::getCollision()->getCollisionBlock(pos.vx,pos.vy)&COLLISION_TYPE_MASK)==COLLISION_TYPE_FLAG_DESTRUCTABLE_WALL)
		{
			CLevel	&level=GameScene.GetLevel();
			level.destroyMapArea(pos);
			m_bounceOffFloor=true;
		}
	}
	if(!m_bounceOffFloor)
	{
		CPadVibrationManager::setVibration(0,CPadVibrationManager::VIBE_SHORT);
		CGameScene::setCameraShake(0,8);
	}
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CPlayerStateButtBounceLand::think(CPlayerModeBase *_playerMode)
{
	if(m_bounceOffFloor)
	{
		_playerMode->setState(STATE_BUTTBOUNCEUP);
	}

	if(_playerMode->advanceAnimFrameAndCheckForEndOfAnim())
	{
		_playerMode->setState(STATE_IDLE);
	}
	_playerMode->zeroMoveVelocity();	
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CPlayerStateButtBounceUp::enter(CPlayerModeBase *_playerMode)
{
	if(_playerMode->getIsInWater())
	{
		DVECTOR	const &pos=_playerMode->getPlayerPos();
		CGameBubicleFactory::spawnBubicles(pos.vx-20,pos.vy,40,10,CGameBubicleFactory::TYPE_MEDIUM);
		CPadVibrationManager::setVibration(0,CPadVibrationManager::VIBE_MEDIUM);
		CGameScene::setCameraShake(0,8);
	}

	_playerMode->setAnimNo(ANIM_SPONGEBOB_BUTTBOUNCEEND);
	m_bounceFrames=0;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int bounceUpFrames=5;
void CPlayerStateButtBounceUp::think(CPlayerModeBase *_playerMode)
{
	int					controlHeld;

	controlHeld=_playerMode->getPadInputHeld();
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

	if(m_bounceFrames<=bounceUpFrames)
	{
		m_bounceFrames++;
		_playerMode->jump();
	}
	else
	{
		_playerMode->setState(STATE_FALL);
	}
}


/*===========================================================================
 end */

/*=========================================================================

	pmbloon.cpp

	Author:		PKG
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

/*----------------------------------------------------------------------
	Includes
	-------- */

#include "player\pmbloon.h"

#ifndef __PAD_VIBE_H__
#include "pad\vibe.h"
#endif

#ifndef __GFX_SPRBANK_H__
#include "gfx\sprbank.h"
#endif

#ifndef __LEVEL_LEVEL_H__
#include "level\level.h"
#endif

#include	"game/game.h"

/*	Std Lib
	------- */

/*	Data
	---- */

#ifndef	__ANIM_SPONGEBOB_HEADER__
#include <ACTOR_SPONGEBOB_ANIM.h>
#endif

#ifndef __SPR_SPRITES_H__
#include <sprites.h>
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

static PlayerMetrics	s_playerMetrics=
{	{
	DEFAULT_PLAYER_JUMP_VELOCITY,			// PM__JUMP_VELOCITY
	DEFAULT_PLAYER_MAX_JUMP_FRAMES,			// PM__MAX_JUMP_FRAMES
	DEFAULT_PLAYER_MAX_SAFE_FALL_FRAMES,	// PM__MAX_SAFE_FALL_FRAMES
	DEFAULT_PLAYER_MAX_RUN_VELOCITY,		// PM__MAX_RUN_VELOCITY
	DEFAULT_PLAYER_RUN_SPEEDUP,				// PM__RUN_SPEEDUP
	DEFAULT_PLAYER_RUN_REVERSESLOWDOWN,		// PM__RUN_REVERSESLOWDOWN
	DEFAULT_PLAYER_RUN_SLOWDOWN,			// PM__RUN_SLOWDOWN
	DEFAULT_PLAYER_PLAYER_GRAVITY/3,		// PM__GRAVITY
	DEFAULT_PLAYER_TERMINAL_VELOCITY/3,		// PM__TERMINAL_VELOCITY
	DEFAULT_BUTT_FALL_VELOCITY,				// PM__BUTT_FALL_VELOCITY
	DEFAULT_HITREACT_XVELOCITY,				// PM__HITREACT_XVELOCITY
	DEFAULT_HITREACT_YVELOCITY,				// PM__HITREACT_YVELOCITY
	DEFAULT_HITREACT_FRAMES,				// PM__HITREACT_FRAMES
}	};


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CPlayerModeBalloon::enter()
{
	CPlayerModeBase::enter();
	m_timer=0;
	m_playedPopSound=false;
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CPlayerModeBalloon::think()
{
	CPlayerModeBase::think();
	if(++m_timer>BALLOON_TIMEOUT)
	{
		if(!m_playedPopSound)
		{
			CSoundMediator::playSfx(CSoundMediator::SFX_BALLOON_POP);
			CPadVibrationManager::setVibration(0,CPadVibrationManager::VIBE_SHORT);
		}
		m_player->setMode(PLAYER_MODE_BASICUNARMED);
	}
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int balloonx=-14;
int balloony=-120;
int balloonsize=350;
void	CPlayerModeBalloon::render(DVECTOR *_pos)
{
	DVECTOR	pos;
	int		frame;

	CPlayerModeBase::render();

	pos.vx=_pos->vx+balloonx;
	pos.vy=_pos->vy+balloony;
	if(m_timer<BALLOON_TIMEOUT-BALLOON_POP_FRAMES)
	{
		frame=FRM__SMALL_BALLOON;
	}
	else
	{
		frame=FRM__BALLOONBURST;
		if(!m_playedPopSound)
		{
			CSoundMediator::playSfx(CSoundMediator::SFX_BALLOON_POP);
			CPadVibrationManager::setVibration(0,CPadVibrationManager::VIBE_SHORT);
			m_playedPopSound=true;
		}
	}
	CGameScene::getSpriteBank()->printFT4Scaled(frame,pos.vx,pos.vy,0,0,5,balloonsize);
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CPlayerModeBalloon::renderModeUi()
{
	if(m_timer<BALLOON_TIMEOUT-BALLOON_FLASH_TIME||m_timer&2)
	{
		SpriteBank	*sb;
		sFrameHdr	*fh;

		sb=CGameScene::getSpriteBank();
		fh=sb->getFrameHeader(FRM__SMALL_BALLOON);

		sb->printFT4Scaled(fh,CPlayer::POWERUPUI_ICONX,CPlayer::POWERUPUI_ICONY,0,0,CPlayer::POWERUPUI_OT,384);
	}
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
const struct PlayerMetrics	*CPlayerModeBalloon::getPlayerMetrics()
{
	return &s_playerMetrics;
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CPlayerModeBalloon::setAnimNo(int _animNo)
{
	// I think I'll overload the JUMP anim to the BALLOONJUMP here..
	// Just 'cos I can :)
	if(_animNo==ANIM_SPONGEBOB_JUMP)
	{
		_animNo=ANIM_SPONGEBOB_BALLOONJUMP;
	}
	CPlayerModeBase::setAnimNo(_animNo);
}

/*===========================================================================
end */

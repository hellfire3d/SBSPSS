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

#ifndef __GFX_SPRBANK_H__
#include "gfx\sprbank.h"
#endif

#ifndef __LEVEL_LEVEL_H__
#include "level\level.h"
#endif


/*	Std Lib
	------- */

/*	Data
	---- */

#ifndef	__ANIM_SPONGEBOB_HEADER__
#include <ACTOR_SPONGEBOB_ANIM.h>
#endif

#ifndef __SPR_INGAMEFX_H__
#include <ingamefx.h>
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
}	};


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CPlayerModeBalloon::think()
{
	CPlayerModeBase::think();
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

	CPlayerModeBase::render();

	pos.vx=_pos->vx+balloonx;
	pos.vy=_pos->vy+balloony;
	m_player->getSpriteBank()->printFT4Scaled(FRM__BALLOON,pos.vx,pos.vy,0,0,5,balloonsize);
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

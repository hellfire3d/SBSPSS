
/*=========================================================================

	psfall.cpp

	Author:		PKG
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/


/*----------------------------------------------------------------------
	Includes
	-------- */

#include "player\psfall.h"

#ifndef __PLAYER_PLAYER_H__
#include "player\player.h"
#endif

#ifndef __PAD_PADS_H__
#include "pad\pads.h"
#endif




/*	Std Lib
	------- */

/*	Data
	---- */

#ifndef	__ANIM_PLAYER_ANIM_HEADER__
#include <player_anim.h>
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
void CPlayerStateFall::enter(CPlayer *_player)
{
	setAnimNo(_player,ANIM_PLAYER_ANIM_JUMPSTART);
	m_fallFrames=0;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CPlayerStateFall::think(CPlayer *_player)
{
	const PlayerMetrics	*metrics;
	int					control;
	DVECTOR				move;

	metrics=getPlayerMetrics(_player);
	control=getPadInput(_player);

	move=getMoveVelocity(_player);
	move.vy+=metrics->m_metric[PM__GRAVITY_VALUE];
	if(move.vy>=metrics->m_metric[PM__TERMINAL_VELOCITY]<<CPlayer::VELOCITY_SHIFT)
	{
		move.vy=metrics->m_metric[PM__TERMINAL_VELOCITY]<<CPlayer::VELOCITY_SHIFT;
		m_fallFrames++;
		if(m_fallFrames>metrics->m_metric[PM__MAX_SAFE_FALL_FRAMES])
		{
			setState(_player,STATE_FALLFAR);
		}
	}
	setMoveVelocity(_player,&move);

	if(control&CPadConfig::getButton(CPadConfig::PAD_CFG_LEFT))
	{
		moveLeft(_player);
	}
	else if(control&CPadConfig::getButton(CPadConfig::PAD_CFG_RIGHT))
	{
		moveRight(_player);
	}
	else
	{
		slowdown(_player);
	}

	if(control&CPadConfig::getButton(CPadConfig::PAD_CFG_DOWN))
	{
		move.vx=0;
		move.vy=0;
		setMoveVelocity(_player,&move);
		setState(_player,STATE_BUTTBOUNCE);
	}
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CPlayerStateFallFar::enter(CPlayer *_player)
{
	setAnimNo(_player,ANIM_PLAYER_ANIM_FALL);
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CPlayerStateFallFar::think(CPlayer *_player)
{
	int				control;
	DVECTOR			move;

	control=getPadInput(_player);

	if(control&CPadConfig::getButton(CPadConfig::PAD_CFG_LEFT))
	{
		moveLeft(_player);
	}
	else if(control&CPadConfig::getButton(CPadConfig::PAD_CFG_RIGHT))
	{
		moveRight(_player);
	}
	else
	{
		slowdown(_player);
	}
}


/*===========================================================================
 end */

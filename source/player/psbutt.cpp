
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
void CPlayerStateButtBounce::enter(CPlayer *_player)
{
	setAnimNo(_player,ANIM_PLAYER_ANIM_BUTTBOUNCE);
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CPlayerStateButtBounce::think(CPlayer *_player)
{
	if(advanceAnimFrameAndCheckForEndOfAnim(_player))
	{
		setState(_player,STATE_BUTTFALL);
	}
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CPlayerStateButtBounceFall::enter(CPlayer *_player)
{
//	setAnimNo(_player,ANIM_PLAYER_ANIM_JUMPSTART);
	m_fallFrames=0;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CPlayerStateButtBounceFall::think(CPlayer *_player)
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
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CPlayerStateButtBounceLand::enter(CPlayer *_player)
{
	setAnimNo(_player,ANIM_PLAYER_ANIM_BUTTBOUNCE);
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CPlayerStateButtBounceLand::think(CPlayer *_player)
{
	if(advanceAnimFrameAndCheckForEndOfAnim(_player))
	{
		setState(_player,STATE_IDLE);
	}
}


/*===========================================================================
 end */

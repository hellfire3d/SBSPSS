
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
	int	control;
	DVECTOR	move;

	control=getPadInput(_player);

	move=getMoveVelocity(_player);
	move.vy+=GRAVITY_VALUE;
	if(move.vy>=TERMINAL_VELOCITY<<PSHIFT)
	{
		move.vy=TERMINAL_VELOCITY<<PSHIFT;
		m_fallFrames++;
		if(m_fallFrames>MAX_SAFE_FALL_FRAMES)
		{
			setState(_player,STATE_FALLFAR);
		}
	}
	setMoveVelocity(_player,&move);

	if(control&PAD_LEFT)
	{
		moveLeft(_player);
	}
	else if(control&PAD_RIGHT)
	{
		moveRight(_player);
	}
	else
	{
		slowdown(_player);
	}

/*
	if(isOnSolidGround(_player))
	{
		move=getMoveVelocity(_player);
//		if(move.vx)
//		{
//			setState(_player,STATE_RUN);
//		}
//		else
		{
			setState(_player,STATE_IDLE);
		}
		move.vy=0;
		setMoveVelocity(_player,&move);
	}
*/
}


/*===========================================================================
 end */


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
void CPlayerStateJump::enter(CPlayer *_player)
{
	setAnimNo(_player,ANIM_PLAYER_ANIM_JUMPSTART);
	m_jumpFrames=0;
	DVECTOR	move=getMoveVelocity(_player);
	move.vy=-JUMP_VELOCITY<<PSHIFT;
	setMoveVelocity(_player,&move);
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CPlayerStateJump::think(CPlayer *_player)
{
	int	control;
	control=getPadInput(_player);

	if(m_jumpFrames<=MAX_JUMP_FRAMES&&control&PAD_CROSS)
	{
		m_jumpFrames++;
	}
	else
	{
		DVECTOR	move;
		move=getMoveVelocity(_player);
		if(move.vy>=0)
		{
			setState(_player,STATE_FALL);
		}
		else
		{
			move.vy+=GRAVITY_VALUE;
		}
		setMoveVelocity(_player,&move);
	}

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
}


/*===========================================================================
 end */

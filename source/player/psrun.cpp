/*=========================================================================

	psrun.cpp

	Author:		PKG
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/


/*----------------------------------------------------------------------
	Includes
	-------- */

#include "player\psrun.h"

#ifndef __PLAYER_PLAYER_H__
#include "player\player.h"
#endif

#ifndef	__UTILS_HEADER__
#include "utils\utils.h"
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
void CPlayerStateRun::enter(CPlayer *_player)
{
	int	control;
	control=getPadInput(_player);

	if(getMoveVelocity(_player).vx)
	{
		setAnimNo(_player,ANIM_PLAYER_ANIM_RUN);
	}
	else
	{
		setAnimNo(_player,ANIM_PLAYER_ANIM_RUNSTART);
	}

	if(control&PAD_LEFT)
	{
		setFacing(_player,FACING_LEFT);
	}
	else if(control&PAD_RIGHT)
	{
		setFacing(_player,FACING_RIGHT);
	}
}


/*----------------------------------------------------------------------
  Function:
  Purpose:
  Params:
  Returns:
---------------------------------------------------------------------- */
void CPlayerStateRun::think(CPlayer *_player)
{
	int	control;
	control=getPadInput(_player);

	if(control&PAD_CROSS)
	{
		setState(_player,STATE_JUMP);
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
		DVECTOR	move;
		move=getMoveVelocity(_player);
		if(move.vx==0)
		{
			setState(_player,STATE_IDLE);
			setAnimNo(_player,ANIM_PLAYER_ANIM_RUNSTOP);
		}
		else
		{
			slowdown(_player);
		}
	}

	if(advanceAnimFrameAndCheckForEndOfAnim(_player))
	{
		setAnimNo(_player,ANIM_PLAYER_ANIM_RUN);
	}
}


/*===========================================================================
 end */


/*=========================================================================

	pschop.cpp

	Author:		PKG
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/


/*----------------------------------------------------------------------
	Includes
	-------- */

#include "player\pschop.h"

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
void CPlayerStateChop::enter(CPlayer *_player)
{
	setAnimNo(_player,ANIM_PLAYER_ANIM_KARATE);
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CPlayerStateChop::think(CPlayer *_player)
{
	if(advanceAnimFrameAndCheckForEndOfAnim(_player))
	{
		setState(_player,STATE_IDLE);
	}
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CPlayerStateRunChop::think(CPlayer *_player)
{
	int	control;
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
		setState(_player,STATE_RUN);
	}
}


/*===========================================================================
 end */

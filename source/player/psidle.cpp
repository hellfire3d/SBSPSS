/*=========================================================================

	psidle.cpp

	Author:		PKG
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/


/*----------------------------------------------------------------------
	Includes
	-------- */

#include "player\psidle.h"

#ifndef __PLAYER_PLAYER_H__
#include "player\player.h"
#endif

#ifndef __PAD_PADS_H__
#include "pad\pads.h"
#endif

#ifndef	__UTILS_HEADER__
#include "utils\utils.h"
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
void CPlayerStateIdle::enter(CPlayer *_player)
{
	setAnimNo(_player,ANIM_PLAYER_ANIM_IDLEGENERIC04);
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CPlayerStateIdle::think(CPlayer *_player)
{
	int	control;
	control=getPadInput(_player);
	
	if(control&CPadConfig::getButton(CPadConfig::PAD_CFG_JUMP))
	{
		setState(_player,STATE_JUMP);
	}
	else if(control&(CPadConfig::getButton(CPadConfig::PAD_CFG_LEFT)|CPadConfig::getButton(CPadConfig::PAD_CFG_RIGHT)))
	{
		setState(_player,STATE_RUN);
	}
	else if(control&CPadConfig::getButton(CPadConfig::PAD_CFG_ACTION))
	{
		setState(_player,STATE_ATTACK);
	}
	else if(control&CPadConfig::getButton(CPadConfig::PAD_CFG_DOWN))
	{
		setState(_player,STATE_DUCK);
	}
	else if(advanceAnimFrameAndCheckForEndOfAnim(_player))
	{
		if(getRndRange(100)<95)
			setAnimNo(_player,ANIM_PLAYER_ANIM_IDLEGENERIC04);
		else
			setAnimNo(_player,ANIM_PLAYER_ANIM_IDLEGENERIC03);
	}
}


/*===========================================================================
 end */

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
	int	controlHeld;
	controlHeld=getPadInputHeld(_player);

	if(getMoveVelocity(_player).vx)
	{
		setAnimNo(_player,ANIM_PLAYER_ANIM_RUN);
	}
	else
	{
		setAnimNo(_player,ANIM_PLAYER_ANIM_RUNSTART);
	}

	if(controlHeld&CPadConfig::getButton(CPadConfig::PAD_CFG_LEFT))
	{
		setFacing(_player,FACING_LEFT);
	}
	else if(controlHeld&CPadConfig::getButton(CPadConfig::PAD_CFG_RIGHT))
	{
		setFacing(_player,FACING_RIGHT);
	}

//	m_numberOfTimeAnimHasLooped=0;
}


/*----------------------------------------------------------------------
  Function:
  Purpose:
  Params:
  Returns:
---------------------------------------------------------------------- */
void CPlayerStateRun::think(CPlayer *_player)
{
	int	controlDown,controlHeld;
	controlDown=getPadInputDown(_player);
	controlHeld=getPadInputHeld(_player);

	if(controlDown&CPadConfig::getButton(CPadConfig::PAD_CFG_JUMP))
	{
		setState(_player,STATE_JUMP);
	}
	if(controlHeld&CPadConfig::getButton(CPadConfig::PAD_CFG_DOWN))
	{
		setState(_player,STATE_DUCK);
	}
	if(controlHeld&CPadConfig::getButton(CPadConfig::PAD_CFG_ACTION))
	{
		setState(_player,STATE_RUNATTACK);
	}

	if(controlHeld&CPadConfig::getButton(CPadConfig::PAD_CFG_LEFT))
	{
		moveLeft(_player);
	}
	else if(controlHeld&CPadConfig::getButton(CPadConfig::PAD_CFG_RIGHT))
	{
		moveRight(_player);
	}
	else
	{
		if(getMoveVelocity(_player).vx==0)
		{
			setState(_player,STATE_IDLE);
//			if(m_numberOfTimeAnimHasLooped>=2)
//			{
//				setAnimNo(_player,ANIM_PLAYER_ANIM_RUNSTOP);
//			}
		}
		else
		{
			slowdown(_player);
		}
	}

	if(advanceAnimFrameAndCheckForEndOfAnim(_player))
	{
		setAnimNo(_player,ANIM_PLAYER_ANIM_RUN);
//		m_numberOfTimeAnimHasLooped++;
	}
}


/*===========================================================================
 end */

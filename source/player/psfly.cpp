/*=========================================================================

	psfly.cpp

	Author:		PKG
	Created: 
	Project:	Spongebob
	Purpose:	Debugginf mode to allow SB to fly around the map

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/


/*----------------------------------------------------------------------
	Includes
	-------- */

#include "player\psfly.h"

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
	setAnimNo(_player,ANIM_PLAYER_ANIM_IDLEHOOLA);
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CPlayerStateFall::think(CPlayer *_player)
{
	int		controlHeld;
	DVECTOR	pos;

	controlHeld=getPadInputHeld(_player);
	pos=getPlayerPos(_player);
	if(controlHeld&PI_UP)
	{
		pos.vx-=5;
	}
	else if(controlHeld&PI_DOWN)
	{
		pos.vx+=5;
	}
	if(controlHeld&PI_LEFT)
	{
		pos.vy-=5;
	}
	else if(controlHeld&PI_RIGHT)
	{
		pos.vy+=5;
	}
	this->setPlayerPos(_player,pos);

	advanceAnimFrameAndCheckForEndOfAnim(_player);	
}


/*===========================================================================
 end */

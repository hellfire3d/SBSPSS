/*=========================================================================

	psdead.cpp

	Author:		PKG
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/


/*----------------------------------------------------------------------
	Includes
	-------- */

#include "player\psdead.h"

#ifndef __PLAYER_PLAYER_H__
#include "player\player.h"
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
void CPlayerStateDead::enter(CPlayer *_player)
{
	setAnimNo(_player,ANIM_PLAYER_ANIM_DEATHSPIN);
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CPlayerStateDead::think(CPlayer *_player)
{
	if(advanceAnimFrameAndCheckForEndOfAnim(_player))
	{
		setState(_player,STATE_IDLE);
	}

}


/*===========================================================================
 end */

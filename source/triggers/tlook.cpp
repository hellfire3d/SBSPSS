/*=========================================================================

	tlook.cpp

	Author:		PKG
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

/*----------------------------------------------------------------------
	Includes
	-------- */

#include "triggers\tlook.h"

#ifndef __GAME_GAME_H__
#include "game\game.h"
#endif

#ifndef	__PLAYER_PLAYER_H__
#include "player\player.h"
#endif

	
/*	Std Lib
	------- */

/*	Data
	---- */

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
void	CLookTrigger::collidedWith(CThing *_thisThing)
{
	CPlayer	*player;
	DVECTOR	collCentre;
	CRECT	collArea;

	ASSERT(_thisThing->getThingType()==TYPE_PLAYER);

	// Only collide if players feet are inside the trigger
	player=GameScene.getPlayer();
	collCentre=player->getCollisionCentre();
	collArea.x1=collCentre.vx-5;
	collArea.y1=collCentre.vy-5;
	collArea.x2=collArea.x1+10;
	collArea.y2=collArea.y1+10;
	if(checkCollisionAgainstArea(&collArea))
	{
		player->setLedgeLookAhead(+4);
	}
}

/*===========================================================================
end */

/*=========================================================================

	trestart.cpp

	Author:		PKG
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

/*----------------------------------------------------------------------
	Includes
	-------- */

#include "triggers\trestart.h"

#ifndef __GAME_GAME_H__
#include "game\game.h"
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
void	CLevelExitTrigger::collidedWith(CThing *_thisThing)
{
	CRect	collisionSize;
	DVECTOR	respawnPos;

	ASSERT(_thisThing->getThingType()==TYPE_PLAYER);

	collisionSize=getCollisionSize();
	respawnPos.vx=collisionSize.x1+((collisionSize.x2-collisionSize.x1)/2);
	respawnPos.vy=collisionSize.y2;
	((CPlayer*)_thisThing)->setRespawnPos(respawnPos);
}

/*===========================================================================
end */

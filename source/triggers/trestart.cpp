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

#include "triggers\trigger.h"
#include "triggers\trestart.h"

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
void	CRestartPointTrigger::collidedWith(CThing *_thisThing)
{
//	CRECT	collisionArea;
	DVECTOR	respawnPos;

	switch( _thisThing->getThingType() )
	{
		case TYPE_PLAYER:
		{
CRECT const &collisionArea=getCollisionArea();
			respawnPos.vx=collisionArea.x1+((collisionArea.x2-collisionArea.x1)/2);
			respawnPos.vy=collisionArea.y2;
			((CPlayer*)_thisThing)->setRespawnPos(respawnPos);

			break;
		}

		default:
			break;
	}
}

/*===========================================================================
end */

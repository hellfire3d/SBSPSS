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

#include "triggers\trigger.h"
#include "triggers\tlook.h"

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
	DVECTOR	collCentre;
	CRECT	collArea;

	switch( _thisThing->getThingType() )
	{
		case TYPE_PLAYER:
		{
			// Only collide if players feet are inside the trigger
			collCentre=((CPlayer*)_thisThing)->getCollisionCentre();
			collArea.x1=collCentre.vx-5;
			collArea.y1=collCentre.vy-5;
			collArea.x2=collArea.x1+10;
			collArea.y2=collArea.y1+10;
			if(checkCollisionAgainstArea(&collArea))
			{
				((CPlayer*)_thisThing)->setLedgeLookAhead(m_val0);
			}

			break;
		}

		default:
			break;
	}
}

/*===========================================================================
end */

/*=========================================================================

	tteleprt.cpp

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
#include "triggers\tteleprt.h"

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
void	CTeleportTrigger::collidedWith(CThing *_thisThing)
{
	ASSERT(_thisThing->getThingType()==TYPE_PLAYER);

	((CPlayer*)_thisThing)->teleportTo(m_boxX1+8,m_boxY1+16);

PAUL_DBGMSG("HIT TELEPORT TRIGGER");
}

/*===========================================================================
end */

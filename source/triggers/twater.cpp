/*=========================================================================

	twater.cpp

	Author:		PKG
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

/*----------------------------------------------------------------------
	Includes
	-------- */

#include "triggers\twater.h"

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
void	CInWaterTrigger::collidedWith(CThing *_thisThing)
{
	if(_thisThing->getThingType()==TYPE_PLAYER)
	{
		((CPlayer*)_thisThing)->setIsInWater(true);
	}
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	COutOfWaterTrigger::collidedWith(CThing *_thisThing)
{
	if(_thisThing->getThingType()==TYPE_PLAYER)
	{
		((CPlayer*)_thisThing)->setIsInWater(false);
	}
}

/*===========================================================================
end */

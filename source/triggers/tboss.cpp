/*=========================================================================

	tboss.cpp

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
#include "triggers\tboss.h"

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
void	CBossArenaTrigger::collidedWith(CThing *_thisThing)
{
	if ( !GameScene.getBossHasBeenKilled() )
	{
		switch( _thisThing->getThingType() )
		{
			case TYPE_PLAYER:
			{
				GameScene.hitBossArenaTrigger();
				shutdown();
				delete this;
				break;
			}

			default:
				break;
		}
	}
}

/*===========================================================================
end */

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

#ifndef __GAME_GAME_H__
#include	"game\game.h"
#endif

#ifndef	__PLAYER_PLAYER_H__
#include "player\player.h"
#endif

#ifndef __FRIEND_FRIEND_H__
#include "friend\friend.h"
#endif

#ifndef __FRIEND_FGARY_H__
#include "friend\fgary.h"
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
	int chapter=GameScene.getChapterNumber();
	int level=GameScene.getLevelNumber();

	switch( _thisThing->getThingType() )
	{
		case TYPE_PLAYER:
		{
			if ( chapter == 1 && level == 5 )
			{
				if ( !CNpcGaryFriend::hasReachedDoor() )
				{
					// if we're on chapter 1, level 5, check Gary has reached the teleport point
					// if he hasn't, don't let Spongey through

					return;
				}
			}

			((CPlayer*)_thisThing)->clearPlatform();
			((CPlayer*)_thisThing)->teleportTo(m_boxX1+8,m_boxY1+16);
			break;
		}

		case TYPE_NPC:
		{
			if ( _thisThing->getThingSubType() == CNpcFriend::NPC_FRIEND_GARY )
			{
				if ( chapter == 1 && level == 5 )
				{
					// if Gary has touched a teleport on chapter 1, level 5, remember that he has done so

					CNpcGaryFriend::setReachedDoor();
				}
			}

			break;
		}

		default:
			break;
	}
}

/*===========================================================================
end */

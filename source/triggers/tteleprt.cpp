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
#include "game\game.h"
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

#ifndef	__SOUND_SOUND_H__
#include "sound\sound.h"
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
			// SB *might* be chopping to hit this trigger, so we only do the collision
			// if he would normally hit it.
			int			x,y,w,h;
			DVECTOR		pos;
			CRECT		thisRect;
			CRECT const &thatRect=getCollisionArea();
			int collided=true;

			((CPlayer*)_thisThing)->getPlayerNormalCollisionSize(&x,&y,&w,&h);
			pos=_thisThing->getPos();
			thisRect.x1=pos.vx+x-(w/2);
			thisRect.y1=pos.vy+y-(h/2);
			thisRect.x2=thisRect.x1+w;
			thisRect.y2=thisRect.y1+h;

			if (thisRect.x2<thatRect.x1 || thisRect.x1>thatRect.x2) collided=false;
			if (thisRect.y2<thatRect.y1 || thisRect.y1>thatRect.y2) collided=false;
			if(collided)
			{
				// Ok.. so we've actually hit the thing properly :)
				if ( chapter == 5 && level == 5 )
				{
					if ( !CNpcGaryFriend::hasReachedDoor() )
					{
						// if we're on chapter 1, level 5, check Gary has reached the teleport point
						// if he hasn't, don't let Spongey through

						return;
					}
				}

				if(GameScene.triggerTeleportEffect())
				{
					CSoundMediator::SFXID	sfx;

					((CPlayer*)_thisThing)->clearPlatform();
					((CPlayer*)_thisThing)->teleportTo(m_boxX1+8,m_boxY1+16);
					
					if(chapter==5&&level==4)
					{
						sfx=CSoundMediator::SFX_TELEPORT1;
					}
					else
					{
						sfx=CSoundMediator::SFX_TELEPORT2;
					}

					CSoundMediator::playSfx(sfx);
				}
			}
			break;
		}

		case TYPE_NPC:
		{
			if ( _thisThing->getThingSubType() == CNpcFriend::NPC_FRIEND_GARY )
			{
				if ( chapter == 5 && level == 5 )
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

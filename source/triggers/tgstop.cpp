/*=========================================================================

	tgstop.cpp

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

/*----------------------------------------------------------------------
	Includes
	-------- */

#include "triggers\trigger.h"
#include "triggers\tgstop.h"

#ifndef __GAME_GAME_H__
#include "game\game.h"
#endif

#ifndef __FRIEND_FRIEND_H__
#include "friend\friend.h"
#endif

#ifndef __FRIEND_FGARY_H__
#include "friend\fgary.h"
#endif

#ifndef __PICKUPS_PICKUP_H__
#include "pickups\pickup.h"
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
void	CGaryStopTrigger::init()
{
	CTrigger::init();

	m_dropped = false;
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CGaryStopTrigger::setPositionAndSize(int _x,int _y,int _w,int _h)
{
	Pos.vx=_x+(_w/2) - 8;
	Pos.vy=_y+(_h/2);
	setCollisionSize(4,_h);
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CGaryStopTrigger::collidedWith(CThing *_thisThing)
{
	switch( _thisThing->getThingType() )
	{
		case TYPE_NPC:
		{
			if ( _thisThing->getThingSubType() == CNpcFriend::NPC_FRIEND_GARY )
			{
				CNpcGaryFriend *gary = (CNpcGaryFriend *) _thisThing;

				gary->stop( Pos.vx );

				if ( !m_dropped )
				{
					if ( GameScene.getChapterNumber() != 5 )
					{
						DVECTOR newPos = Pos;
						newPos.vy -= 8;

						createPickup(PICKUP__LIFE,&newPos);
					}

					m_dropped = true;
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

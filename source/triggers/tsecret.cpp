/*=========================================================================

	tsecret.cpp

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
#include "triggers\tsecret.h"

#ifndef	__SOUND_SOUND_H__
#include "sound\sound.h"
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
void	CSecretAreaTrigger::collidedWith(CThing *_thisThing)
{
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
				CSoundMediator::playSfx(CSoundMediator::SFX_SECRET_AREA);
				shutdown();
				delete this;
			}
			break;
		}

		default:
			break;
	}
}

/*===========================================================================
end */

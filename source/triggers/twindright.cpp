/*=========================================================================

	twindright.cpp

	Author:		Charles Blair
	Created:	
	Project:	Spongebob
	Purpose:	

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef	__TRIGGERS_TWINDRIGHT_H__
#include "triggers\twindright.h"
#endif

#ifndef	__PLAYER_PLAYER_H__
#include "player\player.h"
#endif

#ifndef __SYSTEM_GSTATE_H__
#include "system\gstate.h"
#endif


void	CWindRightTrigger::collidedWith(CThing *_thisThing)
{
	switch( _thisThing->getThingType() )
	{
		case TYPE_PLAYER:
		{
			CPlayer *player = (CPlayer *) _thisThing;

			DVECTOR move;
			move.vx = 4 * GameState::getFramesSinceLast();
			move.vy = 0;

			player->shove( move );
			player->setMoveVelocity( &move );
			player->setFloating();

			break;
		}

		default:
			break;
	}
}


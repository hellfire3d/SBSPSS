/*=========================================================================

	tifemit.h

	Author:		PKG
	Created:	
	Project:	Spongebob
	Purpose:	

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef	__TRIGGERS_TIFEMIT_H__
#include "triggers\tifemit.h"
#endif

#ifndef	__PLAYER_PLAYER_H__
#include	"player\player.h"
#endif

#ifndef __GAME_GAME_H__
#include	"game\game.h"
#endif


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void	CIntermittentFlameEmitterTrigger::init()
{
	CFlameEmitterTrigger::init();

	m_timer = 0;
	m_isActive = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void	CIntermittentFlameEmitterTrigger::think(int _frames)
{
	CTrigger::think(_frames);

	if ( m_timer <= 0 )
	{
		m_timer = GameState::getOneSecondInFrames() * 2;
		m_effect->toggleVisible();
		m_isActive = !m_isActive;
	}
	else
	{
		m_timer -= _frames;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void	CIntermittentFlameEmitterTrigger::collidedWith(CThing *_thisThing)
{
	if ( m_isActive )
	{
		switch(_thisThing->getThingType())
		{
			case TYPE_PLAYER:
			{
				CPlayer *player = (CPlayer *) _thisThing;

				if ( !player->isRecoveringFromHit() )
				{
					player->takeDamage( DAMAGE__KILL_OUTRIGHT );
				}

				break;
			}

			default:
				break;
		}
	}
}

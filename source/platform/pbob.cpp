/*=========================================================================

	pbob.cpp

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __PLATFORM_PBOB_H__
#include "platform\pbob.h"
#endif

#ifndef __GAME_GAME_H__
#include	"game\game.h"
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcBobbingPlatform::processMovement( int _frames )
{
	if ( m_contact )
	{
		CPlayer *player = GameScene.getPlayer();
		DVECTOR playerPos = player->getPos();

		int height = player->getHeightFromGroundNoPlatform( playerPos.vx, playerPos.vy );

		// if stood on, increase velocity

		if ( m_velocity < 0 )
		{
			m_velocity = 0;
		}
		else if ( m_velocity < 4 )
		{
			if ( height <= 0 )
			{
				m_velocity = 0;
			}
			else
			{
				m_velocity += 1;
			}
		}

		m_state = NPC_BOB_MOVE;
	}
	else
	{
		if ( m_state == NPC_BOB_MOVE )
		{
			// otherwise drop velocity and ultimately reverse course

			if ( m_velocity > -2 )
			{
				m_velocity--;
			}
		}
	}

	if ( m_velocity )
	{
		s32 moveY = m_velocity * _frames;

		if ( Pos.vy + moveY < m_initPos.vy )
		{
			Pos.vy = m_initPos.vy;
			m_velocity = 0;
			m_state = NPC_BOB_STOP;
			moveY = 0;
		}
		else
		{
			Pos.vy += moveY;
		}
	}
}
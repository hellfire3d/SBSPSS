/*=========================================================================

	pretract.cpp

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __PLATFORM_PRETRACT_H__
#include "platform\pretract.h"
#endif

#ifndef __GAME_GAME_H__
#include	"game\game.h"
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcRetractingPlatform::postInit()
{
	CNpcPlatform::postInit();

	m_timer = NPC_PLATFORM_TIMER_RETRACT;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcRetractingPlatform::processTimer( int _frames )
{
	switch( m_timerType )
	{
		case NPC_PLATFORM_TIMER_RETRACT:
		{
			if ( m_timer > 0 )
			{
				m_timer -= _frames;
			}
			else
			{
				m_timer = 4 * GameState::getOneSecondInFrames();
				m_timerType = NPC_PLATFORM_TIMER_EXTEND;
				m_detectCollision = false;
			}

			break;
		}

		case NPC_PLATFORM_TIMER_EXTEND:
		{
			if ( m_timer > 0 )
			{
				m_timer -= _frames;
			}
			else
			{
				m_timer = 4 * GameState::getOneSecondInFrames();
				m_timerType = NPC_PLATFORM_TIMER_RETRACT;
				m_detectCollision = true;
			}

			break;
		}
	}
}
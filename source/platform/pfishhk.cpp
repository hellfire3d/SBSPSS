/*=========================================================================

	pfishhk.cpp

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __PLATFORM_PFISHHK_H__
#include "platform\pfishhk.h"
#endif

#ifndef	__UTILS_HEADER__
#include	"utils\utils.h"
#endif

#ifndef __GAME_GAME_H__
#include "game\game.h"
#endif


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcFishHookPlatform::postInit()
{
	CNpcPlatform::postInit();

	m_isMoving = false;
	m_isShuttingDown = false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcFishHookPlatform::processLifetime( int _frames )
{
	if ( m_contact )
	{
		m_isMoving = true;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcFishHookPlatform::processMovement( int _frames )
{
	if ( m_isMoving )
	{
		Pos.vy -= m_data[m_type].speed * _frames;

		if ( Pos.vy < 0 )
		{
			m_isActive = false;
			m_timer = getRnd() % ( 4 * GameState::getOneSecondInFrames() );
			m_timerType = NPC_PLATFORM_TIMER_RESPAWN;
			m_isMoving = false;
		}
	}
}
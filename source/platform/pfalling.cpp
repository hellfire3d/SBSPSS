/*=========================================================================

	pfalling.cpp

	Author:		CRB
	Created:
	Project:	Spongebob
	Purpose:

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __PLATFORM_PFALLING_H__
#include "platform\pfalling.h"
#endif

#ifndef __GAME_GAME_H__
#include	"game\game.h"
#endif

#ifndef	__UTILS_HEADER__
#include	"utils\utils.h"
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcFallingPlatform::postInit()
{
	CNpcPlatform::postInit();

	m_npcPath.setPathType( CNpcPath::SINGLE_USE_PATH );

	m_isActive = false;
	m_timer = 4 * GameState::getOneSecondInFrames();
	m_timerType = NPC_PLATFORM_TIMER_RESPAWN;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcFallingPlatform::processMovement( int _frames )
{
	s32 moveX, moveY;
	s32 distX, distY, heading;
	bool pathComplete;

	m_npcPath.thinkVertical( Pos, &pathComplete, &distX, &distY, &heading );

	if ( pathComplete )
	{
		m_isActive = false;
		m_timer = getRnd() % ( 4 * GameState::getOneSecondInFrames() );
		m_timerType = NPC_PLATFORM_TIMER_RESPAWN;
	}
	else
	{
		moveX = 0;
		moveY = m_speed * _frames;

		if ( heading == 3072 )
		{
			moveY = -moveY;
		}

		s32 groundHeight = CGameScene::getCollision()->getHeightFromGround( Pos.vx + moveX, Pos.vy + moveY, 16 );

		if ( groundHeight < moveY )
		{
			moveY = groundHeight;
			moveX = 2 * _frames;
		}

		Pos.vx += moveX;
		Pos.vy += moveY;
	}
}
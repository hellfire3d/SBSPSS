/*=========================================================================

	nffolk.cpp

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/

#ifndef __ENEMY_NPC_H__
#include "enemy\npc.h"
#endif

#ifndef __ENEMY_NFFOLK_H__
#include "enemy\nffolk.h"
#endif

#ifndef	__UTILS_HEADER__
#include "utils\utils.h"
#endif

#ifndef __GAME_GAME_H__
#include	"game\game.h"
#endif


void CNpcFishFolk::postInit()
{
	m_heading = 0;

	DVECTOR newPos;

	newPos.vx = 200;
	newPos.vy = 400;

	//m_npcPath.addWaypoint( newPos );

	newPos.vx = 500;
	newPos.vy = 400;

	//m_npcPath.addWaypoint( newPos );

	m_npcPath.setPathType( CNpcPath::PONG_PATH );
}

void CNpcFishFolk::processMovementModifier( int _frames, s32 distX, s32 distY, s32 dist, s16 headingChange )
{
	Pos.vy += distY;

	if ( m_movementTimer > 0 )
	{
		Pos.vx += distX;

		m_movementTimer -= _frames;

		if ( m_movementTimer <= 0 )
		{
			m_timerTimer = GameState::getOneSecondInFrames() * ( ( getRnd() % 5 ) + 1 );
		}
	}
	else
	{
		if ( m_timerTimer > 0 )
		{
			// pause and look around

			m_timerTimer -= _frames;
		}
		else
		{
			if ( getRnd() % 2 )
			{
				m_npcPath.reversePathDir();
			}

			m_movementTimer = GameState::getOneSecondInFrames() * ( ( getRnd() & 10 ) + 4 );
		}
	}
}

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

#ifndef	__UTILS_HEADER__
#include "utils\utils.h"
#endif

#ifndef __GAME_GAME_H__
#include	"game\game.h"
#endif


void CNpc::processFishFolkMovementModifier( int _frames, s32 distX, s32 distY )
{
	if ( m_movementTimer > 0 )
	{
		Pos.vx += distX;
		Pos.vy += distY;

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

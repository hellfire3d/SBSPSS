/*=========================================================================

	nsstomp.cpp

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/

#ifndef __ENEMY_NPC_H__
#include "enemy\npc.h"
#endif

#ifndef	__ENEMY_NSSTOMP_H__
#include "enemy\nsstomp.h"
#endif

#ifndef __GAME_GAME_H__
#include "game\game.h"
#endif

#ifndef	__PLAYER_PLAYER_H__
#include "player\player.h"
#endif

#ifndef __ANIM_STOMPER_HEADER__
#include <ACTOR_STOMPER_ANIM.h>
#endif


void CNpcSkullStomperEnemy::postInit()
{
	m_extendDir = EXTEND_DOWN;
}

bool CNpcSkullStomperEnemy::processSensor()
{
	switch( m_sensorFunc )
	{
		case NPC_SENSOR_NONE:
			return( false );

		default:
		{
			if ( playerXDistSqr < 10000 && playerYDist >= 0 )
			{
				m_controlFunc = NPC_CONTROL_CLOSE;
				m_extendDir = EXTEND_DOWN;

				return( true );
			}
			else
			{
				return( false );
			}
		}
	}
}

void CNpcSkullStomperEnemy::processClose( int _frames )
{
	s8 groundHeight;
	s8 yMovement;

	if ( !m_animPlaying )
	{
		m_animPlaying = true;
		m_animNo = ANIM_STOMPER_CRUSH;
		m_frame = 0;
	}

	if ( m_timerTimer > 0 )
	{
		// wait
	}
	else
	{
		if ( m_extendDir == EXTEND_DOWN )
		{
			yMovement = m_data[m_type].speed * _frames;

			groundHeight = m_layerCollision->getHeightFromGround( Pos.vx, Pos.vy, yMovement + 16 );

			if ( groundHeight < yMovement )
			{
				// colliding with ground

				Pos.vy += groundHeight;

				// pause and change direction

				m_timerTimer = 3 * GameState::getOneSecondInFrames();
				m_extendDir = EXTEND_UP;
			}
			else
			{
				// drop down

				Pos.vy += yMovement;
			}
		}
		else
		{
			if ( m_base.vx - Pos.vx == 0 && m_base.vy - Pos.vy == 0 )
			{
				m_controlFunc = NPC_CONTROL_MOVEMENT;
				m_timerFunc = NPC_TIMER_ATTACK_DONE;
				m_timerTimer = GameState::getOneSecondInFrames();
				m_sensorFunc = NPC_SENSOR_NONE;
				m_npcPath.resetPath();
			}
			else
			{
				// return to original position

				processGenericGotoTarget( _frames, m_base.vx - Pos.vx, m_base.vy - Pos.vy, 1 );
			}
		}
	}
}
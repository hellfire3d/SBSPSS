/*=========================================================================

	nsshark.cpp

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/

#ifndef __ENEMY_NPC_H__
#include "enemy\npc.h"
#endif

#ifndef __GAME_GAME_H__
#include	"game\game.h"
#endif

#ifndef	__PLAYER_PLAYER_H__
#include	"player\player.h"
#endif

#ifndef __PROJECTL_PROJECTL_H__
#include "projectl\projectl.h"
#endif

void CNpc::processSubSharkMovement( int _frames )
{
	if ( m_timerTimer <= 0 )
	{
		if ( m_salvoCount > 0 )
		{
			// drop mine

			CProjectile *projectile;
			projectile = new( "test projectile" ) CProjectile;
			projectile->init( Pos, 1024 );

			m_salvoCount--;

			m_timerTimer = GameState::getOneSecondInFrames() * 1;
		}
	}

	if ( m_movementTimer > 0 )
	{
		m_movementTimer -= _frames;

		if ( m_extendDir == EXTEND_RIGHT )
		{
			s32 xDist = 300 - Pos.vx;
			s32 xDistSqr = xDist * xDist;

			if ( xDistSqr > 100 )
			{
				processGenericGotoTarget( _frames, xDist, 0, m_data[m_type].speed );
			}
			else
			{
				m_extendDir = EXTEND_LEFT;
			}
		}
		else
		{
			s32 xDist = -300 - Pos.vx;
			s32 xDistSqr = xDist * xDist;

			if ( xDistSqr > 100 )
			{
				processGenericGotoTarget( _frames, xDist, 0, m_data[m_type].speed );
			}
			else
			{
				m_extendDir = EXTEND_RIGHT;
			}
		}
	}
	else
	{
		m_controlFunc = NPC_CONTROL_CLOSE;
	}
}

void CNpc::processCloseSubSharkAttack( int _frames )
{
	CPlayer *player = GameScene.getPlayer();
	DVECTOR playerPos = player->getPos();

	s32 xDist = playerPos.vx - Pos.vx;
	s32 xDistSqr = xDist * xDist;

	switch( m_state )
	{
		case SUB_SHARK_MINE_1:
		{
			if ( xDist > 0 )
			{
				m_extendDir = EXTEND_RIGHT;
			}
			else
			{
				m_extendDir = EXTEND_LEFT;
			}

			if ( xDistSqr > 100 )
			{
				processGenericGotoTarget( _frames, xDist, 0, m_data[m_type].speed );
			}
			else
			{
				// fire at player

				m_salvoCount = 5;
				m_state = SUB_SHARK_MINE_2;
				m_movementTimer = GameState::getOneSecondInFrames() * 8;
				m_controlFunc = NPC_CONTROL_MOVEMENT;
			}

			break;
		}

		case SUB_SHARK_MINE_2:
		{
			if ( xDist > 0 )
			{
				m_extendDir = EXTEND_RIGHT;
			}
			else
			{
				m_extendDir = EXTEND_LEFT;
			}

			if ( xDistSqr > 100 )
			{
				processGenericGotoTarget( _frames, xDist, 0, m_data[m_type].speed );
			}
			else
			{
				// fire at player

				m_salvoCount = 5;
				m_state = SUB_SHARK_CYCLE;
				m_movementTimer = GameState::getOneSecondInFrames() * 8;
				m_controlFunc = NPC_CONTROL_MOVEMENT;
			}

			break;
		}

		case SUB_SHARK_CYCLE:
		{
			// charge player

			//s32 yDist = playerPos.vy - Pos.vy;
			//s32 yDistSqr = yDist * yDist;

			//if ( xDistSqr + yDistSqr > 100 )
			if ( xDistSqr > 100 )
			{
				processGenericGotoTarget( _frames, xDist, 0, 6 );
			}
			else
			{
				m_controlFunc = NPC_CONTROL_MOVEMENT;
				m_movementTimer = GameState::getOneSecondInFrames() * 8;
				m_state = SUB_SHARK_MINE_1;
			}

			break;
		}
	}
}
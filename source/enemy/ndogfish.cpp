/*=========================================================================

	ndogfish.cpp

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

void CNpc::processIronDogfishMovement( int _frames )
{
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
		processStandardIronDogfishAttack( _frames );
	}
}

void CNpc::processStandardIronDogfishAttack( int _frames )
{
	CPlayer *player = GameScene.getPlayer();
	DVECTOR playerPos = player->getPos();

	s32 xDist = playerPos.vx - Pos.vx;
	s32 xDistSqr = xDist * xDist;

	if ( xDist > 0 )
	{
		m_extendDir = EXTEND_RIGHT;
	}
	else
	{
		m_extendDir = EXTEND_LEFT;
	}

	switch( m_state )
	{
		case IRON_DOGFISH_THUMP_1:
		case IRON_DOGFISH_THUMP_2:
		{
			if ( xDistSqr > 100 )
			{
				processGenericGotoTarget( _frames, xDist, 0, m_data[m_type].speed );
			}
			else
			{
				// thump player

				m_state++;
				m_movementTimer = GameState::getOneSecondInFrames() * 3;
			}

			break;
		}

		case IRON_DOGFISH_LASER_EYE_1:
		case IRON_DOGFISH_LASER_EYE_2:
		{
			// fire at user

			s32 yDist = playerPos.vy - Pos.vy;

			s16 headingToPlayer = ratan2( yDist, xDist );

			CProjectile *projectile;
			projectile = new( "test projectile" ) CProjectile;
			projectile->init( Pos, headingToPlayer );

			m_state++;

			if ( m_state > IRON_DOGFISH_LASER_EYE_2 )
			{
				// return to first state

				m_state = IRON_DOGFISH_THUMP_1;
			}

			break;
		}

		case IRON_DOGFISH_ROLL:
		{
			// charge user

			if ( xDistSqr > 100 )
			{
				processGenericGotoTarget( _frames, xDist, 0, 6 );
			}
			else
			{
				m_movementTimer = GameState::getOneSecondInFrames() * 3;
				m_state++;
			}

			break;
		}
	}
}

void CNpc::processCloseIronDogfishAttack( int _frames )
{
	// swipe at player

	m_movementTimer = GameState::getOneSecondInFrames() * 3;
	m_controlFunc = NPC_CONTROL_MOVEMENT;
}
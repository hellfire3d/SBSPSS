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

#ifndef	__ENEMY_NDOGFISH_H__
#include "enemy\ndogfish.h"
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

#ifndef __ANIM_IRONDOGFISH_HEADER__
#include <ACTOR_IRONDOGFISH_ANIM.h>
#endif

void CNpcIronDogfishEnemy::postInit()
{
	m_state = IRON_DOGFISH_THUMP_1;
	m_extendDir = EXTEND_RIGHT;
}

bool CNpcIronDogfishEnemy::processSensor()
{
	switch( m_sensorFunc )
	{
		case NPC_SENSOR_NONE:
			return( false );

		default:
		{
			if ( playerXDistSqr + playerYDistSqr < 10000 )
			{
				m_controlFunc = NPC_CONTROL_CLOSE;

				return( true );
			}
			else
			{
				return( false );
			}
		}
	}
}

void CNpcIronDogfishEnemy::processMovement( int _frames )
{
	if ( m_movementTimer > 0 )
	{
		if ( !m_animPlaying )
		{
			m_animPlaying = true;
			m_animNo = ANIM_IRONDOGFISH_WALK;
			m_frame = 0;
		}

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

void CNpcIronDogfishEnemy::processStandardIronDogfishAttack( int _frames )
{
	if ( playerXDist > 0 )
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
			if ( playerXDistSqr > 100 )
			{
				if ( !m_animPlaying )
				{
					m_animPlaying = true;
					m_animNo = ANIM_IRONDOGFISH_WALK;
					m_frame = 0;
				}

				processGenericGotoTarget( _frames, playerXDist, 0, m_data[m_type].speed );
			}
			else
			{
				// thump player

				if ( m_animNo != ANIM_IRONDOGFISH_PUNCH )
				{
					m_animPlaying = true;
					m_animNo = ANIM_IRONDOGFISH_PUNCH;
					m_frame = 0;
				}
				else if ( !m_animPlaying )
				{
					m_state++;
					m_movementTimer = GameState::getOneSecondInFrames() * 3;
				}
			}

			break;
		}

		case IRON_DOGFISH_LASER_EYE_1:
		case IRON_DOGFISH_LASER_EYE_2:
		{
			// fire at user

			s16 headingToPlayer = ratan2( playerYDist, playerXDist );

			CProjectile *projectile;
			projectile = new( "test projectile" ) CProjectile;
			projectile->init( Pos, headingToPlayer );
			projectile->setLayerCollision( m_layerCollision );

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

			if ( playerXDistSqr > 100 )
			{
				if ( !m_animPlaying )
				{
					m_animPlaying = true;
					m_animNo = ANIM_IRONDOGFISH_WALK;
					m_frame = 0;
				}

				processGenericGotoTarget( _frames, playerXDist, 0, 6 );
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

void CNpcIronDogfishEnemy::processClose( int _frames )
{
	// swipe at player

	m_movementTimer = GameState::getOneSecondInFrames() * 3;
	m_controlFunc = NPC_CONTROL_MOVEMENT;
}
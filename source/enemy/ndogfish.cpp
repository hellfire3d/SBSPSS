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

#ifndef __VID_HEADER_
#include "system\vid.h"
#endif


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcIronDogfishEnemy::postInit()
{
	m_state = IRON_DOGFISH_THUMP_1;
	m_extendDir = EXTEND_RIGHT;
	m_npcPath.setPathType( CNpcPath::PONG_PATH );
	m_steamTimer = 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool CNpcIronDogfishEnemy::processSensor()
{
	/*switch( m_sensorFunc )
	{
		case NPC_SENSOR_NONE:
			return( false );

		default:
		{
			if ( playerXDistSqr + playerYDistSqr < 5000 )
			{
				m_controlFunc = NPC_CONTROL_CLOSE;

				return( true );
			}
			else
			{
				return( false );
			}
		}
	}*/

	return( false );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcIronDogfishEnemy::processMovement( int _frames )
{
	s32 moveX = 0, moveY = 0;
	s32 moveVel = 0;
	s32 moveDist = 0;

	if ( m_movementTimer > 0 )
	{
		if ( !m_animPlaying )
		{
			m_animPlaying = true;
			m_animNo = ANIM_IRONDOGFISH_WALK;
			m_frame = 0;
		}

		processGenericFixedPathWalk( _frames, &moveX, &moveY );

		Pos.vx += moveX;
		Pos.vy += moveY;

		m_movementTimer -= _frames;
	}
	else
	{
		processStandardIronDogfishAttack( _frames );
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcIronDogfishEnemy::processWalkToUser( int _frames, int speed )
{
	s32 minX, maxX;
	m_npcPath.getPathXExtents( &minX, &maxX );

	if ( playerXDist > 0 )
	{
		m_heading = 0;

		if ( Pos.vx + playerXDist > maxX )
		{
			// abort

			m_movementTimer = GameState::getOneSecondInFrames() * 3;
		}
		else
		{
			Pos.vx += _frames * speed;
		}
	}
	else
	{
		m_heading = 2048;

		if ( Pos.vx + playerXDist < minX )
		{
			// abort

			m_movementTimer = GameState::getOneSecondInFrames() * 3;
		}
		else
		{
			Pos.vx -= _frames * speed;
		}
	}

	s32 fallSpeed = 3;
	s8 yMovement = fallSpeed * _frames;
	s32 groundHeight = CGameScene::getCollision()->getHeightFromGround( Pos.vx, Pos.vy, yMovement + 16 );

	if ( groundHeight <= yMovement )
	{
		// move to ground level

		Pos.vx += groundHeight;
	}
	else
	{
		// fall

		Pos.vx += yMovement;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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

				processWalkToUser( _frames, m_speed );
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

			s16 headingToPlayer = ratan2( playerYDist, playerXDist ) & 4095;

			CProjectile *projectile;
			projectile = new( "test projectile" ) CProjectile;
			DVECTOR startPos = Pos;
			startPos.vy -= 20;
			projectile->init( startPos, headingToPlayer );

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

				processWalkToUser( _frames, m_speed );
			}
			else
			{
				if ( m_animNo != ANIM_IRONDOGFISH_TAILSMASH )
				{
					m_animPlaying = true;
					m_animNo = ANIM_IRONDOGFISH_TAILSMASH;
					m_frame = 0;
				}
				else if ( !m_animPlaying )
				{
					m_movementTimer = GameState::getOneSecondInFrames() * 3;
					m_state++;
				}
			}

			break;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcIronDogfishEnemy::processClose( int _frames )
{
	// swipe at player

	if ( m_animNo != ANIM_IRONDOGFISH_PUNCH )
	{
		m_animPlaying = true;
		m_animNo = ANIM_IRONDOGFISH_PUNCH;
		m_frame = 0;
	}
	else if ( !m_animPlaying )
	{
		m_controlFunc = NPC_CONTROL_MOVEMENT;
		m_timerFunc = NPC_TIMER_ATTACK_DONE;
		m_timerTimer = GameState::getOneSecondInFrames();
		m_sensorFunc = NPC_SENSOR_NONE;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcIronDogfishEnemy::processAttackCollision()
{
	switch( m_animNo )
	{
		case ANIM_IRONDOGFISH_PUNCH:
		case ANIM_IRONDOGFISH_TAILSMASH:
		{
			// only detect collision if in attack mode

			m_oldControlFunc = m_controlFunc;
			m_controlFunc = NPC_CONTROL_COLLISION;

			break;
		}

		default:
			break;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcIronDogfishEnemy::hasBeenSteamed( DVECTOR &steamPos )
{
	if ( m_steamTimer <= 0 )
	{
		hasBeenAttacked();
		m_steamTimer = 4 * GameState::getOneSecondInFrames();
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcIronDogfishEnemy::processTimer(int _frames)
{
	if ( m_steamTimer > 0 )
	{
		m_steamTimer -= _frames;
	}

	CNpcEnemy::processTimer( _frames );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcIronDogfishEnemy::processShot( int _frames )
{
	switch( m_data[m_type].shotFunc )
	{
		case NPC_SHOT_NONE:
		{
			// do nothing

			break;
		}

		case NPC_SHOT_GENERIC:
		{
			switch ( m_state )
			{
				case NPC_GENERIC_HIT_CHECK_HEALTH:
				{
					m_health -= 5;

					if ( m_health < 0 )
					{
						m_state = NPC_GENERIC_HIT_DEATH_START;
						m_isDying = true;
					}
					else
					{
						m_state = NPC_GENERIC_HIT_RECOIL;

						m_animPlaying = true;
						m_animNo = m_data[m_type].recoilAnim;
						m_frame = 0;
					}

					break;
				}

				case NPC_GENERIC_HIT_RECOIL:
				{
					if ( !m_animPlaying )
					{
						m_state = 0;
						m_controlFunc = NPC_CONTROL_MOVEMENT;
					}

					break;
				}

				case NPC_GENERIC_HIT_DEATH_START:
				{
					m_animPlaying = true;
					m_animNo = m_data[m_type].dieAnim;
					m_frame = 0;
					m_state = NPC_GENERIC_HIT_DEATH_END;
					m_isDying = true;

					if ( m_data[m_type].deathSfx < CSoundMediator::NUM_SFXIDS )
					{
						CSoundMediator::playSfx( m_data[m_type].deathSfx );
					}

					m_speed = -5;

					if (m_data[m_type].skelType)
					{
						m_actorGfx->SetOtPos( 0 );
					}

					break;
				}

				case NPC_GENERIC_HIT_DEATH_END:
				{
					if ( !m_animPlaying )
					{
						m_drawRotation += 64 * _frames;
						m_drawRotation &= 4095;

						Pos.vy += m_speed * _frames;

						if ( m_speed < 5 )
						{
							m_speed++;
						}

						DVECTOR	offset = CLevel::getCameraPos();

						if ( Pos.vy - offset.vy > VidGetScrH() )
						{
							if ( m_data[m_type].respawning )
							{
								m_isActive = false;

								m_timerFunc = NPC_TIMER_RESPAWN;
								m_timerTimer = 4 * GameState::getOneSecondInFrames();
							}
							else
							{
								setToShutdown();
							}
						}
					}

					break;
				}
			}

			break;
		}
	}
}

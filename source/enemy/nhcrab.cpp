/*=========================================================================

	nhcrab.cpp

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __ENEMY_NPC_H__
#include "enemy\npc.h"
#endif

#ifndef __ENEMY_NHCRAB_H__
#include "enemy\nhcrab.h"
#endif

#ifndef __GAME_GAME_H__
#include "game\game.h"
#endif

#ifndef	__UTILS_HEADER__
#include	"utils\utils.h"
#endif

#ifndef __ANIM_HERMITCRAB_HEADER__
#include <ACTOR_HERMITCRAB_ANIM.h>
#endif

#ifndef __VID_HEADER_
#include "system\vid.h"
#endif


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcHermitCrabEnemy::postInit()
{
	m_npcPath.setPathType( CNpcPath::PONG_PATH );

	m_state = HERMIT_CRAB_NO_ATTACK;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool CNpcHermitCrabEnemy::processSensor()
{
	switch( m_sensorFunc )
	{
		case NPC_SENSOR_NONE:
			return( false );

		default:
		{
			if ( playerXDistSqr + playerYDistSqr < 4000 )
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

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcHermitCrabEnemy::processClose( int _frames )
{
	if ( m_state == HERMIT_CRAB_NO_ATTACK )
	{
		//m_state = ( getRnd() % 2 ) + 1;
		m_state = HERMIT_CRAB_ROLL_ATTACK_JUMP1;
		m_movementTimer = 0;
		m_jumpBase = Pos.vy;

		if ( playerXDist > 0 )
		{
			m_extendDir = EXTEND_RIGHT;
			m_heading = 0;
		}
		else
		{
			m_extendDir = EXTEND_LEFT;
			m_heading = 2048;
		}
	}

	switch( m_state )
	{
		case HERMIT_CRAB_PUNCH_ATTACK:
		{
			if ( m_animNo != ANIM_HERMITCRAB_PUNCH )
			{
				m_animPlaying = true;
				m_animNo = ANIM_HERMITCRAB_PUNCH;
				m_frame = 0;
			}
			else if ( !m_animPlaying )
			{
				m_state = HERMIT_CRAB_NO_ATTACK;
				m_controlFunc = NPC_CONTROL_MOVEMENT;
				m_timerFunc = NPC_TIMER_ATTACK_DONE;
				m_timerTimer = GameState::getOneSecondInFrames();
				m_sensorFunc = NPC_SENSOR_NONE;
			}

			break;
		}

		case HERMIT_CRAB_ROLL_ATTACK_JUMP1:
		case HERMIT_CRAB_ROLL_ATTACK_JUMP2:
		{
			s16 sineVal = ( m_movementTimer << 11 ) / ( GameState::getOneSecondInFrames() >> 3 );
			Pos.vy = m_jumpBase - ( ( 10 * rsin( sineVal ) ) >> 12 );

			if ( m_movementTimer < ( GameState::getOneSecondInFrames() >> 3 ) )
			{
				m_movementTimer++;
			}
			else
			{
				m_state++;
				m_movementTimer = 0;
			}

			break;
		}

		case HERMIT_CRAB_ROLL_ATTACK_ROLL:
		{
			if ( !m_animPlaying || m_animNo == m_data[m_type].moveAnim )
			{
				switch( m_animNo )
				{
					case ANIM_HERMITCRAB_ROLLATTACKSTART:
					{
						m_animPlaying = true;
						m_animNo = ANIM_HERMITCRAB_ROLLATTACK;
						m_frame = 0;

						break;
					}

					case ANIM_HERMITCRAB_ROLLATTACK:
					{
						m_animPlaying = true;
						m_animNo = ANIM_HERMITCRAB_ROLLATTACKEND;
						m_frame = 0;

						break;
					}

					case ANIM_HERMITCRAB_ROLLATTACKEND:
					{
						m_state = HERMIT_CRAB_NO_ATTACK;
						m_controlFunc = NPC_CONTROL_MOVEMENT;
						m_timerFunc = NPC_TIMER_ATTACK_DONE;
						m_timerTimer = GameState::getOneSecondInFrames();
						m_sensorFunc = NPC_SENSOR_NONE;

						break;
					}

					default:
					{
						m_animPlaying = true;
						m_animNo = ANIM_HERMITCRAB_ROLLATTACKSTART;
						m_frame = 0;

						break;
					}
				}
			}

			if ( m_animNo == ANIM_HERMITCRAB_ROLLATTACK )
			{
				// check for vertical movement

				s32 moveX = 0, moveY = 0;
				s32 groundHeight;
				s32 fallSpeed = 3;
				s8 yMovement = fallSpeed * _frames;

				moveX = 6 * _frames;

				m_heading = 0;

				if ( m_extendDir == EXTEND_LEFT )
				{
					moveX = -moveX;
					m_heading = 2048;
				}

				s32 minX, maxX, newX;

				m_npcPath.getPathXExtents( &minX, &maxX );

				newX = Pos.vx + moveX;

				if ( newX < minX || newX > maxX )
				{
					// moving outside path constraints, abort

					m_animPlaying = true;
					m_animNo = ANIM_HERMITCRAB_ROLLATTACKEND;
					m_frame = 0;
				}
				else
				{
					groundHeight = CGameScene::getCollision()->getHeightFromGround( newX, Pos.vy, yMovement + 16 );

					if ( groundHeight <= yMovement )
					{
						// groundHeight <= yMovement indicates either just above ground or on or below ground

						moveY = groundHeight;
					}
					else
					{
						// fall

						moveY = yMovement;
					}

					Pos.vx = newX;
					Pos.vy += moveY;

					// sound

					CSoundMediator::playSfx( CSoundMediator::SFX_HERMIT_CRAB_ATTACK );
				}
			}
		}

		default:
			break;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcHermitCrabEnemy::processMovementModifier( int _frames, s32 distX, s32 distY, s32 dist, s16 headingChange )
{
	Pos.vx += distX;
	Pos.vy += distY;

	// sort out draw rotation

	DVECTOR testPos1, testPos2;

	testPos1 = testPos2 = Pos;
	testPos1.vx -= 10;
	testPos2.vx += 10;

	testPos1.vy += CGameScene::getCollision()->getHeightFromGround( testPos1.vx, testPos1.vy, 16 );
	testPos2.vy += CGameScene::getCollision()->getHeightFromGround( testPos2.vx, testPos2.vy, 16 );

	s32 xDist = testPos2.vx - testPos1.vx;
	s32 yDist = testPos2.vy - testPos1.vy;

	s16 heading = ratan2( yDist, xDist );

	m_drawRotation = heading;

	// sound

	CSoundMediator::playSfx( CSoundMediator::SFX_HERMIT_CRAB_MOVE );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcHermitCrabEnemy::processShot( int _frames )
{
	switch ( m_state )
	{
		case NPC_GENERIC_HIT_CHECK_HEALTH:
		{
			if ( CLevel::getCurrentChapter() == 1 && CLevel::getCurrentChapterLevel() == 1 )
			{
				m_state = NPC_GENERIC_HIT_DEATH_START;
			}
			else
			{
				m_health -= 5;

				if ( m_health < 0 )
				{
					m_state = NPC_GENERIC_HIT_DEATH_START;

					m_animPlaying = true;
					m_animNo = m_data[m_type].dieAnim;
					m_frame = 0;
				}
				else
				{
					m_state = NPC_GENERIC_HIT_RECOIL;

					m_animPlaying = true;
					m_animNo = m_data[m_type].recoilAnim;
					m_frame = 0;
				}
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
			if ( !m_animPlaying )
			{
				m_state = NPC_GENERIC_HIT_DEATH_END;

				if ( m_data[m_type].deathSfx < CSoundMediator::NUM_SFXIDS )
				{
					CSoundMediator::playSfx( m_data[m_type].deathSfx );
				}

				m_isDying = true;
				m_speed = -5;

				if (m_data[m_type].skelType)
				{
					m_actorGfx->SetOtPos( 0 );
				}
			}

			break;
		}

		case NPC_GENERIC_HIT_DEATH_END:
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

			break;
		}
	}
}

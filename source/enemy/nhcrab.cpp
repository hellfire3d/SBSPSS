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

				if( m_soundId != NOT_PLAYING )
				{
					CSoundMediator::stopAndUnlockSfx( (xmPlayingId) m_soundId );
					m_soundId = NOT_PLAYING;
				}

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
					case ANIM_HERMITCRAB_ROLLATTACK:
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
						m_animNo = ANIM_HERMITCRAB_ROLLATTACK;
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

					m_animPlaying = false;
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

					if ( m_soundId == NOT_PLAYING )
					{
						m_soundId = (int) CSoundMediator::playSfx( CSoundMediator::SFX_HERMIT_CRAB_ATTACK, true );
					}
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

	int groundDist = CGameScene::getCollision()->getHeightFromGround( testPos1.vx, testPos1.vy, 16 );

	if ( abs( groundDist ) > 15 )
	{
		m_drawRotation = 0;
		return;
	}

	testPos1.vy += groundDist;

	groundDist = CGameScene::getCollision()->getHeightFromGround( testPos2.vx, testPos2.vy, 16 );

	if ( abs( groundDist ) > 15 )
	{
		m_drawRotation = 0;
		return;
	}

	testPos2.vy += groundDist;

	s32 xDist = testPos2.vx - testPos1.vx;
	s32 yDist = testPos2.vy - testPos1.vy;

	s16 heading = ratan2( yDist, xDist ) & 4095;

	m_drawRotation = heading;

	// sound

	if( m_soundId == NOT_PLAYING )
	{
		m_soundId = (int) CSoundMediator::playSfx( CSoundMediator::SFX_HERMIT_CRAB_MOVE, true, true );
	}
}

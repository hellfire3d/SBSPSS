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

void CNpcHermitCrabEnemy::postInit()
{
	m_npcPath.setPathType( CNpcPath::PONG_PATH );

	m_state = HERMIT_CRAB_NO_ATTACK;
}

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

void CNpcHermitCrabEnemy::processClose( int _frames )
{
	if ( m_state == HERMIT_CRAB_NO_ATTACK )
	{
		m_state = ( getRnd() % 2 ) + 1;
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

		case HERMIT_CRAB_ROLL_ATTACK:
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

						if ( playerXDist > 0 )
						{
							m_extendDir = EXTEND_RIGHT;
						}
						else
						{
							m_extendDir = EXTEND_LEFT;
						}

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
					groundHeight = m_layerCollision->getHeightFromGround( newX, Pos.vy, yMovement + 16 );

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
				}
			}
		}

		default:
			break;
	}
}

void CNpcHermitCrabEnemy::processMovementModifier( int _frames, s32 distX, s32 distY, s32 dist, s16 headingChange )
{
	Pos.vx += distX;
	Pos.vy += distY;

	// sort out draw rotation

	DVECTOR testPos1, testPos2;

	testPos1 = testPos2 = Pos;
	testPos1.vx -= 10;
	testPos2.vx += 10;

	testPos1.vy += m_layerCollision->getHeightFromGround( testPos1.vx, testPos1.vy, 16 );
	testPos2.vy += m_layerCollision->getHeightFromGround( testPos2.vx, testPos2.vy, 16 );

	s32 xDist = testPos2.vx - testPos1.vx;
	s32 yDist = testPos2.vy - testPos1.vy;

	s16 heading = ratan2( yDist, xDist );

	m_drawRotation = heading;
}

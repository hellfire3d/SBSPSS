/*=========================================================================

	nshrkman.cpp

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

#ifndef	__UTILS_HEADER__
#include "utils\utils.h"
#endif

#ifndef __ANIM_SHARKMAN_HEADER__
#include <ACTOR_SHARKMAN_ANIM.h>
#endif


void CNpcEnemy::processSharkManMovement( int _frames, s32 *moveX, s32 *moveY )
{
	if ( m_movementTimer > 0 )
	{
		m_movementTimer -= _frames;

		if ( m_animNo == m_data[m_type].moveAnim )
		{
			processGenericFixedPathWalk( _frames, moveX, moveY );
		}

		if ( !m_animPlaying )
		{
			m_animPlaying = true;
			m_frame = 0;
		}
	}
	else
	{
		// change anim

		if ( m_animNo == m_data[m_type].moveAnim )
		{
			u8 newAction = getRnd() % 5;

			switch( newAction )
			{
				case 0:
				{
					m_movementTimer = GameState::getOneSecondInFrames() * 3;
					m_animNo = ANIM_SHARKMAN_IDLE1_;

					break;
				}

				case 1:
				{
					m_movementTimer = GameState::getOneSecondInFrames() * 8;
					m_animNo = m_data[m_type].moveAnim;

					break;
				}

				case 2:
				{
					m_movementTimer = GameState::getOneSecondInFrames() * 1;
					m_animNo = ANIM_SHARKMAN_IDLE2_;

					break;
				}

				case 3:
				{
					m_movementTimer = GameState::getOneSecondInFrames() >> 1;
					m_animNo = ANIM_SHARKMAN_KICK_SAND;

					break;
				}

				case 4:
				{
					m_movementTimer = GameState::getOneSecondInFrames() * 3;
					m_animNo = ANIM_SHARKMAN_PUSHUPS;

					break;
				}
			}
		}
		else
		{
			// return to move anim

			m_movementTimer = GameState::getOneSecondInFrames() * 8;
			m_animNo = m_data[m_type].moveAnim;
		}

		m_animPlaying = true;
		m_frame = 0;
	}
}

void CNpcEnemy::processCloseSharkManAttack( int _frames )
{
	s32 moveX = 0, moveY = 0;
	s32 groundHeight;
	s16 decDir, incDir, moveDist;
	s32 direction;
	s32 maxHeight = 20;
	s32 fallSpeed = 3;
	s8 yMovement = fallSpeed * _frames;

	s16 headingToPlayer = ratan2( playerYDist, playerXDist );

	if ( !m_animPlaying )
	{
		m_animPlaying = true;
		m_animNo = ANIM_SHARKMAN_RUN;
		m_frame = 0;
	}

	decDir = m_heading - headingToPlayer;

	if ( decDir < 0 )
	{
		decDir += ONE;
	}

	incDir = headingToPlayer - m_heading;

	if ( incDir < 0 )
	{
		incDir += ONE;
	}

	if ( decDir < incDir )
	{
		moveDist = -decDir;
	}
	else
	{
		moveDist = incDir;
	}

	s32 xDistWaypoint, yDistWaypoint;

	m_npcPath.getDistToNextWaypoint( Pos, &xDistWaypoint, &yDistWaypoint );

	if ( abs( moveDist ) < 1024 && abs( xDistWaypoint ) >= abs( playerXDist ) )
	{
		// continue charge

		if ( playerXDist != 0 )
		{
			direction = playerXDist / abs( playerXDist );
		}
		else
		{
			direction = 1;
		}

		if ( m_layerCollision->getHeightFromGround( Pos.vx + ( _frames * m_velocity * direction ), Pos.vy ) < -maxHeight )
		{
			// there is an obstacle in the way, abort

			m_controlFunc = NPC_CONTROL_MOVEMENT;
			m_timerFunc = NPC_TIMER_ATTACK_DONE;
			m_timerTimer = GameState::getOneSecondInFrames();
			m_sensorFunc = NPC_SENSOR_NONE;
			m_animNo = m_data[m_type].moveAnim;
		}
		else
		{
			// check for vertical movement

			groundHeight = m_layerCollision->getHeightFromGround( Pos.vx, Pos.vy, yMovement + 16 );

			if ( groundHeight <= yMovement )
			{
				// groundHeight <= yMovement indicates either just above ground or on or below ground

				moveX = _frames * m_velocity * direction;
				moveY = groundHeight;
			}
			else
			{
				// fall

				moveY = yMovement;
			}

			Pos.vx += moveX;
			Pos.vy += moveY;
		}
	}
	else
	{
		m_controlFunc = NPC_CONTROL_MOVEMENT;
		m_animNo = m_data[m_type].moveAnim;
	}
}

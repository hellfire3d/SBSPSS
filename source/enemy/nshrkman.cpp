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

#ifndef	__ENEMY_NSHRKMAN_H__
#include "enemy\nshrkman.h"
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

#ifndef	__UTILS_HEADER__
#include "utils\utils.h"
#endif

#ifndef __SPR_SPRITES_H__
#include <sprites.h>
#endif

#ifndef __ANIM_MANRAY_HEADER__
#include <ACTOR_MANRAY_ANIM.h>
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcSharkManEnemy::postInit()
{
	CNpcEnemy::postInit();

	m_fired = false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcSharkManEnemy::processMovement( int _frames )
{
	s32 moveX = 0, moveY = 0;

	if ( !m_animPlaying )
	{
		m_animPlaying = true;
		m_frame = 0;
		m_animNo = m_data[m_type].moveAnim;
	}

	processGenericFixedPathWalk( _frames, &moveX, &moveY );

	/*if ( m_movementTimer > 0 )
	{
		m_movementTimer -= _frames;

		if ( m_animNo == m_data[m_type].moveAnim )
		{
			processGenericFixedPathWalk( _frames, &moveX, &moveY );
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
	}*/

	processMovementModifier( _frames, moveX, moveY, 0, 0 );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcSharkManEnemy::processClose( int _frames )
{
	s32 xDist, yDist;

	DVECTOR newPos = Pos;
	newPos.vy -= 50;

	CPlayer *player = GameScene.getPlayer();
	DVECTOR const &playerPos = player->getPos();

	xDist = playerPos.vx - newPos.vx;
	yDist = playerPos.vy - newPos.vy;

	if ( xDist > 0 )
	{
		m_heading = 0;
	}
	else
	{
		m_heading = 2048;
	}

	if ( m_animNo != ANIM_MANRAY_ATTACK )
	{
		m_animPlaying = true;
		m_animNo = ANIM_MANRAY_ATTACK;
		m_frame = 0;
	}
	else if ( ( ( m_frame >> 8 ) > ( ( 2 * getFrameCount() ) >> 2 ) ) && !m_fired )
	{
		// fire

		s16 heading = ratan2( yDist, xDist ) & 4095;

		CProjectile *projectile;
		projectile = CProjectile::Create();
		projectile->init( newPos, heading );
		projectile->setShock();
		projectile->setGraphic( FRM__LIGHTNING2 );

		m_fired = true;
	}
	else if ( !m_animPlaying )
	{
		// anim finished
		m_controlFunc = NPC_CONTROL_MOVEMENT;
		m_timerFunc = NPC_TIMER_ATTACK_DONE;
		m_timerTimer = GameState::getOneSecondInFrames();
		m_sensorFunc = NPC_SENSOR_NONE;
		m_animNo = m_data[m_type].moveAnim;
		m_animPlaying = true;
		m_frame = 0;
		m_fired = false;
	}
}

/*void CNpcSharkManEnemy::processClose( int _frames )
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

		if ( CGameScene::getCollision()->getHeightFromGround( Pos.vx + ( _frames * m_velocity * direction ), Pos.vy ) < -maxHeight )
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

			groundHeight = CGameScene::getCollision()->getHeightFromGround( Pos.vx, Pos.vy, yMovement + 16 );

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
}*/

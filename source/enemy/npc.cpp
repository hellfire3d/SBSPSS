/*=========================================================================

	npc.cpp

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/

#include "enemy\npc.h"

#ifndef __LEVEL_LEVEL_H__
#include "level\level.h"
#endif

#ifndef __FILE_EQUATES_H__
#include <biglump.h>
#endif

#ifndef __SPR_UIGFX_H__
#include <uigfx.h>
#endif

#ifndef __GAME_GAME_H__
#include	"game\game.h"
#endif

#ifndef	__PLAYER_PLAYER_H__
#include	"player\player.h"
#endif

#ifndef __ENEMY_NPCPATH_H__
#include	"enemy\npcpath.h"
#endif

#ifndef	__UTILS_HEADER__
#include	"utils\utils.h"
#endif


CNpc::NPC_DATA CNpc::m_data[NPC_UNIT_TYPE_MAX] =
{
	{
		NPC_INIT_DEFAULT,
		NPC_SENSOR_USER_CLOSE,
		NPC_MOVEMENT_FIXED_PATH,
		NPC_MOVEMENT_MODIFIER_JELLYFISH,
		NPC_CLOSE_EVADE,
		NPC_TIMER_NONE,
		false,
	},

	{
		NPC_INIT_DEFAULT,
		NPC_SENSOR_NONE,
		NPC_MOVEMENT_STATIC,
		NPC_MOVEMENT_MODIFIER_NONE,
		NPC_CLOSE_EVADE,
		NPC_TIMER_NONE,
		true,
	},
};


void CNpc::init()
{
	m_type = NPC_TEST_TYPE;

	m_heading = 0;
	m_movementTimer = 0;
	m_timerTimer = 0;
	m_velocity = 0;

	Pos.vx = 100;
	Pos.vy = 100;

	m_timerFunc = m_data[this->m_type].timerFunc;
	m_sensorFunc = m_data[this->m_type].sensorFunc;

	switch ( m_data[this->m_type].initFunc )
	{
		case NPC_INIT_DEFAULT:
			m_controlFunc = NPC_CONTROL_MOVEMENT;

			m_npcPath.initPath();

			DVECTOR newPos;

			newPos.vx = 100;
			newPos.vy = 10;

			m_npcPath.addWaypoint( newPos );

			newPos.vx = 500;
			newPos.vy = 10;

			m_npcPath.addWaypoint( newPos );

			newPos.vx = 500;
			newPos.vy = 100;

			m_npcPath.addWaypoint( newPos );

			newPos.vx = 100;
			newPos.vy = 100;

			m_npcPath.addWaypoint( newPos );

			m_npcPath.setPathType( REPEATING_PATH );

			break;

		default:
			m_controlFunc = NPC_CONTROL_MOVEMENT;

			break;
	}
}


void CNpc::shutdown()
{
}


void CNpc::think(int _frames)
{
	switch ( this->m_controlFunc )
	{
		case NPC_CONTROL_MOVEMENT:
			if ( !processSensor() )
			{
				processMovement(_frames);
			}
			else
			{
				processClose(_frames);
			}

			break;

		case NPC_CONTROL_SHOT:
			processShot();

			break;

		case NPC_CONTROL_CLOSE:
			processClose(_frames);

			break;

		case NPC_CONTROL_COLLISION:
			processCollision();

			break;
	}

	processTimer(_frames);
}


bool CNpc::processSensor()
{
	switch( m_sensorFunc )
	{
		case NPC_SENSOR_NONE:
			return( false );

		case NPC_SENSOR_USER_CLOSE:
		{
			CPlayer *player = GameScene.getPlayer();

			DVECTOR playerPos = player->getPos();

			s32 xDistSqr, yDistSqr;

			xDistSqr = playerPos.vx - this->Pos.vx;
			xDistSqr *= xDistSqr;

			yDistSqr = playerPos.vy - this->Pos.vy;
			yDistSqr *= yDistSqr;

			if ( xDistSqr + yDistSqr < 10000 )
			{
				this->m_controlFunc = NPC_CONTROL_CLOSE;

				this->m_evadeClockwise = ( getRnd() % 2 ) - 1;

				return( true );
			}
			else
			{
				return( false );
			}
		}

		default:
			return( false );
	}
}

void CNpc::processMovement(int _frames)
{
	if ( _frames > 2 )
	{
		_frames = 2;
	}

	s32 moveX = 0, moveY = 0;

	s16 moveDist = 0;

	s32 moveVel = 0;

	switch( m_data[this->m_type].movementFunc )
	{
		case NPC_MOVEMENT_STATIC:
		{
			break;
		}

		case NPC_MOVEMENT_FIXED_PATH:
		{
			bool pathComplete;

			s16 headingToTarget = m_npcPath.think( Pos, &pathComplete );

			if ( !pathComplete )
			{
				s16 decDir, incDir;

				decDir = m_heading - headingToTarget;

				if ( decDir < 0 )
				{
					decDir += ONE;
				}

				incDir = headingToTarget - m_heading;

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

				if ( moveDist < -128 )
				{
					moveDist = -128;
				}
				else if ( moveDist > 128 )
				{
					moveDist = 128;
				}

				m_heading += moveDist;

				m_heading = m_heading % ONE;
				
				moveX = ( _frames * 3 * rcos( m_heading ) ) >> 12;
				moveY = ( _frames * 3 * rsin( m_heading ) ) >> 12;
				moveVel = ( _frames * 3 ) << 8;
			}

			break;
		}

		case NPC_MOVEMENT_USER_SEEK:
		{
			CPlayer *player;
			
			player = GameScene.getPlayer();

			break;
		}

		case NPC_MOVEMENT_VERTICAL:
		{
			Pos.vy--;

			break;
		}

		default:

			break;
	}

	processMovementModifier(_frames, moveX, moveY, moveVel, moveDist);
}

void CNpc::processMovementModifier(int _frames, s32 distX, s32 distY, s32 dist, s32 headingChange)
{
	switch( m_data[m_type].movementModifierFunc )
	{
		case NPC_MOVEMENT_MODIFIER_NONE:
		{
			Pos.vx += distX;
			Pos.vy += distY;

			break;
		}

		case NPC_MOVEMENT_MODIFIER_BOB:
		{
			break;
		}

		case NPC_MOVEMENT_MODIFIER_JELLYFISH:
		{
#define NPC_JELLYFISH_RESISTANCE	64

			s32 newX, newY;
			s32 preShiftX, preShiftY;

			u16 jellyfishData[5] = { 96, 192, 256, 192, 128, };

			u32 dataPoint;

			m_movementTimer += _frames;

			if ( m_movementTimer > GameState::getOneSecondInFrames() )
			{
				m_movementTimer = 0;
			}

			dataPoint = 4 * m_movementTimer;

			if ( dataPoint != 0 )
			{
				dataPoint /= GameState::getOneSecondInFrames();
			}

			s32 resistance;
			s32 absVelocity = abs( m_velocity );
			s32 reqVelocity = dist;

			resistance = _frames * NPC_JELLYFISH_RESISTANCE;

			if ( absVelocity < resistance )
			{
				resistance = absVelocity;
			}

			if ( absVelocity != 0 )
			{
				resistance = -( resistance * m_velocity ) / absVelocity;
			}
			else
			{
				resistance = 0;
			}

			m_velocity += resistance;

			reqVelocity = dist * jellyfishData[dataPoint];
			reqVelocity >>= 8;
			reqVelocity *= 128 + ( 128 - headingChange );
			reqVelocity >>= 8;

			s32 absReqVelocity = abs( reqVelocity );

			if ( absReqVelocity > absVelocity )
			{
				m_velocity += reqVelocity >> 1;
			}

			preShiftX = ( m_velocity >> 8 ) * rcos( m_heading );
			preShiftY = ( m_velocity >> 8 ) * rsin( m_heading );

			newX = preShiftX >> 12;
			if ( !newX && preShiftX )
			{
				newX = preShiftX / abs( preShiftX );
			}

			newY = preShiftY >> 12;
			if ( !newY && preShiftY )
			{
				newY = preShiftY / abs( preShiftY );
			}

			Pos.vx += newX;
			Pos.vy += newY;

			break;
		}
	}
}

void CNpc::processShot()
{
}

void CNpc::processClose(int _frames)
{
	switch( m_data[this->m_type].closeFunc )
	{
		case NPC_CLOSE_EVADE:
			{
				s32 moveX = 0, moveY = 0;

				s16 moveDist = 0;

				s32 moveVel = 0;

				CPlayer *player = GameScene.getPlayer();

				DVECTOR playerPos = player->getPos();

				s32 xDist, yDist;
				s32 xDistSqr, yDistSqr;

				xDist = playerPos.vx - this->Pos.vx;
				xDistSqr = xDist * xDist;

				yDist = playerPos.vy - this->Pos.vy;
				yDistSqr = yDist * yDist;

				if ( xDistSqr + yDistSqr > 22500 )
				{
					this->m_controlFunc = NPC_CONTROL_MOVEMENT;
				}
				else
				{
					bool pathComplete;

					s16 headingToTarget = m_npcPath.think( Pos, &pathComplete );

					if ( pathComplete )
					{
						this->m_controlFunc = NPC_CONTROL_MOVEMENT;
					}
					else
					{
						s16 headingToPlayer = ratan2( yDist, xDist );

						if ( m_evadeClockwise )
						{
							headingToPlayer += 1024;
						}
						else
						{
							headingToPlayer -= 1024;
						}

						headingToPlayer %= ONE;

						s16 decDir, incDir;

						decDir = headingToPlayer - headingToTarget;

						if ( decDir < 0 )
						{
							decDir += ONE;
						}

						incDir = headingToTarget - headingToPlayer;

						if ( incDir < 0 )
						{
							incDir += ONE;
						}

						if ( decDir < incDir )
						{
							moveDist = decDir;
						}
						else
						{
							moveDist = incDir;
						}

						if ( moveDist < 128 )
						{
							// evasion angle is close to angle required to go to waypoint, hence resume waypoint movement

							this->m_controlFunc = NPC_CONTROL_MOVEMENT;
							this->m_timerFunc = NPC_TIMER_EVADE_DONE;
							this->m_timerTimer = GameState::getOneSecondInFrames();
							this->m_sensorFunc = NPC_SENSOR_NONE;
						}
						else
						{
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

							if ( moveDist < -128 )
							{
								moveDist = -128;
							}
							else if ( moveDist > 128 )
							{
								moveDist = 128;
							}

							m_heading += moveDist;

							m_heading = m_heading % ONE;
							
							moveX = ( _frames * 3 * rcos( m_heading ) ) >> 12;
							moveY = ( _frames * 3 * rsin( m_heading ) ) >> 12;
							moveVel = ( _frames * 3 ) << 8;

							processMovementModifier(_frames, moveX, moveY, moveVel, moveDist);
						}
					}
				}
			}

			break;

		default:
			break;
	}
}

void CNpc::processCollision()
{
}

void CNpc::processTimer(int _frames)
{
	switch( m_timerFunc )
	{
		case NPC_TIMER_NONE:
			{
				break;
			}

		case NPC_TIMER_EVADE_DONE:
			{
				this->m_timerTimer -= _frames;

				if ( m_timerTimer <= 0 )
				{
					this->m_timerFunc = NPC_TIMER_NONE;
					this->m_sensorFunc = NPC_SENSOR_USER_CLOSE;
				}

				break;
			}

		default:
			break;
	}
}

void CNpc::render()
{
}

void CNpc::processEvent( GAME_EVENT evt, CThing *sourceThing )
{
	CConversation *currentConversation = GameScene.getConversation();

	if ( m_data[this->m_type].canTalk )
	{
		DVECTOR sourcePos;
		s32 xDiffSqr, yDiffSqr;

		// check talk distance

		sourcePos = sourceThing->getPos();

		xDiffSqr = this->Pos.vx - sourcePos.vx;
		xDiffSqr *= xDiffSqr;

		yDiffSqr = this->Pos.vy - sourcePos.vy;
		yDiffSqr *= yDiffSqr;

		if ( xDiffSqr + yDiffSqr < 250 )
		{
			if( !currentConversation->isActive() )
			{
				currentConversation->trigger( SCRIPTS_SPEECHTEST_DAT );
			}
		}
	}
}

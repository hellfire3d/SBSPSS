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

#ifndef	__GAME_CONVO_H__
#include "game\convo.h"
#endif


void CNpc::init()
{
	m_type = NPC_FISH_FOLK;

	m_heading = m_fireHeading = 0;
	m_movementTimer = 0;
	m_timerTimer = 0;
	m_velocity = 0;
	m_extension = 0;

	Pos.vx = 100;
	Pos.vy = 100;

	m_base = Pos;

	m_timerFunc = m_data[this->m_type].timerFunc;
	m_sensorFunc = m_data[this->m_type].sensorFunc;

	m_controlFunc = NPC_CONTROL_MOVEMENT;

	switch ( m_data[this->m_type].initFunc )
	{
		case NPC_INIT_DEFAULT:
		{
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

			m_npcPath.setPathType( PONG_PATH );

			break;
		}

		case NPC_INIT_GHOST_PIRATE:
			m_heading = m_fireHeading = 3072;

			break;

		case NPC_INIT_SKULL_STOMPER:
		{
			m_heading = m_fireHeading = 1024;
			
			m_npcPath.initPath();

			DVECTOR newPos;

			newPos.vx = 100;
			newPos.vy = 100;

			m_npcPath.addWaypoint( newPos );

			newPos.vx = 100;
			newPos.vy = 10;

			m_npcPath.addWaypoint( newPos );

			m_npcPath.setPathType( SINGLE_USE_PATH );

			break;
		}

		case NPC_INIT_MOTHER_JELLYFISH:
		{
			m_state = MOTHER_JELLYFISH_RETURN_TO_START;

			break;
		}

		case NPC_INIT_FLYING_DUTCHMAN:
		{
			m_state = FLYING_DUTCHMAN_ATTACK_PLAYER_1;
			m_extendDir = EXTEND_UP;

			break;
		}

		case NPC_INIT_SUB_SHARK:
		{
			m_state = SUB_SHARK_CYCLE;
			m_extendDir = EXTEND_RIGHT;

			break;
		}

		case NPC_INIT_IRON_DOGFISH:
		{
			m_state = IRON_DOGFISH_THUMP_1;
			m_extendDir = EXTEND_RIGHT;

			break;
		}

		case NPC_INIT_FALLING_ITEM:
		{
			m_heading = m_fireHeading = 1024;
			
			m_npcPath.initPath();

			DVECTOR newPos;

			newPos.vx = 100;
			newPos.vy = 200;

			m_npcPath.addWaypoint( newPos );

			m_npcPath.setPathType( SINGLE_USE_PATH );

			break;
		}

		case NPC_INIT_FISH_HOOK:
		{
			m_heading = m_fireHeading = 3072;
			
			m_npcPath.initPath();

			DVECTOR newPos;

			newPos.vx = 100;
			newPos.vy = -100;

			m_npcPath.addWaypoint( newPos );

			m_npcPath.setPathType( SINGLE_USE_PATH );

			break;
		}

		case NPC_INIT_PENDULUM:
		{
			m_extendDir = EXTEND_LEFT;
			m_extension = 0;
			m_heading = 1024;

			break;
		}

		case NPC_INIT_FIREBALL:
		{
			m_npcPath.initPath();

			DVECTOR newPos;

			newPos.vx = 300;
			newPos.vy = 100;

			m_npcPath.addWaypoint( newPos );
			m_npcPath.setPathType( SINGLE_USE_PATH );

			m_extension = 0;
			m_velocity = m_data[m_type].speed;
			m_timerTimer = GameState::getOneSecondInFrames() * 4;

			break;
		}

		case NPC_INIT_RETURNING_HAZARD:
		{
			m_npcPath.initPath();

			DVECTOR newPos;

			newPos.vx = 100;
			newPos.vy = 10;

			m_npcPath.addWaypoint( newPos );

			newPos.vx = 500;
			newPos.vy = 10;

			m_npcPath.addWaypoint( newPos );

			m_npcPath.setPathType( SINGLE_USE_PATH );

			break;
		}

		case NPC_INIT_FISH_FOLK:
		{
			m_heading = m_fireHeading = 0;
			
			m_npcPath.initPath();

			DVECTOR newPos;

			newPos.vx = 100;
			newPos.vy = 100;

			m_npcPath.addWaypoint( newPos );

			newPos.vx = 500;
			newPos.vy = 100;

			m_npcPath.addWaypoint( newPos );

			m_npcPath.setPathType( PONG_PATH );

			break;
		}

		default:

			break;
	}
}


void CNpc::shutdown()
{
	m_npcPath.removeAllWaypoints();
}


void CNpc::think(int _frames)
{
	switch ( this->m_controlFunc )
	{
		case NPC_CONTROL_NONE:
			return;

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

		default:
			{
				CPlayer *player = GameScene.getPlayer();

				DVECTOR playerPos = player->getPos();

				s32 xDist, yDist;
				s32 xDistSqr, yDistSqr;

				xDist = playerPos.vx - this->Pos.vx;
				xDistSqr = xDist * xDist;

				yDist = playerPos.vy - this->Pos.vy;
				yDistSqr = yDist * yDist;

				switch( m_sensorFunc )
				{
					case NPC_SENSOR_JELLYFISH_USER_CLOSE:
					{
						if ( xDistSqr + yDistSqr < 10000 )
						{
							m_controlFunc = NPC_CONTROL_CLOSE;
							m_evadeClockwise = ( getRnd() % 2 ) - 1;

							return( true );
						}
						else
						{
							return( false );
						}
					}

					case NPC_SENSOR_CLAM_USER_CLOSE:
					{
						if ( xDistSqr + yDistSqr < 10000 )
						{
							m_controlFunc = NPC_CONTROL_CLOSE;
							m_extendDir = EXTEND_UP;
							m_extension = 0;
							m_movementTimer = GameState::getOneSecondInFrames() >> 3;
							m_velocity = ( getRnd() % 6 ) + 1;

							return( true );
						}
						else
						{
							return( false );
						}
					}

					case NPC_SENSOR_SPIDER_CRAB_USER_CLOSE:
					{
						if ( xDistSqr + yDistSqr < 10000 )
						{
							m_controlFunc = NPC_CONTROL_CLOSE;
							m_extension = 0;
							m_velocity = 5;
							m_base = Pos;

							if ( playerPos.vx < Pos.vx )
							{
								m_extendDir = EXTEND_LEFT;
							}
							else
							{
								m_extendDir = EXTEND_RIGHT;
							}

							return( true );
						}
						else
						{
							return( false );
						}
					}

					case NPC_SENSOR_OIL_BLOB_USER_CLOSE:
					case NPC_SENSOR_NINJA_STARFISH_USER_CLOSE:
					{
						if ( xDistSqr + yDistSqr < 10000 )
						{
							m_controlFunc = NPC_CONTROL_CLOSE;
							m_velocity = m_data[m_type].speed;

							return( true );
						}
						else
						{
							return( false );
						}
					}

					case NPC_SENSOR_GHOST_PIRATE_USER_CLOSE:
					{
						if ( xDistSqr + yDistSqr < 10000 )
						{
							m_controlFunc = NPC_CONTROL_CLOSE;
							m_extendDir = EXTEND_UP;
							m_extension = 0;
							m_movementTimer = GameState::getOneSecondInFrames() >> 1;
							m_velocity = 4;

							return( true );
						}
						else
						{
							return( false );
						}
					}

					case NPC_SENSOR_SHARK_MAN_USER_VISIBLE:
					{
						s32 xDistWaypoint, yDistWaypoint;

						if ( abs( xDist ) < 500 )
						{
							// within range

							// make sure user is closer to shark man than next waypoint

							s32 xDistWaypoint, yDistWaypoint;

							m_npcPath.getDistToNextWaypoint( Pos, &xDistWaypoint, &yDistWaypoint );

							if ( abs( xDist ) < abs( xDistWaypoint ) )
							{
								s16 headingToPlayer = ratan2( yDist, xDist );

								s16 decDir, incDir, moveDist;

								s32 headingToWaypoint = ratan2( yDistWaypoint, xDistWaypoint );

								// check waypoint is in the same direction as the user

								decDir = headingToPlayer - headingToWaypoint;

								if ( decDir < 0 )
								{
									decDir += ONE;
								}

								incDir = headingToWaypoint - headingToPlayer;

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

								if ( moveDist > 512 )
								{
									return( false );
								}
								else
								{
									// check if shark man is facing user

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
										moveDist = decDir;
									}
									else
									{
										moveDist = incDir;
									}

									if ( moveDist < 1024 )
									{
										m_controlFunc = NPC_CONTROL_CLOSE;
										m_velocity = 8;

										return( true );
									}
									else
									{
										return( false );
									}
								}
							}
							else
							{
								return( false );
							}
						}
						else
						{
							return( false );
						}
					}

					case NPC_SENSOR_ANEMONE_USER_CLOSE:
					case NPC_SENSOR_EYEBALL_USER_CLOSE:
					{
						if ( xDistSqr + yDistSqr < 40000 )
						{
							m_controlFunc = NPC_CONTROL_CLOSE;

							return( true );
						}
						else
						{
							return( false );
						}
					}

					case NPC_SENSOR_SKULL_STOMPER_USER_CLOSE:
					{
						if ( xDistSqr + yDistSqr < 40000 )
						{
							m_controlFunc = NPC_CONTROL_CLOSE;
							m_extendDir = EXTEND_DOWN;

							return( true );
						}
						else
						{
							return( false );
						}
					}

					case NPC_SENSOR_BOOGER_MONSTER_USER_CLOSE:
					{
						if ( xDistSqr + yDistSqr < 400 )
						{
							m_controlFunc = NPC_CONTROL_CLOSE;
							m_extendDir = EXTEND_UP;

							return( true );
						}
						else
						{
							return( false );
						}
					}

					case NPC_SENSOR_IRON_DOGFISH_USER_CLOSE:
					{
						if ( xDistSqr + yDistSqr < 10000 )
						{
							m_controlFunc = NPC_CONTROL_CLOSE;

							return( true );
						}
						else
						{
							return( false );
						}
					}

					case NPC_SENSOR_FISH_HOOK_USER_CLOSE:
					{
						if ( xDistSqr + yDistSqr < 400 )
						{
							m_controlFunc = NPC_CONTROL_CLOSE;

							return( true );
						}
						else
						{
							return( false );
						}
					}

					case NPC_SENSOR_FALLING_ITEM_USER_CLOSE:
					{
						if ( xDistSqr + yDistSqr < 40000 )
						{
							m_controlFunc = NPC_CONTROL_CLOSE;
							m_movementTimer = GameState::getOneSecondInFrames() * 3;

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

			break;
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
			bool waypointChange;

			s16 headingToTarget = m_npcPath.think( Pos, &pathComplete, &waypointChange );

			if ( !pathComplete )
			{
				s16 decDir, incDir;
				s16 maxTurnRate = m_data[m_type].turnSpeed;

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

				if ( moveDist < -maxTurnRate )
				{
					moveDist = -maxTurnRate;
				}
				else if ( moveDist > maxTurnRate )
				{
					moveDist = maxTurnRate;
				}

				m_heading += moveDist;

				m_heading = m_heading % ONE;

				s32 preShiftX = _frames * m_data[m_type].speed * rcos( m_heading );
				s32 preShiftY = _frames * m_data[m_type].speed * rsin( m_heading );

				moveX = preShiftX >> 12;
				if ( !moveX && preShiftX )
				{
					moveX = preShiftX / abs( preShiftX );
				}

				moveY = preShiftY >> 12;
				if ( !moveY && preShiftY )
				{
					moveY = preShiftY / abs( preShiftY );
				}

				moveVel = ( _frames * m_data[m_type].speed ) << 8;
			}

			break;
		}

		case NPC_MOVEMENT_MOTHER_JELLYFISH:
		{
			processMotherJellyfishMovement( _frames );

			break;
		}

		case NPC_MOVEMENT_FLYING_DUTCHMAN:
		{
			processFlyingDutchmanMovement( _frames );

			break;
		}

		case NPC_MOVEMENT_SUB_SHARK:
		{
			processSubSharkMovement( _frames );

			break;
		}

		case NPC_MOVEMENT_IRON_DOGFISH:
		{
			processIronDogfishMovement( _frames );

			break;
		}

		case NPC_MOVEMENT_PENDULUM:
		{
			processPendulumMovement( _frames );

			break;
		}

		case NPC_MOVEMENT_FIREBALL:
		{
			processFireballMovement( _frames );

			break;
		}

		case NPC_MOVEMENT_RETURNING_HAZARD:
		{
			processReturningHazardMovement( _frames );

			break;
		}

		default:

			break;
	}

	processMovementModifier(_frames, moveX, moveY, moveVel, moveDist);
}

void CNpc::processMovementModifier(int _frames, s32 distX, s32 distY, s32 dist, s16 headingChange)
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
			processSmallJellyfishMovementModifier( _frames, distX, distY, dist, headingChange );

			break;
		}

		case NPC_MOVEMENT_MODIFIER_FISH_FOLK:
		{
			processFishFolkMovementModifier( _frames, distX, distY );

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
		case NPC_CLOSE_JELLYFISH_EVADE:
			processCloseSmallJellyfishEvade( _frames );

			break;

		case NPC_CLOSE_CLAM_ATTACK:
			processCloseClamAttack( _frames );

			break;

		case NPC_CLOSE_SPIDER_CRAB_ATTACK:
			processCloseSpiderCrabAttack( _frames );

			break;

		case NPC_CLOSE_GENERIC_USER_SEEK:
			processCloseGenericUserSeek( _frames );

			break;

		case NPC_CLOSE_GHOST_PIRATE_ATTACK:
			processCloseGhostPirateAttack( _frames );

			break;

		case NPC_CLOSE_SHARK_MAN_ATTACK:
			processCloseSharkManAttack( _frames );

			break;

		case NPC_CLOSE_ANEMONE_1_ATTACK:
			processCloseAnemone1Attack( _frames );

			break;

		case NPC_CLOSE_ANEMONE_2_ATTACK:
			processCloseAnemone2Attack( _frames );

			break;

		case NPC_CLOSE_EYEBALL_ATTACK:
			processCloseEyeballAttack( _frames );

			break;

		case NPC_CLOSE_SKULL_STOMPER_ATTACK:
			processCloseSkullStomperAttack( _frames );

			break;

		case NPC_CLOSE_BOOGER_MONSTER_ATTACK:
			processCloseBoogerMonsterAttack( _frames );

			break;

		case NPC_CLOSE_MOTHER_JELLYFISH_ATTACK:
			processCloseMotherJellyfishAttack( _frames );

			break;

		case NPC_CLOSE_FLYING_DUTCHMAN_ATTACK:
			processCloseFlyingDutchmanAttack( _frames );

			break;

		case NPC_CLOSE_SUB_SHARK_ATTACK:
			processCloseSubSharkAttack( _frames );

			break;

		case NPC_CLOSE_IRON_DOGFISH_ATTACK:
			processCloseIronDogfishAttack( _frames );

			break;

		case NPC_CLOSE_FALLING_ITEM_FALL:
			processCloseFallingItemFall( _frames );

			break;

		case NPC_CLOSE_FISH_HOOK_RISE:
			processCloseFishHookRise( _frames );

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
	if ( m_timerTimer > 0 )
	{
		this->m_timerTimer -= _frames;
	}

	switch( m_timerFunc )
	{
		case NPC_TIMER_NONE:
			{
				break;
			}

		case NPC_TIMER_EVADE_DONE:
		case NPC_TIMER_ATTACK_DONE:
			{
				if ( m_timerTimer <= 0 )
				{
					this->m_timerFunc = NPC_TIMER_NONE;
					this->m_sensorFunc = m_data[this->m_type].sensorFunc;
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
	switch( evt )
	{
		case USER_REQUEST_TALK_EVENT:
		{
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

				if ( xDiffSqr + yDiffSqr < 10000 )
				{
					if( !CConversation::isActive() )
					{
						CConversation::trigger( SCRIPTS_SPEECHTEST_DAT );
					}
				}
			}

			break;
		}

		case PROJECTILE_RETURNED_TO_SOURCE_EVENT:
		{
			m_controlFunc = NPC_CONTROL_MOVEMENT;
			m_timerFunc = NPC_TIMER_ATTACK_DONE;
			m_timerTimer = GameState::getOneSecondInFrames();
			m_sensorFunc = NPC_SENSOR_NONE;

			removeChild( sourceThing );
			sourceThing->shutdown();
			delete sourceThing;

			break;
		}
	}
}
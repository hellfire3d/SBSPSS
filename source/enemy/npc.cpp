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


#include "Gfx\actor.h"

#ifndef __VID_HEADER_
#include "system\vid.h"
#endif

#ifndef __PROJECTL_PROJECTL_H__
#include "projectl\projectl.h"
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Friend NPCs
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcFriend::init()
{
	CNpcThing::init();

	Pos.vx = 100;
	Pos.vy = 100;

	m_extension = EXTEND_RIGHT;

	// temporary
	m_actorGfx=CActorPool::GetActor(ACTORS_CLAM_SBK);

	//m_animPlaying = true;
	m_animNo = 0;
	m_frame = 0;
	m_reversed = false;

	DVECTOR ofs = getCollisionSize();

	m_drawOffset.vx = 0;
	m_drawOffset.vy = -( ofs.vy >> 1 );

	setCollisionCentreOffset( 0, -( ofs.vy >> 1 ) );

	//m_spriteBank=new ("enemy sprites") SpriteBank();
	//m_spriteBank->load(UI_UIGFX_SPR);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcFriend::shutdown()
{
	//m_spriteBank->dump();		delete m_spriteBank;

	CNpcThing::shutdown();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcFriend::think(int _frames)
{
	CNpcThing::think(_frames);

	switch( m_data[m_type].movementFunc )
	{
		case NPC_FRIEND_MOVEMENT_GARY:
			processGaryMovement( _frames );

			break;

		case NPC_FRIEND_MOVEMENT_STATIC:
		default:
			break;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcFriend::render()
{
	CNpcThing::render();

	// Render
	DVECTOR renderPos;
	DVECTOR	offset = CLevel::getCameraPos();

	renderPos.vx = Pos.vx - offset.vx;
	renderPos.vy = Pos.vy - offset.vy;

	if ( renderPos.vx >= 0 && renderPos.vx <= VidGetScrW() )
	{
		if ( renderPos.vy >= 0 && renderPos.vy <= VidGetScrH() )
		{
			m_actorGfx->Render(renderPos,m_animNo,m_frame,m_reversed);
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcFriend::processEvent( GAME_EVENT evt, CThing *sourceThing )
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

		default:
			// ignore

			break;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Enemy NPCs
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

s32 CNpcEnemy::playerXDist;
s32 CNpcEnemy::playerYDist;
s32 CNpcEnemy::playerXDistSqr;
s32 CNpcEnemy::playerYDistSqr;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcEnemy::setTypeFromMapEdit( u16 newType )
{
	m_type = mapEditConvertTable[newType - NPC_ENEMY_MAPEDIT_OFFSET];
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcEnemy::addWaypoint( s32 xPos, s32 yPos )
{
	DVECTOR newPos;

	newPos.vx = xPos << 4;
	newPos.vy = yPos << 4;

	m_npcPath.addWaypoint( newPos );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcEnemy::setStartPos( s32 xPos, s32 yPos )
{
	Pos.vx = xPos << 4;
	Pos.vy = yPos << 4;

	m_base = Pos;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Static function to initialise the actor pool with levels nasty folk
void	CNpcEnemy::CacheActor(int Type)
{
int	m_type = mapEditConvertTable[Type - NPC_ENEMY_MAPEDIT_OFFSET];
		
		CActorPool::GetActor(m_data[m_type].skelType);

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcEnemy::init()
{
	CEnemyThing::init();

	m_actorGfx=CActorPool::GetActor(m_data[m_type].skelType);

	m_animPlaying = true;
	m_animNo = m_data[m_type].initAnim;
	m_frame = 0;

	m_heading = m_fireHeading = 0;
	m_movementTimer = 0;
	m_timerTimer = 0;
	m_velocity = 0;
	m_extension = 0;
	m_rotation = 0;
	m_reversed = false;
	m_salvoCount = 0;

	m_health = m_data[this->m_type].initHealth;

	m_extendDir = EXTEND_RIGHT;

	m_timerFunc = m_data[this->m_type].timerFunc;
	m_sensorFunc = m_data[this->m_type].sensorFunc;

	m_controlFunc = NPC_CONTROL_MOVEMENT;

	m_npcPath.initPath();

	DVECTOR ofs = getCollisionSize();

	m_drawOffset.vx = 0;
	m_drawOffset.vy = 0;

	setCollisionCentreOffset( 0, -( ofs.vy >> 1 ) );

	m_positionHistory = NULL;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcEnemy::postInit()
{
	switch ( m_data[this->m_type].initFunc )
	{
		case NPC_INIT_DEFAULT:
		{
			m_npcPath.setPathType( CNpcPath::PONG_PATH );

			break;
		}

		case NPC_INIT_BALL_BLOB:
		{
			m_heading = m_fireHeading = 128;

			m_npcPath.setPathType( CNpcPath::PONG_PATH );

			break;
		}

		case NPC_INIT_HERMIT_CRAB:
		{
			m_npcPath.setPathType( CNpcPath::PONG_PATH );

			m_state = HERMIT_CRAB_NO_ATTACK;

			break;
		}

		case NPC_INIT_GHOST_PIRATE:
			m_heading = m_fireHeading = 3072;

			break;

		case NPC_INIT_SKULL_STOMPER:
		{
			m_extendDir = EXTEND_DOWN;

			break;
		}

		case NPC_INIT_MOTHER_JELLYFISH:
		{
			m_state = MOTHER_JELLYFISH_RETURN_TO_START_1;

			break;
		}

		case NPC_INIT_FLYING_DUTCHMAN:
		{
			m_state = FLYING_DUTCHMAN_ATTACK_PLAYER_1;
			m_extendDir = EXTEND_UP;
			m_extension = 100;

			break;
		}

		case NPC_INIT_SUB_SHARK:
		{
			m_state = SUB_SHARK_MINE_1;
			m_extendDir = EXTEND_RIGHT;

			break;
		}

		case NPC_INIT_IRON_DOGFISH:
		{
			m_state = IRON_DOGFISH_THUMP_1;
			m_extendDir = EXTEND_RIGHT;

			break;
		}

		case NPC_INIT_FISH_HOOK:
		{
			m_heading = m_fireHeading = 3072;
			
			DVECTOR newPos;

			newPos.vx = 100;
			newPos.vy = -100;

			m_npcPath.addWaypoint( newPos );

			m_npcPath.setPathType( CNpcPath::SINGLE_USE_PATH );

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
			DVECTOR newPos;

			newPos.vx = 300;
			newPos.vy = 100;

			m_npcPath.addWaypoint( newPos );
			m_npcPath.setPathType( CNpcPath::SINGLE_USE_PATH );

			m_extension = 0;
			m_velocity = m_data[m_type].speed;
			m_timerTimer = GameState::getOneSecondInFrames() * 4;

			break;
		}

		case NPC_INIT_RETURNING_HAZARD:
		{
			m_npcPath.setPathType( CNpcPath::SINGLE_USE_PATH );

			break;
		}

		case NPC_INIT_FISH_FOLK:
		{
			m_heading = m_fireHeading = 0;
			
			DVECTOR newPos;

			newPos.vx = 200;
			newPos.vy = 400;

			m_npcPath.addWaypoint( newPos );

			newPos.vx = 500;
			newPos.vy = 400;

			m_npcPath.addWaypoint( newPos );

			m_npcPath.setPathType( CNpcPath::PONG_PATH );

			break;
		}

		case NPC_INIT_FLAMING_SKULL:
		{
			m_state = FLAMING_SKULL_ATTACK;

			break;
		}

		case NPC_INIT_EYEBALL:
		{
			CProjectile *projectile;
			projectile = new ( "eyeball projectile" ) CProjectile;
			projectile->init( Pos, m_fireHeading, CProjectile::PROJECTILE_FIXED, CProjectile::PROJECTILE_INFINITE_LIFE );

			addChild( projectile );

			break;
		}

		case NPC_INIT_ANEMONE_2:
		{
			CProjectile *projectile;
			s16 heading;

			for ( int fireLoop = 0 ; fireLoop < 5 ; fireLoop++ )
			{
				DVECTOR spikePos;

				heading = m_heading - 1024 + ( fireLoop * 512 );
				heading &= 4095;

				spikePos = Pos;
				spikePos.vx += ( 10 * rcos( heading ) ) >> 12;
				spikePos.vy += ( 10 * rsin( heading ) ) >> 12;

				projectile = new( "anemone lev2 projectile" ) CProjectile;
				projectile->init( spikePos, heading, CProjectile::PROJECTILE_FIXED, CProjectile::PROJECTILE_INFINITE_LIFE );

				addChild( projectile );
			}

			break;
		}

		case NPC_INIT_CIRCULAR_PLATFORM:
		{
			Pos.vx = 300;
			Pos.vy = 300;

			m_base = Pos;

			m_extendDir = EXTEND_CLOCKWISE;
			m_extension = 100;

			break;
		}

		case NPC_INIT_PARASITIC_WORM:
		{
			DVECTOR newPos;

			newPos.vx = 100;
			//newPos.vy = 10;
			newPos.vy = 100;

			m_npcPath.addWaypoint( newPos );

			newPos.vx = 500;
			//newPos.vy = 10;
			newPos.vy = 100;

			m_npcPath.addWaypoint( newPos );

			newPos.vx = 500;
			//newPos.vy = 100;
			newPos.vy = 300;

			m_npcPath.addWaypoint( newPos );

			newPos.vx = 100;
			//newPos.vy = 100;
			newPos.vy = 300;

			m_npcPath.addWaypoint( newPos );

			m_npcPath.setPathType( CNpcPath::PONG_PATH );

			// create head of list
			CNpcPositionHistory *newPosition;
			newPosition = new ("position history") CNpcPositionHistory;
			newPosition->pos = Pos;
			m_positionHistory = newPosition;

			CNpcPositionHistory *currentPosition = m_positionHistory;

			// create rest of list

			for ( int histLength = 1 ; histLength < ( 10 * NPC_PARASITIC_WORM_SPACING ) ; histLength++ )
			{
				newPosition = new ("position history") CNpcPositionHistory;
				newPosition->pos = Pos;
				newPosition->next = NULL;

				currentPosition->next = newPosition;
				currentPosition = newPosition;
			}

			for ( int segCount = 0 ; segCount < 10 ; segCount++ )
			{
				CNpcEnemy *segment;
				segment = new ("segment") CNpcEnemy;
				segment->setType( CNpcEnemy::NPC_PARASITIC_WORM_SEGMENT );
				segment->init();
				segment->setLayerCollision( m_layerCollision );

				this->addChild( segment );
			}

			break;
		}

		case NPC_INIT_PARASITIC_WORM_SEGMENT:
		{
			break;
		}

		default:

			break;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcEnemy::shutdown()
{
	// remove waypoints

	m_npcPath.removeAllWaypoints();

	// remove position history

	CNpcPositionHistory *currentPosition;
	CNpcPositionHistory *oldPosition;

	currentPosition = m_positionHistory;

	while( currentPosition )
	{
		oldPosition = currentPosition;
		currentPosition = currentPosition->next;

		delete oldPosition;
	}

	m_positionHistory = NULL;

	CEnemyThing::shutdown();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcEnemy::think(int _frames)
{
	CEnemyThing::think(_frames);

	processGenericGetUserDist( _frames, &playerXDist, &playerYDist );
	playerXDistSqr = playerXDist * playerXDist;
	playerYDistSqr = playerYDist * playerYDist;

	if ( m_animPlaying )
	{
		s32 frameCount = m_actorGfx->getFrameCount(m_animNo);
		s32 frameShift = ( _frames << 8 ) >> 1;

		if ( ( frameCount << 8 ) - m_frame > frameShift ) //( _frames >> 1 ) )
		{
			//m_frame += _frames >> 1;
			m_frame += frameShift;
		}
		else
		{
			m_frame = ( frameCount - 1 ) << 8;
			m_animPlaying = false;
		}
	}

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

	if ( m_heading > 1024 && m_heading < 3072 )
	{
		m_reversed = true;
	}
	else
	{
		m_reversed = false;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcEnemy::collidedWith( CThing *_thisThing )
{
	switch(_thisThing->getThingType())
	{
		case TYPE_PLAYER:
		{
			if ( m_data[m_type].detectCollision )
			{
				if ( m_data[m_type].damageToUserType == DAMAGE__NONE )
				{
					// if we can detect a collision, but the collision does no damage, this must be a platform

					CPlayer *player = (CPlayer *) _thisThing;

					player->setPlatform( this );
				}
				else
				{
					m_oldControlFunc = m_controlFunc;
					m_controlFunc = NPC_CONTROL_COLLISION;
				}
			}

			break;
		}

		default:
			ASSERT(0);
			break;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool CNpcEnemy::processSensor()
{
	/*// temporary
	if ( playerXDistSqr + playerYDistSqr < 10000 )
	{
		m_controlFunc = NPC_CONTROL_SHOT;
		m_state = NPC_GENERIC_DEATH_START;

		return( true );
	}
	// temporary*/

	switch( m_sensorFunc )
	{
		case NPC_SENSOR_NONE:
			return( false );

		default:
			{
				switch( m_sensorFunc )
				{
					case NPC_SENSOR_JELLYFISH_USER_CLOSE:
					{
						if ( playerXDistSqr + playerYDistSqr < 5625 )
						{
							m_controlFunc = NPC_CONTROL_CLOSE;
							m_evadeClockwise = getRnd() % 2;

							return( true );
						}
						else
						{
							return( false );
						}
					}

					case NPC_SENSOR_CLAM_USER_CLOSE:
					{
						if ( playerXDistSqr + playerYDistSqr < 10000 )
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
						if ( playerXDistSqr + playerYDistSqr < 10000 )
						{
							// only attack if within path extents

							s32 minX, maxX;
							m_npcPath.getPathXExtents( &minX, &maxX );

							if ( playerXDist < 0 )
							{
								m_extendDir = EXTEND_LEFT;

								if ( ( Pos.vx + playerXDist - 128 ) < minX )
								{
									return( false );
								}
							}
							else
							{
								m_extendDir = EXTEND_RIGHT;

								if ( ( Pos.vx + playerXDist + 128 ) > maxX )
								{
									return( false );
								}
							}

							m_controlFunc = NPC_CONTROL_CLOSE;
							m_extension = 0;
							m_velocity = 5;
							m_base = Pos;

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
						if ( playerXDistSqr + playerYDistSqr < 10000 )
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
						if ( playerXDistSqr + playerYDistSqr < 10000 )
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

					case NPC_SENSOR_GENERIC_USER_VISIBLE:
					{
						s32 xDistWaypoint, yDistWaypoint;

						if ( abs( playerXDist ) < 500 )
						{
							// within range

							// make sure user is closer to npc than next waypoint

							s32 xDistWaypoint, yDistWaypoint;

							m_npcPath.getDistToNextWaypoint( Pos, &xDistWaypoint, &yDistWaypoint );

							if ( abs( playerXDist ) < abs( xDistWaypoint ) )
							{
								s16 headingToPlayer = ratan2( playerYDist, playerXDist );

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
									// check if npc is facing user

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
					case NPC_SENSOR_FLAMING_SKULL_USER_CLOSE:
					{
						if ( playerXDistSqr + playerYDistSqr < 40000 )
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
						if ( playerXDistSqr + playerYDistSqr < 40000 )
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
						if ( playerXDistSqr + playerYDistSqr < 400 )
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

					case NPC_SENSOR_PUFFA_FISH_USER_CLOSE:
					{
						if ( playerXDistSqr + playerYDistSqr < 10000 )
						{
							m_state = PUFFA_FISH_NO_INFLATE;
							m_controlFunc = NPC_CONTROL_CLOSE;

							return( true );
						}
						else
						{
							return( false );
						}
					}

					case NPC_SENSOR_FISH_HOOK_USER_CLOSE:
					case NPC_SENSOR_OCTOPUS_USER_CLOSE:
					case NPC_SENSOR_HERMIT_CRAB_USER_CLOSE:
					{
						if ( playerXDistSqr + playerYDistSqr < 400 )
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
						if ( playerXDistSqr + playerYDistSqr < 40000 )
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

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcEnemy::processMovement(int _frames)
{
	if ( _frames > 2 )
	{
		_frames = 2;
	}

	s32 moveX = 0, moveY = 0;
	s32 moveVel = 0;
	s32 moveDist = 0;

	switch( m_data[this->m_type].movementFunc )
	{
		case NPC_MOVEMENT_STATIC:
		{
			break;
		}

		case NPC_MOVEMENT_STATIC_CYCLE_ANIM:
		{
			if ( !m_animPlaying )
			{
				m_animPlaying = true;
				m_animNo = m_data[m_type].initAnim;
				m_frame = 0;
			}

			break;
		}

		case NPC_MOVEMENT_FIXED_PATH:
		{
			processGenericFixedPathMove( _frames, &moveX, &moveY, &moveVel, &moveDist );

			if ( !m_animPlaying )
			{
				m_animPlaying = true;
				m_animNo = m_data[m_type].moveAnim;
				m_frame = 0;
			}

			break;
		}

		case NPC_MOVEMENT_FIXED_PATH_WALK:
		{
			processGenericFixedPathWalk( _frames, &moveX, &moveY );

			if ( !m_animPlaying )
			{
				m_animPlaying = true;
				m_animNo = m_data[m_type].moveAnim;
				m_frame = 0;
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

		case NPC_MOVEMENT_PARASITIC_WORM:
		{
			processParasiticWormMovement( _frames );

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

		case NPC_MOVEMENT_SHARK_MAN:
		{
			processSharkManMovement( _frames, &moveX, &moveY );

			break;
		}

		case NPC_MOVEMENT_BALL_BLOB:
		{
			processBallBlobMovement( _frames, &moveX, &moveY );

			break;
		}

		default:

			break;
	}

	processMovementModifier( _frames, moveX, moveY, moveVel, moveDist );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcEnemy::processMovementModifier(int _frames, s32 distX, s32 distY, s32 dist, s16 headingChange)
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

		case NPC_MOVEMENT_MODIFIER_OCTOPUS:
		{
			processBabyOctopusMovementModifier( _frames, dist, headingChange );

			break;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcEnemy::hasBeenAttacked()
{
	m_controlFunc = NPC_CONTROL_SHOT;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcEnemy::processShot()
{
	switch( m_data[m_type].shotFunc )
	{
		case NPC_SHOT_NONE:
		{
			// do nothing

			break;
		}

		case NPC_SHOT_GENERIC_DIE:
		{
			switch ( m_state )
			{
				case NPC_GENERIC_DEATH_START:
				{
					m_animPlaying = true;
					m_animNo = m_data[m_type].dieAnim;
					m_frame = 0;
					m_state = NPC_GENERIC_DEATH_END;

					break;
				}

				case NPC_GENERIC_DEATH_END:
				{
					if ( !m_animPlaying )
					{
						this->shutdown();
					}

					break;
				}
			}

			break;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcEnemy::processClose(int _frames)
{
	switch( m_data[this->m_type].closeFunc )
	{
		case NPC_CLOSE_JELLYFISH_EVADE:
			processCloseSmallJellyfishEvade( _frames );

			break;

		case NPC_CLOSE_CLAM_JUMP_ATTACK:
			processCloseClamJumpAttack( _frames );

			break;

		case NPC_CLOSE_CLAM_SNAP_ATTACK:
			processCloseClamSnapAttack( _frames );

			break;

		case NPC_CLOSE_SPIDER_CRAB_ATTACK:
			processCloseSpiderCrabAttack( _frames );

			break;

		case NPC_CLOSE_GENERIC_USER_SEEK:
		{
			processGenericGotoTarget( _frames, playerXDist, playerYDist, m_data[m_type].speed );

			break;
		}

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

		case NPC_CLOSE_ANEMONE_3_ATTACK:
			processCloseAnemone3Attack( _frames );

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

		case NPC_CLOSE_FLAMING_SKULL_ATTACK:
			processCloseFlamingSkullAttack( _frames );

			break;

		case NPC_CLOSE_SKELETAL_FISH_ATTACK:
			processCloseSkeletalFishAttack( _frames );

			break;

		case NPC_CLOSE_HERMIT_CRAB_ATTACK:
			processCloseHermitCrabAttack( _frames );

			break;

		case NPC_CLOSE_OCTOPUS_ATTACK:
			processCloseOctopusAttack( _frames );

			break;

		case NPC_CLOSE_PUFFA_FISH_INFLATE:
			processClosePuffaFishInflate( _frames );

			break;

		default:
			break;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcEnemy::processCollision()
{
	CPlayer *player = GameScene.getPlayer();

	//player->takeDamage( m_data[m_type].damageToUserType );

	m_controlFunc = m_oldControlFunc;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcEnemy::processTimer(int _frames)
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

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcEnemy::render()
{
	CEnemyThing::render();

	// Render
	DVECTOR renderPos;
	DVECTOR	offset = CLevel::getCameraPos();

	renderPos.vx = Pos.vx - offset.vx;
	renderPos.vy = Pos.vy - offset.vy;

	if ( renderPos.vx >= 0 && renderPos.vx <= VidGetScrW() )
	{
		if ( renderPos.vy >= 0 && renderPos.vy <= VidGetScrH() )
		{
			m_actorGfx->Render(renderPos,m_animNo,( m_frame >> 8 ),m_reversed);
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcEnemy::processEvent( GAME_EVENT evt, CThing *sourceThing )
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

			//removeChild( sourceThing );
			//sourceThing->shutdown();
			//delete sourceThing;

			CProjectile *projectile;
			projectile = (CProjectile *) sourceThing;
			projectile->setMovementType( CProjectile::PROJECTILE_FIXED );
			projectile->setPosition( Pos );

			break;
		}
	}
}
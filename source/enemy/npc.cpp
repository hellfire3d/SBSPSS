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

#include "Gfx\Skel.h"
#include "gfx\anim.h"

#ifndef __VID_HEADER_
#include "system\vid.h"
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Friend NPCs
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class CLayerCollision	*CNpcFriend::m_layerCollision;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcFriend::init()
{
	CNpcThing::init();

	sActorHdr	*Hdr=m_skel.Load(ACTORS_SPONGEBOB_A3D);		
	m_skel.Init(Hdr);
	TPLoadTex(ACTORS_ACTOR_SPONGEBOB_TEX);
	m_skel.setAnimDatabase(CAnimDB::Load(ACTORS_SPONGEBOB_ABK));

	Pos.vx = 100;
	Pos.vy = 100;

	m_extension = EXTEND_RIGHT;

	// temporary
	m_animNo = 0;
	m_frame = 0;

	m_type = NPC_FRIEND_GARY;

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

	renderPos.vx = ( Pos.vx - offset.vx - ( VidGetScrW() >> 1 ) ) * 20;
	renderPos.vy = ( Pos.vy - offset.vy - ( VidGetScrH() >> 1 ) ) * 20;

	m_skel.setPos( renderPos );
	m_skel.setFrame(m_frame);
	m_skel.setAnimNo(m_animNo);
	m_skel.Animate(this);
	m_skel.Render(this);

	/*s32		x,y;
	s32		scrnWidth = VidGetScrW();
	s32		scrnHeight = VidGetScrH();
	s32		spriteWidth = m_spriteBank->getFrameWidth(FRM_BARNACLEBOY);
	s32		spriteHeight = m_spriteBank->getFrameHeight(FRM_BARNACLEBOY);

	x = Pos.vx - offset.vx - ( spriteWidth >> 1 );
	y = Pos.vy - offset.vy - ( spriteHeight >> 1 );

	//if ( x < -spriteWidth || y < -spriteHeight || x > scrnWidth || y > scrnHeight )
	//{
		//return;
	//}

	m_spriteBank->printFT4(FRM_BARNACLEBOY,x,y,0,0,0);*/
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
class CLayerCollision	*CNpcEnemy::m_layerCollision;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcEnemy::init()
{
	CEnemyThing::init();

	sActorHdr	*Hdr=m_skel.Load(ACTORS_SPONGEBOB_A3D);		
	m_skel.Init(Hdr);
	TPLoadTex(ACTORS_ACTOR_SPONGEBOB_TEX);
	m_skel.setAnimDatabase(CAnimDB::Load(ACTORS_SPONGEBOB_ABK));

	// temporary
	m_animNo = 0;
	m_frame = 0;

	m_type = NPC_CIRCULAR_PLATFORM;

	m_heading = m_fireHeading = 0;
	m_movementTimer = 0;
	m_timerTimer = 0;
	m_velocity = 0;
	m_extension = 0;
	m_rotation = 0;

	m_extension = EXTEND_RIGHT;

	Pos.vx = 100;
	Pos.vy = 100;

	m_base = Pos;

	m_timerFunc = m_data[this->m_type].timerFunc;
	m_sensorFunc = m_data[this->m_type].sensorFunc;

	m_controlFunc = NPC_CONTROL_MOVEMENT;

	m_layerCollision = NULL;

//pkg
m_npcPath.initPath();
//pkg
	switch ( m_data[this->m_type].initFunc )
	{
		case NPC_INIT_DEFAULT:
		{
			m_npcPath.initPath();

			DVECTOR newPos;

			newPos.vx = 100;
			//newPos.vy = 10;
			newPos.vy = 400;

			m_npcPath.addWaypoint( newPos );

			newPos.vx = 500;
			//newPos.vy = 10;
			newPos.vy = 400;

			m_npcPath.addWaypoint( newPos );

			newPos.vx = 500;
			//newPos.vy = 100;
			newPos.vy = 350;

			m_npcPath.addWaypoint( newPos );

			newPos.vx = 100;
			//newPos.vy = 100;
			newPos.vy = 350;

			m_npcPath.addWaypoint( newPos );

			m_npcPath.setPathType( PONG_PATH );

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

			newPos.vx = 200;
			newPos.vy = 400;

			m_npcPath.addWaypoint( newPos );

			newPos.vx = 500;
			newPos.vy = 400;

			m_npcPath.addWaypoint( newPos );

			m_npcPath.setPathType( PONG_PATH );

			break;
		}

		case NPC_INIT_FLAMING_SKULL:
		{
			m_state = FLAMING_SKULL_ATTACK;

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

		default:

			break;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcEnemy::shutdown()
{
	m_npcPath.removeAllWaypoints();

	CEnemyThing::shutdown();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcEnemy::think(int _frames)
{
	CEnemyThing::think(_frames);

	processGenericGetUserDist( _frames, &playerXDist, &playerYDist );
	playerXDistSqr = playerXDist * playerXDist;
	playerYDistSqr = playerYDist * playerYDist;

	detectCollisionWithPlayer();

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

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcEnemy::detectCollisionWithPlayer()
{
	if ( m_data[m_type].detectCollision && playerXDistSqr + playerYDistSqr < 400 )
	{
		// close enough for collision

		m_oldControlFunc = m_controlFunc;
		m_controlFunc = NPC_CONTROL_COLLISION;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool CNpcEnemy::processSensor()
{
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
							m_controlFunc = NPC_CONTROL_CLOSE;
							m_extension = 0;
							m_velocity = 5;
							m_base = Pos;

							if ( playerXDist < 0 )
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

					case NPC_SENSOR_FISH_HOOK_USER_CLOSE:
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

		case NPC_MOVEMENT_FIXED_PATH:
		{
			processGenericFixedPathMove( _frames, &moveX, &moveY, &moveVel, &moveDist );

			break;
		}

		case NPC_MOVEMENT_FIXED_PATH_WALK:
		{
			processGenericFixedPathWalk( _frames, &moveX, &moveY );

			break;
		}

		case NPC_MOVEMENT_FIXED_CIRCULAR:
		{
			processGenericCircularPath( _frames );

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

void CNpcEnemy::processShot()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcEnemy::processClose(int _frames)
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

	renderPos.vx = ( Pos.vx - offset.vx ) * 20;
	renderPos.vy = ( Pos.vy - offset.vy ) * 20;

	m_skel.setPos( renderPos );
	m_skel.setFrame(m_frame);
	m_skel.setAnimNo(m_animNo);
	m_skel.Animate(this);
	m_skel.Render(this);
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

			removeChild( sourceThing );
			sourceThing->shutdown();
			delete sourceThing;

			break;
		}
	}
}
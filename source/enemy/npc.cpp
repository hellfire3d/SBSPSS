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

#ifndef __PROJECTL_PRNPC_H__
#include "projectl\prnpc.h"
#endif


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef __ENEMY_NSJFISH_H__
#include "enemy\nsjfish.h"
#endif

#ifndef __ENEMY_NHCRAB_H__
#include "enemy\nhcrab.h"
#endif

#ifndef __ENEMY_NSCRAB_H__
#include "enemy\nscrab.h"
#endif

#ifndef __ENEMY_NGEN_H__
#include "enemy\ngen.h"
#endif

#ifndef	__ENEMY_NANEMONE_H__
#include "enemy\nanemone.h"
#endif

#ifndef	__ENEMY_NCLAM_H__
#include "enemy\nclam.h"
#endif

#ifndef	__ENEMY_NOCTO_H__
#include "enemy\nocto.h"
#endif

#ifndef __ENEMY_NFFOLK_H__
#include "enemy\nffolk.h"
#endif

#ifndef __ENEMY_NBBLOB_H__
#include "enemy\nbblob.h"
#endif

#ifndef	__ENEMY_NGPIRATE_H__
#include "enemy\ngpirate.h"
#endif

#ifndef	__ENEMY_NPUFFA_H__
#include "enemy\npuffa.h"
#endif

#ifndef	__ENEMY_NSHRKMAN_H__
#include "enemy\nshrkman.h"
#endif

#ifndef	__ENEMY_NSKLFISH_H__
#include "enemy\nsklfish.h"
#endif

#ifndef	__ENEMY_NEYEBALL_H__
#include "enemy\neyeball.h"
#endif

#ifndef	__ENEMY_NFSKULL_H__
#include "enemy\nfskull.h"
#endif

#ifndef	__ENEMY_NSSTOMP_H__
#include "enemy\nsstomp.h"
#endif

#ifndef	__ENEMY_NBOOGER_H__
#include "enemy\nbooger.h"
#endif

#ifndef	__ENEMY_NMJFISH_H__
#include "enemy\nmjfish.h"
#endif

#ifndef	__ENEMY_NSSHARK_H__
#include "enemy\nsshark.h"
#endif

#ifndef	__ENEMY_NWORM_H__
#include "enemy\nworm.h"
#endif

#ifndef	__ENEMY_NFDUTCH_H__
#include "enemy\nfdutch.h"
#endif

#ifndef	__ENEMY_NDOGFISH_H__
#include "enemy\ndogfish.h"
#endif

#ifndef	__ENEMY_NDUSTDEV_H__
#include "enemy\ndustdev.h"
#endif

#ifndef __ENEMY_NSDART_H__
#include "enemy\nsdart.h"
#endif

#ifndef __ENEMY_NPBUG_H__
#include "enemy\npbug.h"
#endif

#ifndef __ENEMY_NSSNAKE_H__
#include "enemy\nssnake.h"
#endif

#ifndef __ENEMY_NANGLER_H__
#include "enemy\nangler.h"
#endif

#ifndef __ENEMY_NMINE_H__
#include "enemy\nmine.h"
#endif

#ifndef __ENEMY_NSTRFISH_H__
#include "enemy\nstrfish.h"
#endif

#ifndef __ENEMY_NGHOST_H__
#include "enemy\nghost.h"
#endif

#ifndef __ENEMY_NOILBLOB_H__
#include "enemy\noilblob.h"
#endif

#ifndef __ENEMY_NSHELL_H__
#include "enemy\nshell.h"
#endif


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Enemy NPCs
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

s32 CNpcEnemy::playerXDist;
s32 CNpcEnemy::playerYDist;
s32 CNpcEnemy::playerXDistSqr;
s32 CNpcEnemy::playerYDistSqr;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CNpcEnemy::NPC_UNIT_TYPE CNpcEnemy::getTypeFromMapEdit( u16 newType )
{
	return( mapEditConvertTable[newType - NPC_ENEMY_MAPEDIT_OFFSET] );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CNpcEnemy	*CNpcEnemy::Create(sThingActor *ThisActor)
{
	CNpcEnemy *enemy;

	NPC_UNIT_TYPE enemyType = CNpcEnemy::getTypeFromMapEdit( ThisActor->Type );

	switch( enemyType )
	{
		case CNpcEnemy::NPC_SMALL_JELLYFISH_1:
		case CNpcEnemy::NPC_SMALL_JELLYFISH_2:
		{
			printf("JFish\n");
			enemy = new ("small jellyfish") CNpcSmallJellyfishEnemy;
			break;
		}

		case CNpcEnemy::NPC_HERMIT_CRAB:
		{
			printf("NPC_HERMIT_CRAB\n");
			enemy = new ("hermit crab") CNpcHermitCrabEnemy;
			break;
		}

		case CNpcEnemy::NPC_SPIDER_CRAB:
		{
			printf("NPC_SPIDER_CRAB\n");
			enemy = new ("spider crab") CNpcSpiderCrabEnemy;
			break;
		}

		case CNpcEnemy::NPC_SPIDER_CRAB_SPAWNER:
		{
			printf("NPC_SPIDER_CRAB_SPAWNER\n");
			enemy = new ("spider crab spawner") CNpcEnemyGenerator;
			break;
		}

		case CNpcEnemy::NPC_ANEMONE_1:
		{
			printf("NPC_ANEMONE_1\n");
			enemy = new ("anemone 1") CNpcAnemone1Enemy;
			break;
		}

		case CNpcEnemy::NPC_ANEMONE_2:
		{
			printf("NPC_ANEMONE_2\n");
			enemy = new ("anemone 2") CNpcAnemone2Enemy;
			break;
		}

		case CNpcEnemy::NPC_ANEMONE_3:
		{
			printf("NPC_ANEMONE_3\n");
			enemy = new ("anemone 3") CNpcAnemone3Enemy;
			break;
		}

		case CNpcEnemy::NPC_CLAM_JUMP:
		{
			printf("NPC_CLAM_JUMP\n");
			enemy = new ("jumping clam") CNpcJumpingClamEnemy;
			break;
		}

		case CNpcEnemy::NPC_CLAM_STATIC:
		{
			printf("NPC_CLAM_STATIC\n");
			enemy = new ("static clam") CNpcStaticClamEnemy;
			break;
		}

		case CNpcEnemy::NPC_BABY_OCTOPUS:
		{
			printf("NPC_BABY_OCTOPUS\n");
			enemy = new ("baby octopus") CNpcBabyOctopusEnemy;
			break;
		}

		case CNpcEnemy::NPC_FISH_FOLK:
		case CNpcEnemy::NPC_ZOMBIE_FISH_FOLK:
		{
			printf("NPC_FISH_FOLK\n");
			enemy = new ("fish folk") CNpcFishFolk;
			break;
		}

		case CNpcEnemy::NPC_BALL_BLOB:
		{
			printf("NPC_BALL_BLOB\n");
			enemy = new ("ball blob") CNpcBallBlobEnemy;
			break;
		}

		case CNpcEnemy::NPC_GHOST_PIRATE:
		{
			printf("NPC_GHOST_PIRATE\n");
			enemy = new ("ghost pirate") CNpcGhostPirateEnemy;
			break;
		}

		case CNpcEnemy::NPC_PUFFA_FISH:
		{
			printf("NPC_PUFFA_FISH\n");
			enemy = new ("puffa fish") CNpcPuffaFishEnemy;
			break;
		}

		case CNpcEnemy::NPC_SHARK_MAN:
		{
			printf("NPC_SHARK_MAN\n");
			enemy = new ("shark man") CNpcSharkManEnemy;
			break;
		}

		case CNpcEnemy::NPC_SKELETAL_FISH:
		{
			printf("NPC_SKELETAL_FISH\n");
			enemy = new ("skeletal fish") CNpcSkeletalFishEnemy;
			break;
		}

		case CNpcEnemy::NPC_EYEBALL:
		{
			printf("NPC_EYEBALL\n");
			enemy = new ("eyeball") CNpcEyeballEnemy;
			break;
		}

		case CNpcEnemy::NPC_FLAMING_SKULL:
		{
			printf("NPC_FLAMING_SKULL\n");
			enemy = new ("flaming skull") CNpcFlamingSkullEnemy;
			break;
		}

		case CNpcEnemy::NPC_SKULL_STOMPER:
		{
			printf("NPC_SKULL_STOMPER\n");
			enemy = new ("skull stomper") CNpcSkullStomperEnemy;
			break;
		}

		case CNpcEnemy::NPC_BOOGER_MONSTER:
		{
			printf("NPC_BOOGER_MONSTER\n");
			enemy = new ("booger monster") CNpcBoogerMonsterEnemy;
			break;
		}

		case CNpcEnemy::NPC_MOTHER_JELLYFISH:
		{
			printf("NPC_MOTHER_JELLYFISH\n");
			enemy = new ("mother jellyfish") CNpcMotherJellyfishEnemy;
			break;
		}

		case CNpcEnemy::NPC_SUB_SHARK:
		{
			printf("NPC_SUB_SHARK\n");
			enemy = new ("sub shark") CNpcSubSharkEnemy;
			break;
		}

		case CNpcEnemy::NPC_PARASITIC_WORM:
		{
			printf("NPC_PARASITIC_WORM\n");
			enemy = new ("parasitic worm") CNpcParasiticWormEnemy;
			break;
		}

		case CNpcEnemy::NPC_FLYING_DUTCHMAN:
		{
			printf("NPC_FLYING_DUTCHMAN\n");
			enemy = new ("flying dutchman") CNpcFlyingDutchmanEnemy;
			break;
		}

		case CNpcEnemy::NPC_IRON_DOGFISH:
		{
			printf("NPC_IRON_DOGFISH\n");
			enemy = new ("iron dogfish") CNpcIronDogfishEnemy;
			break;
		}

		/*case CNpcEnemy::NPC_FALLING_ITEM:
		{
			printf("NPC_FALLING_ITEM\n");
			enemy = new ("falling item") CNpcFallingItemHazard;
			break;
		}*/

		/*case CNpcEnemy::NPC_FISH_HOOK:
		{
			printf("NPC_FISH_HOOK\n");
			enemy = new ("fish hook") CNpcFishHookHazard;
			break;
		}*/

		/*case CNpcEnemy::NPC_PENDULUM:
		{
			printf("NPC_PENDULUM\n");
			enemy = new ("pendulum") CNpcPendulumHazard;
			break;
		}*/

		/*case CNpcEnemy::NPC_FIREBALL:
		{
			printf("NPC_FIREBALL\n");
			enemy = new ("fireball") CNpcFireballHazard;
			break;
		}*/

		/*case CNpcEnemy::NPC_SAW_BLADE:
		{
			printf("NPC_SAW_BLADE\n");
			enemy = new ("saw blade") CNpcReturningHazard;
			break;
		}*/

		case CNpcEnemy::NPC_DUST_DEVIL:
		{
			printf("NPC_DUST_DEVIL\n");
			enemy = new ("dust devil") CNpcDustDevilEnemy;
			break;
		}

		case CNpcEnemy::NPC_SQUID_DART:
		{
			enemy = new ("squid dart") CNpcSquidDartEnemy;
			break;
		}

		case CNpcEnemy::NPC_PRICKLY_BUG:
		{
			enemy = new ("prickly bug") CNpcPricklyBugEnemy;
			break;
		}

		case CNpcEnemy::NPC_SEA_SNAKE:
		{
			enemy = new ("sea snake") CNpcSeaSnakeEnemy;
			break;
		}

		case CNpcEnemy::NPC_ANGLER_FISH:
		{
			enemy = new ("angler fish") CNpcAnglerFish;
			break;
		}

		case CNpcEnemy::NPC_MINE:
		{
			enemy = new ("mine") CNpcMineEnemy;
			break;
		}

		case CNpcEnemy::NPC_NINJA_STARFISH:
		{
			enemy = new ("ninja starfish") CNpcNinjaStarfishEnemy;
			break;
		}

		case CNpcEnemy::NPC_GHOST:
		{
			enemy = new ("ghost") CNpcGhostEnemy;
			break;
		}

		case CNpcEnemy::NPC_OIL_BLOB:
		{
			enemy = new ("oil blob") CNpcOilBlobEnemy;
			break;
		}

		case CNpcEnemy::NPC_SHELL:
		{
			enemy = new ("shell") CNpcShellEnemy;
			break;
		}

		default:
		{
			printf("UNKNOWN %i\n",enemyType);
			enemy = new ("npc enemy") CNpcEnemy;
			ASSERT(0);
			break;
		}
	}

	ASSERT(enemy);

	enemy->setType( enemyType );
	enemy->init();

	return(enemy);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcEnemy::setupWaypoints( sThingActor *ThisActor )
{
	u16	*PntList=(u16*)MakePtr(ThisActor,sizeof(sThingActor));

	u16 newXPos, newYPos;

	newXPos = (u16) *PntList;
	PntList++;
	newYPos = (u16) *PntList;
	PntList++;

	setStartPos( newXPos, newYPos );
	addWaypoint( newXPos, newYPos );

	if ( ThisActor->PointCount > 1 )
	{
		for (int pointNum = 1 ; pointNum < ThisActor->PointCount ; pointNum++ )
		{
			newXPos = (u16) *PntList;
			PntList++;
			newYPos = (u16) *PntList;
			PntList++;

			addWaypoint( newXPos, newYPos );

			if ( pointNum == 1 )
			{
				setHeading( newXPos, newYPos );
			}
		}
	}
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

	m_initPos = m_base = Pos;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcEnemy::setHeading( s32 xPos, s32 yPos )
{
	m_heading = ratan2( ( yPos << 4 ) - Pos.vy, ( xPos << 4 ) - Pos.vx );
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcEnemy::init()
{
	CEnemyThing::init();
	if (m_data[m_type].skelType)
	{
		m_actorGfx=CActorPool::GetActor((FileEquate)m_data[m_type].skelType);
		m_spriteBank=0;

	}
	else
	{
		m_actorGfx=0;
		m_spriteBank=new ("enemy sprites") SpriteBank();
		m_spriteBank->load(SPRITES_SPRITES_SPR);
	}

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
	m_isActive = true;

	m_health = m_data[this->m_type].initHealth;

	m_extendDir = EXTEND_RIGHT;

	m_timerFunc = m_data[this->m_type].timerFunc;
	m_sensorFunc = m_data[this->m_type].sensorFunc;
	m_movementFunc = m_data[this->m_type].movementFunc;

	m_controlFunc = NPC_CONTROL_MOVEMENT;

	m_npcPath.initPath();

	DVECTOR ofs = getCollisionSize();

	m_drawOffset.vx = 0;
	m_drawOffset.vy = -( ofs.vy >> 1 );

	m_positionHistory = NULL;

	m_isShuttingDown = false;
	m_drawRotation = 0;
	m_isCaught = false;
	m_isBlowerOn = false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcEnemy::postInit()
{
	m_npcPath.setPathType( CNpcPath::PONG_PATH );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcEnemy::reinit()
{
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
	m_isActive = true;

	m_health = m_data[this->m_type].initHealth;

	m_extendDir = EXTEND_RIGHT;

	m_timerFunc = m_data[this->m_type].timerFunc;
	m_sensorFunc = m_data[this->m_type].sensorFunc;
	m_movementFunc = m_data[this->m_type].movementFunc;

	m_controlFunc = NPC_CONTROL_MOVEMENT;

	Pos = m_initPos;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcEnemy::shutdown()
{
	if (m_spriteBank) m_spriteBank->dump();		delete m_spriteBank;
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

		oldPosition->prev->next = NULL;
		delete oldPosition;
	}

	m_positionHistory = NULL;

	if (m_actorGfx)	delete m_actorGfx;

	CEnemyThing::shutdown();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcEnemy::setToShutdown()
{
	m_isShuttingDown = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool gitTrigger = false;

int CNpcEnemy::getFrameCount()
{
	return( m_actorGfx->getFrameCount( m_animNo ) );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcEnemy::think(int _frames)
{
	int moveFrames = _frames;

	if ( moveFrames > 2 )
	{
		// make sure enemies don't go berserk if too many frames are dropped

		moveFrames = 2;
	}

	CEnemyThing::think(_frames);

	processGenericGetUserDist( moveFrames, &playerXDist, &playerYDist );
	playerXDistSqr = playerXDist * playerXDist;
	playerYDistSqr = playerYDist * playerYDist;

	if ( m_isCaught )
	{
		processCoralBlower( moveFrames );
	}
	else
	{
		if ( m_isActive )
		{
			if ( m_animPlaying )
			{
				s32 frameCount;

				frameCount = getFrameCount();

				s32 frameShift = ( moveFrames << 8 ) >> 1;

				if ( ( frameCount << 8 ) - m_frame > frameShift )
				{
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
						processMovement( moveFrames );
					}
					else
					{
						processClose( moveFrames );
					}

					break;

				case NPC_CONTROL_SHOT:
					processShot();

					break;

				case NPC_CONTROL_CLOSE:
					processClose( moveFrames );

					break;

				case NPC_CONTROL_COLLISION:
					processCollision();

					break;
			}

			if ( m_heading > 1024 && m_heading < 3072 )
			{
				m_reversed = true;
			}
			else
			{
				m_reversed = false;
			}
		}
	}

	if ( !m_isCaught )
	{
		processTimer( moveFrames );
	}

	if ( gitTrigger )
	{
		fireAsProjectile( 0 );
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcEnemy::collidedWith( CThing *_thisThing )
{
	if ( m_isActive && !m_isCaught )
	{
		switch(_thisThing->getThingType())
		{
			case TYPE_PLAYER:
			{
				CPlayer *player = (CPlayer *) _thisThing;

				ATTACK_STATE playerState = player->getAttackState();

				switch( playerState )
				{
					case ATTACK_STATE__NONE:
					{
						if ( !player->isRecoveringFromHit() )
						{
							switch( m_data[m_type].detectCollision )
							{
								case DETECT_NO_COLLISION:
								{
									// ignore

									break;
								}

								case DETECT_ALL_COLLISION:
								{
									m_oldControlFunc = m_controlFunc;
									m_controlFunc = NPC_CONTROL_COLLISION;

									break;
								}

								case DETECT_ATTACK_COLLISION_GENERIC:
								{
									//if ( m_controlFunc == NPC_CONTROL_CLOSE && m_data[m_type].closeFunc != NPC_CLOSE_NONE )
									if ( m_controlFunc == NPC_CONTROL_CLOSE )
									{
										// only detect collision if in attack mode

										m_oldControlFunc = m_controlFunc;
										m_controlFunc = NPC_CONTROL_COLLISION;
									}

									break;
								}
							}
						}

						break;
					}

					default:
					{
						// player is attacking, respond appropriately

						if ( m_controlFunc != NPC_CONTROL_SHOT )
						{
							m_controlFunc = NPC_CONTROL_SHOT;
							m_state = NPC_GENERIC_HIT_CHECK_HEALTH;
						}

						break;
					}
				}

				break;
			}

			case TYPE_ENEMY:
			{
				CNpcEnemy *enemy = (CNpcEnemy *) _thisThing;

				if ( enemy->canCollideWithEnemy() )
				{
					processEnemyCollision( _thisThing );
				}

				break;
			}

			default:
				ASSERT(0);
				break;
		}
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
					case NPC_SENSOR_GENERIC_USER_CLOSE:
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
	s32 moveX = 0, moveY = 0;
	s32 moveVel = 0;
	s32 moveDist = 0;

	switch( m_movementFunc )
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

		default:

			break;
	}

	processMovementModifier( _frames, moveX, moveY, moveVel, moveDist );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcEnemy::processMovementModifier(int _frames, s32 distX, s32 distY, s32 dist, s16 headingChange)
{
	Pos.vx += distX;
	Pos.vy += distY;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcEnemy::hasBeenAttacked()
{
	m_controlFunc = NPC_CONTROL_SHOT;
	m_state = NPC_GENERIC_HIT_CHECK_HEALTH;
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

					break;
				}

				case NPC_GENERIC_HIT_DEATH_END:
				{
					if ( !m_animPlaying )
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

			break;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcEnemy::processClose(int _frames)
{
	switch( m_data[this->m_type].closeFunc )
	{
		case NPC_CLOSE_GENERIC_USER_SEEK:
		{
			processGenericGotoTarget( _frames, playerXDist, playerYDist, m_data[m_type].speed );

			break;
		}

		default:
			break;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcEnemy::processCollision()
{
	CPlayer *player = GameScene.getPlayer();

	player->takeDamage( m_data[m_type].damageToUserType );

	m_controlFunc = m_oldControlFunc;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcEnemy::processTimer(int _frames)
{
	if ( m_timerTimer > 0 )
	{
		m_timerTimer -= _frames;
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
					m_timerFunc = NPC_TIMER_NONE;
					m_sensorFunc = m_data[this->m_type].sensorFunc;
				}

				break;
			}

		case NPC_TIMER_RESPAWN:
			{
				if ( m_timerTimer <= 0 )
				{
					reinit();
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
	SprFrame = NULL;

	if ( m_isActive )
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
				SprFrame = m_actorGfx->Render(renderPos,m_animNo,( m_frame >> 8 ),m_reversed);
				m_actorGfx->RotateScale( SprFrame, renderPos, m_drawRotation, 4096, 4096 );

				sBBox boundingBox = m_actorGfx->GetBBox();
				setCollisionSize( ( boundingBox.XMax - boundingBox.XMin ), ( boundingBox.YMax - boundingBox.YMin ) );
				setCollisionCentreOffset( ( boundingBox.XMax + boundingBox.XMin ) >> 1, ( boundingBox.YMax + boundingBox.YMin ) >> 1 );
			}
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcEnemy::processEvent( GAME_EVENT evt, CThing *sourceThing )
{
	switch( evt )
	{
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

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool CNpcEnemy::canBeCaughtByNet()
{
	return( m_isActive && m_data[m_type].canBeNetted );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcEnemy::caughtWithNet()
{
	if ( m_data[m_type].respawning )
	{
		if ( m_isActive )
		{
			m_isActive = false;

			m_timerFunc = NPC_TIMER_RESPAWN;
			m_timerTimer = 4 * GameState::getOneSecondInFrames();
		}
	}
	else
	{
		setToShutdown();
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int CNpcEnemy::canCollide()
{
	return( m_isActive );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcEnemy::processEnemyCollision( CThing *thisThing )
{
	DVECTOR otherPos = thisThing->getPos();
	DVECTOR otherDelta = thisThing->getPosDelta();

	s32 xDist = Pos.vx - otherPos.vx;
	s32 yDist = Pos.vy - otherPos.vy;

	s16 headingFromTarget = ratan2( yDist, xDist );

	if ( ( xDist > 0 && otherDelta.vx < 0 ) || ( xDist < 0 && otherDelta.vx > 0 ) )
	{
		otherDelta.vx = -otherDelta.vx;
	}

	if ( ( yDist > 0 && otherDelta.vy < 0 ) || ( yDist < 0 && otherDelta.vy > 0 ) )
	{
		otherDelta.vy = -otherDelta.vy;
	}

	Pos.vx += otherDelta.vx;
	Pos.vy += otherDelta.vy;

	m_heading = headingFromTarget;

	s32 waypointXDist;
	s32 waypointYDist;

	m_npcPath.getDistToNextWaypoint( Pos, &waypointXDist, &waypointYDist );

	if ( ( xDist > 0 && waypointXDist < 0 ) || ( xDist < 0 && waypointXDist > 0 ) )
	{
		// try next waypoint to get around other enemy

		m_npcPath.incPath();
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool CNpcEnemy::processCoralBlowerMovement( int _frames, s32 xDist, s32 yDist )
{
	s32 moveX, moveY;
	s16 headingToTarget;
	
	headingToTarget = ratan2( yDist, xDist );

	s32 preShiftX = _frames * 3 * rcos( headingToTarget );
	s32 preShiftY = _frames * 3 * rsin( headingToTarget );

	moveX = preShiftX >> 12;
	if ( !moveX && preShiftX )
	{
		moveX = preShiftX / abs( preShiftX );
	}

	if ( xDist > 0 )
	{
		if ( moveX > xDist )
		{
			moveX = xDist;
		}
	}
	else if ( xDist < 0 )
	{
		if ( moveX < xDist )
		{
			moveX = xDist;
		}
	}
	else
	{
		moveX = 0;
	}

	moveY = preShiftY >> 12;
	if ( !moveY && preShiftY )
	{
		moveY = preShiftY / abs( preShiftY );
	}

	if ( yDist > 0 )
	{
		if ( moveY > yDist )
		{
			moveY = yDist;
		}
	}
	else if ( yDist < 0 )
	{
		if ( moveY < yDist )
		{
			moveY = yDist;
		}
	}
	else
	{
		moveY = 0;
	}

	Pos.vx += moveX;
	Pos.vy += moveY;

	if ( moveX || moveY )
	{
		return( false );
	}
	else
	{
		return( true );
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool CNpcEnemy::canBeSuckedUp()
{
	return( m_data[m_type].canBeSuckedUp );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool CNpcEnemy::suckUp( DVECTOR *suckPos, int _frames )
{
	m_isCaught = true;
	m_isBlowerOn = true;

	bool returnVal = false;

	switch( m_state )
	{
		case NPC_CORAL_BLOWER_SUCK:
		{
			// go to user

			s32 targetXDist = suckPos->vx - Pos.vx;
			s32 targetYDist = suckPos->vy - Pos.vy;

			returnVal = processCoralBlowerMovement( _frames, targetXDist, targetYDist );

			break;
		}

		case NPC_CORAL_BLOWER_RETURN:
		{
			m_state = NPC_CORAL_BLOWER_SUCK;

			break;
		}

		default:
		{
			m_state = NPC_CORAL_BLOWER_SUCK;
			m_oldState = m_state;
			m_caughtPos = Pos;

			break;
		}
	}

	return( returnVal );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcEnemy::processCoralBlower( int _frames )
{
	s32 targetXDist, targetYDist;

	switch( m_state )
	{
		case NPC_CORAL_BLOWER_SUCK:
		{
			if ( !m_isBlowerOn )
			{
				m_state = NPC_CORAL_BLOWER_RETURN;
			}

			break;
		}

		case NPC_CORAL_BLOWER_RETURN:
		{
			// go to original position

			targetXDist = m_caughtPos.vx - Pos.vx;
			targetYDist = m_caughtPos.vy - Pos.vy;

			processCoralBlowerMovement( _frames, targetXDist, targetYDist );

			if ( !targetXDist && !targetYDist )
			{
				m_state = m_oldState;
				m_isCaught = false;
			}

			break;
		}

		default:
		{
			m_oldState = m_state;
			m_state = NPC_CORAL_BLOWER_SUCK;
			m_caughtPos = Pos;

			break;
		}
	}

	m_isBlowerOn = false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcEnemy::fireAsProjectile( s16 heading )
{
	m_isActive = false;
	setToShutdown();

	DVECTOR newPos = Pos;

	newPos.vy -= 10;

	CEnemyAsProjectile *projectile;
	projectile = new( "blower projectile" ) CEnemyAsProjectile;
	projectile->init(	newPos,
						heading,
						CPlayerProjectile::PLAYER_PROJECTILE_DUMBFIRE,
						CPlayerProjectile::PLAYER_PROJECTILE_FINITE_LIFE,
						5*60);
	projectile->setLayerCollision( m_layerCollision );
	projectile->setGraphic( m_actorGfx );
}
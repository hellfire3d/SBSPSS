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

#ifndef	__ENEMY_NHAZARD_H__
#include "enemy\nhazard.h"
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

	delete m_actorGfx;
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

//!!!!!! Sort you life out charles - Unhappy Dave
	if (enemyType==14)
	{
		enemyType=CNpcEnemy::NPC_SMALL_JELLYFISH_1;
	}

	switch( enemyType )
	{
		case CNpcEnemy::NPC_SMALL_JELLYFISH_1:
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

		case CNpcEnemy::NPC_FALLING_ITEM:
		{
			printf("NPC_FALLING_ITEM\n");
			enemy = new ("falling item") CNpcFallingItemHazard;
			break;
		}

		case CNpcEnemy::NPC_FISH_HOOK:
		{
			printf("NPC_FISH_HOOK\n");
			enemy = new ("fish hook") CNpcFishHookHazard;
			break;
		}

		case CNpcEnemy::NPC_PENDULUM:
		{
			printf("NPC_PENDULUM\n");
			enemy = new ("pendulum") CNpcPendulumHazard;
			break;
		}

		case CNpcEnemy::NPC_FIREBALL:
		{
			printf("NPC_FIREBALL\n");
			enemy = new ("fireball") CNpcFireballHazard;
			break;
		}

		case CNpcEnemy::NPC_SAW_BLADE:
		{
			printf("NPC_SAW_BLADE\n");
			enemy = new ("saw blade") CNpcReturningHazard;
			break;
		}

		case CNpcEnemy::NPC_DUST_DEVIL:
		{
			printf("NPC_DUST_DEVIL\n");
			enemy = new ("dust devil") CNpcReturningGroundHazard;
			break;
		}

		default:
		{
			printf("UNKNOWN %i\n",enemyType);
			enemy = new ("npc enemy") CNpcEnemy;
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
	m_drawOffset.vy = 0;

	setCollisionCentreOffset( 0, -( ofs.vy >> 1 ) );

	m_positionHistory = NULL;
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

		delete oldPosition;
	}

	m_positionHistory = NULL;

	if (m_actorGfx)	delete m_actorGfx;

	CEnemyThing::shutdown();

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int CNpcEnemy::getFrameCount()
{
	return( m_actorGfx->getFrameCount( m_animNo ) );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcEnemy::think(int _frames)
{
	CEnemyThing::think(_frames);

	if ( m_isActive )
	{
		processGenericGetUserDist( _frames, &playerXDist, &playerYDist );
		playerXDistSqr = playerXDist * playerXDist;
		playerYDistSqr = playerYDist * playerYDist;

		if ( m_animPlaying )
		{
			s32 frameCount;

			frameCount = getFrameCount();

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

		if ( m_heading > 1024 && m_heading < 3072 )
		{
			m_reversed = true;
		}
		else
		{
			m_reversed = false;
		}
	}

	processTimer(_frames);

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcEnemy::collidedWith( CThing *_thisThing )
{
	if ( m_isActive )
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
	//if ( _frames > 2 )
	//{
		//_frames = 2;
	//}

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
							shutdown();
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
			}
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
		shutdown();
	}
}

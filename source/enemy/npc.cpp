/*=========================================================================

	npc.cpp

	Author:		CRB
	Created:
	Project:	Spongebob
	Purpose:

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/

#include "enemy\npc.h"

#ifndef __PAD_VIBE_H__
#include "pad\vibe.h"
#endif

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

#include "fx\fx.h"


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

#ifndef __ENEMY_NGHOST_H__
#include "enemy\nghost.h"
#endif

#ifndef __ENEMY_NSHELL_H__
#include "enemy\nshell.h"
#endif

#ifndef	__ENEMY_NMJBACK_H__
#include "enemy\nmjback.h"
#endif

#ifndef __ENEMY_NSJBACK_H__
#include "enemy\nsjback.h"
#endif

#ifndef __ENEMY_NBUTTFLY_H__
#include "enemy\nbuttfly.h"
#endif

#ifndef __ENEMY_NPROJJF_H__
#include "enemy\nprojjf.h"
#endif

#ifndef __ENEMY_NBOSS_H__
#include "enemy\nboss.h"
#endif


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Enemy NPCs
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

s32 CNpcEnemy::playerXDist;
s32 CNpcEnemy::playerYDist;
s32 CNpcEnemy::playerXDistSqr;
s32 CNpcEnemy::playerYDistSqr;

extern CThing	*g_bossThing;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CNpcEnemy::NPC_UNIT_TYPE CNpcEnemy::getTypeFromMapEdit( u16 newType )
{
	return( mapEditConvertTable[newType - NPC_ENEMY_MAPEDIT_OFFSET] );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CNpcEnemy	*CNpcEnemy::Create(int enemyType)
{
	switch( enemyType )
	{
		case CNpcEnemy::NPC_MOTHER_JELLYFISH:
		case CNpcEnemy::NPC_SUB_SHARK:
		case CNpcEnemy::NPC_FLYING_DUTCHMAN:
		case CNpcEnemy::NPC_IRON_DOGFISH:
		case CNpcEnemy::NPC_SEA_SNAKE:
		{
			if ( CLevel::getIsBossRespawn() )
			{
				if ( CLevel::getBossHealth() <= 0 )
				{
					return( NULL );
				}
			}
		}

		default:
			break;
	}

	CNpcEnemy *enemy;

	enemy = (CNpcEnemy*)CThingManager::GetThing(TYPE_ENEMY,enemyType);
	if (!enemy)
	switch( enemyType )
	{
		case CNpcEnemy::NPC_SMALL_JELLYFISH_1:
		case CNpcEnemy::NPC_SMALL_JELLYFISH_2:
		{
			enemy = new ("small jellyfish") CNpcSmallJellyfishEnemy;
			break;
		}

		case CNpcEnemy::NPC_HERMIT_CRAB:
		{
			enemy = new ("hermit crab") CNpcHermitCrabEnemy;
			break;
		}

		case CNpcEnemy::NPC_SPIDER_CRAB:
		{
			enemy = new ("spider crab") CNpcSpiderCrabEnemy;
			break;
		}

		case CNpcEnemy::NPC_SPIDER_CRAB_SPAWNER:
		{
			enemy = new ("spider crab spawner") CNpcEnemyGenerator;
			break;
		}

		case CNpcEnemy::NPC_ANEMONE_1:
		{
			enemy = new ("anemone 1") CNpcAnemone1Enemy;
			break;
		}

		case CNpcEnemy::NPC_ANEMONE_2:
		{
			enemy = new ("anemone 2") CNpcAnemone2Enemy;
			break;
		}

		case CNpcEnemy::NPC_ANEMONE_3:
		{
			enemy = new ("anemone 3") CNpcAnemone3Enemy;
			break;
		}

		case CNpcEnemy::NPC_CLAM_JUMP:
		{
			enemy = new ("jumping clam") CNpcJumpingClamEnemy;
			break;
		}

		case CNpcEnemy::NPC_CLAM_STATIC:
		{
			enemy = new ("static clam") CNpcStaticClamEnemy;
			break;
		}

		case CNpcEnemy::NPC_BABY_OCTOPUS:
		{
			enemy = new ("baby octopus") CNpcBabyOctopusEnemy;
			break;
		}

		case CNpcEnemy::NPC_FISH_FOLK:
		case CNpcEnemy::NPC_ZOMBIE_FISH_FOLK:
		{
			enemy = new ("fish folk") CNpcFishFolk;
			break;
		}

		case CNpcEnemy::NPC_BALL_BLOB:
		{
			enemy = new ("ball blob") CNpcBallBlobEnemy;
			break;
		}

		case CNpcEnemy::NPC_PUFFA_FISH:
		{
			enemy = new ("puffa fish") CNpcPuffaFishEnemy;
			break;
		}

		case CNpcEnemy::NPC_SHARK_MAN:
		{
			enemy = new ("shark man") CNpcSharkManEnemy;
			break;
		}

		case CNpcEnemy::NPC_SKELETAL_FISH:
		{
			enemy = new ("skeletal fish") CNpcSkeletalFishEnemy;
			break;
		}

		case CNpcEnemy::NPC_EYEBALL:
		{
			enemy = new ("eyeball") CNpcEyeballEnemy;
			break;
		}

		case CNpcEnemy::NPC_FLAMING_SKULL:
		{
			enemy = new ("flaming skull") CNpcFlamingSkullEnemy;
			break;
		}

		case CNpcEnemy::NPC_SKULL_STOMPER:
		{
			enemy = new ("skull stomper") CNpcSkullStomperEnemy;
			break;
		}

		case CNpcEnemy::NPC_BOOGER_MONSTER:
		{
			enemy = new ("booger monster") CNpcBoogerMonsterEnemy;
			break;
		}

		case CNpcEnemy::NPC_MOTHER_JELLYFISH:
		{
			enemy = new ("mother jellyfish") CNpcMotherJellyfishEnemy;
			g_bossThing = (CThing *) enemy;
			break;
		}

		case CNpcEnemy::NPC_SUB_SHARK:
		{
			enemy = new ("sub shark") CNpcSubSharkEnemy;
			g_bossThing = (CThing *) enemy;
			break;
		}

		case CNpcEnemy::NPC_PARASITIC_WORM:
		{
			enemy = new ("parasitic worm") CNpcParasiticWormEnemy;
			break;
		}

		case CNpcEnemy::NPC_FLYING_DUTCHMAN:
		{
			enemy = new ("flying dutchman") CNpcFlyingDutchmanEnemy;
			g_bossThing = (CThing *) enemy;
			break;
		}

		case CNpcEnemy::NPC_IRON_DOGFISH:
		{
			enemy = new ("iron dogfish") CNpcIronDogfishEnemy;
			g_bossThing = (CThing *) enemy;
			break;
		}

		case CNpcEnemy::NPC_DUST_DEVIL:
		{
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
			g_bossThing = (CThing *) enemy;
			break;
		}

		case CNpcEnemy::NPC_ANGLER_FISH:
		{
			enemy = new ("angler fish") CNpcAnglerFish;
			break;
		}

		case CNpcEnemy::NPC_GHOST:
		{
			enemy = new ("ghost") CNpcGhostEnemy;
			break;
		}

		case CNpcEnemy::NPC_SHELL:
		{
			enemy = new ("shell") CNpcShellEnemy;
			break;
		}

		case CNpcEnemy::NPC_SMALL_JELLYFISH_BACKGROUND:
		{
			enemy = new ("small jellyfish background") CNpcSmallJellyfishBackgroundEnemy;
			break;
		}

		case CNpcEnemy::NPC_BUTTERFLY_BACKGROUND:
		{
			enemy = new ("butterfly background") CNpcButterflyBackgroundEnemy;
			break;
		}

		case CNpcEnemy::NPC_PROJECTILE_JELLYFISH:
		{
			enemy = new ("projectile jellyfish") CNpcSmallJellyfishProjectileEnemy;
			break;
		}

		default:
		{
			SYSTEM_DBGMSG("UNKNOWN %i\n",enemyType);
			enemy = new ("npc enemy") CNpcEnemy;
			ASSERT(0);
			break;
		}
	}

	ASSERT(enemy);

	enemy->setType( (NPC_UNIT_TYPE)enemyType );
	enemy->setThingSubType(enemyType);

	enemy->init();

	return(enemy);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CNpcEnemy	*CNpcEnemy::Create(sThingActor *ThisActor)
{
CNpcEnemy *enemy;

	NPC_UNIT_TYPE enemyType = CNpcEnemy::getTypeFromMapEdit( ThisActor->Type );

	enemy = (CNpcEnemy*)Create(enemyType);

	return(enemy);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcEnemy::setupWaypoints( sThingActor *ThisActor )
{
	u16	*PntList=(u16*)MakePtr(ThisActor,sizeof(sThingActor));

	u16 newXPos, newYPos;
	u16 origXPos, origYPos;

	m_npcPath.setWaypointCount( ThisActor->PointCount - 1 );

	origXPos = newXPos = (u16) *PntList;
	setWaypointPtr( PntList );
	PntList++;
	origYPos = newYPos = (u16) *PntList;
	PntList++;

	setStartPos( newXPos, newYPos );

	if ( ThisActor->PointCount > 1 )
	{
		newXPos = (u16) *PntList;
		PntList++;
		newYPos = (u16) *PntList;
		PntList++;

		setHeading( newXPos, newYPos, origXPos, origYPos );
	}

	setThinkArea();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcEnemy::setThinkArea()
{
	s32 minX, maxX, minY, maxY;

	m_npcPath.getPathXExtents( &minX, &maxX );
	m_npcPath.getPathYExtents( &minY, &maxY );

	m_thinkArea.x1 = minX;
	m_thinkArea.x2 = maxX;
	m_thinkArea.y1 = minY;
	m_thinkArea.y2 = maxY;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcEnemy::setStartPos( s32 xPos, s32 yPos )
{
	Pos.vx = ( xPos << 4 ) + 8;
	Pos.vy = ( yPos << 4 ) + 16;

	m_initPos = m_base = Pos;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcEnemy::setStartPosHighRes( s32 xPos, s32 yPos )
{
	Pos.vx = xPos;
	Pos.vy = yPos;

	m_initPos = m_base = Pos;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcEnemy::setHeading( s32 xPos, s32 yPos, s32 xOrig, s32 yOrig )
{
	m_heading = ( ratan2( yPos - yOrig, xPos - xOrig ) ) & 4095;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcEnemy::init()
{
	CEnemyThing::init();
	if (m_data[m_type].skelType)
	{
		m_actorGfx=CActorPool::GetActor((FileEquate)m_data[m_type].skelType);
	}
	else
	{
		m_actorGfx=0;
	}

	m_animPlaying = true;
	m_animNo = m_data[m_type].initAnim;
	m_frame = 0;

	m_movementTimer = 0;
	m_timerTimer = 0;
	m_velocity = 0;
	m_extension = 0;
	m_reversed = false;
	m_isActive = true;
	m_isDying = false;

	m_health = m_data[this->m_type].initHealth;

	m_extendDir = EXTEND_RIGHT;

	m_timerFunc = m_data[this->m_type].timerFunc;
	m_sensorFunc = m_data[this->m_type].sensorFunc;
	m_movementFunc = m_data[this->m_type].movementFunc;

	m_controlFunc = NPC_CONTROL_MOVEMENT;

	m_npcPath.initPath();

	m_drawRotation = 0;
	m_isCaught = false;
	m_isBlowerOn = false;
	m_speed = m_data[m_type].speed;
	m_heading = 0;
	m_RGB = 0;

	m_soundId = (int) NOT_PLAYING;

	updateCollisionArea();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcEnemy::setSpeed( s16 newSpeed )
{
	switch( m_type )
	{
		case CNpcEnemy::NPC_MOTHER_JELLYFISH:
		case CNpcEnemy::NPC_SUB_SHARK:
		case CNpcEnemy::NPC_FLYING_DUTCHMAN:
		case CNpcEnemy::NPC_IRON_DOGFISH:
		case CNpcEnemy::NPC_SEA_SNAKE:
		{
			if ( CLevel::getIsBossRespawn() )
			{
				break;
			}
			else
			{
				m_speed = newSpeed;
				break;
			}
		}

		default:
		{
			m_speed = newSpeed;
			break;
		}
	}
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

	m_movementTimer = 0;
	m_timerTimer = 0;
	m_velocity = 0;
	m_extension = 0;
	m_reversed = false;
	m_isActive = true;
	m_isDying = false;

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
	if ( m_soundId != NOT_PLAYING )
	{
		CSoundMediator::stopAndUnlockSfx( (xmPlayingId) m_soundId );
	}

	if (m_actorGfx)	delete m_actorGfx;

	CEnemyThing::shutdown();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcEnemy::leftThinkZone(int _frames)
{
	if ( m_soundId != NOT_PLAYING )
	{
		CSoundMediator::stopAndUnlockSfx( (xmPlayingId) m_soundId );
		m_soundId = NOT_PLAYING;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int CNpcEnemy::getFrameCount()
{
	return( m_actorGfx->getFrameCount( m_animNo ) );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcEnemy::processGraphicFlipping()
{
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

s32 CNpcEnemy::getFrameShift( int _frames )
{
	return( ( _frames << 8 ) >> 1 );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcEnemy::processAnimFrames( int _frames )
{
	if ( m_animPlaying && ( ( m_isDying && m_data[m_type].playDeathAnim ) || !m_isDying ) )
	{
		s32 frameCount;

		frameCount = getFrameCount();

		s32 frameShift = getFrameShift( _frames );

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

	if ( m_soundId != NOT_PLAYING )
	{
		if( !CSoundMediator::isSfxStillPlaying( (xmPlayingId) m_soundId ) )
		{
			// unlock sound if it has finished

			CSoundMediator::stopAndUnlockSfx( (xmPlayingId) m_soundId );
			m_soundId = NOT_PLAYING;
		}
	}

	if ( m_isCaught )
	{
		processCoralBlower( moveFrames );
	}
	else
	{
		if ( m_isActive )
		{
			processAnimFrames( moveFrames );

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
					processShot( moveFrames );

					break;

				case NPC_CONTROL_CLOSE:
					processClose( moveFrames );

					break;

				case NPC_CONTROL_COLLISION:
					processCollision();

					break;
			}

			processGraphicFlipping();
		}
	}

	if ( !m_isCaught )
	{
		processTimer( moveFrames );
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcEnemy::processAttackCollision()
{
	//if ( m_controlFunc == NPC_CONTROL_CLOSE && m_data[m_type].closeFunc != NPC_CLOSE_NONE )
	if ( m_controlFunc == NPC_CONTROL_CLOSE )
	{
		// only detect collision if in attack mode

		m_oldControlFunc = m_controlFunc;
		m_controlFunc = NPC_CONTROL_COLLISION;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcEnemy::drawAttackEffect()
{
	
CRECT const	&rect = getCollisionArea();

	DVECTOR thwakPos;

	s32 xDist;

	CPlayer *player = GameScene.getPlayer();
	DVECTOR const &playerPos = player->getPos();

	xDist = playerPos.vx - this->Pos.vx;

	if ( xDist > 0 )
	{
		thwakPos.vx = rect.x2;

		if ( rect.x1 > thwakPos.vx )
		{
			thwakPos.vx = rect.x1;
		}
	}
	else
	{
		thwakPos.vx = rect.x1;

		if ( rect.x2 < thwakPos.vx )
		{
			thwakPos.vx = rect.x2;
		}
	}

	thwakPos.vy = ( rect.y1 + rect.y2 ) >> 1;

	CFX::Create( CFX::FX_TYPE_THWACK, thwakPos );
	CPadVibrationManager::setVibration(0,CPadVibrationManager::VIBE_CHOP);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcEnemy::collidedWith( CThing *_thisThing )
{
	if ( m_isActive && !m_isCaught && !m_isDying )
	{
		switch(_thisThing->getThingType())
		{
			case TYPE_PLAYER:
			{
				CPlayer *player = (CPlayer *) _thisThing;

				ATTACK_STATE playerState = player->getAttackState();

				if(playerState==ATTACK_STATE__NONE)
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
								if ( m_controlFunc != NPC_CONTROL_COLLISION )
								{
									m_oldControlFunc = m_controlFunc;
									m_controlFunc = NPC_CONTROL_COLLISION;
								}

								processUserCollision( _thisThing );

								break;
							}

							case DETECT_ATTACK_COLLISION_GENERIC:
							{
								processAttackCollision();
								processUserCollision( _thisThing );

								break;
							}
						}
					}
				}
				else
				{
					// player is attacking, respond appropriately

					if ( m_controlFunc != NPC_CONTROL_SHOT )
					{
						if(playerState==ATTACK_STATE__BUTT_BOUNCE)
						{
							player->justButtBouncedABadGuy();
						}
						m_controlFunc = NPC_CONTROL_SHOT;
						m_state = NPC_GENERIC_HIT_CHECK_HEALTH;

						drawAttackEffect();
					}
				}

				break;
			}

			case TYPE_ENEMY:
			{
				CNpcEnemy *enemy = (CNpcEnemy *) _thisThing;

				if ( canCollideWithEnemy() && enemy->canCollideWithEnemy() )
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
							m_velocity = m_speed;

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
								s16 headingToPlayer = ratan2( playerYDist, playerXDist ) & 4095;

								s16 decDir, incDir, moveDist;

								s32 headingToWaypoint = ratan2( yDistWaypoint, xDistWaypoint ) & 4095;

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

	if ( m_soundId == NOT_PLAYING && m_data[m_type].moveSfx < CSoundMediator::NUM_SFXIDS )
	{
		m_soundId = (int) CSoundMediator::playSfx( m_data[m_type].moveSfx, true );
	}

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

u8 CNpcEnemy::hasBeenAttacked()
{
	if ( m_controlFunc != NPC_CONTROL_SHOT )
	{
		m_controlFunc = NPC_CONTROL_SHOT;
		m_state = NPC_GENERIC_HIT_CHECK_HEALTH;
	}

	return( true );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcEnemy::processShot( int _frames )
{
	switch( m_data[m_type].shotFunc )
	{
		case NPC_SHOT_NONE:
		{
			// do nothing
			m_controlFunc = m_oldControlFunc;

			break;
		}

		case NPC_SHOT_GENERIC:
		{
			switch ( m_state )
			{
				case NPC_GENERIC_HIT_CHECK_HEALTH:
				{
					m_health -= 3;

					if ( m_health <= 0 )
					{
						m_state = NPC_GENERIC_HIT_DEATH_START;
						m_isDying = true;
						m_health = 0;
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
					processShotRecoil( _frames );

					break;
				}

				case NPC_GENERIC_HIT_DEATH_START:
				{
					processShotDeathStart( _frames );

					break;
				}

				case NPC_GENERIC_HIT_DEATH_END:
				{
					processShotDeathEnd( _frames );

					break;
				}
			}

			break;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcEnemy::processShotRecoil( int _frames )
{
	if ( !m_animPlaying )
	{
		m_state = 0;
		m_controlFunc = NPC_CONTROL_MOVEMENT;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcEnemy::processShotDeathStart( int _frames )
{
	m_animPlaying = true;
	m_animNo = m_data[m_type].dieAnim;
	m_frame = 0;
	m_state = NPC_GENERIC_HIT_DEATH_END;
	m_isDying = true;

	if ( m_data[m_type].deathSfx < CSoundMediator::NUM_SFXIDS )
	{
		if( m_soundId != NOT_PLAYING )
		{
			CSoundMediator::stopAndUnlockSfx( (xmPlayingId) m_soundId );
		}

		m_soundId = (int) CSoundMediator::playSfx( m_data[m_type].deathSfx, true );
	}

	m_speed = -5;

	if (m_data[m_type].skelType)
	{
		m_actorGfx->SetOtPos( 0 );
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcEnemy::processShotDeathEnd( int _frames )
{
	m_drawRotation += 64 * _frames;
	m_drawRotation &= 4095;

	Pos.vy += m_speed * _frames;

	if ( m_speed < 5 )
	{
		m_speed++;
	}

	if ( !canRender() )
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
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcEnemy::processClose(int _frames)
{
	switch( m_data[this->m_type].closeFunc )
	{
		case NPC_CLOSE_GENERIC_USER_SEEK:
		{
			processGenericGotoTarget( _frames, playerXDist, playerYDist, m_speed );

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

	player->takeDamage( m_data[m_type].damageToUserType,REACT__GET_DIRECTION_FROM_THING,(CThing*)this );
	processUserCollision( (CThing *) player );

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

		if (canRender())
		{
			DVECTOR &renderPos=getRenderPos();

			SprFrame = m_actorGfx->Render(renderPos,m_animNo,( m_frame >> 8 ),m_reversed);
			m_actorGfx->RotateScale( SprFrame, renderPos, m_drawRotation, 4096, 4096 );

			sBBox boundingBox = m_actorGfx->GetBBox();
			setCollisionSize( ( boundingBox.XMax - boundingBox.XMin ), ( boundingBox.YMax - boundingBox.YMin ) );
			setCollisionCentreOffset( ( boundingBox.XMax + boundingBox.XMin ) >> 1, ( boundingBox.YMax + boundingBox.YMin ) >> 1 );
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

		case BOSS_FOUND_EVENT:
		{
			addHealthMeter();

			break;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

u8 CNpcEnemy::canBeCaughtByNet()
{
	return( m_isActive && !m_isDying && m_data[m_type].canBeNetted );
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
	return( m_isActive && !m_isDying );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcEnemy::processEnemyCollision( CThing *thisThing )
{
	DVECTOR const &otherPos = thisThing->getPos();
	DVECTOR			otherDelta = thisThing->getPosDelta();

	s32 xDist = Pos.vx - otherPos.vx;
	s32 yDist = Pos.vy - otherPos.vy;

	s16 headingFromTarget = ratan2( yDist, xDist ) & 4095;

	if ( ( xDist > 0 && otherDelta.vx < 0 ) || ( xDist < 0 && otherDelta.vx > 0 ) )
	{
		otherDelta.vx = -otherDelta.vx;
	}

	if ( ( yDist > 0 && otherDelta.vy < 0 ) || ( yDist < 0 && otherDelta.vy > 0 ) )
	{
		otherDelta.vy = -otherDelta.vy;
	}

	Pos.vx += otherDelta.vx;

	if ( m_npcPath.getWaypointCount() )
	{
		s32 minX, maxX;
		m_npcPath.getPathXExtents( &minX, &maxX );

		if ( Pos.vx < minX )
		{
			Pos.vx = minX;
		}
		else if ( Pos.vx > maxX )
		{
			Pos.vx = maxX;
		}
	}

	s32 groundHeight = CGameScene::getCollision()->getHeightFromGround( Pos.vx, Pos.vy, 16 );

	if ( groundHeight < 8 )
	{
		Pos.vy += groundHeight;
	}
	else
	{
		Pos.vy += otherDelta.vy;
	}

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

void CNpcEnemy::processUserCollision( CThing *thisThing )
{
	DVECTOR const &otherPos = thisThing->getPos();
	DVECTOR const &otherDelta = thisThing->getPosDelta();

	s32 xDist = Pos.vx - otherPos.vx;
	s32 yDist = Pos.vy - otherPos.vy;

	s16 headingFromTarget = ratan2( yDist, xDist ) & 4095;

	/*if ( ( xDist > 0 && otherDelta.vx < 0 ) || ( xDist < 0 && otherDelta.vx > 0 ) )
	{
		otherDelta.vx = -otherDelta.vx;
	}

	if ( ( yDist > 0 && otherDelta.vy < 0 ) || ( yDist < 0 && otherDelta.vy > 0 ) )
	{
		otherDelta.vy = -otherDelta.vy;
	}

	Pos.vx += otherDelta.vx;

	s32 groundHeight = CGameScene::getCollision()->getHeightFromGround( Pos.vx, Pos.vy, 16 );

	if ( groundHeight < 8 )
	{
		Pos.vy += groundHeight;
	}
	else
	{
		Pos.vy += otherDelta.vy;
	}*/

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

bool CNpcEnemy::processCoralBlowerMovement( int _frames, s32 xDist, s32 yDist, u8 destroyAtTarget )
{
	s32 moveX, moveY;
	s16 headingToTarget;

	headingToTarget = ratan2( yDist, xDist ) & 4095;

	s32 preShiftX = _frames * 6 * rcos( headingToTarget );
	s32 preShiftY = _frames * 6 * rsin( headingToTarget );

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
		// has reached target point

		if ( destroyAtTarget )
		{
			if ( m_data[m_type].respawning )
			{
				if ( m_isActive )
				{
					m_isCaught = false;
					m_isActive = false;

					m_timerFunc = NPC_TIMER_RESPAWN;
					m_timerTimer = 1 * GameState::getOneSecondInFrames();
				}
			}
			else
			{
				setToShutdown();
			}
		}

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

			returnVal = processCoralBlowerMovement( _frames, targetXDist, targetYDist, true );

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

			processCoralBlowerMovement( _frames, targetXDist, targetYDist, false );

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
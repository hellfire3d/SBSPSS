/*=========================================================================

	platform.cpp

	Author:		CRB
	Created:
	Project:	Spongebob
	Purpose:

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/

#include "platform\platform.h"

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

#ifndef __FRIEND_FRIEND_H__
#include "friend\friend.h"
#endif

#ifndef __HAZARD_HAZARD_H__
#include "hazard\hazard.h"
#endif

#ifndef __ENEMY_NPCPATH_H__
#include	"enemy\npcpath.h"
#endif

#ifndef	__UTILS_HEADER__
#include	"utils\utils.h"
#endif

#include "Gfx\actor.h"

#ifndef __VID_HEADER_
#include "system\vid.h"
#endif

#ifndef __PLATFORM_PLINEAR_H__
#include "platform\plinear.h"
#endif

#ifndef __PLATFORM_PCIRCULR_H__
#include "platform\pcirculr.h"
#endif

#ifndef __PLATFORM_PBUBBLE_H__
#include "platform\pbubble.h"
#endif

#ifndef __PLATFORM_PCBUBBLE_H__
#include "platform\pcbubble.h"
#endif

#ifndef __PLATFORM_PFISHHK_H__
#include "platform\pfishhk.h"
#endif

#ifndef __PLATFORM_PRETRACT_H__
#include "platform\pretract.h"
#endif

#ifndef __PLATFORM_PGEYSER_H__
#include "platform\pgeyser.h"
#endif

#ifndef __PLATFORM_PBOB_H__
#include "platform\pbob.h"
#endif

#ifndef __PLATFORM_PFALLING_H__
#include "platform\pfalling.h"
#endif

#ifndef __PLATFORM_PPENDULM_H__
#include "platform\ppendulm.h"
#endif

#ifndef __PLATFORM_PBRANCH_H__
#include "platform\pbranch.h"
#endif

#ifndef __PLATFORM_PCART_H__
#include "platform\pcart.h"
#endif

#ifndef __PLATFORM_PSEESAW_H__
#include "platform\pseesaw.h"
#endif

#ifndef __PLATFORM_PBOUNCE_H__
#include "platform\pbounce.h"
#endif

#ifndef __PLATFORM_PDUAL_H__
#include "platform\pdual.h"
#endif

#ifndef __PLATFORM_PPLAYER_H__
#include "platform\pplayer.h"
#endif

#ifndef __PLATFORM_PFGEN_H__
#include "platform\pfgen.h"
#endif

#ifndef __PLATFORM_PRAFT_H__
#include "platform\praft.h"
#endif

#ifndef __PLATFORM_PLANTERN_H__
#include "platform\plantern.h"
#endif

#ifndef __PLATFORM_PBGEYSER_H__
#include "platform\pbgeyser.h"
#endif

#ifndef __PLATFORM_PLEAF_H__
#include "platform\pleaf.h"
#endif

#ifndef __PLATFORM_PBWHEEL_H__
#include "platform\pbwheel.h"
#endif

#ifndef __PLATFORM_PSBARREL_H__
#include "platform\psbarrel.h"
#endif

#ifndef __PLATFORM_PJELLFSH_H__
#include "platform\pjellfsh.h"
#endif

#ifndef __PLATFORM_PFISHHK3_H__
#include "platform\pfishhk3.h"
#endif

#ifndef __PLATFORM_PRBRIDGE_H__
#include "platform\prbridge.h"
#endif

#ifndef __PLATFORM_PBALOON_H__
#include "platform\pbaloon.h"
#endif

#ifndef __PLATFORM_PTRPDOOR_H__
#include "platform\ptrpdoor.h"
#endif

#ifndef __PLATFORM_PCONVEYR_H__
#include "platform\pconveyr.h"
#endif

#ifndef __PLATFORM_PFALLNOR_H__
#include "platform\pfallnor.h"
#endif

#ifndef __PLATFORM_PDROP_H__
#include "platform\pdrop.h"
#endif

#ifndef __PLATFORM_PSSWITCH_H__
#include "platform\psswitch.h"
#endif

#ifndef __PLATFORM_PLIFT_H__
#include "platform\plift.h"
#endif

#ifndef __PLATFORM_PLURVE_H__
#include "platform\plurve.h"
#endif

#ifndef __PLATFORM_PSOIL_H__
#include "platform\psoil.h"
#endif

#ifndef __PLATFORM_PBUBTUBE_H__
#include "platform\pbubtube.h"
#endif

#ifndef __PLATFORM_PCLAM_H__
#include "platform\pclam.h"
#endif

#ifndef __PLATFORM_PPLAYER_H__
#include "platform\pplayer.h"
#endif

#ifndef __PLATFORM_PGBUBBLE_H__
#include "platform\pgbubble.h"
#endif

#ifndef __PLATFORM_PFBLOCK_H__
#include "platform\pfblock.h"
#endif

#ifndef __PLATFORM_PGHOST_H__
#include "platform\pghost.h"
#endif

#include "fx\fx.h"
#include "fx\fxjfish.h"


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CNpcPlatform	*CNpcPlatform::Create(int Type)
{
	CNpcPlatform *platform;

	platform = (CNpcPlatform*)CThingManager::GetThing(CThing::TYPE_PLATFORM,Type);
	if (!platform)
	switch( Type )
	{
		case NPC_LINEAR_PLATFORM:
		{
			platform = new ("linear platform") CNpcLinearPlatform;
			break;
		}

		case NPC_CIRCULAR_PLATFORM:
		{
			platform = new ("circular platform") CNpcCircularPlatform;
			break;
		}

		case NPC_BUBBLE_PLATFORM:
		{
			platform = new ("geyser bubble platform") CNpcGeyserBubblePlatform;
			break;
		}

		case NPC_COLLAPSING_BUBBLE_PLATFORM:
		{
			platform = new ("collapsing bubble platform") CNpcCollapsingBubblePlatform;
			break;
		}

		case NPC_COLLAPSING_ACRID_PLATFORM:
		{
			platform = new ("collapsing bubble platform") CNpcCollapsingAcridPlatform;
			break;
		}

		case NPC_FISH_HOOK_PLATFORM:
		{
			platform = new ("fish hook platform") CNpcFishHookPlatform;
			break;
		}

		case NPC_RETRACTING_PLATFORM:
		{
			platform = new ("retracting platform") CNpcRetractingPlatform;
			break;
		}

		case NPC_GEYSER_PLATFORM:
		{
			platform = new ("geyser platform") CNpcGeyserPlatform;
			break;
		}

		case NPC_BOBBING_PLATFORM:
		{
			platform = new ("bobbing platform") CNpcBobbingPlatform;
			break;
		}

		case NPC_OILDRUM_PLATFORM:
		case NPC_CRATE_PLATFORM:
		case NPC_FALLING_PLATFORM:
		{
			platform = new ("falling platform") CNpcFallingNoRespawnPlatform;
			break;
		}

		case NPC_FISH_HOOK_2_PLATFORM:
		{
			platform = new ("fish hook 2 platform") CNpcPendulumPlatform;
			break;
		}

		case NPC_BRANCH_PLATFORM:
		{
			platform = new ("branch platform") CNpcBranchPlatform;
			break;
		}

		case NPC_CART_PLATFORM:
		{
			//platform = new ("cart platform") CNpcCartPlatform;
			platform = new ("ghost train platform") CNpcGhostTrainPlatform;
			break;
		}

		case NPC_SEESAW_PLATFORM:
		{
			platform = new ("seesaw platform") CNpcSeesawPlatform;
			break;
		}

		case NPC_BOUNCE_PLATFORM:
		{
			platform = new ("bounce platform") CNpcBouncePlatform;
			break;
		}

		case NPC_DUAL_PLATFORM:
		{
			platform = new ("dual platform master") CNpcDualPlatform;
			break;
		}

		case NPC_OILDRUM_GENERATOR:
		{
			CNpcFallingPlatformGenerator *generator;
			generator = new ("oildrum generator") CNpcFallingPlatformGenerator;
			generator->setTargetType( NPC_OILDRUM_PLATFORM );
			platform = generator;
			break;
		}

		case NPC_CRATE_GENERATOR:
		{
			CNpcFallingPlatformGenerator *generator;
			generator = new ("crate generator") CNpcFallingPlatformGenerator;
			generator->setTargetType( NPC_CRATE_PLATFORM );
			platform = generator;
			break;
		}

		case NPC_RAFT_PLATFORM:
		{
			platform = new ("raft platform") CNpcRaftPlatform;
			break;
		}

		case NPC_VERTICAL_OILDRUM_GENERATOR:
		{
			CNpcFallingPlatformGenerator *generator;
			generator = new ("vertical oildrum generator") CNpcFallingPlatformGenerator;
			generator->setTargetType( NPC_VERTICAL_OILDRUM_PLATFORM );
			platform = generator;
			break;
		}

		case NPC_BUBBLE_GEYSER_GENERATOR:
		{
			CNpcGeyserPlatformGenerator *generator;
			generator = new ("bubble geyser generator") CNpcGeyserPlatformGenerator;
			generator->setTargetType( NPC_BUBBLE_PLATFORM );
			platform = generator;
			break;
		}

		case NPC_LANTERN_PLATFORM:
		{
			platform = new ("lantern platform") CNpcLanternPlatform;
			break;
		}

		case NPC_LEAF_PLATFORM:
		{
			platform = new ("leaf platform") CNpcLeafPlatform;
			break;
		}

		case NPC_BIG_WHEEL_PLATFORM:
		{
			platform = new ("big wheel platform") CNpcBigWheelPlatform;
			break;
		}

		case NPC_STEERABLE_BARREL_PLATFORM:
		{
			platform = new ("steerable barrel platform") CNpcSteerableBarrelPlatform;
			break;
		}

		case NPC_JELLYFISH_PLATFORM:
		{
			platform = new ("jellyfish platform") CNpcJellyfishPlatform;
			break;
		}

		case NPC_FISH_HOOK_3_PLATFORM:
		{
			platform = new ("fish hook 3 platform") CNpcFishHook3Platform;
			break;
		}

		case NPC_RISING_BRIDGE_PLATFORM:
		{
			platform = new ("rising bridge platform") CNpcRisingBridgePlatform;
			break;
		}

		case NPC_BALLOON_BRIDGE_PLATFORM:
		{
			platform = new ("balloon bridge platform") CNpcBalloonBridgePlatform;
			break;
		}

		case NPC_TRAPDOOR_PLATFORM:
		{
			platform = new ("trapdoor platform") CNpcTrapdoorPlatform;
			break;
		}

		case NPC_CONVEYOR_GENERATOR:
		{
			platform = new ("conveyor generator") CNpcConveyorPlatformGenerator;
			break;
		}

		case NPC_CONVEYOR_PLATFORM:
		{
			platform = new ("conveyor platform") CNpcConveyorPlatform;
			break;
		}

		case NPC_DROP_PLATFORM:
		{
			platform = new ("drop platform") CNpcDropPlatform;
			break;
		}

		case NPC_STEAM_SWITCH_PLATFORM:
		{
			platform = new ("steam switch platform") CNpcSteamSwitchPlatform;
			break;
		}

		case NPC_LIFT_PLATFORM:
		{
			platform = new ("lift platform") CNpcLiftPlatform;
			break;
		}

		case NPC_LOVE_BOAT_PLATFORM:
		{
			platform = new ("love boat platform") CNpcLoveBoatPlatform;
			break;
		}

		case NPC_STEERABLE_OILDRUM_PLATFORM:
		{
			platform = new ("steerable oildrum platform") CNpcSteerableOildrumPlatform;
			break;
		}

		case NPC_BUBBLE_TUBE_PLATFORM:
		{
			platform = new ("bubble tube platform") CNpcBubbleTubePlatform;
			break;
		}

		case NPC_FALLING_BLOCK_PLATFORM:
		{
			platform = new ("falling block platform") CNpcFallingBlockPlatform;
			break;
		}

		case NPC_GHOST_TRAIN_PLATFORM:
		{
			platform = new ("ghost train platform") CNpcGhostTrainPlatform;
			break;
		}

		case NPC_CLAM_PLATFORM:
		{
			platform = new ("clam platform") CNpcClamPlatform;
			break;
		}

		case NPC_PLAYER_BUBBLE_PLATFORM:
		{
			platform = new ("player bubble platform") CNpcPlayerBubblePlatform;
			break;
		}

		default:
		{
			ASSERT( 0 );
			platform = new ("platform") CNpcPlatform;
			break;
		}
	}

	ASSERT(platform);
	platform->setThingSubType( Type );
	platform->setTiltable( false );

	return( platform );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CNpcPlatform	*CNpcPlatform::Create(sThingPlatform *ThisPlatform)
{
	CNpcPlatform *platform;

	NPC_PLATFORM_UNIT_TYPE Type = getTypeFromMapEdit( ThisPlatform->Type );

	platform=Create(Type);

	switch( Type )
	{
		case NPC_DUAL_PLATFORM:
		{
			CNpcDualPlatform *dualPlatformMaster = (CNpcDualPlatform *) platform;
			dualPlatformMaster->setMaster( true );

			CNpcDualPlatform *dualPlatformSlave;
			dualPlatformSlave = (CNpcDualPlatform *) CNpcPlatform::Create( NPC_DUAL_PLATFORM );
			dualPlatformSlave->setMaster( false );

			dualPlatformSlave->setThingSubType( Type );
			dualPlatformSlave->setGraphic( ThisPlatform );
			dualPlatformSlave->setTiltable( false );

			dualPlatformMaster->setOtherPlatform( dualPlatformSlave );
			dualPlatformSlave->setOtherPlatform( dualPlatformMaster );

			break;
		}

		default:
			break;
	}

	ASSERT(platform);
	platform->setGraphic( ThisPlatform );
	platform->setWaypoints( ThisPlatform );

	return( platform );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcPlatform::setWaypoints( sThingPlatform *ThisPlatform )
{
	int pointNum;

	u16	*PntList=(u16*)MakePtr(ThisPlatform,sizeof(sThingPlatform));
	u16 *waypoints;

	u16 newXPos, newYPos;

	newXPos = (u16) *PntList;
	waypoints = PntList;
	PntList++;
	newYPos = (u16) *PntList;
	PntList++;

	DVECTOR startPos;
	startPos.vx = ( newXPos << 4 ) + 8;
	startPos.vy = ( newYPos << 4 ) + 16;

	init( startPos );

	setWaypointCount( ThisPlatform->PointCount - 1 );

	setWaypointPtr( waypoints );

	s32 minX, maxX, minY, maxY;

	m_npcPath.getPathXExtents( &minX, &maxX );
	m_npcPath.getPathYExtents( &minY, &maxY );

	m_thinkArea.x1 = minX;
	m_thinkArea.x2 = maxX;
	m_thinkArea.y1 = minY;
	m_thinkArea.y2 = maxY;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcPlatform::setGraphic( sThingPlatform *ThisPlatform )
{
	m_graphicNum = ThisPlatform->Gfx;
	m_modelGfx = new ("ModelGfx") CModelGfx;
	m_modelGfx->SetModel( m_graphicNum );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcPlatform::setGraphic( u8 graphicNum )
{
	m_graphicNum = graphicNum;
	m_modelGfx = new ("ModelGfx") CModelGfx;
	m_modelGfx->SetModel( graphicNum );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcPlatform::init()
{
	CPlatformThing::init();

	m_dataPtr=&m_data[getThingSubType()];

	m_heading = 0;
	m_velocity = 0;
	m_rotation = 0;
	m_reversed = false;
	m_extension = 0;
	m_contact = false;
	m_timer = m_dataPtr->initTimer * GameState::getOneSecondInFrames();
	m_timerType = m_dataPtr->initTimerType;
	m_isActive = true;
	m_detectCollision = m_dataPtr->detectCollision;
	m_state = 0;
	m_tiltAngle = 0;
	m_tiltVelocity = 0;
	m_tiltable = false;
	m_initRotation = 0;

	m_lifetime = 0;
	m_lifetimeType = m_dataPtr->lifetimeType;

	m_npcPath.initPath();

	m_speed = m_dataPtr->speed;

	m_soundId = (int) NOT_PLAYING;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcPlatform::init( DVECTOR initPos )
{
	init();

	Pos = m_initPos = m_base = initPos;

	m_initLifetime = m_lifetime = GameState::getOneSecondInFrames() * m_dataPtr->lifetime;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcPlatform::init( DVECTOR initPos, s32 initLifetime )
{
	init( initPos );

	m_initLifetime = m_lifetime = GameState::getOneSecondInFrames() * initLifetime;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcPlatform::reinit()
{
	m_heading = 0;
	m_velocity = 0;
	m_rotation = 0;
	m_reversed = false;
	m_contact = false;
	m_timer = m_dataPtr->initTimer * GameState::getOneSecondInFrames();
	m_timerType = m_dataPtr->initTimerType;
	m_isActive = true;
	m_detectCollision = m_dataPtr->detectCollision;
	m_state = 0;
	m_tiltAngle = 0;
	m_tiltVelocity = 0;

	m_lifetime = m_initLifetime;

	Pos = m_initPos;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcPlatform::calculateNonRotatedCollisionData()
{
DVECTOR const	&collisionSize = getCollisionSize();
DVECTOR const	&collisionOffset = getCollisionCentreOffset();

	m_nonRotatedCollisionArea.XMax = ( collisionSize.vx >> 1 ) + collisionOffset.vx;
	m_nonRotatedCollisionArea.XMin = -( collisionSize.vx >> 1 ) + collisionOffset.vx;
	m_nonRotatedCollisionArea.YMax = ( collisionSize.vy >> 1 ) + collisionOffset.vy;
	m_nonRotatedCollisionArea.YMin = -( collisionSize.vy >> 1 ) + collisionOffset.vy;

	m_nonRotatedCollisionOffset = collisionOffset;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcPlatform::postInit()
{
	sBBox boundingBox = m_modelGfx->GetBBox();
	setCollisionSize( ( boundingBox.XMax - boundingBox.XMin ), ( boundingBox.YMax - boundingBox.YMin ) );
	setCollisionCentreOffset( ( boundingBox.XMax + boundingBox.XMin ) >> 1, ( boundingBox.YMax + boundingBox.YMin ) >> 1 );

	calculateNonRotatedCollisionData();
	setCollisionAngle( m_tiltAngle >> 8 );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcPlatform::shutdown()
{
	if ( m_soundId != NOT_PLAYING )
	{
		CSoundMediator::stopAndUnlockSfx( (xmPlayingId) m_soundId );
	}

	delete m_modelGfx;

	// temporary
	CPlatformThing::shutdown();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcPlatform::leftThinkZone(int _frames)
{
	if ( m_soundId != NOT_PLAYING )
	{
		CSoundMediator::stopAndUnlockSfx( (xmPlayingId) m_soundId );
		m_soundId = NOT_PLAYING;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcPlatform::processLifetime( int _frames )
{
	switch( m_lifetimeType )
	{
		case NPC_PLATFORM_FINITE_LIFE:
		{
			m_lifetime -= _frames;

			if ( m_lifetime <= 0 )
			{
				setToShutdown();

				return;
			}

			break;
		}

		case NPC_PLATFORM_FINITE_LIFE_RESPAWN:
		{
			m_lifetime -= _frames;

			if ( m_lifetime <= 0 )
			{
				reinit();
			}

			break;
		}

		case NPC_PLATFORM_INFINITE_LIFE_COLLAPSIBLE:
		{
			if ( m_contact )
			{
				m_lifetime -= _frames;

				if ( m_lifetime <= 0 )
				{
					m_isActive = false;
					m_timer = 3 * GameState::getOneSecondInFrames();
					m_timerType = NPC_PLATFORM_TIMER_RESPAWN;
				}
			}

			break;
		}

		default:
			break;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcPlatform::think(int _frames)
{
	if ( m_soundId != NOT_PLAYING )
	{
		if( !CSoundMediator::isSfxStillPlaying( (xmPlayingId) m_soundId ) )
		{
			// unlock sound if it has finished

			CSoundMediator::stopAndUnlockSfx( (xmPlayingId) m_soundId );
			m_soundId = NOT_PLAYING;
		}
	}

	if ( m_isActive )
	{
		if ( m_tiltable )
		{
			processTilt( _frames );
		}

		processLifetime( _frames );

		if ( m_heading > 1024 && m_heading < 3072 )
		{
			m_reversed = true;
		}
		else
		{
			m_reversed = false;
		}

		int moveFrames = _frames;

		if ( moveFrames > 2 )
		{
			// make sure platforms don't go berserk if too many frames are dropped

			moveFrames = 2;
		}

		processMovement( moveFrames );

		m_contact = false;
	}

	processTimer( _frames );

	CPlatformThing::think(_frames);
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcPlatform::setCollisionAngle(int newAngle)
{
	m_collisionAngle=newAngle;
//	CPlatformThing::setCollisionAngle(newAngle);
	calculateBoundingBoxSize();

	CPlayer	*player;

	// Is the player stood on this platform as it rotates?
	player=GameScene.getPlayer();
	if(player->isOnPlatform()==this)
	{
		DVECTOR	const &playerPos=player->getPos();

		DVECTOR shove;
		shove.vx = 0;
		shove.vy = getHeightFromPlatformAtPosition( playerPos.vx, playerPos.vy );
		player->shove(shove);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcPlatform::calculateBoundingBoxSize()
{
	sBBox boundingBox = m_nonRotatedCollisionArea;

	// 'render' collision box at correct angle

	SVECTOR testPointsNonRel[4];
	VECTOR testPoints[4];

	testPointsNonRel[0].vx = boundingBox.XMin;
	testPointsNonRel[0].vy = boundingBox.YMin;

	testPointsNonRel[1].vx = boundingBox.XMax;
	testPointsNonRel[1].vy = boundingBox.YMin;

	testPointsNonRel[2].vx = boundingBox.XMax;
	testPointsNonRel[2].vy = boundingBox.YMax;

	testPointsNonRel[3].vx = boundingBox.XMin;
	testPointsNonRel[3].vy = boundingBox.YMax;

	MATRIX mtx;
	SetIdentNoTrans(&mtx );
	RotMatrixZ( getCollisionAngle() + m_initRotation, &mtx );

	int i;

	for ( i = 0 ; i < 4 ; i++ )
	{
		ApplyMatrix( &mtx, &testPointsNonRel[i], &testPoints[i] );
	}

	SVECTOR offsetSource;
	VECTOR offsetTarget;

	offsetSource.vx = m_nonRotatedCollisionOffset.vx;
	offsetSource.vy = m_nonRotatedCollisionOffset.vy;

	ApplyMatrix( &mtx, &offsetSource, &offsetTarget );

	int x1, x2, y1, y2;

	x1 = x2 = testPoints[0].vx;
	y1 = y2 = testPoints[0].vy;

	for ( i = 1 ; i < 4 ; i++ )
	{
		if ( testPoints[i].vy < y1 )
		{
			y1 = testPoints[i].vy;
		}
		else if ( testPoints[i].vy > y2 )
		{
			y2 = testPoints[i].vy;
		}

		if ( testPoints[i].vx < x1 )
		{
			x1 = testPoints[i].vx;
		}
		else if ( testPoints[i].vx > x2 )
		{
			x2 = testPoints[i].vx;
		}
	}

	setCollisionSize( x2 - x1 + 1, y2 - y1 + 1 );

	setCollisionCentreOffset( offsetTarget.vx, offsetTarget.vy );
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcPlatform::processTilt( int _frames )
{
	bool forceActing = false;

	if ( m_contact )
	{
		// user is touching platform, tilt accordingly

		CPlayer *player = GameScene.getPlayer();
		DVECTOR const &playerPos = player->getPos();

		if ( playerPos.vx > Pos.vx + 10 )
		{
			forceActing = true;

			// tilt clockwise

			if ( m_tiltVelocity < 2560 )
			{
				m_tiltVelocity += 64;
			}
		}
		else if ( playerPos.vx < Pos.vx - 10 )
		{
			forceActing = true;

			// tilt anticlockwise

			if ( m_tiltVelocity > -2560 )
			{
				m_tiltVelocity -= 64;
			}
		}
	}

	if ( !forceActing )
	{
		// no force acting, hence reduce velocity

		s32 reduction = abs( m_tiltVelocity );

		if ( reduction > 64 )
		{
			reduction = 64;
		}

		if ( m_tiltVelocity >= 0 )
		{
			reduction *= -1;
		}

		m_tiltVelocity += reduction;
	}

	m_tiltAngle += m_tiltVelocity;

	if ( m_tiltAngle > ( 512 << 8 ) )
	{
		m_tiltAngle = ( 512 << 8 );
		m_tiltVelocity = 0;
	}
	else if ( m_tiltAngle < -( 512 << 8 ) )
	{
		m_tiltAngle = -( 512 << 8 );
		m_tiltVelocity = 0;
	}

	setCollisionAngle( m_tiltAngle >> 8 );

	/*if ( getCollisionAngle() > 512 && getCollisionAngle() < 3584 )
	{
		m_detectCollision = false;
	}
	else
	{
		m_detectCollision = true;
	}*/
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcPlatform::processTimer( int _frames )
{
	switch( m_timerType )
	{
		case NPC_PLATFORM_TIMER_NONE:
			break;

		case NPC_PLATFORM_TIMER_RESPAWN:
		{
			if ( m_timer > 0 )
			{
				m_timer -= _frames;
			}
			else
			{
				reinit();
			}

			break;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

u8 CNpcPlatform::checkCollisionDelta( CThing *_thisThing, int threshold, CRECT collisionArea )
{
	// check for case of "was not colliding previously because was above" +
	// "is not colliding now because is below"

	DVECTOR	const &otherPos = _thisThing->getPos();

	if ( getHeightFromPlatformAtPosition( otherPos.vx, otherPos.vy ) < 0 )
	{
		// is currently below platform landing point

		DVECTOR const &otherPosDelta = _thisThing->getPosDelta();
		DVECTOR const &posDelta = getPosDelta();

		if ( otherPosDelta.vy > 0 || posDelta.vy < 0 )
		{
			// object is dropping vertically, or platform is rising vertically

			// get both object's previous positions

			s32 oldXPos = otherPos.vx - otherPosDelta.vx;
			s32 oldYPos = otherPos.vy - otherPosDelta.vy;

			CRECT otherCollisionArea = _thisThing->getCollisionArea();
			otherCollisionArea.x1 -= otherPosDelta.vx;
			otherCollisionArea.x2 -= otherPosDelta.vx;
			otherCollisionArea.y1 -= otherPosDelta.vy;
			otherCollisionArea.y2 -= otherPosDelta.vy;

			collisionArea.x1 -= posDelta.vx;
			collisionArea.x2 -= posDelta.vx;
			collisionArea.y1 -= posDelta.vy;
			s32 oldCollisionY = collisionArea.y2;
			collisionArea.y2 -= posDelta.vy;

			//if ( oldXPos >= collisionArea.x1 && oldXPos <= collisionArea.x2 )
			if ( otherCollisionArea.x2 >= collisionArea.x1 && otherCollisionArea.x1 <= collisionArea.x2 )
			{
				//if ( oldYPos < collisionArea.y1 + threshold ) //&& otherPos.vy > oldCollisionY )
				if ( getHeightFromPlatformAtPosition( oldXPos, oldYPos, -posDelta.vx, -posDelta.vy ) >= 0 )
				{
					// if object's old position was above old platform collision area,
					// but is now below current platform collision area, we can assume
					// the player has hit the platform in the intervening time

					return( true );
				}
			}
		}
	}

	return( false );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcPlatform::collidedWith( CThing *_thisThing )
{
	switch(_thisThing->getThingType())
	{
		case TYPE_PLAYER:
		{
			CPlayer *player;
//			DVECTOR	playerPos;
//			CRECT	collisionArea;
//			CRECT	playerCollisionArea;

			// Only interested in SBs feet colliding with the box (pkg)
			player=(CPlayer*)_thisThing;
DVECTOR const &playerPos=player->getPos();
CRECT const &playerCollisionArea = player->getCollisionArea();
CRECT const &collisionArea=getCollisionArea();

			s32 threshold = abs( collisionArea.y2 - collisionArea.y1 );

			if ( threshold > 16 )
			{
				threshold = 16;
			}

			if( playerCollisionArea.x2 >= collisionArea.x1 && playerCollisionArea.x1 <= collisionArea.x2 )
			{
				if ( checkCollisionDelta( _thisThing, threshold, collisionArea ) )
				{
					player->setPlatform( this );

					m_contact = true;
				}
				else
				{
					if( playerPos.vy >= collisionArea.y1 && playerPos.vy <= collisionArea.y2 )
					{
						int height = getHeightFromPlatformAtPosition( playerPos.vx, playerPos.vy );

						if ( height >= -threshold && height < 1 )
						{
							player->setPlatform( this );

							m_contact = true;
						}
					}
				}
			}

			break;
		}

		case TYPE_NPC:
		{
			CNpcFriend *friendNpc;
//			DVECTOR	friendPos;
//			CRECT	collisionArea;

			friendNpc = (CNpcFriend*) _thisThing;
DVECTOR const &friendPos = friendNpc->getPos();
CRECT const &friendCollisionArea = friendNpc->getCollisionArea();
CRECT const &collisionArea=getCollisionArea();

			s32 threshold = abs( collisionArea.y2 - collisionArea.y1 );

			if ( threshold > 16 )
			{
				threshold = 16;
			}

			if( friendCollisionArea.x2 >= collisionArea.x1 && friendCollisionArea.x1 <= collisionArea.x2 )
			{
				if ( checkCollisionDelta( _thisThing, threshold, collisionArea ) )
				{
					int height = getHeightFromPlatformAtPosition( friendPos.vx, friendPos.vy );

					friendNpc->setPlatform( this );
					DVECTOR friendPos = friendNpc->getPos();
					friendPos.vy += height;
					friendNpc->setPos( friendPos );

					m_contact = true;
				}
				else
				{
					if( friendPos.vy >= collisionArea.y1 && friendPos.vy <= collisionArea.y2 )
					{
						int height = getHeightFromPlatformAtPosition( friendPos.vx, friendPos.vy );

						if ( height >= -threshold && height < 1 )
						{
							friendNpc->setPlatform( this );
							DVECTOR friendPos = friendNpc->getPos();
							friendPos.vy += height;
							friendNpc->setPos( friendPos );

							m_contact = true;
						}
					}
				}
			}

			break;
		}

		case TYPE_HAZARD:
			break;

		default:
			ASSERT(0);
			break;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcPlatform::processMovement( int _frames )
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcPlatform::render()
{
	if ( m_isActive )
	{
		CPlatformThing::render();

		// Render
		if (canRender())
		{
			DVECTOR &renderPos=getRenderPos();

			m_modelGfx->Render(renderPos);
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int CNpcPlatform::checkCollisionAgainst(CThing *_thisThing, int _frames)
{
	switch(_thisThing->getThingType())
	{
		case TYPE_PLAYERPROJECTILE:
			return( false );

		default:
		{
			int collided = false;

			if ( m_detectCollision && m_isActive && !isSetToShutdown() )
			{
				CRECT thisRect, thatRect;

				thisRect = getCollisionArea();
				thatRect = _thisThing->getCollisionArea();

				DVECTOR const &thisPosDelta = getPosDelta();
				int	ThisAbsY=abs(thisPosDelta.vy);
				thisRect.y1 -= ThisAbsY;
				thisRect.y2 += ThisAbsY;

				DVECTOR const &ThatPosDelta = _thisThing->getPosDelta();
				int	ThatAbsY=abs(ThatPosDelta.vy);
				thatRect.y1 -= ThatAbsY;
				thatRect.y2 += ThatAbsY;

				if(((thisRect.x1>=thatRect.x1&&thisRect.x1<=thatRect.x2)||(thisRect.x2>=thatRect.x1&&thisRect.x2<=thatRect.x2)||(thisRect.x1<=thatRect.x1&&thisRect.x2>=thatRect.x2))&&
				   ((thisRect.y1>=thatRect.y1&&thisRect.y1<=thatRect.y2)||(thisRect.y2>=thatRect.y1&&thisRect.y2<=thatRect.y2)||(thisRect.y1<=thatRect.y1&&thisRect.y2>=thatRect.y2)))
				{
					collided = true;
				}
			}

			return( collided );
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcPlatform::setTiltable( bool isTiltable )
{
	m_tiltable = isTiltable;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int	CNpcPlatform::getHeightFromPlatformAtPosition(int _x,int _y, int offsetX, int offsetY)
{
	DVECTOR top;
	int		angle;

	CRECT const &collisionArea = getCollisionArea();

	top.vy = offsetY + collisionArea.y1;

	angle=getCollisionAngle() + m_initRotation;
	if(angle==0)
	{
		// Non-rotated platform

		return( top.vy - _y );
	}
	else
	{
		if ( angle > 0 )
		{
			top.vx = offsetX + collisionArea.x1;
		}
		else
		{
			top.vx = offsetX + collisionArea.x2;
		}

		if ( _x > collisionArea.x2 )
		{
			_x = collisionArea.x2;
		}
		else if ( _x < collisionArea.x1 )
		{
			_x = collisionArea.x1;
		}

		// Rotate backwards to find height at current position

		s16 cosVal = rcos( angle );

		if ( !cosVal )
		{
			return( top.vy - _y );
		}
		else
		{
			int hypotenuse = ( ( top.vx - _x ) << 12 ) / cosVal;

			int angleHeight = -( hypotenuse * rsin( angle ) ) >> 12;

			return( ( top.vy - _y ) + angleHeight );
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*void CNpcPlatform::addWaypoint( u16 *ptr )
{
	m_npcPath.addWaypoint( ptr );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcPlatform::addWaypoint( u16 *ptrX, u16 *ptrY )
{
	m_npcPath.addWaypoint( ptrX, ptrY );
}*/

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcPlatform::setTypeFromMapEdit( u16 newType )
{
int	Type= mapEditConvertTable[newType];
	setThingSubType(Type);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CNpcPlatform::NPC_PLATFORM_UNIT_TYPE CNpcPlatform::getTypeFromMapEdit( u16 newType )
{
	return( mapEditConvertTable[newType] );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool CNpcPlatform::alwaysThink()
{
	int chapter=GameScene.getChapterNumber();
	int level=GameScene.getLevelNumber();

	if ( chapter == 5 )
	{
		if ( level == 5 )
		{
			return( true );
		}
	}

	return( false );

	//return( ( chapter == 1 ) && ( level == 5 ) );
}

/*=========================================================================

	hazard.h

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __HAZARD_HAZARD_H__
#include "hazard\hazard.h"
#endif

#ifndef __GAME_GAME_H__
#include	"game\game.h"
#endif

#ifndef __VID_HEADER_
#include "system\vid.h"
#endif

#ifndef	__UTILS_HEADER__
#include	"utils\utils.h"
#endif

#ifndef	__PLAYER_PLAYER_H__
#include	"player\player.h"
#endif

#ifndef __HAZARD_HFALLING_H__
#include "hazard\hfalling.h"
#endif

#ifndef __HAZARD_HPENDULM_H__
#include "hazard\hpendulm.h"
#endif

#ifndef __HAZARD_HBOAT_H__
#include "hazard\hboat.h"
#endif

#ifndef __HAZARD_HBARREL_H__
#include "hazard\hbarrel.h"
#endif

#ifndef __HAZARD_HSTATIC_H__
#include "hazard\hstatic.h"
#endif

#ifndef __HAZARD_HCSAW_H__
#include "hazard\hcsaw.h"
#endif

#ifndef __HAZARD_HMOWER_H__
#include "hazard\hmower.h"
#endif

#ifndef __HAZARD_HMASHER_H__
#include "hazard\hmasher.h"
#endif

#ifndef __HAZARD_HFAN_H__
#include "hazard\hfan.h"
#endif

#ifndef __HAZARD_HSPIKES_H__
#include "hazard\hspikes.h"
#endif

#ifndef __HAZARD_HBWHEEL_H__
#include "hazard\hbwheel.h"
#endif

#ifndef __HAZARD_HDBARREL_H__
#include "hazard\hdbarrel.h"
#endif

#ifndef __HAZARD_HBBARREL_H__
#include "hazard\hbbarrel.h"
#endif

#ifndef __HAZARD_HFIREBAL_H__
#include "hazard\hfirebal.h"
#endif

#ifndef __HAZARD_HRROCK_H__
#include "hazard\hrrock.h"
#endif

#ifndef __HAZARD_HFLYTRAP_H__
#include "hazard\hflytrap.h"
#endif

#ifndef __HAZARD_HRWEIGHT_H__
#include "hazard\hrweight.h"
#endif

#ifndef __HAZARD_HPSWITCH_H__
#include "hazard\hpswitch.h"
#endif

#ifndef __HAZARD_HRCKSHRD_H__
#include "hazard\hrckshrd.h"
#endif

#ifndef __HAZARD_HINERT_H__
#include "hazard\hinert.h"
#endif

#ifndef __HAZARD_HCSWITCH_H__
#include "hazard\hcswitch.h"
#endif

#ifndef __HAZARD_HBROCK_H__
#include "hazard\hbrock.h"
#endif

#ifndef __HAZARD_HSRDFISH_H__
#include "hazard\hsrdfish.h"
#endif

#ifndef __HAZARD_HLOG_H__
#include "hazard\hlog.h"
#endif

#ifndef __HAZARD_HCHECK_H__
#include "hazard\hcheck.h"
#endif


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CNpcHazard::NPC_HAZARD_UNIT_TYPE CNpcHazard::mapEditConvertTable[NPC_HAZARD_TYPE_MAX] =
{
	NPC_FALLING_HAZARD,
	NPC_PENDULUM_HAZARD,
	NPC_BOAT_HAZARD,
	NPC_BARREL_HAZARD,
	NPC_STATIC_HAZARD,
	NPC_CIRCULAR_SAW_HAZARD,
	NPC_MOWER_HAZARD,
	NPC_MASHER_HAZARD,
	NPC_FAN_HAZARD,
	NPC_SPIKES_HAZARD,
	NPC_BIG_WHEEL_HAZARD,
	NPC_DUAL_PLATFORM_BARREL_HAZARD,
	NPC_BOUNCING_BARREL_HAZARD,
	NPC_FIREBALL_HAZARD,
	NPC_ROLLING_ROCK_HAZARD,
	NPC_FLY_TRAP_HAZARD,
	NPC_RISING_WEIGHT_HAZARD,
	NPC_RISING_WEIGHT_WHEEL_HAZARD,
	NPC_PRESSURE_SWITCH_HAZARD,
	NPC_ROCKSHARD_HAZARD,
	NPC_INERT_HAZARD,
	NPC_CONVEYOR_SWITCH_HAZARD,
	NPC_BOUNCING_ROCK_HAZARD,
	NPC_SWORDFISH_HAZARD,
	NPC_LOG_HAZARD,
	NPC_CHECKPOINT_HAZARD,
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CNpcHazard *CNpcHazard::Create(int Type)
{
CNpcHazard *hazard;

	hazard = (CNpcHazard*)CThingManager::GetThing(CThing::TYPE_HAZARD,Type);
	if (!hazard)
	switch( Type )
	{
		case NPC_FALLING_HAZARD:
		{
			hazard = new ("falling hazard") CNpcFallingHazard;
			break;
		}

		case NPC_PENDULUM_HAZARD:
		{
			hazard = new ("pendulum hazard") CNpcPendulumHazard;
			break;
		}

		case NPC_BOAT_HAZARD:
		{
			hazard = new ("boat hazard") CNpcBoatHazard;
			break;
		}

		case NPC_BARREL_HAZARD:
		{
			hazard = new ("barrel hazard") CNpcBarrelHazard;
			break;
		}

		case NPC_STATIC_HAZARD:
		{
			hazard = new ("static hazard") CNpcStaticHazard;
			break;
		}

		case NPC_CIRCULAR_SAW_HAZARD:
		{
			hazard = new ("circular saw hazard") CNpcCircularSawHazard;
			break;
		}

		case NPC_MOWER_HAZARD:
		{
			hazard = new ("mower hazard") CNpcMowerHazard;
			break;
		}

		case NPC_MASHER_HAZARD:
		{
			hazard = new ("masher hazard") CNpcMasherHazard;
			break;
		}

		case NPC_FAN_HAZARD:
		{
			hazard = new ("fan hazard") CNpcFanHazard;
			break;
		}

		case NPC_SPIKES_HAZARD:
		{
			hazard = new ("spikes hazard") CNpcSpikesHazard;
			break;
		}

		case NPC_BIG_WHEEL_HAZARD:
		{
			hazard = new ("big wheel") CNpcBigWheelHazard;
			break;
		}

		case NPC_DUAL_PLATFORM_BARREL_HAZARD:
		{
			hazard = new ("dual platform barrel hazard") CNpcDualPlatformBarrelHazard;
			break;
		}

		case NPC_BOUNCING_BARREL_HAZARD:
		{
			hazard = new ("bouncing barrel hazard") CNpcBouncingBarrelHazard;
			break;
		}

		case NPC_FIREBALL_HAZARD:
		{
			hazard = new ("fireball hazard") CNpcFireballHazard;
			break;
		}

		case NPC_ROLLING_ROCK_HAZARD:
		{
			hazard = new ("rolling rock hazard") CNpcRollingRockHazard;
			break;
		}

		case NPC_FLY_TRAP_HAZARD:
		{
			hazard = new ("fly trap hazard") CNpcFlyTrapHazard;
			break;
		}

		case NPC_RISING_WEIGHT_HAZARD:
		{
			hazard = new ("rising weight hazard") CNpcRisingWeightHazard;
			break;
		}

		case NPC_RISING_WEIGHT_WHEEL_HAZARD:
		{
			hazard = new ("rising weight wheel hazard") CNpcRisingWeightWheelHazard;
			break;
		}

		case NPC_PRESSURE_SWITCH_HAZARD:
		{
			hazard = new ("pressure switch hazard") CNpcPressureSwitchHazard;
			break;
		}

		case NPC_ROCKSHARD_HAZARD:
		{
			hazard = new ("rock shard hazard") CNpcRockShardHazard;
			break;
		}

		case NPC_INERT_HAZARD:
		{
			hazard = new ("inert hazard") CNpcInertHazard;
			break;
		}

		case NPC_CONVEYOR_SWITCH_HAZARD:
		{
			hazard = new ("conveyor switch hazard") CNpcConveyorSwitchHazard;
			break;
		}

		case NPC_BOUNCING_ROCK_HAZARD:
		{
			hazard = new ("bouncing rock hazard") CNpcBouncingRockHazard;
			break;
		}

		case NPC_SWORDFISH_HAZARD:
		{
			hazard = new ("swordfish hazard") CNpcSwordfishHazard;
			break;
		}

		case NPC_LOG_HAZARD:
		{
			hazard = new ("log hazard") CNpcLogHazard;
			break;
		}

		case NPC_CHECKPOINT_HAZARD:
		{
			hazard = new ("checkpoint hazard") CNpcCheckpointHazard;
			break;
		}

		default:
		{
			hazard = NULL;
			break;
		}
	}

	ASSERT( hazard );

	hazard->setThingSubType(Type);
	hazard->init();

	return( hazard );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CNpcHazard *CNpcHazard::Create(sThingHazard *ThisHazard)
{
	NPC_HAZARD_UNIT_TYPE Type = getTypeFromMapEdit( ThisHazard->Type );

	CNpcHazard *hazard=Create(ThisHazard->Type);

	hazard->setWaypoints( ThisHazard );
	hazard->setGraphic( ThisHazard );

	return( hazard );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcHazard::setWaypoints( sThingHazard *ThisHazard )
{
	int pointNum;

	u16	*PntList=(u16*)MakePtr(ThisHazard,sizeof(sThingHazard));

	u16 newXPos, newYPos;

	m_npcPath.setWaypointCount( ThisHazard->PointCount - 1 );

	newXPos = (u16) *PntList;
	setWaypointPtr( PntList );
	PntList++;
	newYPos = (u16) *PntList;
	PntList++;

	DVECTOR startPos;
	startPos.vx = ( newXPos << 4 ) + 8;
	startPos.vy = ( newYPos << 4 ) + 16;

	Pos = startPos;
	m_base = Pos;

	s32 minX, maxX, minY, maxY;

	m_npcPath.getPathXExtents( &minX, &maxX );
	m_npcPath.getPathYExtents( &minY, &maxY );

	m_thinkArea.x1 = minX;
	m_thinkArea.x2 = maxX;
	m_thinkArea.y1 = minY;
	m_thinkArea.y2 = maxY;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CNpcHazard::NPC_HAZARD_UNIT_TYPE CNpcHazard::getTypeFromMapEdit( u16 newType )
{
	return( mapEditConvertTable[newType] );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcHazard::init()
{
	CHazardThing::init();

	m_npcPath.initPath();

	m_timer = 0;
	m_timerActive = false;
	m_isActive = true;

	m_extension = 0;
	m_extendDir = 0;
	m_heading = 0;

	m_soundId = (int) NOT_PLAYING;

	clearPlatform();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcHazard::setGraphic( sThingHazard *ThisHazard )
{
	m_modelGfx = new ("ModelGfx") CModelGfx;
	m_modelGfx->SetModel( ThisHazard->Gfx );

	sBBox boundingBox = m_modelGfx->GetBBox();
	setCollisionSize( ( boundingBox.XMax - boundingBox.XMin ), ( boundingBox.YMax - boundingBox.YMin ) );
	setCollisionCentreOffset( ( boundingBox.XMax + boundingBox.XMin ) >> 1, ( boundingBox.YMax + boundingBox.YMin ) >> 1 );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcHazard::shutdown()
{
	if ( m_soundId != NOT_PLAYING )
	{
		CSoundMediator::stopAndUnlockSfx( (xmPlayingId) m_soundId );
	}

	delete m_modelGfx;

	//if (m_actorGfx)	delete m_actorGfx;

	CHazardThing::shutdown();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcHazard::leftThinkZone(int _frames)
{
	if ( m_soundId != NOT_PLAYING )
	{
		CSoundMediator::stopAndUnlockSfx( (xmPlayingId) m_soundId );
		m_soundId = NOT_PLAYING;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcHazard::think(int _frames)
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

	CHazardThing::think(_frames);

	if ( m_isActive )
	{
		int moveFrames = _frames;

		if ( moveFrames > 2 )
		{
			// make sure hazards don't go berserk if too many frames are dropped

			moveFrames = 2;
		}

		processMovement( moveFrames );
	}

	if ( m_timerActive )
	{
		processTimer( _frames );
	}

	clearPlatform();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcHazard::processMovement( int _frames )
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcHazard::processTimer( int _frames )
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcHazard::render()
{
	if ( m_isActive )
	{
		CHazardThing::render();

		if (canRender())
		{
			DVECTOR &renderPos=getRenderPos();

			m_modelGfx->Render(renderPos);
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcHazard::collidedWith( CThing *_thisThing )
{
	if ( m_isActive )
	{
		switch(_thisThing->getThingType())
		{
			case TYPE_PLAYER:
			{
				CPlayer *player = (CPlayer *) _thisThing;

				if ( !player->isRecoveringFromHit() )
				{
					player->takeDamage( DAMAGE__HIT_ENEMY );
				}

				break;
			}

			default:
				break;
		}
	}
}

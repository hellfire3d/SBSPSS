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
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CNpcHazard *CNpcHazard::Create(sThingHazard *ThisHazard)
{
	CNpcHazard *hazard;

	NPC_HAZARD_UNIT_TYPE hazardType = getTypeFromMapEdit( ThisHazard->Type );

	switch( hazardType )
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

		default:
		{
			hazard = NULL;
			break;
		}
	}

	ASSERT( hazard );

	hazard->init();
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

	newXPos = (u16) *PntList;
	PntList++;
	newYPos = (u16) *PntList;
	PntList++;

	DVECTOR startPos;
	startPos.vx = newXPos << 4;
	startPos.vy = newYPos << 4;

	Pos = startPos;
	m_base = Pos;

	addWaypoint( newXPos, newYPos );

	if ( ThisHazard->PointCount > 1 )
	{
		for ( pointNum = 1 ; pointNum < ThisHazard->PointCount ; pointNum++ )
		{
			newXPos = (u16) *PntList;
			PntList++;
			newYPos = (u16) *PntList;
			PntList++;

			addWaypoint( newXPos, newYPos );
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcHazard::addWaypoint( s32 xPos, s32 yPos )
{
	DVECTOR newPos;

	newPos.vx = xPos << 4;
	newPos.vy = yPos << 4;

	m_npcPath.addWaypoint( newPos );
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

	//m_actorGfx=CActorPool::GetActor( (FileEquate) ACTORS_CLAM_SBK );
	//m_spriteBank=0;

	m_npcPath.initPath();

	m_timer = 0;
	m_timerActive = false;
	m_isActive = true;

	m_extension = 0;
	m_extendDir = 0;
	m_heading = 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcHazard::setGraphic( sThingHazard *ThisHazard )
{
	m_modelGfx = new ("ModelGfx") CModelGfx;
	m_modelGfx->SetModel( ThisHazard->Gfx );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcHazard::shutdown()
{
	delete m_modelGfx;
	//if (m_spriteBank) m_spriteBank->dump();		delete m_spriteBank;
	// remove waypoints

	m_npcPath.removeAllWaypoints();

	//if (m_actorGfx)	delete m_actorGfx;

	CHazardThing::shutdown();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcHazard::think(int _frames)
{
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

		// Render
		DVECTOR renderPos;
		DVECTOR	offset = CLevel::getCameraPos();

		renderPos.vx = Pos.vx - offset.vx;
		renderPos.vy = Pos.vy - offset.vy;

		if ( renderPos.vx >= 0 && renderPos.vx <= VidGetScrW() )
		{
			if ( renderPos.vy >= 0 && renderPos.vy <= VidGetScrH() )
			{
				m_modelGfx->Render(renderPos);
				//m_actorGfx->Render(renderPos,0,0,0);
			}
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
				ASSERT(0);
				break;
		}
	}
}

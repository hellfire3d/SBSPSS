/*=========================================================================

	pbgeyser.h

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __PLATFORM_PBGEYSER_H__
#include "platform\pbgeyser.h"
#endif

#ifndef	__UTILS_HEADER__
#include	"utils\utils.h"
#endif

#ifndef __GAME_GAME_H__
#include "game\game.h"
#endif

#ifndef __PLATFORM_PBUBBLE_H__
#include "platform\pbubble.h"
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcGeyserPlatformGenerator::collidedWith(CThing *_thisThing)
{
	// do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcGeyserPlatformGenerator::render()
{
	// no rendering
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcGeyserPlatformGenerator::think( int _frames )
{
	m_timer -= _frames;

	if ( m_timer < 0 )
	{
		//m_timer = getRnd() % ( m_data[m_type].initTimer * GameState::getOneSecondInFrames() );
		m_timer = m_data[m_type].initTimer * GameState::getOneSecondInFrames();

		// generate new falling platform

		CNpcPlatform *newPlatform;
		newPlatform = NULL;

		switch( m_targetType )
		{
			case NPC_BUBBLE_PLATFORM:
			{
				newPlatform = new ("falling platform") CNpcBubblePlatform;
				break;
			}

			default:
			{
				ASSERT( 0 );
				break;
			}
		}

		ASSERT(newPlatform);

		newPlatform->setType( m_targetType );
		newPlatform->setGraphic( m_graphicNum );

		DVECTOR startPos = Pos;
		startPos.vx += ( -5 + ( getRnd() % 11 ) );
		newPlatform->init( startPos );

		CNpcWaypoint *sourceWaypoint = m_npcPath.getWaypointList();

		if ( sourceWaypoint )
		{
			while( sourceWaypoint )
			{
				newPlatform->addWaypoint( sourceWaypoint->pos.vx >> 4, sourceWaypoint->pos.vy >> 4 );
				sourceWaypoint = sourceWaypoint->nextWaypoint;
			}
		}

		newPlatform->setLayerCollision( m_layerCollision );
		newPlatform->setTiltable( false );
		newPlatform->postInit();
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const CRECT *CNpcGeyserPlatformGenerator::getThinkBBox()
{
	CRECT objThinkBox = getCollisionArea();

	sBBox &thinkBBox = CThingManager::getThinkBBox();
	objThinkBox.x1 = thinkBBox.XMin;
	objThinkBox.x2 = thinkBBox.XMax;
	objThinkBox.y1 = thinkBBox.YMin;
	objThinkBox.y2 = thinkBBox.YMax;

	return &objThinkBox;
}
/*=========================================================================

	pfgen.h

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __PLATFORM_PFGEN_H__
#include "platform\pfgen.h"
#endif

#ifndef	__UTILS_HEADER__
#include	"utils\utils.h"
#endif

#ifndef __GAME_GAME_H__
#include "game\game.h"
#endif

#ifndef __PLATFORM_PFALLNOR_H__
#include "platform\pfallnor.h"
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcFallingPlatformGenerator::collidedWith(CThing *_thisThing)
{
	// do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcFallingPlatformGenerator::render()
{
	// no rendering
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcFallingPlatformGenerator::think( int _frames )
{
	m_timer -= _frames;

	if ( m_timer < 0 )
	{
		s32 second = GameState::getOneSecondInFrames();

		m_timer = second;

		m_timer += m_data[m_type].initTimer * second;

		m_timer += second - ( getRnd() % ( 2 * second ) );

		//m_timer = ( 3 * GameState::getOneSecondInFrames() ) + ( getRnd() % ( ( m_data[m_type].initTimer - 1 ) * GameState::getOneSecondInFrames() ) );

		// generate new falling platform

		CNpcPlatform *newPlatform;
		newPlatform = NULL;

		switch( m_targetType )
		{
			case NPC_OILDRUM_PLATFORM:
			case NPC_CRATE_PLATFORM:
			case NPC_VERTICAL_OILDRUM_PLATFORM:
			{
				newPlatform = new ("falling platform") CNpcFallingNoRespawnPlatform;
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

		CNpcWaypoint *sourceWaypoint = m_npcPath.getWaypointList();

		if ( sourceWaypoint )
		{
			DVECTOR startPos;
			startPos.vx = sourceWaypoint->pos.vx;
			startPos.vy = sourceWaypoint->pos.vy;
			//startPos.vx = 100;
			//startPos.vy = 100;

			newPlatform->init( startPos );
			ASSERT(m_layerCollision);

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

const CRECT *CNpcFallingPlatformGenerator::getThinkBBox()
{
	CRECT objThinkBox = getCollisionArea();

	sBBox &thinkBBox = CThingManager::getThinkBBox();
	objThinkBox.x1 = thinkBBox.XMin;
	objThinkBox.x2 = thinkBBox.XMax;
	objThinkBox.y1 = thinkBBox.YMin;
	objThinkBox.y2 = thinkBBox.YMax;

	return &objThinkBox;
}
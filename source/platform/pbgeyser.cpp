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
		m_timer = m_dataPtr->initTimer * GameState::getOneSecondInFrames();

		// generate new falling platform

		CNpcPlatform *newPlatform;
		newPlatform = NULL;

		switch( m_targetType )
		{
			case NPC_BUBBLE_PLATFORM:
			{
				//newPlatform = new ("falling platform") CNpcBubblePlatform;
				newPlatform = CNpcPlatform::Create( m_targetType );
				break;
			}

			default:
			{
				ASSERT( 0 );
				break;
			}
		}

		ASSERT(newPlatform);

		newPlatform->setThingSubType( m_targetType );
		newPlatform->setGraphic( m_graphicNum );

		DVECTOR startPos = Pos;
		startPos.vx += ( -5 + ( getRnd() % 11 ) );
		newPlatform->init( startPos );

		newPlatform->setWaypointCount( m_npcPath.getWaypointCount() );
		newPlatform->setWaypointPtr( m_npcPath.getWaypointPtr() );

		newPlatform->setTiltable( false );
		newPlatform->postInit();
	}
}

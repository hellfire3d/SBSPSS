/*=========================================================================

	ngen.cpp

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __ENEMY_NPC_H__
#include "enemy\npc.h"
#endif

#ifndef	__ENEMY_NGEN_H__
#include "enemy\ngen.h"
#endif

#ifndef __ENEMY_NSCRAB_H__
#include "enemy\nscrab.h"
#endif

#ifndef __GAME_GAME_H__
#include "game\game.h"
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcEnemyGenerator::think(int _frames)
{
	if ( getNumChildren() < 3 )
	{
		m_movementTimer -= _frames;

		if ( m_movementTimer < 0 )
		{
			m_movementTimer = 3 * GameState::getOneSecondInFrames();

			CNpcEnemy *enemy;
			enemy = new( "spider crab" ) CNpcSpiderCrabEnemy;
			ASSERT(enemy);
			enemy->setType( CNpcEnemy::NPC_SPIDER_CRAB );
			enemy->init();
			enemy->setLayerCollision( m_layerCollision );
			enemy->setStartPos( Pos.vx >> 4, Pos.vy >> 4 );

			CNpcWaypoint *sourceWaypoint = m_npcPath.getWaypointList();

			if ( sourceWaypoint )
			{
				// skip first waypoint

				sourceWaypoint = sourceWaypoint->nextWaypoint;

				while( sourceWaypoint )
				{
					enemy->addWaypoint( sourceWaypoint->pos.vx >> 4, sourceWaypoint->pos.vy >> 4 );
					sourceWaypoint = sourceWaypoint->nextWaypoint;
				}
			}

			enemy->setPathType( m_npcPath.getPathType() );

			enemy->postInit();

			addChild( enemy );
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcEnemyGenerator::render()
{
	// do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


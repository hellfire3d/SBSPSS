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

void CNpcEnemyGenerator::shutdown()
{
	deleteAllChild();
	CNpcEnemy::shutdown();
}

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
			enemy=CNpcEnemy::Create( CNpcEnemy::NPC_SPIDER_CRAB );
			enemy->setStartPosHighRes( Pos.vx, Pos.vy - 8 );

			u16 *waypointPtr = m_npcPath.getWaypointPtr();

			// skip first waypoint
			waypointPtr++;
			waypointPtr++;

			enemy->setWaypointCount( m_npcPath.getWaypointCount() - 1 );
			enemy->setWaypointPtr( waypointPtr );

			enemy->setPathType( m_npcPath.getPathType() );
			enemy->postInit();
			enemy->setThinkArea();
			enemy->updateCollisionArea();

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

void CNpcEnemyGenerator::processEnemyCollision( CThing *thisThing )
{
	// do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


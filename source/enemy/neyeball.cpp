/*=========================================================================

	neyeball.cpp

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/

#ifndef __ENEMY_NPC_H__
#include "enemy\npc.h"
#endif

#ifndef	__ENEMY_NEYEBALL_H__
#include "enemy\neyeball.h"
#endif

#ifndef __GAME_GAME_H__
#include "game\game.h"
#endif

#ifndef	__PLAYER_PLAYER_H__
#include "player\player.h"
#endif

#ifndef __PROJECTL_PROJECTL_H__
#include "projectl\projectl.h"
#endif


void CNpcEyeballEnemy::processEnemyCollision( CThing *thisThing )
{
	// do nothing
}

void CNpcEyeballEnemy::postInit()
{
return;
	m_fireHeading = m_heading;
	CProjectile *projectile;
	projectile = new ( "eyeball projectile" ) CProjectile;
	projectile->init( Pos, m_fireHeading, CProjectile::PROJECTILE_FIXED, CProjectile::PROJECTILE_INFINITE_LIFE );
	projectile->setLayerCollision( m_layerCollision );

	addChild( projectile );
}

bool CNpcEyeballEnemy::processSensor()
{
return(false);
	switch( m_sensorFunc )
	{
		case NPC_SENSOR_NONE:
			return( false );

		default:
			{
				if ( playerXDistSqr + playerYDistSqr < 40000 )
				{
					m_controlFunc = NPC_CONTROL_CLOSE;

					return( true );
				}
				else
				{
					return( false );
				}
			}
	}
}

void CNpcEyeballEnemy::processClose( int _frames )
{
return;
	if ( Next )
	{
		CProjectile *projectile;

		projectile = (CProjectile *) Next;

		if ( projectile->getMovementType() == CProjectile::PROJECTILE_FIXED )
		{
			projectile->setMovementType( CProjectile::PROJECTILE_USER_SEEK );
			projectile->setState( CProjectile::PROJECTILE_ATTACK );
		}
	}
	else
	{
		CProjectile *projectile;
		projectile = new ( "eyeball projectile" ) CProjectile;
		projectile->init( Pos, m_fireHeading, CProjectile::PROJECTILE_USER_SEEK, CProjectile::PROJECTILE_INFINITE_LIFE );
		projectile->setLayerCollision( m_layerCollision );
		projectile->setState( CProjectile::PROJECTILE_ATTACK );

		addChild( projectile );
	}
}
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

#ifndef __GAME_GAME_H__
#include "game\game.h"
#endif

#ifndef	__PLAYER_PLAYER_H__
#include "player\player.h"
#endif

#ifndef __PROJECTL_PROJECTL_H__
#include "projectl\projectl.h"
#endif


void CNpc::processCloseEyeballAttack( int _frames )
{
	if ( Next )
	{
		// already have child, ignore
	}
	else
	{
		// create child projectile which will be deleted when the projectile has returned

		CProjectile *projectile;
		projectile = new ( "test projectile" ) CProjectile;
		projectile->init( Pos, m_fireHeading, CProjectile::PROJECTILE_USER_SEEK, CProjectile::PROJECTILE_INFINITE_LIFE );

		addChild( projectile );
	}
}
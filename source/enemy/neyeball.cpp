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


void CNpcEnemy::processCloseEyeballAttack( int _frames )
{
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
}
/*=========================================================================

	nsklfish.cpp

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


void CNpcEnemy::processCloseSkeletalFishAttack( int _frames )
{
	s32 moveX, moveY;
	s16 decDir, incDir, moveDist;

	s16 headingToPlayer = ratan2( playerYDist, playerXDist );

	decDir = m_heading - headingToPlayer;

	if ( decDir < 0 )
	{
		decDir += ONE;
	}

	incDir = headingToPlayer - m_heading;

	if ( incDir < 0 )
	{
		incDir += ONE;
	}

	if ( decDir < incDir )
	{
		moveDist = -decDir;
	}
	else
	{
		moveDist = incDir;
	}

	s32 xDistWaypoint, yDistWaypoint;

	m_npcPath.getDistToNextWaypoint( Pos, &xDistWaypoint, &yDistWaypoint );

	if ( abs( moveDist ) < 1024 && abs( xDistWaypoint ) >= abs( playerXDist ) )
	{
		// continue charge

		processGenericGotoTarget( _frames, playerXDist, playerYDist, m_velocity );
	}
	else
	{
		m_controlFunc = NPC_CONTROL_MOVEMENT;
	}
}

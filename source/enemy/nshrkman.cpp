/*=========================================================================

	nshrkman.cpp

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
#include	"game\game.h"
#endif

#ifndef	__PLAYER_PLAYER_H__
#include	"player\player.h"
#endif


void CNpc::processCloseSharkManAttack( int _frames )
{
	s32 moveX, moveY;
	s16 decDir, incDir, moveDist;
	CPlayer *player = GameScene.getPlayer();
	DVECTOR playerPos = player->getPos();
	s32 xDist, yDist;

	xDist = playerPos.vx - this->Pos.vx;

	yDist = playerPos.vy - this->Pos.vy;

	s16 headingToPlayer = ratan2( yDist, xDist );

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

	if ( abs( moveDist ) < 1024 && abs( xDistWaypoint ) >= abs( xDist ) )
	{
		// continue charge

		if ( playerPos.vx < Pos.vx )
		{
			m_heading = 2048;
		}
		else
		{
			m_heading = 0;
		}

		s32 preShiftX = _frames * m_velocity * rcos( m_heading );
		s32 preShiftY = _frames * m_velocity * rsin( m_heading );

		moveX = preShiftX >> 12;
		if ( !moveX && preShiftX )
		{
			moveX = preShiftX / abs( preShiftX );
		}

		moveY = preShiftY >> 12;
		if ( !moveY && preShiftY )
		{
			moveY = preShiftY / abs( preShiftY );
		}

		Pos.vx += moveX;
		Pos.vy += moveY;
	}
	else
	{
		m_controlFunc = NPC_CONTROL_MOVEMENT;
	}
}

/*=========================================================================

	nnsfish.cpp

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


void CNpc::processCloseNinjaStarfishAttack( int _frames )
{
	s32 moveX = 0, moveY = 0;

	s16 moveDist = 0;

	s32 moveVel = 0;

	//if ( playerXDistSqr + playerYDistSqr > 22500 )
	//{
		//this->m_controlFunc = NPC_CONTROL_MOVEMENT;
	//}
	//else
	{
		bool pathComplete;

		s16 headingToPlayer = ratan2( playerYDist, playerXDist );
		s16 maxTurnRate = m_data[m_type].turnSpeed;
		s16 decDir, incDir;

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

		if ( moveDist < -maxTurnRate )
		{
			moveDist = -maxTurnRate;
		}
		else if ( moveDist > maxTurnRate )
		{
			moveDist = maxTurnRate;
		}

		m_heading += moveDist;

		m_heading = m_heading % ONE;
		
		s32 preShiftX = _frames * m_data[m_type].speed * rcos( m_heading );
		s32 preShiftY = _frames * m_data[m_type].speed * rsin( m_heading );

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
}

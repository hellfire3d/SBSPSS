/*=========================================================================

	nsstomp.cpp

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


void CNpc::processCloseSkullStomperAttack( int _frames )
{
	if ( m_timerTimer > 0 )
	{
		// wait
	}
	else
	{
		s32 velocity;
		bool pathComplete;
		bool waypointChange;
		s16 headingToTarget = m_npcPath.think( Pos, &pathComplete, &waypointChange );
		s32 moveX, moveY;

		if ( waypointChange )
		{
			if ( !pathComplete )
			{
				// pause and change direction

				m_timerTimer = GameState::getOneSecondInFrames();
				m_extendDir = EXTEND_UP;

				return;
			}
		}

		if ( pathComplete )
		{
			m_controlFunc = NPC_CONTROL_MOVEMENT;
			m_timerFunc = NPC_TIMER_ATTACK_DONE;
			m_timerTimer = GameState::getOneSecondInFrames();
			m_sensorFunc = NPC_SENSOR_NONE;
			m_npcPath.resetPath();
		}
		else
		{
			s32 preShiftX;
			s32 preShiftY;
				
			m_heading = headingToTarget;

			if ( m_extendDir == EXTEND_DOWN )
			{
				preShiftX = _frames * 8 * rcos( m_heading );
				preShiftY = _frames * 8 * rsin( m_heading );
			}
			else
			{
				preShiftX = _frames * 2 * rcos( m_heading );
				preShiftY = _frames * 2 * rsin( m_heading );
			}

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
}
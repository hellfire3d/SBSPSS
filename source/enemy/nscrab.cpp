/*=========================================================================

	nscrab.cpp

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


void CNpc::processCloseSpiderCrabAttack( int _frames )
{
	s32 velocity;
	DVECTOR newPos = Pos;

	velocity = m_velocity * _frames;

	if ( m_extendDir == EXTEND_RIGHT )
	{
		newPos.vx += velocity;
		m_heading = 0;
	}
	else
	{
		newPos.vx -= velocity;
		m_heading = 2048;
	}

	s32 horizontalExtension = abs( newPos.vx - m_base.vx );

	if ( horizontalExtension > 128 )
	{
		if ( m_extendDir == EXTEND_RIGHT )
		{
			newPos.vx = m_base.vx + 128;
		}
		else
		{
			newPos.vx = m_base.vx - 128;
		}

		newPos.vy = m_base.vy;

		m_controlFunc = NPC_CONTROL_MOVEMENT;
		m_timerFunc = NPC_TIMER_ATTACK_DONE;
		m_timerTimer = GameState::getOneSecondInFrames();
		m_sensorFunc = NPC_SENSOR_NONE;
	}
	else
	{
		newPos.vy = m_base.vy - ( ( 20 * rsin( horizontalExtension << 4 ) ) >> 12 );
	}

	// check for collision with ground

	if ( m_layerCollision->getHeightFromGround( newPos.vx, newPos.vy ) < 0 )
	{
		// abort jump

		m_controlFunc = NPC_CONTROL_MOVEMENT;
		m_timerFunc = NPC_TIMER_ATTACK_DONE;
		m_timerTimer = GameState::getOneSecondInFrames();
		m_sensorFunc = NPC_SENSOR_NONE;
	}
	else
	{
		Pos = newPos;
	}
}

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

	velocity = m_velocity * _frames;

	if ( m_extendDir == EXTEND_RIGHT )
	{
		Pos.vx += velocity;
		m_heading = 0;
	}
	else
	{
		Pos.vx -= velocity;
		m_heading = 2048;
	}

	s32 horizontalExtension = abs( Pos.vx - m_extensionBase.vx );

	if ( horizontalExtension > 128 )
	{
		if ( m_extendDir == EXTEND_RIGHT )
		{
			Pos.vx = m_extensionBase.vx + 128;
		}
		else
		{
			Pos.vx = m_extensionBase.vx - 128;
		}

		Pos.vy = m_extensionBase.vy;

		m_controlFunc = NPC_CONTROL_MOVEMENT;
		m_timerFunc = NPC_TIMER_ATTACK_DONE;
		m_timerTimer = GameState::getOneSecondInFrames();
		m_sensorFunc = NPC_SENSOR_NONE;
	}
	else
	{
		Pos.vy = m_extensionBase.vy - ( ( 20 * rsin( horizontalExtension << 4 ) ) >> 12 );
	}
}

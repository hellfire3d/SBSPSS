/*=========================================================================

	ngpirate.cpp

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

#ifndef __PROJECTL_PROJECTL_H__
#include "projectl\projectl.h"
#endif


void CNpc::processCloseGhostPirateAttack( int _frames )
{
	s32 velocity;

	if ( m_extendDir == EXTEND_UP )
	{
		m_movementTimer -= _frames;

		if ( m_movementTimer > 0 )
		{
			// extend

			velocity = m_velocity * _frames;

			m_extension += velocity;

			Pos.vx += ( velocity * rcos( m_heading ) ) >> 12;
			Pos.vy += ( velocity * rsin( m_heading ) ) >> 12;
		}
		else
		{
			m_extendDir = EXTEND_DOWN;

			// fire

			s16 heading;

			if ( playerXDist > 0 )
			{
				heading = 0;
			}
			else
			{
				heading = 2048;
			}

			CProjectile *projectile;

			projectile = new( "test projectile" ) CProjectile;
			projectile->init( Pos, heading );
		}
	}
	else if ( m_extendDir == EXTEND_DOWN )
	{
		// retract

		if ( m_extension > 0 )
		{
			velocity = - ( m_velocity * _frames );

			if ( m_extension < _frames )
			{
				velocity = m_extension - _frames;
			}
			
			m_extension += velocity;


			Pos.vx += ( velocity * rcos( m_heading ) ) >> 12;
			Pos.vy += ( velocity * rsin( m_heading ) ) >> 12;
		}
		else
		{
			this->m_controlFunc = NPC_CONTROL_MOVEMENT;
			this->m_timerFunc = NPC_TIMER_ATTACK_DONE;
			this->m_timerTimer = GameState::getOneSecondInFrames();
			this->m_sensorFunc = NPC_SENSOR_NONE;
		}
	}
}
/*=========================================================================

	nbooger.cpp

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


void CNpc::processCloseBoogerMonsterAttack( int _frames )
{
	CPlayer *player = GameScene.getPlayer();
	DVECTOR playerPos = player->getPos();
	s32 xDist, yDist;
	s32 xDistSqr, yDistSqr;
	s32 velocity;

	xDist = playerPos.vx - this->Pos.vx;
	xDistSqr = xDist * xDist;
	yDist = playerPos.vy - this->Pos.vy;
	yDistSqr = yDist * yDist;

	if ( xDistSqr + yDistSqr > 10000 )
	{
		// user out of range, retract

		if ( m_extension > 0 )
		{
			// retract

			velocity = 1 * _frames;

			if ( m_extension < velocity )
			{
				velocity = m_extension;
			}

			m_extension -= velocity;

			Pos.vx += ( -velocity * rcos( m_heading ) ) >> 12;
			Pos.vy += ( -velocity * rsin( m_heading ) ) >> 12;
		}
		else
		{
			m_controlFunc = NPC_CONTROL_MOVEMENT;
			m_timerFunc = NPC_TIMER_ATTACK_DONE;
			m_timerTimer = GameState::getOneSecondInFrames();
			m_sensorFunc = NPC_SENSOR_NONE;
		}
	}
	else
	{
		if ( m_extension < NPC_BOOGER_MONSTER_MAX_EXTENSION )
		{
			// extend

			velocity = 1 * _frames;

			if ( m_extension + velocity < NPC_BOOGER_MONSTER_MAX_EXTENSION )
			{
				m_extension += velocity;
			}
			else
			{
				velocity = NPC_BOOGER_MONSTER_MAX_EXTENSION - m_extension;

				m_extension = NPC_BOOGER_MONSTER_MAX_EXTENSION;
			}

			Pos.vx += ( velocity * rcos( m_heading ) ) >> 12;
			Pos.vy += ( velocity * rsin( m_heading ) ) >> 12;
		}
		else
		{
			// grab player
		}
	}
}
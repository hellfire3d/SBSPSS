/*=========================================================================

	nanemone.cpp

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/

#ifndef __ENEMY_NPC_H__
#include "enemy\npc.h"
#endif

#ifndef __PROJECTL_PROJECTL_H__
#include "projectl\projectl.h"
#endif

#ifndef __GAME_GAME_H__
#include "game\game.h"
#endif


void CNpc::processCloseAnemone2Attack( int _frames )
{
	int fireLoop;
	CProjectile *projectile;
	s16 heading;

	for ( fireLoop = 0 ; fireLoop < 5 ; fireLoop++ )
	{
		heading = m_baseHeading - 1024 + ( fireLoop * 512 );
		heading %= 4096;

		projectile = new( "test projectile" ) CProjectile;
		projectile->init( Pos, heading );
	}

	m_controlFunc = NPC_CONTROL_MOVEMENT;
	m_timerFunc = NPC_TIMER_ATTACK_DONE;
	m_timerTimer = GameState::getOneSecondInFrames();
	m_sensorFunc = NPC_SENSOR_NONE;
}
/*=========================================================================

	nocto.cpp

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


void CNpcEnemy::processBabyOctopusMovementModifier( int _frames, s32 dist, s16 headingChange )
{
	s32 newX, newY;
	s32 preShiftX, preShiftY;

	u16 octopusData[11] = { 96, 192, 256, 256, 256, 192, 192, 192, 128, 128, 96 };

	u32 dataPoint;

	m_movementTimer += _frames;

	if ( m_movementTimer > ( 3 * GameState::getOneSecondInFrames() ) )
	{
		m_movementTimer = 0;
	}

	dataPoint = 10 * m_movementTimer;

	if ( dataPoint != 0 )
	{
		dataPoint /= ( 3 * GameState::getOneSecondInFrames() );
	}

	s32 resistance;
	s32 absVelocity = abs( m_velocity );
	s32 reqVelocity = dist;

	resistance = _frames * NPC_JELLYFISH_RESISTANCE;

	if ( absVelocity < resistance )
	{
		resistance = absVelocity;
	}

	if ( absVelocity != 0 )
	{
		resistance = -( resistance * m_velocity ) / absVelocity;
	}
	else
	{
		resistance = 0;
	}

	m_velocity += resistance;

	reqVelocity = dist * octopusData[dataPoint];
	reqVelocity >>= 8;
	reqVelocity *= 128 + ( 128 - headingChange );
	reqVelocity >>= 8;

	s32 absReqVelocity = abs( reqVelocity );

	if ( absReqVelocity > absVelocity )
	{
		m_velocity += reqVelocity >> 1;
	}

	preShiftX = ( m_velocity >> 8 ) * rcos( m_heading );
	preShiftY = ( m_velocity >> 8 ) * rsin( m_heading );

	newX = preShiftX >> 12;
	if ( !newX && preShiftX )
	{
		newX = preShiftX / abs( preShiftX );
	}

	newY = preShiftY >> 12;
	if ( !newY && preShiftY )
	{
		newY = preShiftY / abs( preShiftY );
	}

	Pos.vx += newX;
	Pos.vy += newY;
}

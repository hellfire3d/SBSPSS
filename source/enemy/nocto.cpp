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

#ifndef	__UTILS_HEADER__
#include "utils\utils.h"
#endif

#ifndef __ANIM_BABYOCTOPUS_HEADER__
#include <ACTOR_BABYOCTOPUS_ANIM.h>
#endif

void CNpcEnemy::processBabyOctopusMovementModifier( int _frames, s32 dist, s16 headingChange )
{
	s32 newX, newY;
	s32 preShiftX, preShiftY;

	//u16 octopusData[11] = { 96, 192, 256, 256, 256, 192, 192, 192, 128, 128, 96 };
	u16 octopusData[11] = { 96, 256, 96, 256, 96, 256, 96, 256, 96, 256, 96 };

	u32 dataPoint;

	m_movementTimer += _frames;

	if ( m_movementTimer > ( 2 * GameState::getOneSecondInFrames() ) )
	{
		m_movementTimer -= ( 2 * GameState::getOneSecondInFrames() );
	}

	dataPoint = 10 * m_movementTimer;

	if ( dataPoint != 0 )
	{
		dataPoint /= ( 2 * GameState::getOneSecondInFrames() );
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

void CNpcEnemy::processCloseOctopusAttack( int _frames )
{
	if ( !m_animPlaying || m_animNo == m_data[m_type].initAnim || m_animNo == m_data[m_type].moveAnim )
	{
		// not playing an attack anim, hence choose one

		u8 newAction = getRnd() % 3;

		switch( newAction )
		{
			case 0:
			{
				m_animNo = ANIM_BABYOCTOPUS_HEAD_BUTT;

				break;
			}

			case 1:
			{
				m_animNo = ANIM_BABYOCTOPUS_HIT;

				break;
			}

			case 2:
			{
				m_animNo = ANIM_BABYOCTOPUS_INK;

				break;
			}
		}

		m_animPlaying = true;
		m_frame = 0;

		m_controlFunc = NPC_CONTROL_MOVEMENT;
		m_timerFunc = NPC_TIMER_ATTACK_DONE;
		m_timerTimer = GameState::getOneSecondInFrames();
		m_sensorFunc = NPC_SENSOR_NONE;
	}
}
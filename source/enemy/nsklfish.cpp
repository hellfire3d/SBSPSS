/*=========================================================================

	nsklfish.cpp

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/

#ifndef __ENEMY_NPC_H__
#include "enemy\npc.h"
#endif

#ifndef	__ENEMY_NSKLFISH_H__
#include "enemy\nsklfish.h"
#endif

#ifndef __GAME_GAME_H__
#include "game\game.h"
#endif

#ifndef	__PLAYER_PLAYER_H__
#include "player\player.h"
#endif

#ifndef __ANIM_SKELETALFISH_HEADER__
#include <ACTOR_SKELETALFISH_ANIM.h>
#endif


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool CNpcSkeletalFishEnemy::processSensor()
{
	switch( m_sensorFunc )
	{
		case NPC_SENSOR_NONE:
			return( false );

		default:
			{
				if ( playerXDistSqr + playerYDistSqr < 10000 )
				{
					s32 xDistWaypoint, yDistWaypoint;

					m_npcPath.getDistToNextWaypoint( Pos, &xDistWaypoint, &yDistWaypoint );

					if ( abs( xDistWaypoint ) >= abs( playerXDist ) )
					{
						m_controlFunc = NPC_CONTROL_CLOSE;
						m_velocity = 0;
						m_chargeTime = 0;
						m_sensorFunc = NPC_SENSOR_NONE;

						return( true );
					}
					else
					{
						return( false );
					}
				}
				else
				{
					return( false );
				}

				break;
			}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcSkeletalFishEnemy::processClose( int _frames )
{
	s32 moveX, moveY;
	s16 decDir, incDir, moveDist;

	if ( m_chargeTime > GameState::getOneSecondInFrames() )
	{
		if ( m_velocity < SKELETAL_FISH_CHARGE_VELOCITY )
		{
			m_velocity++;
		}
	}
	else
	{
		m_chargeTime += _frames;
	}

	s16 headingToPlayer = ratan2( playerYDist, playerXDist ) & 4095;

	if ( !m_animPlaying )
	{
		m_animPlaying = true;
		m_animNo = ANIM_SKELETALFISH_SWIM;
		m_frame = 0;
	}

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

	if ( ( playerXDistSqr + playerYDistSqr > 100 ) && abs( xDistWaypoint ) >= abs( playerXDist ) )
	{
		// continue charge

		processGenericGotoTarget( _frames, playerXDist, playerYDist, m_velocity );
	}
	else
	{
		m_controlFunc = NPC_CONTROL_MOVEMENT;
		m_timerFunc = NPC_TIMER_ATTACK_DONE;
		m_chargeTime = m_timerTimer = GameState::getOneSecondInFrames();
	}

	if ( m_soundId == NOT_PLAYING )
	{
		m_soundId = (int) CSoundMediator::playSfx( CSoundMediator::SFX_FISH_FOLK_MOVE_2, true );
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

s32 CNpcSkeletalFishEnemy::getFrameShift( int _frames )
{
	if ( m_chargeTime < GameState::getOneSecondInFrames() )
	{
		return( _frames << 8 );
	}
	else
	{
		return( ( _frames << 8 ) >> 2 );
	}
}

/*=========================================================================

	nhcrab.cpp

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __ENEMY_NPC_H__
#include "enemy\npc.h"
#endif

#ifndef __GAME_GAME_H__
#include "game\game.h"
#endif

#ifndef	__UTILS_HEADER__
#include	"utils\utils.h"
#endif

#ifndef __ANIM_HERMITCRAB_HEADER__
#include <ACTOR_HERMITCRAB_ANIM.h>
#endif

void CNpcEnemy::processCloseHermitCrabAttack( int _frames )
{
	if ( m_state == HERMIT_CRAB_NO_ATTACK )
	{
		m_state = ( getRnd() % 2 ) + 1;
	}

	switch( m_state )
	{
		case HERMIT_CRAB_PUNCH_ATTACK:
		{
			if ( m_animNo != ANIM_HERMITCRAB_PUNCH )
			{
				m_animPlaying = true;
				m_animNo = ANIM_HERMITCRAB_PUNCH;
				m_frame = 0;
			}
			else if ( !m_animPlaying )
			{
				m_state = HERMIT_CRAB_NO_ATTACK;
				m_controlFunc = NPC_CONTROL_MOVEMENT;
				m_timerFunc = NPC_TIMER_ATTACK_DONE;
				m_timerTimer = GameState::getOneSecondInFrames();
				m_sensorFunc = NPC_SENSOR_NONE;
			}

			break;
		}

		case HERMIT_CRAB_ROLL_ATTACK:
		{
			if ( !m_animPlaying )
			{
				switch( m_animNo )
				{
					case ANIM_HERMITCRAB_ROLLATTACKSTART:
					{
						m_animPlaying = true;
						m_animNo = ANIM_HERMITCRAB_ROLLATTACK;
						m_frame = 0;

						break;
					}

					case ANIM_HERMITCRAB_ROLLATTACK:
					{
						m_animPlaying = true;
						m_animNo = ANIM_HERMITCRAB_ROLLATTACKEND;
						m_frame = 0;

						break;
					}

					case ANIM_HERMITCRAB_ROLLATTACKEND:
					{
						m_state = HERMIT_CRAB_NO_ATTACK;
						m_controlFunc = NPC_CONTROL_MOVEMENT;
						m_timerFunc = NPC_TIMER_ATTACK_DONE;
						m_timerTimer = GameState::getOneSecondInFrames();
						m_sensorFunc = NPC_SENSOR_NONE;

						break;
					}

					default:
					{
						m_animPlaying = true;
						m_animNo = ANIM_HERMITCRAB_ROLLATTACKSTART;
						m_frame = 0;

						if ( playerXDist > 0 )
						{
							m_extendDir = EXTEND_RIGHT;
						}
						else
						{
							m_extendDir = EXTEND_LEFT;
						}

						break;
					}
				}
			}

			if ( m_animNo == ANIM_HERMITCRAB_ROLLATTACK )
			{
				// check for vertical movement

				s32 moveX = 0, moveY = 0;
				s32 groundHeight;
				s32 fallSpeed = 2;
				s8 yMovement = fallSpeed * _frames;

				moveX = 6 * _frames;

				if ( m_extendDir == EXTEND_LEFT )
				{
					moveX = -moveX;
				}

				groundHeight = m_layerCollision->getHeightFromGround( Pos.vx, Pos.vy, yMovement + 16 );

				if ( groundHeight <= yMovement )
				{
					// groundHeight <= yMovement indicates either just above ground or on or below ground

					moveY = groundHeight;
				}

				Pos.vx += moveX;
				Pos.vy += moveY;
			}
		}

		default:
			break;
	}
}
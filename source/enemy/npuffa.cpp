/*=========================================================================

	npuffa.cpp

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __ENEMY_NPC_H__
#include "enemy\npc.h"
#endif

#ifndef	__ENEMY_NPUFFA_H__
#include "enemy\npuffa.h"
#endif

#ifndef __GAME_GAME_H__
#include	"game\game.h"
#endif

#ifndef __ANIM_PUFFAFISH_HEADER__
#include <ACTOR_PUFFAFISH_ANIM.h>
#endif


bool CNpcPuffaFishEnemy::processSensor()
{
	switch( m_sensorFunc )
	{
		case NPC_SENSOR_NONE:
			return( false );

		default:
			{
				if ( playerXDistSqr + playerYDistSqr < 10000 )
				{
					m_state = PUFFA_FISH_NO_INFLATE;
					m_controlFunc = NPC_CONTROL_CLOSE;

					return( true );
				}
				else
				{
					return( false );
				}
			}
	}
}

void CNpcPuffaFishEnemy::processClose( int _frames )
{
	if ( playerXDistSqr + playerYDistSqr > 15000 )
	{
		if ( !m_animPlaying )
		{
			switch( m_state )
			{
				case PUFFA_FISH_NO_INFLATE:
				{
					m_animNo = ANIM_PUFFAFISH_SWIM;

					m_controlFunc = NPC_CONTROL_MOVEMENT;
					m_timerFunc = NPC_TIMER_ATTACK_DONE;
					m_timerTimer = GameState::getOneSecondInFrames();
					m_sensorFunc = NPC_SENSOR_NONE;

					break;
				}

				case PUFFA_FISH_TURN:
				{
					// reverse turn at some point

					m_state = PUFFA_FISH_NO_INFLATE;
					m_animNo = ANIM_PUFFAFISH_TURN;

					break;
				}

				case PUFFA_FISH_INFLATE:
				{
					// reverse puff up

					m_state = PUFFA_FISH_TURN;
//					m_animNo = ANIM_PUFFAFISH_PUFFUP;
					m_animNo = ANIM_PUFFAFISH_PUFFUPIDLE;	// Changed by dave, cos anims are the same

					break;
				}
			}

			m_frame = 0;
			m_animPlaying = true;
		}
	}
	else if ( !m_animPlaying )
	{
		switch( m_state )
		{
			case PUFFA_FISH_NO_INFLATE:
			{
				m_state = PUFFA_FISH_TURN;
				m_animNo = ANIM_PUFFAFISH_TURN;

				break;
			}

			case PUFFA_FISH_TURN:
			{
				m_state = PUFFA_FISH_INFLATE;
//				m_animNo = ANIM_PUFFAFISH_PUFFUP;
				m_animNo = ANIM_PUFFAFISH_PUFFUPIDLE;	// Changed by dave, cos anims are the same

				break;
			}

			case PUFFA_FISH_INFLATE:
			{
				m_animNo = ANIM_PUFFAFISH_PUFFUPIDLE;

				break;
			}
		}

		m_frame = 0;
		m_animPlaying = true;
	}
}
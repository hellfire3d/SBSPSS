/*=========================================================================

	nfdutch.cpp

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/

#ifndef __ENEMY_NPC_H__
#include "enemy\npc.h"
#endif

#ifndef	__ENEMY_NFDUTCH_H__
#include "enemy\nfdutch.h"
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

#ifndef	__ANIM_FLYINGDUTCHMAN_HEADER__
#include <ACTOR_FLYINGDUTCHMAN_ANIM.h>
#endif

void CNpcFlyingDutchmanEnemy::postInit()
{
	m_state = FLYING_DUTCHMAN_ATTACK_PLAYER_1;
	m_extendDir = EXTEND_UP;

	s32 minX, maxX;
	m_npcPath.getPathXExtents( &minX, &maxX );
	m_npcPath.getPathYExtents( &m_minY, &m_maxY );

	m_extension = minX;
}

void CNpcFlyingDutchmanEnemy::processMovement( int _frames )
{
	if ( !m_animPlaying )
	{
		m_animPlaying = true;
		m_animNo = m_data[m_type].moveAnim;
		m_frame = 0;
	}

	if ( m_movementTimer > 0 )
	{
		m_movementTimer -= _frames;

		s32 xDist = m_extension - Pos.vx;
		s32 xDistSqr = xDist * xDist;

		if ( xDistSqr > 100 )
		{
			processGenericGotoTarget( _frames, xDist, 0, m_speed );
		}
		else
		{
			if ( m_extendDir == EXTEND_UP )
			{
				s32 yDist = m_minY - Pos.vy;
				s32 yDistSqr = yDist * yDist;

				if ( yDistSqr > 100 )
				{
					processGenericGotoTarget( _frames, 0, yDist, m_speed );
				}
				else
				{
					m_extendDir = EXTEND_DOWN;
				}
			}
			else
			{
				s32 yDist = m_maxY - Pos.vy;
				s32 yDistSqr = yDist * yDist;

				if ( yDistSqr > 100 )
				{
					processGenericGotoTarget( _frames, 0, yDist, m_speed );
				}
				else
				{
					m_extendDir = EXTEND_UP;
				}
			}
		}
	}
	else
	{
		m_controlFunc = NPC_CONTROL_CLOSE;
	}

	if ( playerXDist > 0 )
	{
		m_heading = 0;
	}
	else
	{
		m_heading = 2048;
	}
}

void CNpcFlyingDutchmanEnemy::processClose( int _frames )
{
	if ( playerYDist > 0 )
	{
		m_extendDir = EXTEND_DOWN;
	}
	else
	{
		m_extendDir = EXTEND_UP;
	}

	if ( playerYDistSqr > 100 )
	{
		if ( !m_animPlaying )
		{
			m_animPlaying = true;
			m_animNo = m_data[m_type].moveAnim;
			m_frame = 0;
		}

		processGenericGotoTarget( _frames, 0, playerYDist, m_speed );
	}
	else
	{
		switch( m_state )
		{
			case FLYING_DUTCHMAN_ATTACK_PLAYER_1:
			case FLYING_DUTCHMAN_ATTACK_PLAYER_2:
			{
				if ( m_animNo != ANIM_FLYINGDUTCHMAN_FIREATTACK )
				{
					m_animNo = ANIM_FLYINGDUTCHMAN_FIREATTACK;
					m_animPlaying = true;
					m_frame = 0;
				}
				else if ( !m_animPlaying )
				{
					// fire at player

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

					m_controlFunc = NPC_CONTROL_MOVEMENT;
					m_movementTimer = GameState::getOneSecondInFrames() * 3;

					m_state++;
				}

				break;
			}

			default:
			{
				if ( !m_animPlaying )
				{
					m_animPlaying = true;
					m_animNo = m_data[m_type].moveAnim;
					m_frame = 0;
				}

				// charge player

				if ( playerXDistSqr + playerYDistSqr > 100 )
				{
					processGenericGotoTarget( _frames, playerXDist, playerYDist, 6 );
				}
				else
				{
					m_controlFunc = NPC_CONTROL_MOVEMENT;
					m_movementTimer = GameState::getOneSecondInFrames() * 3;
					m_state = FLYING_DUTCHMAN_ATTACK_PLAYER_1;

					s32 minX, maxX;
					m_npcPath.getPathXExtents( &minX, &maxX );

					if ( m_extension == minX )
					{
						m_extension = maxX;
					}
					else
					{
						m_extension = minX;
					}
				}

				break;
			}
		}
	}

	if ( playerXDist > 0 )
	{
		m_heading = 0;
	}
	else
	{
		m_heading = 2048;
	}
}
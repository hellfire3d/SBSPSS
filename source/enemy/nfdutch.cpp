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

#ifndef __GAME_GAME_H__
#include	"game\game.h"
#endif

#ifndef	__PLAYER_PLAYER_H__
#include	"player\player.h"
#endif

#ifndef __PROJECTL_PROJECTL_H__
#include "projectl\projectl.h"
#endif


void CNpc::processFlyingDutchmanMovement( int _frames )
{
	if ( m_movementTimer > 0 )
	{
		m_movementTimer -= _frames;

		if ( m_extendDir == EXTEND_UP )
		{
			s32 yDist = -10 - Pos.vy;
			s32 yDistSqr = yDist * yDist;

			if ( yDistSqr > 100 )
			{
				processGenericGotoTarget( _frames, 0, yDist, m_data[m_type].speed );
			}
			else
			{
				m_extendDir = EXTEND_DOWN;
			}
		}
		else
		{
			s32 yDist = 400 - Pos.vy;
			s32 yDistSqr = yDist * yDist;

			if ( yDistSqr > 100 )
			{
				processGenericGotoTarget( _frames, 0, yDist, m_data[m_type].speed );
			}
			else
			{
				m_extendDir = EXTEND_UP;
			}
		}
	}
	else
	{
		m_controlFunc = NPC_CONTROL_CLOSE;
	}
}

void CNpc::processCloseFlyingDutchmanAttack( int _frames )
{
	CPlayer *player = GameScene.getPlayer();
	DVECTOR playerPos = player->getPos();

	s32 yDist = playerPos.vy - Pos.vy;
	s32 yDistSqr = yDist * yDist;

	if ( yDist > 0 )
	{
		m_extendDir = EXTEND_DOWN;
	}
	else
	{
		m_extendDir = EXTEND_UP;
	}

	if ( yDistSqr > 100 )
	{
		processGenericGotoTarget( _frames, 0, yDist, m_data[m_type].speed );
	}
	else
	{
		switch( m_state )
		{
			case FLYING_DUTCHMAN_ATTACK_PLAYER_1:
			case FLYING_DUTCHMAN_ATTACK_PLAYER_2:
			{
				// fire at player

				s16 heading;

				if ( playerPos.vx - Pos.vx > 0 )
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

				break;
			}

			default:
			{
				// charge player

				s32 xDist = playerPos.vx - Pos.vx;
				s32 xDistSqr = xDist * xDist;

				if ( xDistSqr + yDistSqr > 100 )
				{
					processGenericGotoTarget( _frames, xDist, yDist, 6 );
				}
				else
				{
					m_controlFunc = NPC_CONTROL_MOVEMENT;
					m_movementTimer = GameState::getOneSecondInFrames() * 3;
					m_state = FLYING_DUTCHMAN_ATTACK_PLAYER_1;
				}

				break;
			}
		}
	}
}
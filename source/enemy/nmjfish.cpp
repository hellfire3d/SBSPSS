/*=========================================================================

	nnjfish.cpp

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


void CNpcEnemy::processMotherJellyfishMovement( int _frames )
{
	s32 xDist, yDist;
	s32 xDistSqr, yDistSqr;

	switch( m_state )
	{
		case MOTHER_JELLYFISH_RETURN_TO_START:
		{
			xDist = m_base.vx - this->Pos.vx;
			xDistSqr = xDist * xDist;
			yDist = m_base.vy - this->Pos.vy;
			yDistSqr = yDist * yDist;

			if ( xDistSqr + yDistSqr > 100 )
			{
				processGenericGotoTarget( _frames, xDist, yDist, m_data[m_type].speed );
			}
			else
			{
				// have arrived at base position

				m_movementTimer = GameState::getOneSecondInFrames() * 5;
				m_state = MOTHER_JELLYFISH_CYCLE;
				m_extension = 0;
				m_extendDir = EXTEND_RIGHT;
			}

			break;
		}

		case MOTHER_JELLYFISH_CYCLE:
		{
			if ( m_movementTimer > 0 )
			{
				m_movementTimer -= _frames;

				if ( m_extendDir == EXTEND_RIGHT )
				{
					if ( m_extension < 200 )
					{
						m_extension += 3 * _frames;

						Pos.vx = m_base.vx + m_extension;

						m_heading = 0;
					}
					else
					{
						m_extendDir = EXTEND_LEFT;
					}
				}
				else
				{
					if ( m_extension > -200 )
					{
						m_extension -= 3 * _frames;

						Pos.vx = m_base.vx + m_extension;

						m_heading = 2048;
					}
					else
					{
						m_extendDir = EXTEND_RIGHT;
					}
				}
			}
			else
			{
				m_controlFunc = NPC_CONTROL_CLOSE;
				m_state = MOTHER_JELLYFISH_ATTACK_PLAYER;
			}

			break;
		}

		default:
			break;
	}
}

void CNpcEnemy::processCloseMotherJellyfishAttack( int _frames )
{
	// seek position above user

	CPlayer *player = GameScene.getPlayer();
	DVECTOR playerPos = player->getPos();
	DVECTOR seekPos;
	s32 xDist, yDist;
	s32 xDistSqr, yDistSqr;

	seekPos = playerPos;
	seekPos.vy -= 100;

	xDist = seekPos.vx - this->Pos.vx;
	xDistSqr = xDist * xDist;
	yDist = seekPos.vy - this->Pos.vy;
	yDistSqr = yDist * yDist;

	if ( xDistSqr + yDistSqr > 400 )
	{
		processGenericGotoTarget( _frames, xDist, yDist, m_data[m_type].speed );
	}
	else
	{
		// fire at user

		m_controlFunc = NPC_CONTROL_MOVEMENT;
		m_state = MOTHER_JELLYFISH_RETURN_TO_START;
	}
}
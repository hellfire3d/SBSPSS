/*=========================================================================

	npbug.cpp

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __ENEMY_NPBUG_H__
#include "enemy\npbug.h"
#endif

#ifndef __LAYER_COLLISION_H__
#include	"level\layercollision.h"
#endif

#ifndef __VID_HEADER_
#include "system\vid.h"
#endif

#include	"game/game.h"


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcPricklyBugEnemy::processMovementModifier( int _frames, s32 distX, s32 distY, s32 dist, s16 headingChange )
{
	Pos.vx += distX;
	Pos.vy += distY;

	// sort out draw rotation

	DVECTOR testPos1, testPos2;

	testPos1 = testPos2 = Pos;
	testPos1.vx -= 10;
	testPos2.vx += 10;

	testPos1.vy += CGameScene::getCollision()->getHeightFromGround( testPos1.vx, testPos1.vy, 16 );
	testPos2.vy += CGameScene::getCollision()->getHeightFromGround( testPos2.vx, testPos2.vy, 16 );

	s32 xDist = testPos2.vx - testPos1.vx;
	s32 yDist = testPos2.vy - testPos1.vy;

	s16 heading = ratan2( yDist, xDist );

	m_drawRotation = heading;

	// sound

	//CSoundMediator::playSfx( CSoundMediator::SFX_PRICKLY_BUG_MOVE );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcPricklyBugEnemy::processShot( int _frames )
{
	switch ( m_state )
	{
		case NPC_GENERIC_HIT_CHECK_HEALTH:
		{
			m_health -= 5;

			if ( m_health < 0 )
			{
				m_state = NPC_GENERIC_HIT_DEATH_START;

				m_animPlaying = true;
				m_animNo = m_data[m_type].dieAnim;
				m_frame = 0;
			}
			else
			{
				m_state = NPC_GENERIC_HIT_RECOIL;

				m_animPlaying = true;
				m_animNo = m_data[m_type].recoilAnim;
				m_frame = 0;
			}

			break;
		}

		case NPC_GENERIC_HIT_RECOIL:
		{
			if ( !m_animPlaying )
			{
				m_state = 0;
				m_controlFunc = NPC_CONTROL_MOVEMENT;
			}

			break;
		}

		case NPC_GENERIC_HIT_DEATH_START:
		{
			if ( !m_animPlaying )
			{
				m_state = NPC_GENERIC_HIT_DEATH_END;

				if ( m_data[m_type].deathSfx < CSoundMediator::NUM_SFXIDS )
				{
					CSoundMediator::playSfx( m_data[m_type].deathSfx );
				}

				m_isDying = true;
				m_speed = -5;

				if (m_data[m_type].skelType)
				{
					m_actorGfx->SetOtPos( 0 );
				}
			}

			break;
		}

		case NPC_GENERIC_HIT_DEATH_END:
		{
			m_drawRotation += 64 * _frames;
			m_drawRotation &= 4095;

			Pos.vy += m_speed * _frames;

			if ( m_speed < 5 )
			{
				m_speed++;
			}

			DVECTOR	offset = CLevel::getCameraPos();

			if ( Pos.vy - offset.vy > VidGetScrH() )
			{
				if ( m_data[m_type].respawning )
				{
					m_isActive = false;

					m_timerFunc = NPC_TIMER_RESPAWN;
					m_timerTimer = 4 * GameState::getOneSecondInFrames();
				}
				else
				{
					setToShutdown();
				}
			}

			break;
		}
	}
}

/*=========================================================================

	nsstomp.cpp

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/

#ifndef __ENEMY_NPC_H__
#include "enemy\npc.h"
#endif

#ifndef	__ENEMY_NSSTOMP_H__
#include "enemy\nsstomp.h"
#endif

#ifndef __GAME_GAME_H__
#include "game\game.h"
#endif

#ifndef	__PLAYER_PLAYER_H__
#include "player\player.h"
#endif

#ifndef __PAD_VIBE_H__
#include "pad\vibe.h"
#endif

#ifndef __ANIM_STOMPER_HEADER__
#include <ACTOR_STOMPER_ANIM.h>
#endif

#include "fx\fx.h"

#ifndef	__UTILS_HEADER__
#include	"utils\utils.h"
#endif


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcSkullStomperEnemy::processEnemyCollision( CThing *thisThing )
{
	// do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcSkullStomperEnemy::postInit()
{
	m_extendDir = EXTEND_DOWN;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool CNpcSkullStomperEnemy::processSensor()
{
	switch( m_sensorFunc )
	{
		case NPC_SENSOR_NONE:
			return( false );

		default:
		{
			if ( playerXDistSqr < 10000 && playerYDist >= 0 )
			{
				m_controlFunc = NPC_CONTROL_CLOSE;
				m_extendDir = EXTEND_DOWN;

				if( m_soundId != NOT_PLAYING )
				{
					CSoundMediator::stopAndUnlockSfx( (xmPlayingId) m_soundId );
				}

				m_soundId = (int) CSoundMediator::playSfx( CSoundMediator::SFX_SKULL_ATTACK, true );

				return( true );
			}
			else
			{
				return( false );
			}
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcSkullStomperEnemy::processClose( int _frames )
{
	s8 groundHeight;
	s8 yMovement;

	if ( !m_animPlaying )
	{
		m_animPlaying = true;
		m_animNo = ANIM_STOMPER_CRUSH;
		m_frame = 0;
	}

	if ( m_timerTimer > 0 )
	{
		// wait
	}
	else
	{
		if ( m_extendDir == EXTEND_DOWN )
		{
			yMovement = m_data[m_type].speed * _frames;

			groundHeight = CGameScene::getCollision()->getHeightFromGround( Pos.vx, Pos.vy, yMovement + 16 );

			if ( groundHeight < yMovement )
			{
				// colliding with ground

				if( m_soundId != NOT_PLAYING )
				{
					CSoundMediator::stopAndUnlockSfx( (xmPlayingId) m_soundId );
				}

				m_soundId = (int) CSoundMediator::playSfx( CSoundMediator::SFX_HAZARD__FALLING_ROCK_LAND, true );

				Pos.vy += groundHeight;

				// pause and change direction

				m_timerTimer = GameState::getOneSecondInFrames();
				m_extendDir = EXTEND_UP;

				for ( int i = 0 ; i < 20 ; i++ )
				{
					DVECTOR bubblePos = Pos;
					bubblePos.vx += 20 - ( getRnd() % 41 );
					CFX::Create( CFX::FX_TYPE_BUBBLE_WATER, bubblePos );
				}

				CGameScene::setCameraShake(0,8);
				CPadVibrationManager::setVibration(0,CPadVibrationManager::VIBE_SHORT);
			}
			else
			{
				// drop down

				Pos.vy += yMovement;
			}
		}
		else
		{
			if ( m_base.vx - Pos.vx == 0 && m_base.vy - Pos.vy == 0 )
			{
				m_controlFunc = NPC_CONTROL_MOVEMENT;
				m_timerFunc = NPC_TIMER_ATTACK_DONE;
				m_timerTimer = GameState::getOneSecondInFrames();
				m_sensorFunc = NPC_SENSOR_NONE;
				m_npcPath.resetPath();
			}
			else
			{
				// return to original position

				processGenericGotoTarget( _frames, m_base.vx - Pos.vx, m_base.vy - Pos.vy, 1 );
			}
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcSkullStomperEnemy::collidedWith( CThing *_thisThing )
{
	if ( m_isActive && !m_isCaught && !m_isDying )
	{
		switch(_thisThing->getThingType())
		{
			case TYPE_PLAYER:
			{
				CPlayer *player = (CPlayer *) _thisThing;

				ATTACK_STATE playerState = player->getAttackState();

				if(playerState==ATTACK_STATE__NONE)
				{
					if ( !player->isRecoveringFromHit() )
					{
						CPlayer *player = GameScene.getPlayer();

						player->takeDamage( m_data[m_type].damageToUserType,REACT__GET_DIRECTION_FROM_THING,(CThing*)this );
					}
				}
				else
				{
					drawAttackEffect();
				}

				break;
			}

			case TYPE_ENEMY:
				break;

			default:
				ASSERT(0);
				break;
		}
	}
}
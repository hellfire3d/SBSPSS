/*=========================================================================

	nsdart.cpp

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __ENEMY_NPC_H__
#include "enemy\npc.h"
#endif

#ifndef __ENEMY_NSDART_H__
#include "enemy\nsdart.h"
#endif

#ifndef __GAME_GAME_H__
#include	"game\game.h"
#endif

#ifndef __VID_HEADER_
#include "system\vid.h"
#endif

#ifndef __PROJECTL_PRNPCSPR_H__
#include "projectl\prnpcspr.h"
#endif

#ifndef	__UTILS_HEADER__
#include	"utils\utils.h"
#endif


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcSquidDartEnemy::render()
{
	SprFrame = NULL;

	if ( m_isActive )
	{
		CEnemyThing::render();

		if (canRender())
		{
			DVECTOR &renderPos=getRenderPos();
			
			// bodge
			if ( m_reversed )
			{
				renderPos.vx += 32;
			}
			else
			{
				renderPos.vx -= 32;
			}

			//renderPos.vy -= 6;
			// bodge

			int frame = FRM_SQUIDDART_SWIM0001 + ( m_frame >> 8 );

			SprFrame = CGameScene::getSpriteBank()->printFT4(frame,renderPos.vx,renderPos.vy,m_reversed,0,0);

			//setRGB0( SprFrame, 255, 128, 255 );

			/*s32 XMax = SprFrame->x1 - origRenderPos.vx;
			s32 XMin = SprFrame->x0 - origRenderPos.vx;

			s32 YMax = SprFrame->y2 - origRenderPos.vy;
			s32 YMin = SprFrame->y0 - origRenderPos.vy;*/

			s32 XMax = SprFrame->x1 - renderPos.vx;
			s32 XMin = SprFrame->x0 - renderPos.vx;

			s32 YMax = SprFrame->y2 - renderPos.vy;
			s32 YMin = SprFrame->y0 - renderPos.vy;

			setCollisionSize( ( XMax - XMin ), ( YMax - YMin ) );
			// bodge
			//setCollisionCentreOffset( 0, 0 ); //( XMax + XMin ) >> 1, ( YMax + YMin ) >> 1 );
			setCollisionCentreOffset( 0, ( YMax + YMin ) >> 1 ); //( XMax + XMin ) >> 1, ( YMax + YMin ) >> 1 );
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcSquidDartEnemy::fireAsProjectile( s16 heading )
{
	m_isActive = false;
	setToShutdown();

	DVECTOR newPos = Pos;

	newPos.vy -= 10;

	CEnemyAsSpriteProjectile *projectile;
	projectile = CEnemyAsSpriteProjectile::Create();
	projectile->init(	newPos,
						heading,
						CPlayerProjectile::PLAYER_PROJECTILE_DUMBFIRE,
						CPlayerProjectile::PLAYER_PROJECTILE_FINITE_LIFE,
						5*60);

	projectile->setGraphic( FRM_SQUIDDART_SWIM0001 );
	projectile->setHasRGB( true );
	projectile->setRGB( 255, 128, 255 );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

s32 CNpcSquidDartEnemy::getFrameShift( int _frames )
{
	return( ( _frames << 8 ) >> 2 );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool CNpcSquidDartEnemy::processSensor()
{
	switch( m_sensorFunc )
	{
		case NPC_SENSOR_NONE:
			return( false );

		default:
		{
			if ( playerXDistSqr + playerYDistSqr < 20000 )
			{
				m_controlFunc = NPC_CONTROL_CLOSE;

				m_attack = false;

				// sound

				if( m_soundId != NOT_PLAYING )
				{
					CSoundMediator::stopAndUnlockSfx( (xmPlayingId) m_soundId );
					m_soundId = NOT_PLAYING;
				}

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

void CNpcSquidDartEnemy::processClose( int _frames )
{
	s32 movement;
	s32 yAim = playerYDist - 20;
	//s32 maxSpeed = m_data[m_type].speed * _frames;
	s32 maxSpeed = m_speed * _frames;

	if ( m_attack )
	{
		movement = m_xPos - Pos.vx;

		if ( movement )
		{
			if ( movement > maxSpeed )
			{
				movement = maxSpeed;
			}
			else if ( movement < -maxSpeed )
			{
				movement = -maxSpeed;
			}

			Pos.vx += movement;

			if( m_soundId != NOT_PLAYING )
			{
				m_soundId = (int) CSoundMediator::playSfx( CSoundMediator::SFX_SQUIDDART_ATTACK, true );
			}
		}
		else
		{
			m_controlFunc = NPC_CONTROL_MOVEMENT;
			m_timerFunc = NPC_TIMER_ATTACK_DONE;
			m_timerTimer = GameState::getOneSecondInFrames();
			m_sensorFunc = NPC_SENSOR_NONE;
		}
	}
	else
	{
		if ( abs( yAim ) > 5 )
		{
			movement = yAim;
			
			if ( movement > maxSpeed )
			{
				movement = maxSpeed;
			}
			else if ( movement < -maxSpeed )
			{
				movement = -maxSpeed;
			}

			Pos.vy += movement;
		}
		else
		{
			m_attack = true;
			m_xPos = Pos.vx + playerXDist;
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

	if ( !m_animPlaying )
	{
		m_frame = 0;
		m_animNo = m_data[m_type].moveAnim;
		m_animPlaying = true;

	}
}
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

			renderPos.vy -= 6;
			// bodge

			int frame = FRM_SQUIDDART_SWIM0001 + ( m_frame >> 8 );

			SprFrame = m_spriteBank->printFT4(frame,renderPos.vx,renderPos.vy,m_reversed,0,10);

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
			setCollisionCentreOffset( 0, 0 ); //( XMax + XMin ) >> 1, ( YMax + YMin ) >> 1 );
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
	projectile = new( "blower projectile" ) CEnemyAsSpriteProjectile;
	projectile->init(	newPos,
						heading,
						CPlayerProjectile::PLAYER_PROJECTILE_DUMBFIRE,
						CPlayerProjectile::PLAYER_PROJECTILE_FINITE_LIFE,
						5*60);
	projectile->setLayerCollision( m_layerCollision );
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

				m_circleCentre.vx = Pos.vx + ( playerXDist >> 1 );
				m_circleCentre.vy = Pos.vy + ( playerYDist >> 1 );

				m_startAngle = ratan2( Pos.vy - m_circleCentre.vy, Pos.vx - m_circleCentre.vx );
				m_startAngle &= 4095;

				m_circleRadius = isqrt2( playerXDistSqr + playerYDistSqr ) >> 1;

				m_circleCentre.vx = Pos.vx - ( ( m_circleRadius * rcos( m_startAngle ) ) >> 12 );
				m_circleCentre.vy = Pos.vy - ( ( m_circleRadius * rsin( m_startAngle ) ) >> 12 );

				m_angularDistance = 0;

				// sound

				CSoundMediator::playSfx( CSoundMediator::SFX_SQUIDDART_ATTACK );

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
	s16 m_currentAngle;

	m_currentAngle = ( m_startAngle + m_angularDistance ) & 4095;

	m_heading = ( m_currentAngle + 1024 ) & 4095;

	Pos.vx = m_circleCentre.vx + ( ( m_circleRadius * rcos( m_currentAngle ) ) >> 12 );
	Pos.vy = m_circleCentre.vy + ( ( m_circleRadius * rsin( m_currentAngle ) ) >> 12 );

	m_angularDistance += 32 * _frames;

	if ( m_angularDistance > 4095 )
	{
		m_controlFunc = NPC_CONTROL_MOVEMENT;
		m_timerFunc = NPC_TIMER_ATTACK_DONE;
		m_timerTimer = GameState::getOneSecondInFrames();
		m_sensorFunc = NPC_SENSOR_NONE;
	}

	if ( !m_animPlaying )
	{
		m_frame = 0;
		m_animNo = m_data[m_type].moveAnim;
		m_animPlaying = true;
	}
}
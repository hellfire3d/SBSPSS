/*=========================================================================

	nsjfish.cpp

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/

#ifndef __ENEMY_NPC_H__
#include "enemy\npc.h"
#endif

#ifndef __ENEMY_NSJFISH_H__
#include "enemy\nsjfish.h"
#endif

#ifndef	__UTILS_HEADER__
#include "utils\utils.h"
#endif

#ifndef __GAME_GAME_H__
#include	"game\game.h"
#endif

#ifndef	__PLAYER_PLAYER_H__
#include	"player\player.h"
#endif

#ifndef __VID_HEADER_
#include "system\vid.h"
#endif

void CNpcSmallJellyfishEnemy::render()
{
	SprFrame = NULL;

	if ( m_isActive )
	{
		CEnemyThing::render();

		// Render
		DVECTOR renderPos;
		DVECTOR origRenderPos;
		DVECTOR	offset = CLevel::getCameraPos();
		int		spriteWidth = m_spriteBank->getFrameWidth( m_frame >> 8 );
		int		spriteHeight = m_spriteBank->getFrameHeight( m_frame >> 8 );

		renderPos.vx = Pos.vx - offset.vx;
		origRenderPos.vx = renderPos.vx;
		renderPos.vx += m_drawOffset.vx;

		renderPos.vy = Pos.vy - offset.vy;
		origRenderPos.vy = renderPos.vy;
		renderPos.vy += m_drawOffset.vy - ( spriteHeight >> 1 );

		if ( renderPos.vx >= 0 && renderPos.vx <= VidGetScrW() )
		{
			if ( renderPos.vy >= 0 && renderPos.vy <= VidGetScrH() )
			{
				SprFrame = m_spriteBank->printRotatedScaledSprite( m_frame>>8,renderPos.vx,renderPos.vy,4096,4096,m_drawRotation,10);
				setRGB0( SprFrame, 255, 128, 255 );

				// get xmax, xmin, ymax, ymin

				s32 XMax;
				s32 XMin;

				s32 YMax;
				s32 YMin;

				XMin=SprFrame->x0;
				if (XMin>SprFrame->x1) XMin=SprFrame->x1;
				if (XMin>SprFrame->x2) XMin=SprFrame->x2;
				if (XMin>SprFrame->x3) XMin=SprFrame->x3;
				XMax=SprFrame->x0;
				if (XMax<SprFrame->x1) XMax=SprFrame->x1;
				if (XMax<SprFrame->x2) XMax=SprFrame->x2;
				if (XMax<SprFrame->x3) XMax=SprFrame->x3;
				YMin=SprFrame->y0;
				if (YMin>SprFrame->y1) YMin=SprFrame->y1;
				if (YMin>SprFrame->y2) YMin=SprFrame->y2;
				if (YMin>SprFrame->y3) YMin=SprFrame->y3;
				YMax=SprFrame->y0;
				if (YMax<SprFrame->y1) YMax=SprFrame->y1;
				if (YMax<SprFrame->y2) YMax=SprFrame->y2;
				if (YMax<SprFrame->y3) YMax=SprFrame->y3;

				XMax -= origRenderPos.vx;
				XMin -= origRenderPos.vx;
				YMax -= origRenderPos.vy;
				YMin -= origRenderPos.vy;

				setCollisionSize( ( XMax - XMin ), ( YMax - YMin ) );
				setCollisionCentreOffset( ( XMax + XMin ) >> 1, ( YMax + YMin ) >> 1 );
			}
		}
	}
}

void CNpcSmallJellyfishEnemy::processClose( int _frames )
{
	s32 moveX = 0, moveY = 0;

	s16 moveDist = 0;

	s32 moveVel = 0;

	if ( playerXDistSqr + playerYDistSqr > 10000 )
	{
		this->m_controlFunc = NPC_CONTROL_MOVEMENT;
	}
	else
	{
		bool pathComplete;
		bool waypointChange;

		s16 headingToTarget = m_npcPath.think( Pos, &pathComplete, &waypointChange );

		if ( pathComplete )
		{
			this->m_controlFunc = NPC_CONTROL_MOVEMENT;
		}
		else
		{
			if ( !m_animPlaying )
			{
				m_animPlaying = true;
				m_animNo = m_data[m_type].moveAnim;
				m_frame = 0;
			}

			s16 headingToPlayer = ratan2( playerYDist, playerXDist );

			if ( m_evadeClockwise )
			{
				headingToPlayer += 1024;
			}
			else
			{
				headingToPlayer -= 1024;
			}

			headingToPlayer &= 4095;

			s16 decDir, incDir;

			decDir = headingToPlayer - headingToTarget;

			if ( decDir < 0 )
			{
				decDir += ONE;
			}

			incDir = headingToTarget - headingToPlayer;

			if ( incDir < 0 )
			{
				incDir += ONE;
			}

			if ( decDir < incDir )
			{
				moveDist = decDir;
			}
			else
			{
				moveDist = incDir;
			}

			if ( moveDist < 128 )
			{
				// evasion angle is close to angle required to go to waypoint, hence resume waypoint movement

				this->m_controlFunc = NPC_CONTROL_MOVEMENT;
				this->m_timerFunc = NPC_TIMER_EVADE_DONE;
				this->m_timerTimer = GameState::getOneSecondInFrames();
				this->m_sensorFunc = NPC_SENSOR_NONE;
			}
			else
			{
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

				if ( moveDist < -128 )
				{
					moveDist = -128;
				}
				else if ( moveDist > 128 )
				{
					moveDist = 128;
				}

				m_heading += moveDist;

				m_heading &= 4095;
				
				moveX = ( _frames * 3 * rcos( m_heading ) ) >> 12;
				moveY = ( _frames * 3 * rsin( m_heading ) ) >> 12;
				moveVel = ( _frames * 3 ) << 8;

				if ( processGroundCollisionReverse( &moveX, &moveY ) )
				{
					m_evadeClockwise = !m_evadeClockwise;
				}

				processMovementModifier(_frames, moveX, moveY, moveVel, moveDist);
			}
		}
	}
}

bool CNpcSmallJellyfishEnemy::processSensor()
{
	switch( m_sensorFunc )
	{
		case NPC_SENSOR_NONE:
			return( false );

		default:
		{
			if ( playerXDistSqr + playerYDistSqr < 5625 )
			{
				m_controlFunc = NPC_CONTROL_CLOSE;
				m_evadeClockwise = getRnd() % 2;

				return( true );
			}
			else
			{
				return( false );
			}
		}
	}
}

void CNpcSmallJellyfishEnemy::processMovementModifier( int _frames, s32 distX, s32 distY, s32 dist, s16 headingChange )
{
	s32 newX, newY;
	s32 preShiftX, preShiftY;
	s16 headingVal;

	//u16 jellyfishData[5] = { 96, 192, 256, 192, 128, };
	u16 jellyfishData[6] = { 256, 256, 96, 48, 96, 256, };

	u32 dataPoint;

	m_movementTimer += _frames;

	if ( m_movementTimer > ( GameState::getOneSecondInFrames() >> 1 ) )
	{
		m_movementTimer = 0;
	}

	dataPoint = 5 * m_movementTimer;

	if ( dataPoint != 0 )
	{
		dataPoint /= ( GameState::getOneSecondInFrames() >> 1 );
	}

	m_frame = ( ( m_movementTimer * ( getFrameCount() - 1 ) << 8 ) ) / GameState::getOneSecondInFrames();

	s32 resistance;
	s32 absVelocity = abs( m_velocity );
	s32 reqVelocity = dist;

	resistance = _frames * NPC_JELLYFISH_RESISTANCE;

	if ( absVelocity < resistance )
	{
		resistance = absVelocity;
	}

	if ( absVelocity != 0 )
	{
		resistance = -( resistance * m_velocity ) / absVelocity;
	}
	else
	{
		resistance = 0;
	}

	m_velocity += resistance;

	headingVal = abs( headingChange );
	if ( headingVal > 128 )
	{
		headingVal = 128;
	}

	reqVelocity = dist * jellyfishData[dataPoint];
	reqVelocity >>= 8;
	reqVelocity *= 128 + ( 128 - headingVal );
	reqVelocity >>= 8;

	s32 absReqVelocity = abs( reqVelocity );

	if ( absReqVelocity > absVelocity )
	{
		m_velocity += reqVelocity >> 1;
	}

	preShiftX = ( m_velocity >> 8 ) * rcos( m_heading );
	preShiftY = ( m_velocity >> 8 ) * rsin( m_heading );

	newX = preShiftX >> 12;
	if ( !newX && preShiftX )
	{
		newX = preShiftX / abs( preShiftX );
	}

	newY = preShiftY >> 12;
	if ( !newY && preShiftY )
	{
		newY = preShiftY / abs( preShiftY );
	}

	Pos.vx += newX;
	Pos.vy += newY;

	m_drawRotation = m_heading;
}

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

#ifndef __GAME_GAME_H__
#include	"game\game.h"
#endif

#ifndef	__PLAYER_PLAYER_H__
#include	"player\player.h"
#endif


void CNpc::processSmallJellyfishMovementModifier( int _frames, s32 distX, s32 distY, s32 dist, s16 headingChange )
{
	s32 newX, newY;
	s32 preShiftX, preShiftY;

	u16 jellyfishData[5] = { 96, 192, 256, 192, 128, };

	u32 dataPoint;

	m_movementTimer += _frames;

	if ( m_movementTimer > GameState::getOneSecondInFrames() )
	{
		m_movementTimer = 0;
	}

	dataPoint = 4 * m_movementTimer;

	if ( dataPoint != 0 )
	{
		dataPoint /= GameState::getOneSecondInFrames();
	}

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

	reqVelocity = dist * jellyfishData[dataPoint];
	reqVelocity >>= 8;
	reqVelocity *= 128 + ( 128 - headingChange );
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
}

void CNpc::processCloseSmallJellyfishEvade( int _frames )
{
	s32 moveX = 0, moveY = 0;

	s16 moveDist = 0;

	s32 moveVel = 0;

	if ( playerXDistSqr + playerYDistSqr > 22500 )
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
			s16 headingToPlayer = ratan2( playerYDist, playerXDist );

			if ( m_evadeClockwise )
			{
				headingToPlayer += 1024;
			}
			else
			{
				headingToPlayer -= 1024;
			}

			headingToPlayer %= ONE;

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

				m_heading = m_heading % ONE;
				
				moveX = ( _frames * 3 * rcos( m_heading ) ) >> 12;
				moveY = ( _frames * 3 * rsin( m_heading ) ) >> 12;
				moveVel = ( _frames * 3 ) << 8;

				// check for collision with ground

				if ( m_layerCollision->Get( ( Pos.vx + moveX ) >> 4, ( Pos.vy + moveY ) >> 4 ) )
				{
					bool xBlocked = false;
					bool yBlocked = false;

					// destination point is below ground, check in individual directions

					if ( m_layerCollision->Get( ( Pos.vx + moveX ) >> 4, Pos.vy >> 4 ) )
					{
						// X direction is blocked

						xBlocked = true;
					}

					if ( m_layerCollision->Get( Pos.vx >> 4, ( Pos.vy + moveY ) >> 4 ) )
					{
						yBlocked = true;
					}

					if ( xBlocked && !yBlocked )
					{
						// invert X

						moveX = -moveX;

						m_heading = ratan2( moveY, moveX );
					}
					else if ( !xBlocked && yBlocked )
					{
						// invert Y

						moveY = -moveY;

						m_heading = ratan2( moveY, moveX );
					}
					else
					{
						moveX = -moveX;
						moveY = -moveY;

						m_heading += 2048;
						m_heading &= 4096;
					}
				}

				processMovementModifier(_frames, moveX, moveY, moveVel, moveDist);
			}
		}
	}
}

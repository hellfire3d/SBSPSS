/*=========================================================================

	ngeneric.cpp

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


void CNpc::processGenericGotoTarget( int _frames, s32 xDist, s32 yDist, s32 speed )
{
	s16 decDir, incDir, moveDist;
	s32 moveX, moveY;
	s16 headingToTarget = ratan2( yDist, xDist );
	s16 maxTurnRate = m_data[m_type].turnSpeed;

	decDir = m_heading - headingToTarget;
	if ( decDir < 0 )
	{
		decDir += ONE;
	}

	incDir = headingToTarget - m_heading;
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

	if ( moveDist < -maxTurnRate )
	{
		moveDist = -maxTurnRate;
	}
	else if ( moveDist > maxTurnRate )
	{
		moveDist = maxTurnRate;
	}

	m_heading += moveDist;
	m_heading = m_heading % ONE;

	s32 preShiftX = _frames * speed * rcos( m_heading );
	s32 preShiftY = _frames * speed * rsin( m_heading );

	moveX = preShiftX >> 12;
	if ( !moveX && preShiftX )
	{
		moveX = preShiftX / abs( preShiftX );
	}

	moveY = preShiftY >> 12;
	if ( !moveY && preShiftY )
	{
		moveY = preShiftY / abs( preShiftY );
	}

	Pos.vx += moveX;
	Pos.vy += moveY;
}

void CNpc::processGenericGetUserDist( int _frames, s32 *distX, s32 *distY )
{
	s32 moveX = 0, moveY = 0;

	s16 moveDist = 0;

	s32 moveVel = 0;

	CPlayer *player = GameScene.getPlayer();

	DVECTOR playerPos = player->getPos();

	*distX = playerPos.vx - this->Pos.vx;
	*distY = playerPos.vy - this->Pos.vy;
}

void CNpc::processGenericFixedPathWalk( int _frames, s32 *moveX, s32 *moveY )
{
	s32 maxHeight = 10;
	s32 distX, distY;
	s32 fallSpeed = 5;
	s8 yMovement = fallSpeed * _frames;
	s32 groundHeight;

	*moveX = 0;
	*moveY = 0;

	// ignore y component of waypoint, since we are stuck to the ground

	if ( m_npcPath.think2D( Pos, &distX, &distY ) )
	{
		// path has finished, waypoint has changed, or there are no waypoints - do not move horizontally

		// check for vertical movement

		groundHeight = m_layerCollision->getHeightFromGround( Pos.vx, Pos.vy, yMovement + 16 );

		if ( groundHeight <= yMovement )
		{
			// groundHeight <= yMovement indicates either just above ground or on or below ground

			*moveY = groundHeight;
		}
		else
		{
			// fall

			*moveY = yMovement;
		}
	}
	else
	{
		// check for collision

		distX = distX / abs( distX );

		if ( m_layerCollision->getHeightFromGround( Pos.vx + ( distX * m_data[m_type].speed * _frames ), Pos.vy ) < -maxHeight )
		{
			// there is an obstacle in the way, increment the path point (hopefully this will resolve the problem)

			m_npcPath.incPath();
		}
		else
		{
			// check for vertical movement

			groundHeight = m_layerCollision->getHeightFromGround( Pos.vx, Pos.vy, yMovement + 16 );

			if ( groundHeight <= yMovement )
			{
				// groundHeight <= yMovement indicates either just above ground or on or below ground

				*moveX = distX * m_data[m_type].speed * _frames;
				*moveY = groundHeight;
			}
			else
			{
				// fall

				*moveY = yMovement;
			}
		}
	}
}

bool CNpc::isCollisionWithGround()
{
	ASSERT(m_layerCollision);
	return m_layerCollision->Get( Pos.vx >> 4, ( Pos.vy + 1 ) >> 4 ) ? 16:0;
}

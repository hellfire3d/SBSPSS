/*=========================================================================

	ndustdev.cpp

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/

#ifndef __ENEMY_NPC_H__
#include "enemy\npc.h"
#endif

#ifndef	__ENEMY_NDUSTDEV_H__
#include "enemy\ndustdev.h"
#endif

#ifndef __GAME_GAME_H__
#include	"game\game.h"
#endif


void CNpcDustDevilEnemy::postInit()
{
	m_npcPath.setPathType( CNpcPath::SINGLE_USE_PATH );
}

void CNpcDustDevilEnemy::processMovement( int _frames )
{
	s32 maxHeight = 40;
	s32 distX, distY;
	s32 fallSpeed = 3;
	s8 yMovement = fallSpeed * _frames;
	s32 groundHeight;

	s32 moveX = 0;
	s32 moveY = 0;
	s32 moveVel = 0;
	s32 moveDist = 0;

	bool pathComplete;

	// deal with anims

	if ( !m_animPlaying )
	{
		m_animPlaying = true;
		m_frame = 0;
		m_animNo = m_data[m_type].moveAnim;
	}

	// ignore y component of waypoint, since we are stuck to the ground

	if ( m_npcPath.thinkFlat( Pos, &pathComplete, &distX, &distY, &m_heading ) )
	{
		// path has finished, waypoint has changed, or there are no waypoints - do not move horizontally

		if ( pathComplete )
		{
			m_npcPath.resetPath();

			Pos = m_base;
		}
		else
		{
			// check for vertical movement

			groundHeight = m_layerCollision->getHeightFromGround( Pos.vx, Pos.vy, yMovement + 16 );

			if ( groundHeight <= yMovement )
			{
				// groundHeight <= yMovement indicates either just above ground or on or below ground

				moveY = groundHeight;
			}
			else
			{
				// fall

				moveY = yMovement;
			}
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

				moveX = distX * m_data[m_type].speed * _frames;
				moveY = groundHeight;
			}
			else
			{
				// fall

				moveY = yMovement;
			}
		}
	}

	processMovementModifier( _frames, moveX, moveY, moveVel, moveDist );
}

void CNpcDustDevilEnemy::processMovementModifier( int _frames, s32 distX, s32 distY, s32 dist, s16 headingChange )
{
	Pos.vx += distX;
	Pos.vy += distY;

	// sort out draw rotation

	DVECTOR testPos1, testPos2;

	testPos1 = testPos2 = Pos;
	testPos1.vx -= 10;
	testPos2.vx += 10;

	testPos1.vy += m_layerCollision->getHeightFromGround( testPos1.vx, testPos1.vy, 16 );
	testPos2.vy += m_layerCollision->getHeightFromGround( testPos1.vx, testPos1.vy, 16 );

	s32 xDist = testPos2.vx - testPos1.vx;
	s32 yDist = testPos2.vy - testPos1.vy;

	s16 heading = ratan2( yDist, xDist );

	m_drawRotation = heading;
}

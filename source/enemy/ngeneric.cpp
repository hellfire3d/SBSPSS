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


void CNpcEnemy::processGenericGotoTarget( int _frames, s32 xDist, s32 yDist, s32 speed )
{
	s16 decDir, incDir, moveDist;
	s32 moveX, moveY;
	s16 headingToTarget = ratan2( yDist, xDist ) & 4095;
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
	m_heading &= 4095;

	s32 preShiftX = _frames * speed * rcos( m_heading );
	s32 preShiftY = _frames * speed * rsin( m_heading );

	moveX = preShiftX >> 12;
	if ( !moveX && preShiftX )
	{
		moveX = preShiftX / abs( preShiftX );
	}

	if ( xDist > 0 )
	{
		if ( moveX > xDist )
		{
			moveX = xDist;
		}
	}
	else if ( xDist < 0 )
	{
		if ( moveX < xDist )
		{
			moveX = xDist;
		}
	}
	else
	{
		moveX = 0;
	}

	moveY = preShiftY >> 12;
	if ( !moveY && preShiftY )
	{
		moveY = preShiftY / abs( preShiftY );
	}

	if ( yDist > 0 )
	{
		if ( moveY > yDist )
		{
			moveY = yDist;
		}
	}
	else if ( yDist < 0 )
	{
		if ( moveY < yDist )
		{
			moveY = yDist;
		}
	}
	else
	{
		moveY = 0;
	}

	Pos.vx += moveX;
	Pos.vy += moveY;
}

void CNpcEnemy::processGenericGetUserDist( int _frames, s32 *distX, s32 *distY )
{
	s32 moveX = 0, moveY = 0;

	s16 moveDist = 0;

	s32 moveVel = 0;

	CPlayer *player = GameScene.getPlayer();

	DVECTOR const &playerPos = player->getPos();

	*distX = playerPos.vx - this->Pos.vx;
	*distY = playerPos.vy - this->Pos.vy;
}

bool CNpcEnemy::processGroundCollisionReverse( s32 *moveX, s32 *moveY )
{
	bool xBlocked = false;
	bool yBlocked = false;

	// check for collision with ground

	if ( CGameScene::getCollision()->Get( ( Pos.vx + *moveX ) >> 4, ( Pos.vy + *moveY ) >> 4 ) )
	{
		// destination point is below ground, check in individual directions

		if ( CGameScene::getCollision()->Get( ( Pos.vx + *moveX ) >> 4, Pos.vy >> 4 ) )
		{
			// X direction is blocked

			xBlocked = true;
		}

		if ( CGameScene::getCollision()->Get( Pos.vx >> 4, ( Pos.vy + *moveY ) >> 4 ) )
		{
			yBlocked = true;
		}

		if ( xBlocked && !yBlocked )
		{
			// invert X

			*moveX = -(*moveX);

			m_heading = ratan2( *moveY, *moveX ) & 4095;
		}
		else if ( !xBlocked && yBlocked )
		{
			// invert Y

			*moveY = -(*moveY);

			m_heading = ratan2( *moveY, *moveX ) & 4095;
		}
		else
		{
			// invert both

			*moveX = -(*moveX);
			*moveY = -(*moveY);

			m_heading += 2048;
		}

		m_heading &= 4095;
	}

	return( xBlocked | yBlocked );
}

u8 CNpcEnemy::processGenericFixedPathMove( int _frames, s32 *moveX, s32 *moveY, s32 *moveVel, s32 *moveDist )
{
	bool pathComplete;
	bool waypointChange;
	s32 xDist, yDist;

	s16 headingToTarget = m_npcPath.think( Pos, &pathComplete, &waypointChange, &xDist, &yDist );

	/*if ( waypointChange )
	{
		m_movementTimer = 0;
	}*/

	if ( !pathComplete )
	{
		s16 decDir, incDir;
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
			*moveDist = -decDir;
		}
		else
		{
			*moveDist = incDir;
		}

		if ( *moveDist < -maxTurnRate )
		{
			*moveDist = -maxTurnRate;
		}
		else if ( *moveDist > maxTurnRate )
		{
			*moveDist = maxTurnRate;
		}

		m_heading += *moveDist;
		m_heading &= 4095;

		s32 preShiftX = _frames * m_speed * rcos( m_heading );
		s32 preShiftY = _frames * m_speed * rsin( m_heading );

		*moveX = preShiftX >> 12;
		if ( !(*moveX) && preShiftX )
		{
			*moveX = preShiftX / abs( preShiftX );
		}

		if ( xDist > 0 )
		{
			if ( *moveX > xDist )
			{
				*moveX = xDist;
			}
		}
		else if ( xDist < 0 )
		{
			if ( *moveX < xDist )
			{
				*moveX = xDist;
			}
		}
		else
		{
			*moveX = 0;
		}

		*moveY = preShiftY >> 12;
		if ( !(*moveY) && preShiftY )
		{
			*moveY = preShiftY / abs( preShiftY );
		}

		if ( yDist > 0 )
		{
			if ( *moveY > yDist )
			{
				*moveY = yDist;
			}
		}
		else if ( yDist < 0 )
		{
			if ( *moveY < yDist )
			{
				*moveY = yDist;
			}
		}
		else
		{
			*moveY = 0;
		}

		*moveVel = ( _frames * m_speed ) << 8;

		//processGroundCollisionReverse( moveX, moveY );
	}

	return( waypointChange );
}

void CNpcEnemy::processGenericFixedPathWalk( int _frames, s32 *moveX, s32 *moveY )
{
	s32 maxHeight = 20;
	s32 distX, distY;
	s32 fallSpeed = 3;
	s8 yMovement = fallSpeed * _frames;
	s32 groundHeight;

	*moveX = 0;
	*moveY = 0;

	// ignore y component of waypoint, since we are stuck to the ground

	bool pathComplete;

	if ( m_npcPath.thinkFlat( Pos, &pathComplete, &distX, &distY, &m_heading ) )
	{
		// path has finished, waypoint has changed, or there are no waypoints - do not move horizontally

		// check for vertical movement

		groundHeight = CGameScene::getCollision()->getHeightFromGround( Pos.vx, Pos.vy, yMovement + 16 );

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

		if ( CGameScene::getCollision()->getHeightFromGround( Pos.vx + ( distX * m_speed * _frames ), Pos.vy ) < -maxHeight )
		{
			// there is an obstacle in the way, increment the path point (hopefully this will resolve the problem)

			m_npcPath.incPath();
		}
		else
		{
			// check for vertical movement

			groundHeight = CGameScene::getCollision()->getHeightFromGround( Pos.vx, Pos.vy, yMovement + 16 );

			if ( groundHeight <= yMovement )
			{
				// groundHeight <= yMovement indicates either just above ground or on or below ground

				*moveX = distX * m_speed * _frames;
				*moveY = groundHeight;
			}
			else
			{
				// fall

				*moveY = yMovement;
			}
		}
	}

	switch ( CGameScene::getCollision()->getCollisionBlock( Pos.vx + (*moveX), Pos.vy + (*moveY) + 1 ) & COLLISION_TYPE_MASK )
	{
		case COLLISION_TYPE_FLAG_DEATH_FALL:
		case COLLISION_TYPE_FLAG_DEATH_INSTANT:
		case COLLISION_TYPE_FLAG_DEATH_LIQUID:
		case COLLISION_TYPE_FLAG_DAMAGE:
		{
			m_health = 0;
			m_controlFunc = NPC_CONTROL_SHOT;
			m_state = NPC_GENERIC_HIT_CHECK_HEALTH;

			break;
		}

		default:
			break;
	}
}

bool CNpcEnemy::isCollisionWithGround()
{
	return CGameScene::getCollision()->Get( Pos.vx >> 4, ( Pos.vy + 1 ) >> 4 ) ? 16:0;
}

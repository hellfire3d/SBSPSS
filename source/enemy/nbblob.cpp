/*=========================================================================

	nbblob.cpp

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __ENEMY_NPC_H__
#include "enemy\npc.h"
#endif

#ifndef __ENEMY_NBBLOB_H__
#include "enemy\nbblob.h"
#endif

#ifndef __GAME_GAME_H__
#include	"game\game.h"
#endif

#ifndef __ANIM_BALLBLOB_HEADER__
#include <ACTOR_BALLBLOB_ANIM.h>
#endif

void CNpcBallBlobEnemy::postInit()
{
	m_heading = 128;

	m_npcPath.setPathType( CNpcPath::PONG_PATH );

	m_velocity.vx = 0;
	m_velocity.vy = -5;
}

void CNpcBallBlobEnemy::processMovement( int _frames )
{
	s32 moveX = 0, moveY = 0;
	s32 moveVel = 0;
	s32 moveDist = 0;
	s32 waypointXDist;
	s32 waypointYDist;
	s32 waypointHeading;
	s32 groundHeight;

	if ( !m_animPlaying && m_frame != 0 )
	{
		m_animNo = ANIM_BALLBLOB_IDLE;
		m_frame = 0;
	}
	else if ( m_animNo == ANIM_BALLBLOB_IDLE )
	{
		moveX = 0;
		moveY = 0;

		processMovementModifier( _frames, moveX, moveY, moveVel, moveDist );

		return;
	}

	// deal with vertical

	m_velocity.vy += 128;

	if ( m_velocity.vy > ( 5 << 8 ) )
	{
		m_velocity.vy = 5 << 8;
	}
	else if ( m_velocity.vy < -( 5 << 8 ) )
	{
		m_velocity.vy = -( 5 << 8 );
	}

	moveY = ( m_velocity.vy >> 8 ) * _frames;

	groundHeight = m_layerCollision->getHeightFromGround( Pos.vx + moveX, Pos.vy + moveY, 16 );

	if ( groundHeight < 0 )
	{
		if ( m_velocity.vy > 0 )
		{
			m_velocity.vy = -m_velocity.vy;
			m_animPlaying = true;
			m_animNo = ANIM_BALLBLOB_IDLE;
			m_frame = 0;
		}
		else
		{
			m_velocity.vy = -( 5 << 8 );
			m_animPlaying = true;
			m_animNo = ANIM_BALLBLOB_IDLE;
			m_frame = 0;
		}

		moveY = groundHeight;
	}

	// deal with horizontal

	bool pathComplete;
	
	if ( m_npcPath.thinkFlat( Pos, &pathComplete, &waypointXDist, &waypointYDist, &waypointHeading ) )
	{
		if ( m_animNo != ANIM_BALLBLOB_IDLE)
		{
			m_animPlaying = true;
			m_animNo = ANIM_BALLBLOB_IDLE;
			m_frame = 0;
		}
	}

	if ( waypointHeading == 0 )
	{
		moveX = m_speed * _frames;
	}
	else
	{
		moveX = -m_speed * _frames;
	}

	processMovementModifier( _frames, moveX, moveY, moveVel, moveDist );
}
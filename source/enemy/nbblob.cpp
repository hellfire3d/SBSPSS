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

	if ( !m_animPlaying )
	{
		m_animPlaying = true;
		m_animNo = ANIM_BALLBLOB_IDLE;
		m_frame = 0;
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

	groundHeight = CGameScene::getCollision()->getHeightFromGround( Pos.vx + moveX, Pos.vy + moveY, 16 );

	if ( groundHeight < 0 )
	{
		if ( m_velocity.vy > 0 )
		{
			m_velocity.vy = -m_velocity.vy;
		}
		else
		{
			m_velocity.vy = -( 5 << 8 );
		}

		moveY = groundHeight;

		if ( m_data[m_type].moveSfx < CSoundMediator::NUM_SFXIDS )
		{
			if ( m_soundId == NOT_PLAYING )
			{
				m_soundId = (int) CSoundMediator::playSfx( m_data[m_type].moveSfx, true );
			}
		}
	}

	// deal with horizontal

	bool pathComplete;
	
	m_npcPath.thinkFlat( Pos, &pathComplete, &waypointXDist, &waypointYDist, &waypointHeading );

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
/*=========================================================================

	hfirebal.cpp

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __HAZARD_HFIREBAL_H__
#include "hazard\hfirebal.h"
#endif

#ifndef __GAME_GAME_H__
#include	"game\game.h"
#endif


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcFireballHazard::init()
{
	CNpcHazard::init();

	DVECTOR newPos;

	Pos.vx = 100;
	Pos.vy = 100;
	m_base = Pos;

	newPos.vx = 300;
	newPos.vy = 100;

	m_npcPath.addWaypoint( newPos );
	m_npcPath.setPathType( CNpcPath::SINGLE_USE_PATH );

	m_extension = 0;
	m_velocity = 40;
	m_timer = GameState::getOneSecondInFrames() * 3;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcFireballHazard::processMovement( int _frames )
{
	s32 distX;
	s32 distY;
	s32 velocity;
	s32 distSourceX;
	s32 distSourceY;

	m_npcPath.getDistToNextWaypoint( Pos, &distX, &distY );
	m_npcPath.getDistToNextWaypoint( m_base, &distSourceX, &distSourceY );

	if ( m_extension < 4096 )
	{
		velocity = m_velocity * _frames;

		if ( 4096 - m_extension < velocity )
		{
			velocity = 4096 - m_extension;
		}

		m_extension += velocity;
	}
	else
	{
		// move complete

		Pos = m_base;
		m_extension = 0;
		m_isActive = false;
		m_timerActive = true;
		m_timer = 3 * GameState::getOneSecondInFrames();

		return;
	}

	Pos.vx = m_base.vx + ( ( distSourceX * m_extension ) >> 12 );
	Pos.vy = m_base.vy - ( 50 * rsin( m_extension >> 1 ) >> 12 );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcFireballHazard::processTimer( int _frames )
{
	m_timer -= _frames;

	if ( m_timer < 0 )
	{
		m_timerActive = false;
		m_isActive = true;
	}
}

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

#ifndef	__UTILS_HEADER__
#include	"utils\utils.h"
#endif


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcFireballHazard::init()
{
	CNpcHazard::init();

	m_extension = 0;
	m_velocity = 40;

	m_respawnRate = 4;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcFireballHazard::setWaypoints( sThingHazard *ThisHazard )
{
	int pointNum;

	u16	*PntList=(u16*)MakePtr(ThisHazard,sizeof(sThingHazard));

	u16 newXPos, newYPos;

	newXPos = (u16) *PntList;
	PntList++;
	newYPos = (u16) *PntList;
	PntList++;

	DVECTOR startPos;
	startPos.vx = newXPos << 4;
	startPos.vy = newYPos << 4;

	Pos = startPos;
	m_base = Pos;

	if ( ThisHazard->PointCount > 1 )
	{
		for ( pointNum = 1 ; pointNum < ThisHazard->PointCount ; pointNum++ )
		{
			newXPos = (u16) *PntList;
			PntList++;
			newYPos = (u16) *PntList;
			PntList++;

			addWaypoint( newXPos, newYPos );
		}
	}
	else
	{
		addWaypoint( newXPos, newYPos );
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcFireballHazard::processMovement( int _frames )
{
	s32 velocity;
	s32 distSourceX;
	s32 distSourceY;

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
		m_timer = ( m_respawnRate - 1 ) * GameState::getOneSecondInFrames();

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

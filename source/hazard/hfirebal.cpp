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
	m_height = 50;

	m_respawnRate = 4;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcFireballHazard::setWaypoints( sThingHazard *ThisHazard )
{
	int pointNum;

	u16	*PntList=(u16*)MakePtr(ThisHazard,sizeof(sThingHazard));

	u16 newXPos, newYPos;

	m_npcPath.setWaypointCount( ThisHazard->PointCount - 1 );

	newXPos = (u16) *PntList;
	setWaypointPtr( PntList );
	PntList++;
	newYPos = (u16) *PntList;
	PntList++;

	DVECTOR startPos;
	startPos.vx = ( newXPos << 4 ) + 8;
	startPos.vy = ( newYPos << 4 ) + 16;

	Pos = startPos;
	m_base = Pos;

	s32 minX, maxX, minY, maxY;

	m_npcPath.getPathXExtents( &minX, &maxX );
	m_width = maxX - minX;
	m_npcPath.getPathYExtents( &minY, &maxY );
	m_height = maxY - minY;

	if ( ThisHazard->PointCount > 1 )
	{
		newXPos = (u16) *PntList;

		if ( ( ( newXPos << 4 ) + 8 ) < startPos.vx )
		{
			m_width = -m_width;
		}
	}

	m_thinkArea.x1 = minX;
	m_thinkArea.x2 = maxX;
	m_thinkArea.y1 = minY;
	m_thinkArea.y2 = maxY;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcFireballHazard::processMovement( int _frames )
{
	s32 velocity;

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

		if ( m_soundId == NOT_PLAYING )
		{
			m_soundId = CSoundMediator::playSfx( CSoundMediator::SFX_HAZARD__FIREBALL_LAND, true, true );
		}

		return;
	}

	Pos.vx = m_base.vx + ( ( m_width * m_extension ) >> 12 );
	Pos.vy = m_base.vy - ( m_height * rsin( m_extension >> 1 ) >> 12 );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcFireballHazard::processTimer( int _frames )
{
	m_timer -= _frames;

	if ( m_timer < 0 )
	{
		m_timerActive = false;
		m_isActive = true;

		if ( m_soundId == NOT_PLAYING )
		{
			m_soundId = CSoundMediator::playSfx( CSoundMediator::SFX_HAZARD__FIREBALL_LAUNCH, true, true );
		}
	}
}

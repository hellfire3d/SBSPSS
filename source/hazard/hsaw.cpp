/*=========================================================================

	hsaw.cpp

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __HAZARD_HSAW_H__
#include "hazard\hsaw.h"
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcSawbladeHazard::init()
{
	CNpcHazard::init();

	DVECTOR newPos;

	Pos.vx = 100;
	Pos.vy = 100;
	m_base = Pos;

	newPos.vx = 300;
	newPos.vy = 100;

	//m_npcPath.addWaypoint( newPos );

	m_npcPath.setPathType( CNpcPath::SINGLE_USE_PATH );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcSawbladeHazard::processMovement( int _frames )
{
	bool pathComplete;
	bool waypointChange;
	s32 xDist, yDist;

	s16 headingToTarget = m_npcPath.think( Pos, &pathComplete, &waypointChange, &xDist, &yDist );

	if ( m_soundId == NOT_PLAYING )
	{
		m_soundId = (int) CSoundMediator::playSfx( CSoundMediator::SFX_SAW, true, true );
	}

	if ( !pathComplete )
	{
		s16 decDir, incDir, moveDist;
		s16 maxTurnRate = 2048;
		s32 moveX, moveY;

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

		s32 preShiftX = _frames * 3 * rcos( m_heading );
		s32 preShiftY = _frames * 3 * rsin( m_heading );

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
	else
	{
		m_npcPath.resetPath();

		Pos = m_base;
	}
}

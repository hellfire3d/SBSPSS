/*=========================================================================

	hbbarrel.h

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __HAZARD_HBBARREL_H__
#include "hazard\hbbarrel.h"
#endif

#ifndef __LAYER_COLLISION_H__
#include "level\layercollision.h"
#endif


void CNpcBouncingBarrelHazard::init()
{
	CNpcHazard::init();

	m_lastWaypoint = Pos;
}

void CNpcBouncingBarrelHazard::processMovement( int _frames )
{
	s32 moveX = 0, moveY = 0;
	s32 moveVel = 0;
	s32 moveDist = 0;
	s32 waypointXDist;
	s32 waypointYDist;
	s32 waypointHeading;
	s32 groundHeight;

	// deal with horizontal

	bool pathComplete;
	
	if ( m_npcPath.thinkFlat( Pos, &pathComplete, &waypointXDist, &waypointYDist, &waypointHeading ) )
	{
		if ( pathComplete )
		{
			Pos = m_base;
			m_npcPath.resetPath();
		}

		m_lastWaypoint = Pos;
	}

	moveX = 3 * _frames;
	if ( waypointHeading == 2048 )
	{
		moveX = -moveX;
	}

	Pos.vx += moveX;

	// deal with vertical

	DVECTOR nextWaypoint;
	
	nextWaypoint.vx = waypointXDist + Pos.vx;
	nextWaypoint.vy = waypointYDist + Pos.vy;

	if ( waypointYDist > 0 )
	{
		s32 sineVal = ( abs( Pos.vx - nextWaypoint.vx ) << 11 ) / abs( nextWaypoint.vx - m_lastWaypoint.vx );

		Pos.vy = nextWaypoint.vy + ( ( ( m_lastWaypoint.vy - nextWaypoint.vy ) * rsin( sineVal ) ) >> 12 );
	}
	else if ( waypointYDist < 0 )
	{
		s32 sineVal = ( abs( Pos.vx - m_lastWaypoint.vx ) << 11 ) / abs( nextWaypoint.vx - m_lastWaypoint.vx );

		Pos.vy = m_lastWaypoint.vy + ( ( ( nextWaypoint.vy - m_lastWaypoint.vy ) * rsin( sineVal ) ) >> 12 );
	}

	/*if ( waypointYDist < 0 )
	{
		if ( waypointYDist > -16 )
		{
			s32 multiplier = -waypointYDist;

			m_velocity.vy = ( multiplier * m_velocity.vy ) / 16;

			if ( m_velocity.vy < 1 )
			{
				m_velocity.vy = 1;
			}
		}
		else
		{
			m_velocity.vy = -5 << 8;
		}
	}
	else
	{
		m_velocity.vy += 128;
	}*/
}

/*=========================================================================

	plinear.cpp

	Author:		CRB
	Created:
	Project:	Spongebob
	Purpose:

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __PLATFORM_PLINEAR_H__
#include "platform\plinear.h"
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcLinearPlatform::postInit()
{
	m_npcPath.setPathType( CNpcPath::PONG_PATH );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcLinearPlatform::processMovement( int _frames )
{
	s32 moveX = 0, moveY = 0;
	s32 moveDist = 0;
	s32 xDist, yDist;

	bool pathComplete;
	bool waypointChange;

	s16 headingToTarget = m_npcPath.think( Pos, &pathComplete, &waypointChange, &xDist, &yDist );

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

		s32 preShiftX = _frames * m_data[m_type].speed * rcos( m_heading );
		s32 preShiftY = _frames * m_data[m_type].speed * rsin( m_heading );

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

		//processGroundCollisionReverse( moveX, moveY );
	}

	Pos.vx += moveX;
	Pos.vy += moveY;
}

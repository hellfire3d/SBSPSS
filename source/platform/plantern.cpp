/*=========================================================================

	plantern.cpp

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __PLATFORM_PLANTERN_H__
#include "platform\plantern.h"
#endif

#ifndef	__UTILS_HEADER__
#include	"utils\utils.h"
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcLanternPlatform::postInit()
{
	CNpcPlatform::postInit();

	m_extension = 0;
	m_heading = 1024;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcLanternPlatform::setWaypoints( sThingPlatform *ThisPlatform )
{
	int pointNum;

	u16	*PntList=(u16*)MakePtr(ThisPlatform,sizeof(sThingPlatform));

	u16 newXPos, newYPos;

	newXPos = (u16) *PntList;
	PntList++;
	newYPos = (u16) *PntList;
	PntList++;

	DVECTOR startPos;
	startPos.vx = ( newXPos << 4 ) + 8;
	startPos.vy = ( newYPos << 4 ) + 16;

	if ( ThisPlatform->PointCount > 1 )
	{
		newXPos = (u16) *PntList;
		PntList++;
		newYPos = (u16) *PntList;
		PntList++;

		DVECTOR pivotPos;
		pivotPos.vx = ( newXPos << 4 ) + 8;
		pivotPos.vy = ( newYPos << 4 ) + 16;

		s32 xDist = startPos.vx - pivotPos.vx;
		s32 yDist = startPos.vy - pivotPos.vy;

		m_maxExtension = 1024 - ratan2( abs( yDist ), abs( xDist ) );

		m_length = isqrt2( ( xDist * xDist ) + ( yDist * yDist ) );

		init( pivotPos );

		if ( xDist > 0 )
		{
			m_extendDir = EXTEND_LEFT;
		}
		else
		{
			m_extendDir = EXTEND_RIGHT;
		}
	}
	else
	{
		init( startPos );
	}

	m_thinkArea.x1 = Pos.vx - m_length;
	m_thinkArea.x2 = Pos.vx + m_length;
	m_thinkArea.y1 = Pos.vy - m_length;
	m_thinkArea.y2 = Pos.vy + m_length;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcLanternPlatform::processMovement( int _frames )
{
	if ( m_extendDir == EXTEND_LEFT )
	{
		if ( m_extension > m_maxExtension )
		{
			m_extendDir = EXTEND_RIGHT;
		}
		else
		{
			m_extension += _frames << 3;
		}
	}
	else
	{
		if ( m_extension < -m_maxExtension )
		{
			m_extendDir = EXTEND_LEFT;
		}
		else
		{
			m_extension -= _frames << 3;
		}
	}

	Pos.vx = m_base.vx + ( ( m_length * rcos( m_heading + m_extension ) ) >> 12 );
	Pos.vy = m_base.vy + ( ( m_length * rsin( m_heading + m_extension ) ) >> 12 );
}

/*=========================================================================

	hlog.cpp

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __HAZARD_HLOG_H__
#include "hazard\hlog.h"
#endif

#ifndef	__UTILS_HEADER__
#include	"utils\utils.h"
#endif


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcLogHazard::init()
{
	CNpcHazard::init();

	m_extension = 0;
	m_velocity = 40;
	m_height = 50;

	m_respawnRate = 4;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcLogHazard::setWaypoints( sThingHazard *ThisHazard )
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

	s32 minY, maxY;

	m_npcPath.getPathYExtents( &minY, &maxY );
	m_height = maxY - minY;

	Pos = startPos;
	Pos.vy = minY;
	m_base = Pos;

	s32 minX, maxX;

	m_npcPath.getPathXExtents( &minX, &maxX );

	m_thinkArea.x1 = minX;
	m_thinkArea.x2 = maxX;
	m_thinkArea.y1 = minY;
	m_thinkArea.y2 = maxY;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcLogHazard::processMovement( int _frames )
{
	if ( m_extendDir == EXTEND_DOWN )
	{
		m_extension += _frames;

		if ( m_extension > m_height )
		{
			m_extension = m_height;
			m_extendDir = EXTEND_UP;
		}
	}
	else
	{
		m_extension -= _frames;

		if ( m_extension < 0 )
		{
			m_extension = 0;
			m_extendDir = EXTEND_DOWN;
		}
	}

	Pos.vy = m_base.vy + m_extension;
}

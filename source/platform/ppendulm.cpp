/*=========================================================================

	ppendulm.cpp

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __PLATFORM_PPENDULM_H__
#include "platform\ppendulm.h"
#endif

#ifndef	__UTILS_HEADER__
#include	"utils\utils.h"
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcPendulumPlatform::postInit()
{
	m_extendDir = EXTEND_LEFT;
	m_extension = 0;
	m_heading = 1024;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcPendulumPlatform::setWaypoints( sThingPlatform *ThisPlatform )
{
	int pointNum;

	u16	*PntList=(u16*)MakePtr(ThisPlatform,sizeof(sThingPlatform));

	u16 initYPos, newXPos, newYPos;

	newXPos = (u16) *PntList;
	PntList++;
	initYPos = newYPos = (u16) *PntList;
	PntList++;

	DVECTOR startPos;
	startPos.vx = newXPos << 4;
	startPos.vy = newYPos << 4;

	init( startPos );

	if ( ThisPlatform->PointCount > 1 )
	{
		newXPos = (u16) *PntList;
		PntList++;
		newYPos = (u16) *PntList;
		PntList++;

		m_length = ( newYPos - initYPos ) << 4;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcPendulumPlatform::processMovement( int _frames )
{
	if ( m_extendDir == EXTEND_LEFT )
	{
		if ( m_extension > 512 )
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
		if ( m_extension < -512 )
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


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

	DVECTOR startPos;
		DVECTOR pivotPos;
void CNpcPendulumPlatform::setWaypoints( sThingPlatform *ThisPlatform )
{
	int pointNum;

	u16	*PntList=(u16*)MakePtr(ThisPlatform,sizeof(sThingPlatform));

	u16 newXPos, newYPos;

	newXPos = (u16) *PntList;
	PntList++;
	newYPos = (u16) *PntList;
	PntList++;

	startPos.vx = newXPos << 4;
	startPos.vy = newYPos << 4;

	if ( ThisPlatform->PointCount > 1 )
	{
		newXPos = (u16) *PntList;
		PntList++;
		newYPos = (u16) *PntList;
		PntList++;

		pivotPos.vx = newXPos << 4;
		pivotPos.vy = newYPos << 4;

		m_length = startPos.vy - pivotPos.vy;

		init( pivotPos );
	}
	else
	{
		init( startPos );
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


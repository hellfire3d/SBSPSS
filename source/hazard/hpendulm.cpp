/*=========================================================================

	hpendulm.cpp

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __HAZARD_HPENDULM_H__
#include "hazard\hpendulm.h"
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcPendulumHazard::init()
{
	CNpcHazard::init();

	m_extendDir = EXTEND_LEFT;
	m_extension = 0;
	m_heading = 1024;
	m_length = 200;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcPendulumHazard::processMovement( int _frames )
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
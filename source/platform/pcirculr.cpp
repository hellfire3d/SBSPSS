/*=========================================================================

	pcirculr.cpp

	Author:		CRB
	Created:
	Project:	Spongebob
	Purpose:

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __PLATFORM_PCIRCULR_H__
#include "platform\pcirculr.h"
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcCircularPlatform::postInit()
{
	CNpcPlatform::postInit();

	m_extension = 100;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcCircularPlatform::processMovement( int _frames )
{
	m_rotation += m_speed;
	m_rotation &= 4095;

	Pos.vx = m_base.vx + ( ( m_extension * rcos( m_rotation ) ) >> 12 );
	Pos.vy = m_base.vy + ( ( m_extension * rsin( m_rotation ) ) >> 12 );
}

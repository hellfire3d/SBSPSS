/*=========================================================================

	pfishhk.cpp

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __PLATFORM_PFISHHK_H__
#include "platform\pfishhk.h"
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcFishHookPlatform::processLifetime( int _frames )
{
	if ( m_contact )
	{
		m_isMoving = true;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcFishHookPlatform::processMovement( int _frames )
{
	Pos.vy -= m_data[m_type].speed * _frames;

	if ( Pos.vy < 0 )
	{
		shutdown();
	}
}
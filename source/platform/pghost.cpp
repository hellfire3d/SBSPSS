/*=========================================================================

	pghost.cpp

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __PLATFORM_PGHOST_H__
#include "platform\pghost.h"
#endif


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcGhostTrainPlatform::postInit()
{
	CNpcCartPlatform::postInit();

	m_speedSetting = 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcGhostTrainPlatform::processMovement( int _frames )
{
	CNpcCartPlatform::processMovement( _frames );
}

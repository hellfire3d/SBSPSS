/*=========================================================================

	pgeyser.cpp

	Author:		CRB
	Created:
	Project:	Spongebob
	Purpose:

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __PLATFORM_PGEYSER_H__
#include "platform\pgeyser.h"
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcGeyserPlatform::postInit()
{
	m_npcPath.setPathType( CNpcPath::SINGLE_USE_PATH );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcGeyserPlatform::processMovement( int _frames )
{
	s32 moveY = 0;
	s32 distX, distY, heading;
	bool pathComplete;

	m_npcPath.thinkVertical( Pos, &pathComplete, &distX, &distY, &heading );

	if ( pathComplete )
	{
		m_npcPath.resetPath();
		reinit();
	}
	else
	{
		s32 minY, maxY;

		m_npcPath.getPathYExtents( &minY, &maxY );

		moveY = m_data[m_type].speed * _frames;

		if ( Pos.vy < ( minY + 64 ) )
		{
			s32 multiplier = Pos.vy - minY;

			moveY = ( multiplier * moveY ) >> 6;

			if ( moveY < 1 )
			{
				moveY = 1;
			}
		}

		if ( heading == 3072 )
		{
			moveY = -moveY;
		}

		Pos.vy += moveY;
	}
}
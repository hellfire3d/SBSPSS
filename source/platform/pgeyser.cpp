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

	m_isFiring = false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcGeyserPlatform::processMovement( int _frames )
{
	if ( m_isFiring )
	{
		s32 moveY = 0;
		s32 distX, distY, heading;
		bool pathComplete;

		m_npcPath.thinkVertical( Pos, &pathComplete, &distX, &distY, &heading );

		if ( pathComplete )
		{
			m_npcPath.resetPath();
			reinit();
			m_isFiring = false;
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
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcGeyserPlatform::processTimer( int _frames )
{
	if ( m_timer > 0 )
	{
		m_timer -= _frames;
	}
	else
	{
		m_isFiring = true;
	}
}

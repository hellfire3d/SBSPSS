/*=========================================================================

	hspikes.cpp

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __HAZARD_HSPIKES_H__
#include "hazard\hspikes.h"
#endif

#ifndef __LAYER_COLLISION_H__
#include "level\layercollision.h"
#endif

void CNpcSpikesHazard::init()
{
	CNpcHazard::init();

	m_state = SPIKES_RISING;
}

void CNpcSpikesHazard::processMovement( int _frames )
{
	s32 minY, maxY;
	m_npcPath.getPathYExtents( &minY, &maxY );

	switch ( m_state )
	{
		case SPIKES_DROPPING:
		{
			if ( maxY - Pos.vy == 0 )
			{
				m_state = SPIKES_RISING;
			}
			else
			{
				Pos.vy += 3 * _frames;

				if ( Pos.vy > maxY )
				{
					Pos.vy = maxY;
				}
			}


			break;
		}

		case SPIKES_RISING:
		{
			if ( minY - Pos.vy == 0 )
			{
				m_state = SPIKES_DROPPING;
			}
			else
			{
				Pos.vy -= 3 * _frames;

				if ( Pos.vy < minY )
				{
					Pos.vy = minY;
				}
			}

			break;
		}
	}
}

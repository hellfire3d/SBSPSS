/*=========================================================================

	pbubble.cpp

	Author:		CRB
	Created:
	Project:	Spongebob
	Purpose:

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __PLATFORM_PBUBBLE_H__
#include "platform\pbubble.h"
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcBubblePlatform::processMovement( int _frames )
{
	if ( !isSetToShutdown() )
	{
		Pos.vy -= m_speed * _frames;

		if ( m_npcPath.getWaypointCount() > 1 )
		{
			s32 minY, maxY;

			m_npcPath.getPathYExtents( &minY, &maxY );

			if ( Pos.vy < minY )
			{
				setToShutdown();
			}
		}
		else
		{
			if ( Pos.vy < 0 )
			{
				setToShutdown();
			}
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const CRECT *CNpcBubblePlatform::getThinkBBox()
{
	CRECT objThinkBox = getCollisionArea();

	sBBox &thinkBBox = CThingManager::getThinkBBox();
	objThinkBox.x1 = thinkBBox.XMin;
	objThinkBox.x2 = thinkBBox.XMax;
	objThinkBox.y1 = thinkBBox.YMin;
	objThinkBox.y2 = thinkBBox.YMax;

	return &objThinkBox;
}
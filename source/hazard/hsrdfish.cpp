/*=========================================================================

	hsrdfish.cpp

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __HAZARD_HSRDFISH_H__
#include "hazard\hsrdfish.h"
#endif

#ifndef	__UTILS_HEADER__
#include	"utils\utils.h"
#endif


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcSwordfishHazard::render()
{
	if ( m_isActive )
	{
		CHazardThing::render();

		if (canRender())
		{
			DVECTOR &renderPos=getRenderPos();

			VECTOR flip;

			if ( m_heading )
			{
				flip.vx = ONE;
			}
			else
			{
				flip.vx = -ONE;
			}

			flip.vy = ONE;
			flip.vz = ONE;

			m_modelGfx->Render( renderPos, NULL, &flip );
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcSwordfishHazard::setWaypoints( sThingHazard *ThisHazard )
{
	int pointNum;

	u16	*PntList=(u16*)MakePtr(ThisHazard,sizeof(sThingHazard));

	u16 newXPos, newYPos;

	m_npcPath.setWaypointCount( ThisHazard->PointCount - 1 );

	newXPos = (u16) *PntList;
	setWaypointPtr( PntList );
	PntList++;
	newYPos = (u16) *PntList;
	PntList++;

	DVECTOR startPos;
	startPos.vx = ( newXPos << 4 ) + 8;
	startPos.vy = ( newYPos << 4 ) + 16;

	Pos = startPos;
	m_base = Pos;

	s32 minX, maxX, minY, maxY;

	m_npcPath.getPathXExtents( &minX, &maxX );
	m_width = maxX - minX;
	m_npcPath.getPathYExtents( &minY, &maxY );
	m_height = maxY - minY;

	if ( ThisHazard->PointCount > 1 )
	{
		newXPos = (u16) *PntList;

		if ( ( ( newXPos << 4 ) + 8 ) < startPos.vx )
		{
			m_width = -m_width;
			m_heading = 2048;
		}
	}

	m_thinkArea.x1 = minX;
	m_thinkArea.x2 = maxX;
	m_thinkArea.y1 = minY;
	m_thinkArea.y2 = maxY;
}

/*=========================================================================

	ptrpdoor.cpp

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __PLATFORM_PTRPDOOR_H__
#include "platform\ptrpdoor.h"
#endif

#ifndef	__UTILS_HEADER__
#include	"utils\utils.h"
#endif


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcTrapdoorPlatform::postInit()
{
	CNpcPlatform::postInit();

	m_triggered = false;

	if ( m_pointRight )
	{
		setCollisionAngle( 1024 );
	}
	else
	{
		setCollisionAngle( -1024 );
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcTrapdoorPlatform::setWaypoints( sThingPlatform *ThisPlatform )
{
	ASSERT( ThisPlatform->PointCount == 3 );

	u16	*PntList=(u16*)MakePtr(ThisPlatform,sizeof(sThingPlatform));

	u16 newXPos, newYPos;

	// get master hazard init pos

	newXPos = (u16) *PntList;
	PntList++;
	newYPos = (u16) *PntList;
	PntList++;

	DVECTOR startPos;
	startPos.vx = ( newXPos << 4 ) + 8;
	startPos.vy = ( newYPos << 4 ) + 16;

	Pos = startPos;
	m_base = Pos;

	init( startPos );

	// skip next

	newXPos = (u16) *PntList;
	PntList++;
	newYPos = (u16) *PntList;
	PntList++;

	if ( ( ( newXPos << 4 ) + 8 ) > m_base.vx )
	{
		m_pointRight = true;
	}
	else
	{
		m_pointRight = false;
	}

	// get slave trigger position

	newXPos = (u16) *PntList;
	PntList++;
	newYPos = (u16) *PntList;
	PntList++;

	m_triggerPos.vx = newXPos;
	m_triggerPos.vy = newYPos;

	s32 minX, maxX, minY, maxY;

	m_npcPath.getPathXExtents( &minX, &maxX );
	m_npcPath.getPathYExtents( &minY, &maxY );

	m_thinkArea.x1 = minX;
	m_thinkArea.x2 = maxX;
	m_thinkArea.y1 = minY;
	m_thinkArea.y2 = maxY;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcTrapdoorPlatform::processMovement( int _frames )
{
	s16 newAngle;

	newAngle = getCollisionAngle();

	if ( m_pointRight )
	{
		if ( m_triggered )
		{
			newAngle -= 32 * _frames;

			if ( newAngle < 0 )
			{
				newAngle = 0;
			}

			setCollisionAngle( newAngle );

			m_triggered = false;
		}
		else
		{
			newAngle += 32 * _frames;

			if ( newAngle > 1024 )
			{
				newAngle = 1024;
			}

			setCollisionAngle( newAngle );
		}
	}
	else
	{
		if ( m_triggered )
		{
			newAngle += 32 * _frames;

			if ( newAngle > 0 )
			{
				newAngle = 0;
			}

			setCollisionAngle( newAngle );

			m_triggered = false;
		}
		else
		{
			newAngle -= 32 * _frames;

			if ( newAngle < -1024 )
			{
				newAngle = -1024;
			}

			setCollisionAngle( newAngle );
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcTrapdoorPlatform::render()
{
	if ( m_isActive )
	{
		CPlatformThing::render();

		if (canRender())
		{
			DVECTOR &renderPos=getRenderPos();
			SVECTOR rotation;
			rotation.vx = 0;
			if ( m_reversed )
			{
				rotation.vy = 0;
				rotation.vz = getCollisionAngle() & 4095;
			}
			else
			{
				rotation.vy = 0;
				rotation.vz = getCollisionAngle() & 4095;
			}

			VECTOR scale;
			scale.vx = ONE;
			scale.vy = ONE;
			scale.vz = ONE;

			m_modelGfx->Render(renderPos,&rotation,&scale);
		}
	}
}

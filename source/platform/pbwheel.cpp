/*=========================================================================

	pbwheel.cpp

	Author:		CRB
	Created:
	Project:	Spongebob
	Purpose:

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __PLATFORM_PBWHEEL_H__
#include "platform\pbwheel.h"
#endif

#ifndef	__UTILS_HEADER__
#include	"utils\utils.h"
#endif


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcBigWheelPlatform::postInit()
{
	CNpcPlatform::postInit();
	//sBBox boundingBox = m_modelGfx->GetBBox();
	//boundingBox.YMin = boundingBox.YMax - 32;
	//setCollisionSize( ( boundingBox.XMax - boundingBox.XMin ) - 8, ( boundingBox.YMax - boundingBox.YMin ) );
	//setCollisionCentreOffset( ( boundingBox.XMax + boundingBox.XMin ) >> 1, ( boundingBox.YMax + boundingBox.YMin ) >> 1 );

	//calculateNonRotatedCollisionData();
	//setCollisionAngle( m_tiltAngle >> 8 );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcBigWheelPlatform::processMovement( int _frames )
{
	m_rotation += m_speed * _frames;
	m_rotation &= 4095;

	Pos.vx = m_base.vx + ( ( m_extension * rcos( m_rotation ) ) >> 12 );
	Pos.vy = m_base.vy + ( ( m_extension * rsin( m_rotation ) ) >> 12 );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcBigWheelPlatform::setWaypoints( sThingPlatform *ThisPlatform )
{
	int pointNum;

	u16	*PntList=(u16*)MakePtr(ThisPlatform,sizeof(sThingPlatform));

	u16 newXPos, newYPos;

	newXPos = (u16) *PntList;
	PntList++;
	newYPos = (u16) *PntList;
	PntList++;

	DVECTOR startPos;
	startPos.vx = ( newXPos << 4 ) + 8;
	startPos.vy = ( newYPos << 4 ) + 16;

	if ( ThisPlatform->PointCount > 1 )
	{
		newXPos = (u16) *PntList;
		PntList++;
		newYPos = (u16) *PntList;
		PntList++;

		DVECTOR pivotPos;
		pivotPos.vx = ( newXPos << 4 ) + 8;
		pivotPos.vy = ( newYPos << 4 ) + 16;

		s32 xDist = startPos.vx - pivotPos.vx;
		s32 yDist = startPos.vy - pivotPos.vy;

		init( pivotPos );

		m_rotation = ratan2( yDist, xDist );

		m_extension = isqrt2( ( xDist * xDist ) + ( yDist * yDist ) );

	}
	else
	{
		init( startPos );
	}

	m_thinkArea.x1 = Pos.vx - m_extension;
	m_thinkArea.x2 = Pos.vx + m_extension;
	m_thinkArea.y1 = Pos.vy - m_extension;
	m_thinkArea.y2 = Pos.vy + m_extension;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int	CNpcBigWheelPlatform::getHeightFromPlatformAtPosition(int _x,int _y, int offsetX, int offsetY)
{
	DVECTOR top;

	CRECT const &collisionArea = getCollisionArea();

	top.vy = offsetY + collisionArea.y1;

	s32 centreX = ( ( collisionArea.x1 + collisionArea.x2 ) >> 1 ) + offsetX;

	if ( abs( _x - centreX ) <= 20 )
	{
		top.vy = offsetY + collisionArea.y2 - 16;
	}
	else
	{
		s32 baseY = offsetY + collisionArea.y2 - 16;

		top.vy = baseY + ( ( ( abs( _x - centreX ) - 20 ) * ( offsetY + collisionArea.y1 - baseY ) ) / ( collisionArea.x2 - centreX - 20 ) );
	}

	// Non-rotated platform

	return( top.vy - _y );
}

/*=========================================================================

	pbranch.cpp

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __PLATFORM_PBRANCH_H__
#include "platform\pbranch.h"
#endif

#ifndef	__UTILS_HEADER__
#include	"utils\utils.h"
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcBranchPlatform::postInit()
{
	m_angularVelocity = 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcBranchPlatform::setWaypoints( sThingPlatform *ThisPlatform )
{
	int pointNum;

	u16	*PntList=(u16*)MakePtr(ThisPlatform,sizeof(sThingPlatform));

	u16 initXPos, newXPos, newYPos;

	initXPos = newXPos = (u16) *PntList;
	PntList++;
	newYPos = (u16) *PntList;
	PntList++;

	DVECTOR startPos;
	startPos.vx = newXPos << 4;
	startPos.vy = newYPos << 4;

	init( startPos );

	if ( ThisPlatform->PointCount > 1 )
	{
		newXPos = (u16) *PntList;

		if ( newXPos < initXPos )
		{
			m_reversed = true;
		}
		else
		{
			m_reversed = false;
		}
	}
	else
	{
		m_reversed = false;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcBranchPlatform::processMovement( int _frames )
{
	s16 newAngle = getCollisionAngle();

	if ( m_contact )
	{
		if ( ( m_reversed && newAngle < -256 ) || newAngle > 256 )
		{
			// flick player upwards
		}

		s16 angularForce = 3 * _frames;

		if ( m_reversed )
		{
			angularForce = -angularForce;
		}

		m_angularVelocity += angularForce;
	}

	s32 resistance = -( 10 * newAngle ) >> 8;

	if ( newAngle > 0 && resistance > -2 )
	{
		resistance = -2;
	}
	else if ( newAngle < 0 && resistance < 2 )
	{
		resistance = 2;
	}

	// get direction of resistance

	m_angularVelocity += resistance;

	newAngle += m_angularVelocity;

	if ( m_angularVelocity )
	{
		m_angularVelocity += -m_angularVelocity / abs( m_angularVelocity );
	}

	/*if ( newAngle > 320 )
	{
		newAngle = 320;
	}
	else if ( newAngle < -320 )
	{
		newAngle = -320;
	}*/

	setCollisionAngle( newAngle );
}

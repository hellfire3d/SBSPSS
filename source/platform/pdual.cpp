/*=========================================================================

	pdual.cpp

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __PLATFORM_PDUAL_H__
#include "platform\pdual.h"
#endif

#ifndef	__UTILS_HEADER__
#include	"utils\utils.h"
#endif


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcDualPlatform::setMaster( u8 isMaster )
{
	m_isMaster = isMaster;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcDualPlatform::setOtherPlatform( CNpcDualPlatform *other )
{
	m_otherPlatform = other;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcDualPlatform::setWaypoints( sThingPlatform *ThisPlatform )
{
	ASSERT( ThisPlatform->PointCount == 3 );

	u16	*PntList=(u16*)MakePtr(ThisPlatform,sizeof(sThingPlatform));

	u16 newXPos, newYPos;

	// get master platform init pos

	newXPos = (u16) *PntList;
	PntList++;
	newYPos = (u16) *PntList;
	PntList++;

	DVECTOR startPos;
	startPos.vx = newXPos << 4;
	startPos.vy = newYPos << 4;

	init( startPos );

	m_extension = 0;

	// get master platform max vertical extension

	newXPos = (u16) *PntList;
	PntList++;
	newYPos = (u16) *PntList;
	PntList++;

	m_maxExtension = ( newYPos << 4 ) - startPos.vy;

	ASSERT( m_maxExtension >= 0 );

	// get slave platform init x pos

	newXPos = (u16) *PntList;

	DVECTOR slavePos;
	slavePos.vx = newXPos << 4;
	slavePos.vy = startPos.vy + m_maxExtension;

	m_otherPlatform->init( slavePos );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

u8 CNpcDualPlatform::canDrop()
{
	if ( m_isMaster )
	{
		s32 extensionLeft = m_maxExtension - m_extension;

		if ( extensionLeft == 0 )
		{
			return( false );
		}
		else
		{
			return( true );
		}
	}
	else
	{
		return( false );
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcDualPlatform::processMovement( int _frames )
{
	if ( m_isMaster )
	{
		s32 extensionChange = 0;
		s32 extensionLeft;

		if ( m_contact )
		{
			extensionLeft = m_maxExtension - m_extension;

			extensionChange = 3 * _frames;

			if ( extensionChange > extensionLeft )
			{
				extensionChange = extensionLeft;
			}
		}
		else
		{
			extensionLeft = m_extension;

			extensionChange = -3 * _frames;

			if ( -extensionChange > extensionLeft )
			{
				extensionChange = -extensionLeft;
			}
		}

		m_extension += extensionChange;

		Pos.vy = m_base.vy + m_extension;

		DVECTOR slaveMove;
		slaveMove.vx = 0;
		slaveMove.vy = -extensionChange;

		m_otherPlatform->setMovement( slaveMove );
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcDualPlatform::setMovement( DVECTOR move )
{
	Pos.vx += move.vx;
	Pos.vy += move.vy;
}

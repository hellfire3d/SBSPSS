/*=========================================================================

	prbridge.cpp

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __PLATFORM_PRBRIDGE_H__
#include "platform\prbridge.h"
#endif

#include "triggers\trigger.h"

#ifndef	__TRIGGERS_TPLATFRM_H__
#include "triggers\tplatfrm.h"
#endif

#ifndef	__UTILS_HEADER__
#include	"utils\utils.h"
#endif


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcRisingBridgePlatform::postInit()
{
	CNpcPlatform::postInit();

	m_triggered = false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcRisingBridgePlatform::trigger()
{
	m_triggered = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcRisingBridgePlatform::setWaypoints( sThingPlatform *ThisPlatform )
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
	startPos.vx = ( newXPos << 4 ) + 8;
	startPos.vy = ( newYPos << 4 ) + 16;

	init( startPos );

	m_extension = 0;

	// get master platform max vertical extension

	newXPos = (u16) *PntList;
	PntList++;
	newYPos = (u16) *PntList;
	PntList++;

	m_maxExtension = abs( ( ( newYPos << 4 ) + 16 ) - startPos.vy ) << 8;

	// get slave trigger position

	newXPos = (u16) *PntList;
	PntList++;
	newYPos = (u16) *PntList;
	PntList++;

	CPlatformTrigger *trigger;

//	trigger = new ("PlatformTrigger") CPlatformTrigger();
	trigger=(CPlatformTrigger*)CTrigger::Create(CTrigger::TRIGGER_PLATFORM);
	trigger->setPositionAndSize( ( newXPos << 4 ) + 8 - 20, ( newYPos << 4 ) + 16 - 20, 40, 40 );
	//trigger->setTargetBox(TriggerList->TargetPos.X<<4,TriggerList->TargetPos.Y<<4,TriggerList->TargetSize.X<<4,TriggerList->TargetSize.Y<<4);
	trigger->setPlatform( this );

	s32 minX, maxX, minY, maxY;

	m_npcPath.getPathXExtents( &minX, &maxX );
	m_npcPath.getPathYExtents( &minY, &maxY );

	m_thinkArea.x1 = minX;
	m_thinkArea.x2 = maxX;
	m_thinkArea.y1 = minY;
	m_thinkArea.y2 = maxY;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcRisingBridgePlatform::processMovement( int _frames )
{
	if ( m_triggered )
	{
		m_triggered = false;
		m_extension += ( m_dataPtr->speed * _frames ) << 8;
		if ( m_extension > m_maxExtension )
		{
			m_extension = m_maxExtension;
		}
	}
	else
	{
		m_extension -= 16 * _frames;

		if ( m_extension < 0 )
		{
			m_extension = 0;
		}
	}

	Pos.vy = m_base.vy - ( m_extension >> 8 );
}

/*=========================================================================

	prbridge.h

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __PLATFORM_PRBRIDGE_H__
#define __PLATFORM_PRBRIDGE_H__

#ifndef __PLATFORM_PLATFORM_H__
#include "platform\platform.h"
#endif

class CNpcRisingBridgePlatform : public CNpcPlatform
{
public:
	virtual void		postInit();
	virtual void		trigger();
protected:
	virtual void		setWaypoints( sThingPlatform *ThisPlatform );
	virtual void		processMovement( int _frames );

	s32					m_maxExtension;
	u8					m_triggered;
};

#endif
/*=========================================================================

	pbwheel.h

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __PLATFORM_PBWHEEL_H__
#define __PLATFORM_PBWHEEL_H__

#ifndef __PLATFORM_PLATFORM_H__
#include "platform\platform.h"
#endif

class CNpcBigWheelPlatform : public CNpcPlatform
{
public:
	virtual	CRECT const		*getThinkBBox();
protected:
	virtual void		setWaypoints( sThingPlatform *ThisPlatform );
	virtual void		processMovement( int _frames );
};

#endif
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
	bool				alwaysThink()								{return(true);}
	void				postInit();
	int					getHeightFromPlatformAtPosition(int _x,int _y, int offsetX = 0, int offsetY = 0);
protected:
	void				setWaypoints( sThingPlatform *ThisPlatform );
	void				processMovement( int _frames );
};

#endif
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
	void				postInit();
	void				trigger();
	CRECT const			*getThinkBBox()											{return( CThing::getThinkBBox() );}
protected:
	void				setWaypoints( sThingPlatform *ThisPlatform );
	void				processMovement( int _frames );

	s32					m_maxExtension;
	u8					m_triggered;
};

#endif
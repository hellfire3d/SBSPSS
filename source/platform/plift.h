/*=========================================================================

	plift.h

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __PLATFORM_PLIFT_H__
#define __PLATFORM_PLIFT_H__

#ifndef __PLATFORM_PLATFORM_H__
#include "platform\platform.h"
#endif

class CNpcLiftPlatform : public CNpcPlatform
{
public:
	void				postInit();
	void				setWaypoints( sThingPlatform *ThisPlatform );
protected:
	void				collidedWith(CThing *_thisThing);
	void				processMovement( int _frames );
	void				processTimer( int _frames );

	u8					m_isActivated;
	u8					m_returning;
	s32					m_maxExtension;
};

#endif
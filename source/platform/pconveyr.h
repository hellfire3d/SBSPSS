/*=========================================================================

	pconveyr.h

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __PLATFORM_PCONVEYR_H__
#define __PLATFORM_PCONVEYR_H__

#ifndef __PLATFORM_PLATFORM_H__
#include "platform\platform.h"
#endif

class CNpcConveyorPlatform : public CNpcPlatform
{
public:
	virtual void		postInit();
	virtual	CRECT const		*getThinkBBox();
protected:
	virtual void		processMovement( int _frames );
	virtual void		collidedWith(CThing *_thisThing);
};

class CNpcConveyorPlatformGenerator : public CNpcPlatform
{
public:
	virtual void		render();
	virtual	CRECT const		*getThinkBBox();
protected:
	virtual void		think( int _frames );
	virtual void		collidedWith(CThing *_thisThing);
};

#endif
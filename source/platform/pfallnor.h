/*=========================================================================

	pfallnor.h

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __PLATFORM_PFALLNOR_H__
#define __PLATFORM_PFALLNOR_H__

#ifndef __PLATFORM_PLATFORM_H__
#include "platform\platform.h"
#endif

class CNpcFallingNoRespawnPlatform : public CNpcPlatform
{
public:
	virtual void		postInit();
	virtual	CRECT const		*getThinkBBox();
protected:
	virtual void		processMovement( int _frames );
};

#endif
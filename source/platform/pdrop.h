/*=========================================================================

	pdrop.h

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __PLATFORM_PDROP_H__
#define __PLATFORM_PDROP_H__

#ifndef __PLATFORM_PLATFORM_H__
#include "platform\platform.h"
#endif

class CNpcDropPlatform : public CNpcPlatform
{
public:
	virtual	CRECT const		*getThinkBBox();
protected:
	virtual void		processMovement( int _frames );
};

#endif
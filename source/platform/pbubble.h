/*=========================================================================

	pbubble.h

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __PLATFORM_PBUBBLE_H__
#define __PLATFORM_PBUBBLE_H__

#ifndef __PLATFORM_PLATFORM_H__
#include "platform\platform.h"
#endif

class CNpcBubblePlatform : public CNpcPlatform
{
protected:
	virtual void		processMovement( int _frames );
};

#endif
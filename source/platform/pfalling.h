/*=========================================================================

	pfalling.h

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __PLATFORM_PFALLING_H__
#define __PLATFORM_PFALLING_H__

#ifndef __PLATFORM_PLATFORM_H__
#include "platform\platform.h"
#endif

class CNpcFallingPlatform : public CNpcPlatform
{
public:
	void				postInit();
protected:
	void				processMovement( int _frames );
};

#endif
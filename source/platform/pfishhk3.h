/*=========================================================================

	pfishhk3.h

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __PLATFORM_PFISHHK3_H__
#define __PLATFORM_PFISHHK3_H__

#ifndef __PLATFORM_PFISHHK_H__
#include "platform\pfishhk.h"
#endif

class CNpcFishHook3Platform : public CNpcFishHookPlatform
{
public:
	void				postInit();
};

#endif
/*=========================================================================

	plurve.h

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __PLATFORM_PLURVE_H__
#define __PLATFORM_PLURVE_H__

#ifndef __PLATFORM_PGHOST_H__
#include "platform\pghost.h"
#endif

class CNpcLoveBoatPlatform : public CNpcGhostTrainPlatform
{
public:
	void				postInit();
};

#endif
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

#ifndef __PLATFORM_PRAFT_H__
#include "platform\praft.h"
#endif

class CNpcLoveBoatPlatform : public CNpcRaftPlatform
{
public:
	virtual void		postInit();
};

#endif
/*=========================================================================

	pbranch.h

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __PLATFORM_PBRANCH_H__
#define __PLATFORM_PBRANCH_H__

#ifndef __PLATFORM_PLATFORM_H__
#include "platform\platform.h"
#endif

class CNpcBranchPlatform : public CNpcPlatform
{
public:
	virtual void		postInit();
protected:
	virtual void		processMovement( int _frames );

	enum NPC_BRANCH_STATE
	{
		NPC_BRANCH_STOP = 0,
		NPC_BRANCH_DIP = 1,
		NPC_BRANCH_SPRING,
	};
};

#endif
/*=========================================================================

	pbob.h

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __PLATFORM_PBOB_H__
#define __PLATFORM_PBOB_H__

#ifndef __PLATFORM_PLATFORM_H__
#include "platform\platform.h"
#endif

class CNpcBobbingPlatform : public CNpcPlatform
{
public:
	void				postInit();
protected:
	void				processMovement( int _frames );

	enum NPC_BOB_STATE
	{
		NPC_BOB_STOP = 0,
		NPC_BOB_MOVE = 1,
	};

	s32					m_vertPos;
};

#endif
/*=========================================================================

	pretract.h

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __PLATFORM_PRETRACT_H__
#define __PLATFORM_PRETRACT_H__

#ifndef __PLATFORM_PLATFORM_H__
#include "platform\platform.h"
#endif

class CNpcRetractingPlatform : public CNpcPlatform
{
public:
	virtual void		postInit();
protected:
	void processTimer( int _frames );

	enum NPC_PLATFORM_TIMER_TYPE
	{
		NPC_PLATFORM_TIMER_RETRACT = 0,
		NPC_PLATFORM_TIMER_EXTEND = 1,
	};
};

#endif
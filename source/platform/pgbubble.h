/*=========================================================================

	pgbubble.h

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __PLATFORM_PGBUBBLE_H__
#define __PLATFORM_PGBUBBLE_H__

#ifndef __PLATFORM_PBUBBLE_H__
#include "platform\pbubble.h"
#endif

class CNpcGeyserBubblePlatform : public CNpcBubblePlatform
{
public:
	virtual void		postInit();
protected:
	virtual void		processMovement( int _frames );
	virtual void		processLifetime( int _frames );
};

#endif
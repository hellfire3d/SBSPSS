/*=========================================================================

	pgeyser.h

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __PLATFORM_PGEYSER_H__
#define __PLATFORM_PGEYSER_H__

#ifndef __PLATFORM_PLATFORM_H__
#include "platform\platform.h"
#endif

class CNpcGeyserPlatform : public CNpcPlatform
{
public:
	virtual void		postInit();
protected:
	virtual void		processMovement( int _frames );
	virtual void		processTimer( int _frames );

	bool				m_isFiring;
};

#endif
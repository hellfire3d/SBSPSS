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
	void				postInit();
protected:
	void				processMovement( int _frames );
	void				processTimer( int _frames );

	bool				m_isFiring;

	enum GEYSER_STATE
	{
		GEYSER_READY,
		GEYSER_RISING,
		GEYSER_DROPPING,
	};
};

#endif
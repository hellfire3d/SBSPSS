/*=========================================================================

	pconveyr.h

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __PLATFORM_PCONVEYR_H__
#define __PLATFORM_PCONVEYR_H__

#ifndef __PLATFORM_PLATFORM_H__
#include "platform\platform.h"
#endif

class CNpcConveyorPlatform : public CNpcPlatform
{
public:
	void				postInit();
	void				render();
	int					checkCollisionAgainst(CThing *_thisThing, int _frames);
protected:
	void				processMovement( int _frames );
	void				collidedWith(CThing *_thisThing);

	u8					m_spinFinish;
	s16					m_rotation;
};

class CNpcConveyorPlatformGenerator : public CNpcPlatform
{
public:
	void				render();
protected:
	void				think( int _frames );
	void				collidedWith(CThing *_thisThing);
};

#endif
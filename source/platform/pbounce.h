/*=========================================================================

	pbounce.h

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __PLATFORM_PBOUNCE_H__
#define __PLATFORM_PBOUNCE_H__

#ifndef __PLATFORM_PLATFORM_H__
#include "platform\platform.h"
#endif

class CNpcBouncePlatform : public CNpcPlatform
{
public:
	void				postInit();
	void				think( int _frames );
	void				render();
protected:
	virtual void		collidedWith(CThing *_thisThing);

	s32					m_vertScale;
	s32					m_vertVelocity;
};

#endif
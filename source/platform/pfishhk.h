/*=========================================================================

	pfishhk.h

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __PLATFORM_PFISHHK_H__
#define __PLATFORM_PFISHHK_H__

#ifndef __PLATFORM_PLATFORM_H__
#include "platform\platform.h"
#endif

class CNpcFishHookPlatform : public CNpcPlatform
{
public:
	virtual void		postInit()							{m_isMoving = false;}
protected:
	virtual void		processLifetime( int _frames );
	virtual void		processMovement( int _frames );

	bool	m_isMoving;
};

#endif
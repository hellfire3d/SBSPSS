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
	virtual void		postInit();
	void				setToShutdown();
	u8					isSetToShutdown()										{return( m_isShuttingDown );}
protected:
	virtual void		processLifetime( int _frames );
	virtual void		processMovement( int _frames );

	u8					m_isMoving;
	u8					m_isShuttingDown;
};

#endif
/*=========================================================================

	pcbubble.h

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __PLATFORM_PCBUBBLE_H__
#define __PLATFORM_PCBUBBLE_H__

#ifndef __PLATFORM_PLATFORM_H__
#include "platform\platform.h"
#endif

class CNpcCollapsingBubblePlatform : public CNpcPlatform
{
public:
	virtual void		render();
	virtual void		postInit();
protected:
	virtual void		processLifetime( int _frames );
	virtual int			checkCollisionAgainst(CThing *_thisThing, int _frames);
	u8					m_pop;
};

class CNpcCollapsingAcridPlatform : public CNpcCollapsingBubblePlatform
{
public:
	virtual void		render();
};

#endif
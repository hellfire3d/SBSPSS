/*=========================================================================

	pplayer.h

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __PLATFORM_PPLAYER_H__
#define __PLATFORM_PPLAYER_H__

#ifndef __PLATFORM_PLATFORM_H__
#include "platform\platform.h"
#endif

class CNpcPlayerBubblePlatform : public CNpcPlatform
{
public:
	virtual void		postInit();
	virtual void		render();
	virtual int			checkCollisionAgainst(CThing *_thisThing, int _frames);
protected:
	virtual void		processLifetime( int _frames );

	u8					m_pop;
};

#endif
/*=========================================================================

	pbubble.h

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __PLATFORM_PBUBBLE_H__
#define __PLATFORM_PBUBBLE_H__

#ifndef __PLATFORM_PLATFORM_H__
#include "platform\platform.h"
#endif

class CNpcBubblePlatform : public CNpcPlatform
{
public:
	virtual void		render();
	virtual void		postInit();
	void				shutdown();
	void				setGraphic( sThingPlatform *ThisPlatform )			{}
	void				setGraphic( u8 graphicNum )							{}
protected:
	int					checkCollisionAgainst(CThing *_thisThing, int _frames);

	u8					m_pop;
	s16					m_scale;
	POLY_FT4			*SprFrame;
};

#endif
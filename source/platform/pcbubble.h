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

#ifndef __PLATFORM_PBUBBLE_H__
#include "platform\pbubble.h"
#endif

class CNpcCollapsingBubblePlatform : public CNpcBubblePlatform
{
public:
	void				postInit();
	CRECT const			*getThinkBBox()												{return( CThing::getThinkBBox() );}
protected:
	void				processLifetime( int _frames );
	u8					m_startCollapse;
};

class CNpcCollapsingAcridPlatform : public CNpcCollapsingBubblePlatform
{
public:
	virtual void		render();
};

#endif
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

#ifndef __PLATFORM_PBUBBLE_H__
#include "platform\pbubble.h"
#endif

class CNpcPlayerBubblePlatform : public CNpcBubblePlatform
{
public:
	CRECT const			*getThinkBBox()												{return( CThing::getThinkBBox() );}
protected:
	void				processLifetime( int _frames );
};

#endif
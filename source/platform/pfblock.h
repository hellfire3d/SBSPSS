/*=========================================================================

	pfblock.h

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __PLATFORM_PFBLOCK_H__
#define __PLATFORM_PFBLOCK_H__

#ifndef __PLATFORM_PLATFORM_H__
#include "platform\platform.h"
#endif

class CNpcFallingBlockPlatform : public CNpcPlatform
{
public:
	void				postInit();
	CRECT const			*getThinkBBox()												{return( CThing::getThinkBBox() );}
	int					checkCollisionAgainst(CThing *_thisThing, int _frames);
	void				trigger();
protected:
	void				processMovement( int _frames );

	u8					m_isTriggered;
	u8					m_isFalling;
	s32					m_timer;
};

#endif
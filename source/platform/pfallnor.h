/*=========================================================================

	pfallnor.h

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __PLATFORM_PFALLNOR_H__
#define __PLATFORM_PFALLNOR_H__

#ifndef __PLATFORM_PLATFORM_H__
#include "platform\platform.h"
#endif

class CNpcFallingNoRespawnPlatform : public CNpcPlatform
{
public:
	virtual void		postInit();
	virtual	CRECT const		*getThinkBBox();
	virtual int			checkCollisionAgainst(CThing *_thisThing, int _frames);
protected:
	virtual void		render();
	virtual void		processMovement( int _frames );

	u8					m_spinFinish;
	s16					m_rotation;
	u8					m_bounceDir;
};

#endif
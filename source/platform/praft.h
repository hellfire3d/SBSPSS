/*=========================================================================

	praft.h

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __PLATFORM_PRAFT_H__
#define __PLATFORM_PRAFT_H__

#ifndef __PLATFORM_PLATFORM_H__
#include "platform\platform.h"
#endif

class CNpcRaftPlatform : public CNpcPlatform
{
public:
	virtual void		postInit();
	virtual void		render();
	virtual	CRECT const		*getThinkBBox();
protected:
	virtual void		processMovement( int _frames );

	u8					m_isActivated;
	u8					m_isSinking;
};

#endif
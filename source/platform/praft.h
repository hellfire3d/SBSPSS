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
	void				postInit();
	void				render();
protected:
	void				processMovement( int _frames );

	u8					m_isActivated;
	u8					m_isSinking;
};

#endif
/*=========================================================================

	psbarrel.h

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __PLATFORM_PSBARREL_H__
#define __PLATFORM_PSBARREL_H__

#ifndef __PLATFORM_PLATFORM_H__
#include "platform\platform.h"
#endif

class CNpcSteerableBarrelPlatform : public CNpcPlatform
{
public:
	virtual void		render();
protected:
	virtual void		processMovement( int _frames );

	s16					m_rotation;
};

#endif
/*=========================================================================

	plinear.h

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __PLATFORM_PLINEAR_H__
#define __PLATFORM_PLINEAR_H__

#ifndef __PLATFORM_PLATFORM_H__
#include "platform\platform.h"
#endif

class CNpcLinearPlatform : public CNpcPlatform
{
public:
	virtual void		postInit();
protected:
	void				processMovement( int _frames );

	u8					m_silent;
};

#endif
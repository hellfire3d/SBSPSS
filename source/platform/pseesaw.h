/*=========================================================================

	pseesaw.h

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __PLATFORM_PSEESAW_H__
#define __PLATFORM_PSEESAW_H__

#ifndef __PLATFORM_PLATFORM_H__
#include "platform\platform.h"
#endif

class CNpcSeesawPlatform : public CNpcPlatform
{
public:
	virtual void		postInit();
	virtual void		render();
protected:
	virtual void		processMovement( int _frames );

	s32					m_angularVelocity;
	s32					m_currentAngle;
};

#endif
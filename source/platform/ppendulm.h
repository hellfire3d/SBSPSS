/*=========================================================================

	ppendulm.h

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __PLATFORM_PPENDULM_H__
#define __PLATFORM_PPENDULM_H__

#ifndef __PLATFORM_PLATFORM_H__
#include "platform\platform.h"
#endif

class CNpcPendulumPlatform : public CNpcPlatform
{
public:
	void				postInit();
	void				render();
protected:
	void				setWaypoints( sThingPlatform *ThisPlatform );
	void				processMovement( int _frames );

	s32					m_length;
	s32					m_maxExtension;
	DVECTOR				m_lineBase;
};

#endif
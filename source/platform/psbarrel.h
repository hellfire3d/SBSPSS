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
	void				render();
	void				postInit();
	CRECT const			*getThinkBBox()											{return( CThing::getThinkBBox() );}
protected:
	virtual void		processMovement( int _frames );
	void				collidedWith(CThing *_thisThing);

	s16					m_rotation;
	s32					m_currentSpeed;
	s16					m_moveXHighRes;
};

#endif
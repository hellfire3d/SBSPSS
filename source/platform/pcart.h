/*=========================================================================

	pcart.h

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __PLATFORM_PCART_H__
#define __PLATFORM_PCART_H__

#ifndef __PLATFORM_PLATFORM_H__
#include "platform\platform.h"
#endif

class CNpcCartPlatform : public CNpcPlatform
{
public:
	virtual void		postInit();
	virtual void		render();
protected:
	virtual void		processMovement( int _frames );

	s32					m_carSpeed;
	u8					m_isActivated;
};

#endif
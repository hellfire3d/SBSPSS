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
	void				postInit();
	void				render();
	u8					isCart()												{return( true );}
	void				jump();
	bool				alwaysThink()											{return(true);}
protected:
	void				processMovement( int _frames );
	void				collidedWith(CThing *_thisThing);

	s32					m_carSpeed;
	u8					m_isActivated;
	u8					m_inJump;
	s32					m_vertSpeed;
	u8					m_playerAttached;
	u8					m_rebound;
	u8					m_falling;
	u8					m_trackContact;
};

#endif
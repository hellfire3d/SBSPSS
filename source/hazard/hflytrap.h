/*=========================================================================

	hflytrap.h

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __HAZARD_HFLYTRAP_H__
#define __HAZARD_HFLYTRAP_H__

#ifndef __HAZARD_HAZARD_H__
#include "hazard\hazard.h"
#endif

class CNpcFlyTrapHazard : public CNpcHazard
{
public:
	void				init();
	void				render();
protected:
	void				processMovement( int _frames );
	void				collidedWith(CThing *_thisThing);

	s16					m_rotation;
	u8					m_shut;
	u8					m_contact;
};

#endif
/*=========================================================================

	hbwheel.h

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __HAZARD_HBWHEEL_H__
#define __HAZARD_HBWHEEL_H__

#ifndef __HAZARD_HAZARD_H__
#include "hazard\hazard.h"
#endif

class CNpcBigWheelHazard : public CNpcHazard
{
public:
	void				init();
	void				render();
	bool				alwaysThink()								{return(true);}
protected:
	void				processMovement( int _frames );
	void				collidedWith(CThing *_thisThing);

	s16					m_rotation;
};

#endif
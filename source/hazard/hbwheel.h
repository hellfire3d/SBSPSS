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
	virtual void		render();
	virtual	CRECT const		*getThinkBBox();
protected:
	void				processMovement( int _frames );
	virtual void		collidedWith(CThing *_thisThing);

	s16					m_rotation;
};

#endif
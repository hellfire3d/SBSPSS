/*=========================================================================

	hcswitch.h

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __HAZARD_HCSWITCH_H__
#define __HAZARD_HCSWITCH_H__

#ifndef __HAZARD_HAZARD_H__
#include "hazard\hazard.h"
#endif

class CNpcConveyorSwitchHazard : public CNpcHazard
{
public:
	virtual void		setWaypoints( sThingHazard *ThisHazard );
	void				init();
	virtual void		render();
	virtual void		trigger();
protected:
	virtual void		collidedWith(CThing *_thisThing)		{}

	u8					m_reversed;
};

#endif
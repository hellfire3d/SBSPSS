/*=========================================================================

	hpswitch.h

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __HAZARD_HPSWITCH_H__
#define __HAZARD_HPSWITCH_H__

#ifndef __HAZARD_HAZARD_H__
#include "hazard\hazard.h"
#endif

#ifndef __PLATFORM_PTRPDOOR_H__
#include "platform\ptrpdoor.h"
#endif

class CNpcPressureSwitchHazard : public CNpcHazard
{
public:
	void				init();
	DVECTOR const		&getTriggerPos()										{return( m_triggerPos );}
	void				linkToPlatform( CNpcTrapdoorPlatform *platform )		{m_platform = platform;}
	bool				alwaysThink()											{return(true);}
	//virtual void		render();
protected:
	void				setWaypoints( sThingHazard *ThisHazard );
	void				collidedWith(CThing *_thisThing);

	DVECTOR						m_triggerPos;
	CNpcTrapdoorPlatform		*m_platform;
	s16							m_rotation;
};

#endif
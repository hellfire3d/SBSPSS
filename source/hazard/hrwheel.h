/*=========================================================================

	hrwheel.h

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __HAZARD_HRWHEEL_H__
#define __HAZARD_HRWHEEL_H__

#ifndef __HAZARD_HAZARD_H__
#include "hazard\hazard.h"
#endif

#ifndef __HAZARD_HRWEIGHT_H__
#include "hazard\hrweight.h"
#endif

class CNpcRisingWeightWheelHazard : public CNpcHazard
{
public:
	void				init();
	DVECTOR const		&getWheelPos()											{return( m_wheelPos );}
	void				linkToWeight( CNpcRisingWeightHazard *weight )			{m_weight = weight;}
	void				render();
	bool				alwaysThink()											{return(true);}
	void				weightDrop();
protected:
	void				setWaypoints( sThingHazard *ThisHazard );
	void				collidedWith(CThing *_thisThing);

	DVECTOR				m_wheelPos;
	CNpcRisingWeightHazard		*m_weight;
	s16					m_rotation;
};

#endif
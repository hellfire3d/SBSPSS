/*=========================================================================

	hrweight.h

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __HAZARD_HRWEIGHT_H__
#define __HAZARD_HRWEIGHT_H__

#ifndef __HAZARD_HAZARD_H__
#include "hazard\hazard.h"
#endif

class CNpcRisingWeightWheelHazard;

class CNpcRisingWeightHazard : public CNpcHazard
{
public:
	void				init();
	void				render();
	DVECTOR const		&getWheelPos()											{return( m_wheelPos );}
	void				setTriggered()											{m_triggered = true;}
	bool				alwaysThink()											{return(true);}
	void				linkToWheel( CNpcRisingWeightWheelHazard *wheel )		{m_wheel = wheel;}
protected:
	virtual void		collidedWith(CThing *_thisThing);
	void				setWaypoints( sThingHazard *ThisHazard );
	void				processMovement( int _frames );

	CNpcRisingWeightWheelHazard		*m_wheel;
	s32					m_maxExtension;
	DVECTOR				m_wheelPos;
	DVECTOR				m_pulleyPos;
	u8					m_triggered;
};

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
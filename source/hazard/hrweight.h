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

class CNpcRisingWeightHazard : public CNpcHazard
{
public:
	void				init();
	virtual void		render();
	DVECTOR const		&getWheelPos()											{return( m_wheelPos );}
	void				setTriggered()											{m_triggered = true;}
protected:
	virtual void		setWaypoints( sThingHazard *ThisHazard );
	virtual void		processMovement( int _frames );

	s32					m_maxExtension;
	DVECTOR				m_wheelPos;
	DVECTOR				m_pulleyPos;
	u8					m_triggered;
};

#endif
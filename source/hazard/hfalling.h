/*=========================================================================

	hfalling.h

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __HAZARD_HFALLING_H__
#define __HAZARD_HFALLING_H__

#ifndef __HAZARD_HAZARD_H__
#include "hazard\hazard.h"
#endif

class CNpcFallingHazard : public CNpcHazard
{
public:
	void				init();
	void				setWaypoints( sThingHazard *ThisHazard );
	void				render();
	bool				alwaysThink()											{return(m_bounceFinish);}
protected:
	void				processMovement( int _frames );
	void				processTimer( int _frames );
	void				collidedWith(CThing *_thisThing);

	s32					m_movementTimer;
	u8					m_bounceFinish;
	u8					m_spinFinish;
	s32					m_speed;
	u8					m_bounceDir;
	DVECTOR				m_bouncePos;
	s16					m_rotation;
	s16					m_scale;
	u8					m_growing;
};

#endif
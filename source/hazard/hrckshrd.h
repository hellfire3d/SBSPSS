/*=========================================================================

	hrckshrd.h

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __HAZARD_HRCKSHRD_H__
#define __HAZARD_HRCKSHRD_H__

#ifndef __HAZARD_HAZARD_H__
#include "hazard\hazard.h"
#endif

class CNpcRockShardHazard : public CNpcHazard
{
public:
	void				init();
	CRECT const			*getThinkBBox();
//	virtual void		setWaypoints( sThingHazard *ThisHazard );
protected:
	void				processMovement( int _frames );
	void				processTimer( int _frames );
//	virtual void		collidedWith(CThing *_thisThing);

	s32					m_movementTimer;
//	u8					m_bounceFinish;
//	s32					m_speed;
//	u8					m_bounceDir;
//	DVECTOR				m_bouncePos;
};

#endif
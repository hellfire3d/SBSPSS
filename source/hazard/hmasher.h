/*=========================================================================

	hmasher.h

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __HAZARD_HMASHER_H__
#define __HAZARD_HMASHER_H__

#ifndef __HAZARD_HAZARD_H__
#include "hazard\hazard.h"
#endif

class CNpcMasherHazard : public CNpcHazard
{
public:
	void				init();
	void				setGraphic( sThingHazard *ThisHazard );
protected:
	void				processMovement( int _frames );

	enum MASHER_STATE
	{
		MASHER_DROPPING,
		MASHER_RISING,
	};

	MASHER_STATE		m_state;

	int					m_pauseTimer;
};

#endif
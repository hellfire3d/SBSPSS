/*=========================================================================

	hspikes.h

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __HAZARD_HSPIKES_H__
#define __HAZARD_HSPIKES_H__

#ifndef __HAZARD_HAZARD_H__
#include "hazard\hazard.h"
#endif

class CNpcSpikesHazard : public CNpcHazard
{
public:
	void				init();
	bool				alwaysThink()								{return(true);}
protected:
	void				processTimer( int _frames );
	void				processMovement( int _frames );

	enum SPIKES_STATE
	{
		SPIKES_DROPPING,
		SPIKES_RISING,
	};

	SPIKES_STATE		m_state;
};

#endif
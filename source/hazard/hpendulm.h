/*=========================================================================

	hpendulm.h

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __HAZARD_HPENDULM_H__
#define __HAZARD_HPENDULM_H__

#ifndef __HAZARD_HAZARD_H__
#include "hazard\hazard.h"
#endif

class CNpcPendulumHazard : public CNpcHazard
{
public:
	void				init();
protected:
	void				processMovement( int _frames );
};

#endif
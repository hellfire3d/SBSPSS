/*=========================================================================

	hcheck.h

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __HAZARD_HCHECK_H__
#define __HAZARD_HCHECK_H__

#ifndef __HAZARD_HAZARD_H__
#include "hazard\hazard.h"
#endif

class CNpcCheckpointHazard : public CNpcHazard
{
public:
	void				init();
	void				render();
protected:
	void				collidedWith(CThing *_thisThing);

	u8					m_triggered;
};

#endif
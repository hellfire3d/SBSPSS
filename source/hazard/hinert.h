/*=========================================================================

	hinert.h

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __HAZARD_HINERT_H__
#define __HAZARD_HINERT_H__

#ifndef __HAZARD_HAZARD_H__
#include "hazard\hazard.h"
#endif

class CNpcInertHazard : public CNpcHazard
{
public:
	void				render();
protected:
	void				collidedWith(CThing *_thisThing);
};

#endif
/*=========================================================================

	hsrdfish.h

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __HAZARD_HSRDFISH_H__
#define __HAZARD_HSRDFISH_H__

#ifndef __HAZARD_HFIREBAL_H__
#include "hazard\hfirebal.h"
#endif

class CNpcSwordfishHazard : public CNpcFireballHazard
{
public:
	void				render();
	void				setWaypoints( sThingHazard *ThisHazard );
};

#endif
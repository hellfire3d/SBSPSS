/*=========================================================================

	plurve.h

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __PLATFORM_PLURVE_H__
#define __PLATFORM_PLURVE_H__

#ifndef __PLATFORM_PCART_H__
#include "platform\pcart.h"
#endif

class CNpcLoveBoatPlatform : public CNpcCartPlatform
{
public:
	virtual void		postInit();
};

#endif
/*=========================================================================

	pghost.h

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __PLATFORM_PGHOST_H__
#define __PLATFORM_PGHOST_H__

#ifndef __PLATFORM_PCART_H__
#include "platform\pcart.h"
#endif

class CNpcGhostTrainPlatform : public CNpcCartPlatform
{
public:
	void				postInit();
protected:
	void				processMovement( int _frames );

	u8					m_speedSetting;
};

#endif
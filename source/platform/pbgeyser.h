/*=========================================================================

	pbgeyser.h

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __PLATFORM_PBGEYSER_H__
#define __PLATFORM_PBGEYSER_H__

#ifndef __PLATFORM_PLATFORM_H__
#include "platform\platform.h"
#endif

class CNpcGeyserPlatformGenerator : public CNpcPlatform
{
public:
	void				setTargetType( NPC_PLATFORM_UNIT_TYPE targetType )		{m_targetType = targetType;}
	void				render();
protected:
	void				think( int _frames );
	void				collidedWith(CThing *_thisThing);

	NPC_PLATFORM_UNIT_TYPE	m_targetType;
};

#endif
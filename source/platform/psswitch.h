/*=========================================================================

	psswitch.h

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __PLATFORM_PSSWITCH_H__
#define __PLATFORM_PSSWITCH_H__

#ifndef __PLATFORM_PLATFORM_H__
#include "platform\platform.h"
#endif

class CNpcSteamSwitchPlatform : public CNpcPlatform
{
public:
	virtual void		postInit();
	virtual void		setWaypoints( sThingPlatform *ThisPlatform );
protected:
	virtual void		processMovement( int _frames );

	enum NPC_STEAM_SWITCH_STATE
	{
		NPC_STEAM_SWITCH_STOP = 0,
		NPC_STEAM_SWITCH_DEPRESS = 1,
		NPC_STEAM_SWITCH_RETURN,
	};

	s32					m_maxExtension;
};

#endif
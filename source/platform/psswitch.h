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

#ifndef __TRIGGERS_TSSWITCH_H__
#include "triggers\tsswitch.h"
#endif

class CNpcSteamSwitchPlatform : public CNpcPlatform
{
public:
	void				postInit();
	void				setWaypoints( sThingPlatform *ThisPlatform );
protected:
	void				processMovement( int _frames );
	void				processTimer( int _frames );

	enum NPC_STEAM_SWITCH_STATE
	{
		NPC_STEAM_SWITCH_STOP = 0,
		NPC_STEAM_SWITCH_DEPRESS = 1,
		NPC_STEAM_SWITCH_OFF,
		NPC_STEAM_SWITCH_RETURN,
	};

	s32					m_maxExtension;
	CSteamSwitchEmitterTrigger	*trigger;
};

#endif
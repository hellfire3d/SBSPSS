/*=========================================================================

	trigger.h

	Author:		dave 
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __TRIGGER_TRIGGER_HEADER__
#define __TRIGGER_TRIGGER_HEADER__

#ifndef __THING_THING_H__
#include "thing/thing.h"
#endif

class CTrigger : public CTriggerThing
{
public:
enum TRIGGER_TYPE
	{
	TRIGGER_EXIT=0,
	TRIGGER_RESPAWN,
	TRIGGER_TELEPORT,
	TRIGGER_CAMLOCK,
	TRIGGER_WATER,
	TRIGGER_MAX,
	};

	enum
	{ // For Dynamic ThingCache
		MAX_SUBTYPE	=TRIGGER_MAX,
	};

virtual void		init();
virtual void		shutdown();

static CTrigger		*Create(int Type);
static CTrigger		*Create(sThingTrigger *ThisTrigger);

protected:
virtual void		collidedWith(CThing *_thisThing){};

};

#endif


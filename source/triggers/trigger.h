/*=========================================================================

	trigger.h

	Author:		dave 
	Created:	cos paul was too lazy
	Project:	Spongebob
	Purpose:	Oh, cos I like the sound of my own typing

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
	TRIGGER_INWATER,
	TRIGGER_OUTWATER,
	TRIGGER_GARYSTART,
	TRIGGER_FLAMEEMITTER,
	TRIGGER_INTERMITTENTFLAMEEMITTER,
	TRIGGER_GARYSTOP,
	TRIGGER_GARYGOLEFT,
	TRIGGER_GARYGORIGHT,
	TRIGGER_BUBBLEEMITTER,

// Code based triggers
	TRIGGER_PLATFORM,
	TRIGGER_MAX,
	};

	enum
	{ // For Dynamic ThingCache
		MAX_SUBTYPE	=TRIGGER_MAX,
	};

virtual void		init();
virtual void		shutdown();
virtual void		think(int _frames);
virtual void		render();

static CTrigger		*Create(int Type);
static CTrigger		*Create(sThingTrigger *ThisTrigger);
virtual void		setPositionAndSize(int _x,int _y,int _w,int _h);
virtual void		setTargetBox(int _x,int _y,int _w,int _h);

protected:
virtual void		collidedWith(CThing *_thisThing){};

		int			m_boxX1,m_boxY1,m_boxX2,m_boxY2;

};

#endif


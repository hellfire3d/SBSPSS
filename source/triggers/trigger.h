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
	TRIGGER_GARY_START,
	TRIGGER_FLAME_EMITTER,
	TRIGGER_INTERMITTENT_FLAME_EMITTER,
	TRIGGER_GARY_STOP,
	TRIGGER_GARY_GO_LEFT,
	TRIGGER_GARY_GO_RIGHT,
	TRIGGER_BUBBLE_EMITTER,
	TRIGGER_INTERMITTENT_LEFT_FLAME_EMITTER,
	TRIGGER_INTERMITTENT_DOWN_FLAME_EMITTER,
	TRIGGER_BUBBLE_GEYSER_EMITTER,
	TRIGGER_STEAM_EMITTER,
	TRIGGER_GARY_BOWL,
	TRIGGER_WATER_DRIP_EMITTER,
	TRIGGER_ACID_DRIP_EMITTER,
	TRIGGER_OIL_DRIP_EMITTER,
	TRIGGER_LAVA_DRIP_EMITTER,
	TRIGGER_ACID_BUBBLE_EMITTER,
	TRIGGER_OIL_BUBBLE_EMITTER,
	TRIGGER_LAVA_BUBBLE_EMITTER,
	TRIGGER_STEAM_DOWN_EMITTER,
	TRIGGER_STEAM_LEFT_EMITTER,
	TRIGGER_STEAM_RIGHT_EMITTER,
	TRIGGER_WIND_UP,
	TRIGGER_WIND_DOWN,
	TRIGGER_WIND_LEFT,
	TRIGGER_WIND_RIGHT,
	TRIGGER_OIL_GEYSER,
	TRIGGER_BOSS_ARENA,
	TRIGGER_SECRET_AREA,
	TRIGGER_SPEECH_FIRST_NET,
	TRIGGER_SPEECH_FIRST_CORAL,
	TRIGGER_SPEECH_GARY_CH2L1,
	TRIGGER_SPEECH_BREAK_FLOOR,
	TRIGGER_SPEECH_BUBBLE_GEYSER,
	TRIGGER_SPEECH_RISING_PLATFORM,
	TRIGGER_SPEECH_FIRST_BUBBLE,
	TRIGGER_SPEECH_USE_BUBBLE,
	TRIGGER_SPEECH_WEIGHT,
	TRIGGER_CAMERAYPOSITIONTRIGGER,
	TRIGGER_BUTTERFLY_ON,

// Code based triggers
	TRIGGER_PLATFORM,
	TRIGGER_HAZARD,
	TRIGGER_HAZARD_WALK,
	TRIGGER_STEAM_SWITCH_EMITTER,
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
		void		setVal(int _val);

protected:
virtual void		collidedWith(CThing *_thisThing){};

		int			m_boxX1,m_boxY1,m_boxX2,m_boxY2;
		int			m_val0;

};

#endif


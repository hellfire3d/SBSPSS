/*=========================================================================

	hazard.h

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __HAZARD_HAZARD_H__
#define __HAZARD_HAZARD_H__

#ifndef __THING_THING_H__
#include "thing/thing.h"
#endif

#ifndef __ENEMY_NPCPATH_H__
#include "enemy\npcpath.h"
#endif

#ifndef __GFX_SPRBANK_H__
#include "gfx\sprbank.h"
#endif

#ifndef __ACTOR_HEADER__
#include "gfx\actor.h"
#endif


class CNpcHazard : public CHazardThing
{
public:
	enum NPC_HAZARD_UNIT_TYPE
	{
		NPC_FALLING_HAZARD = 0,
		NPC_PENDULUM_HAZARD = 1,
		NPC_BOAT_HAZARD,
		NPC_BARREL_HAZARD,
		NPC_STATIC_HAZARD,
		NPC_CIRCULAR_SAW_HAZARD,
		NPC_MOWER_HAZARD,
		NPC_MASHER_HAZARD,
		NPC_FAN_HAZARD,
		NPC_SPIKES_HAZARD,
		NPC_BIG_WHEEL_HAZARD,
		NPC_DUAL_PLATFORM_BARREL_HAZARD,
		NPC_BOUNCING_BARREL_HAZARD,

		NPC_HAZARD_TYPE_MAX,
	};

	virtual void		init();
	void				setGraphic( sThingHazard *ThisHazard );
	void				shutdown();
	void				think(int _frames);
	virtual void		render();
	void				setLayerCollision( class CLayerCollision *_layer )		{m_layerCollision=_layer;}
	virtual void		setWaypoints( sThingHazard *ThisHazard );
	void				addWaypoint( s32 xPos, s32 yPos );
	virtual void		setRespawnRate( s16 newRespawnRate )					{m_respawnRate=newRespawnRate;}

	static NPC_HAZARD_UNIT_TYPE	getTypeFromMapEdit( u16 newType );
	static CNpcHazard	*Create(sThingHazard *ThisHazard);

	void				setPlatform(CThing *_newPlatform)	{m_platform = _newPlatform;}
	void				clearPlatform()						{m_platform = NULL;}
	CThing				*isOnPlatform()						{return m_platform;}

protected:
	enum
	{
		EXTEND_UP = true,
		EXTEND_DOWN = false,
		EXTEND_RIGHT = true,
		EXTEND_LEFT = false,
		EXTEND_CLOCKWISE = true,
		EXTEND_ANTICLOCKWISE = false,
	};

	virtual void		processMovement( int _frames );
	virtual void		processTimer( int _frames );
	virtual void		collidedWith(CThing *_thisThing);

	CNpcPath			m_npcPath;
	//CActorGfx			*m_actorGfx;
	//SpriteBank			*m_spriteBank;
	DVECTOR				m_base;
	s32					m_timer;
	bool				m_timerActive;
	bool				m_isActive;
	class CLayerCollision	*m_layerCollision;
	s32					m_extension;
	bool				m_extendDir;
	s32					m_heading;
	CModelGfx			*m_modelGfx;
	s16					m_respawnRate;

	CThing				*m_platform;

	static NPC_HAZARD_UNIT_TYPE mapEditConvertTable[NPC_HAZARD_TYPE_MAX];
};

#endif


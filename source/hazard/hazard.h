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
	virtual void		init();
	void				shutdown();
	void				think(int _frames);
	void				render();
	void				setLayerCollision( class CLayerCollision *_layer )		{m_layerCollision=_layer;}

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
	CNpcPath			m_npcPath;
	CActorGfx			*m_actorGfx;
	SpriteBank			*m_spriteBank;
	DVECTOR				m_base;
	s32					m_timer;
	bool				m_timerActive;
	bool				m_isActive;
	class CLayerCollision	*m_layerCollision;
	s32					m_extension;
	bool				m_extendDir;
	s32					m_heading;
};

#endif


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
	void				init();
	void				shutdown();
	void				think(int _frames);
	void				render();

protected:
	CNpcPath			m_npcPath;
	CActorGfx			*m_actorGfx;
	SpriteBank			*m_spriteBank;
};

#endif


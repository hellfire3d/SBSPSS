/*=========================================================================

	hazard.h

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __HAZARD_HAZARD_H__
#include "hazard\hazard.h"
#endif

#ifndef __GAME_GAME_H__
#include	"game\game.h"
#endif

#ifndef __VID_HEADER_
#include "system\vid.h"
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcHazard::init()
{
	CHazardThing::init();

	m_actorGfx=CActorPool::GetActor( (FileEquate) ACTORS_CLAM_SBK );
	m_spriteBank=0;

	m_npcPath.initPath();

	Pos.vx = 300;
	Pos.vy = 300;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcHazard::shutdown()
{
	if (m_spriteBank) m_spriteBank->dump();		delete m_spriteBank;
	// remove waypoints

	m_npcPath.removeAllWaypoints();

	if (m_actorGfx)	delete m_actorGfx;

	CHazardThing::shutdown();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcHazard::think(int _frames)
{
	CHazardThing::think(_frames);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcHazard::render()
{
	CHazardThing::render();

	// Render
	DVECTOR renderPos;
	DVECTOR	offset = CLevel::getCameraPos();

	renderPos.vx = Pos.vx - offset.vx;
	renderPos.vy = Pos.vy - offset.vy;

	if ( renderPos.vx >= 0 && renderPos.vx <= VidGetScrW() )
	{
		if ( renderPos.vy >= 0 && renderPos.vy <= VidGetScrH() )
		{
			m_actorGfx->Render(renderPos,0,0,0);
		}
	}
}

/*=========================================================================

	fplnkton.cpp

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/

#ifndef __FRIEND_FPLNKTON_H__
#include "friend\fplnkton.h"
#endif

#ifndef __GFX_SPRBANK_H__
#include "gfx\sprbank.h"
#endif

#ifndef __LEVEL_LEVEL_H__
#include "level\level.h"
#endif

#ifndef __VID_HEADER_
#include "system\vid.h"
#endif

#ifndef __SPR_SPRITES_H__
#include <sprites.h>
#endif


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcPlanktonFriend::init()
{
	CNpcThing::init();

	m_extension = EXTEND_RIGHT;

	m_spriteBank=new ("enemy sprites") SpriteBank();
	m_spriteBank->load(SPRITES_SPRITES_SPR);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcPlanktonFriend::shutdown()
{
	m_spriteBank->dump();		delete m_spriteBank;

	CNpcThing::shutdown();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcPlanktonFriend::render()
{
	CNpcThing::render();

	// Render
	DVECTOR renderPos;
	DVECTOR origRenderPos;
	DVECTOR	offset = CLevel::getCameraPos();
	int frame = FRM_PLANKTON_STATIC00;
	int		spriteWidth = m_spriteBank->getFrameWidth( frame );
	int		spriteHeight = m_spriteBank->getFrameHeight( frame );

	renderPos.vx = Pos.vx - offset.vx - ( spriteWidth >> 1 );
	renderPos.vy = Pos.vy - offset.vy - ( spriteHeight >> 1 );

	if ( renderPos.vx >= 0 && renderPos.vx <= VidGetScrW() )
	{
		if ( renderPos.vy >= 0 && renderPos.vy <= VidGetScrH() )
		{
			m_spriteBank->printFT4(frame,renderPos.vx,renderPos.vy,0,0,10);
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcPlanktonFriend::think( int _frames )
{
}
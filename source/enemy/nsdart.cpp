/*=========================================================================

	nsdart.cpp

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __ENEMY_NPC_H__
#include "enemy\npc.h"
#endif

#ifndef __ENEMY_NSDART_H__
#include "enemy\nsdart.h"
#endif

#ifndef __GAME_GAME_H__
#include	"game\game.h"
#endif

#ifndef __VID_HEADER_
#include "system\vid.h"
#endif

void CNpcSquidDartEnemy::render()
{
	SprFrame = NULL;

	if ( m_isActive )
	{
		CEnemyThing::render();

		// Render
		DVECTOR renderPos;
		DVECTOR	offset = CLevel::getCameraPos();

		renderPos.vx = Pos.vx - offset.vx;
		renderPos.vy = Pos.vy - offset.vy;

		if ( renderPos.vx >= 0 && renderPos.vx <= VidGetScrW() )
		{
			if ( renderPos.vy >= 0 && renderPos.vy <= VidGetScrH() )
			{
				SprFrame = m_spriteBank->printFT4(6 + (m_frame>>8),renderPos.vx,renderPos.vy,m_reversed,0,10);
				setRGB0( SprFrame, 255, 128, 255 );
			}
		}
	}
}


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

#ifndef	__GAME_CONVO_H__
#include "game\convo.h"
#endif


#include	"game/game.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcPlanktonFriend::init()
{
	CNpcThing::init();

	m_extension = EXTEND_RIGHT;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcPlanktonFriend::shutdown()
{
	CNpcThing::shutdown();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcPlanktonFriend::render()
{
	CNpcThing::render();

	// Render
	DVECTOR renderPos;
	DVECTOR origRenderPos;
	DVECTOR const &offset = CLevel::getCameraPos();
	int frame = FRM_PLANKTON_STATIC00;
	int		spriteWidth = CGameScene::getSpriteBank()->getFrameWidth( frame );
	int		spriteHeight = CGameScene::getSpriteBank()->getFrameHeight( frame );

	renderPos.vx = Pos.vx - offset.vx - ( spriteWidth >> 1 );
	renderPos.vy = Pos.vy - offset.vy - ( spriteHeight >> 1 );

CRECT const	&collisionRect = getCollisionArea();
	collisionRect.x1 -= Pos.vx;
	collisionRect.x2 -= Pos.vx;
	collisionRect.y1 -= Pos.vy;
	collisionRect.y2 -= Pos.vy;

	if ( renderPos.vx + collisionRect.x2 >= 0 && renderPos.vx + collisionRect.x1 <= VidGetScrW() )
	{
		if ( renderPos.vy + collisionRect.y2 >= 0 && renderPos.vy + collisionRect.y1 <= VidGetScrH() )
		{
			CGameScene::getSpriteBank()->printFT4(frame,renderPos.vx,renderPos.vy,0,0,10);
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcPlanktonFriend::think( int _frames )
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcPlanktonFriend::startConderversation()
{
	if( !CConversation::isActive() )
	{
		CConversation::trigger( SCRIPTS_CH3L1_01_DAT );
	}
}
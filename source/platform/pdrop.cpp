/*=========================================================================

	pdrop.cpp

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __PLATFORM_PDROP_H__
#include "platform\pdrop.h"
#endif

#ifndef __GAME_GAME_H__
#include "game\game.h"
#endif


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcDropPlatform::processMovement( int _frames )
{
	s32 moveY = m_speed * _frames;

	s32 groundHeight = CGameScene::getCollision()->getHeightFromGround( Pos.vx, Pos.vy + moveY, 16 );

	if ( groundHeight < moveY )
	{
		moveY = groundHeight;
	}

	Pos.vy += moveY;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const CRECT *CNpcDropPlatform::getThinkBBox()
{
	CRECT objThinkBox = getCollisionArea();

	sBBox &thinkBBox = CThingManager::getThinkBBox();
	objThinkBox.x1 = thinkBBox.XMin;
	objThinkBox.x2 = thinkBBox.XMax;
	objThinkBox.y1 = thinkBBox.YMin;
	objThinkBox.y2 = thinkBBox.YMax;

	return &objThinkBox;
}
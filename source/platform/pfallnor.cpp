/*=========================================================================

	pfallnor.cpp

	Author:		CRB
	Created:
	Project:	Spongebob
	Purpose:

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __PLATFORM_PFALLNOR_H__
#include "platform\pfallnor.h"
#endif

#ifndef __GAME_GAME_H__
#include	"game\game.h"
#endif

#ifndef	__UTILS_HEADER__
#include	"utils\utils.h"
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcFallingNoRespawnPlatform::postInit()
{
	CNpcPlatform::postInit();

	m_npcPath.setPathType( CNpcPath::SINGLE_USE_PATH );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcFallingNoRespawnPlatform::processMovement( int _frames )
{
	s32 moveX, moveY;
	s32 distX, distY, heading;
	bool pathComplete;

	m_npcPath.thinkVertical( Pos, &pathComplete, &distX, &distY, &heading );

	if ( pathComplete )
	{
		setToShutdown();
	}
	else
	{
		moveX = 0;
		moveY = m_speed * _frames;

		if ( heading == 3072 )
		{
			moveY = -moveY;
		}

		s32 groundHeight = m_layerCollision->getHeightFromGround( Pos.vx + moveX, Pos.vy + moveY, 16 );

		if ( groundHeight < moveY )
		{
			if ( ( m_layerCollision->getCollisionBlock( Pos.vx, Pos.vy + groundHeight + 8 ) & COLLISION_TYPE_MASK ) != (7<<COLLISION_TYPE_FLAG_SHIFT) )
			{
				moveY = groundHeight;
				moveX = 2 * _frames;
			}
		}

		Pos.vx += moveX;
		Pos.vy += moveY;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const CRECT *CNpcFallingNoRespawnPlatform::getThinkBBox()
{
	CRECT objThinkBox = getCollisionArea();

	sBBox &thinkBBox = CThingManager::getThinkBBox();
	objThinkBox.x1 = thinkBBox.XMin;
	objThinkBox.x2 = thinkBBox.XMax;
	objThinkBox.y1 = thinkBBox.YMin;
	objThinkBox.y2 = thinkBBox.YMax;

	return &objThinkBox;
}
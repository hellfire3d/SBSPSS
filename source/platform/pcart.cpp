/*=========================================================================

	pcart.cpp

	Author:		CRB
	Created:
	Project:	Spongebob
	Purpose:

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __PLATFORM_PCART_H__
#include "platform\pcart.h"
#endif

#ifndef __GAME_GAME_H__
#include	"game\game.h"
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcCartPlatform::postInit()
{
	m_npcPath.setPathType( CNpcPath::PONG_PATH );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcCartPlatform::processMovement( int _frames )
{
	s32 fallSpeed = 3;
	s8 yMovement = fallSpeed * _frames;
	s32 distX, distY, heading;
	s32 groundHeight;
	s32 moveX = 0;
	s32 moveY = 0;

	bool pathComplete;

	m_npcPath.thinkFlat( Pos, &pathComplete, &distX, &distY, &heading );

	moveX = m_data[m_type].speed * _frames;

	if ( heading == 2048 )
	{
		moveX = -moveX;
	}

	// check for vertical movement

	groundHeight = m_layerCollision->getHeightFromGround( Pos.vx + moveX, Pos.vy, yMovement + 16 );

	if ( groundHeight <= yMovement )
	{
		// groundHeight <= yMovement indicates either just above ground or on or below ground

		moveY = groundHeight;
	}
	else
	{
		// fall

		moveY = yMovement;
	}

	Pos.vx += moveX;
	Pos.vy += moveY;
}
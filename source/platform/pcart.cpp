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
	CNpcPlatform::postInit();

	m_npcPath.setPathType( CNpcPath::SINGLE_USE_PATH );

	m_carSpeed = m_data[m_type].speed << 8;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcCartPlatform::processMovement( int _frames )
{
	s32 fallSpeed = 2;
	s8 yMovement = fallSpeed * _frames;
	s32 distX, distY, heading;
	s32 groundHeight;
	s32 moveX = 0;
	s32 moveY = 0;

	bool pathComplete;

	m_npcPath.thinkFlat( Pos, &pathComplete, &distX, &distY, &heading );

	if ( !pathComplete )
	{
		moveX = ( m_carSpeed >> 8 ) * _frames;

		if ( heading == 2048 )
		{
			moveX = -moveX;
		}
	}

	// check for vertical movement

	s32 checkDist = yMovement + 50;

	groundHeight = m_layerCollision->getHeightFromGround( Pos.vx + moveX, Pos.vy, checkDist );

	if ( groundHeight < checkDist )
	{
		// groundHeight <= yMovement indicates either just above ground or on or below ground

		moveY = groundHeight;
	}
	else
	{
		// fall

		moveY = yMovement;
	}

	if ( moveY < 0 )
	{
		m_carSpeed -= 20;

		if ( m_carSpeed < ( 2 << 8 ) )
		{
			m_carSpeed = ( 2 << 8 );
		}
	}
	else if ( moveY > 0 )
	{
		m_carSpeed += 20;

		if ( m_carSpeed > ( 6 << 8 ) )
		{
			m_carSpeed = ( 6 << 8 );
		}
	}

	Pos.vx += moveX;
	Pos.vy += moveY;
}
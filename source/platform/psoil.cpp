/*=========================================================================

	psoil.h

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __PLATFORM_PSOIL_H__
#include "platform\psoil.h"
#endif

#ifndef __GAME_GAME_H__
#include	"game\game.h"
#endif

#ifndef __VID_HEADER_
#include "system\vid.h"
#endif

#ifndef	__UTILS_HEADER__
#include	"utils\utils.h"
#endif


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcSteerableOildrumPlatform::processMovement( int _frames )
{
	s32 maxHeight = 20;
	s32 fallSpeed = 3;
	s8 yMovement = fallSpeed * _frames;
	s32 groundHeight;

	s32 moveX = 0;
	s32 moveY = 0;

	CPlayer *player = GameScene.getPlayer();

	if ( m_contact )
	{
		player->setReverseCameraMovement(true);

		DVECTOR const &playerPos = player->getPos();

		s32 playerMovement = player->getMovement();

		/*s32 speedChange = -playerVel->vx << 8;

		if ( speedChange > ( _frames << 5 ) )
		{
			speedChange = _frames << 5;
		}
		else if ( speedChange < -( _frames << 5 ) )
		{
			speedChange = -_frames << 5;
		}

		m_currentSpeed += speedChange;

		if ( m_currentSpeed > ( m_speed << 8 ) )
		{
			m_currentSpeed = ( m_speed << 8 );
		}
		else if ( m_currentSpeed < -( m_speed << 8 ) )
		{
			m_currentSpeed = -( m_speed << 8 );
		}*/

		//m_currentSpeed = -playerVel->vx << 4;
		m_currentSpeed = -playerMovement;

		/*s32 playerX = playerPos.vx - this->Pos.vx;

		if ( playerX > 5 )
		{
			// increase barrel speed to right

			m_currentSpeed += _frames << 2;

			if ( m_currentSpeed > ( m_speed << 8 ) )
			{
				m_currentSpeed = ( m_speed << 8 );
			}
		}
		else if ( playerX < -5 )
		{
			m_currentSpeed -= _frames << 2;

			if ( m_currentSpeed < -( m_speed << 8 ) )
			{
				m_currentSpeed = -( m_speed << 8 );
			}
		}*/
	}
	else
	{
		// reduce speed

		/*s32 speedReduce = -m_currentSpeed;

		if ( speedReduce > _frames )
		{
			speedReduce = _frames;
		}
		else if ( speedReduce < -_frames )
		{
			speedReduce = -_frames;
		}

		m_currentSpeed += speedReduce;*/

		m_currentSpeed = 0;
	}

	/*m_moveXHighRes += m_currentSpeed * _frames;

	moveX = m_moveXHighRes >> 8;
	m_moveXHighRes -= moveX << 8;*/

	moveX = m_currentSpeed;

	// check for collision

	if ( CGameScene::getCollision()->getHeightFromGround( Pos.vx + moveX, Pos.vy ) < -maxHeight )
	{
		moveX = 0;
		m_currentSpeed = 0;
	}

	m_rotation += ( m_currentSpeed * 30 * _frames ) >> 2;
	m_rotation &= 4095;

	if ( m_contact )
	{
		DVECTOR shove;
		shove.vx = moveX;
		shove.vy = 0;
		player->shove(shove);
	}

	// check for vertical movement

	groundHeight = CGameScene::getCollision()->getHeightFromGround( Pos.vx + moveX, Pos.vy, yMovement + 16 );

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

	s32 minX, maxX;

	m_npcPath.getPathXExtents( &minX, &maxX );

	if ( minX != maxX )
	{
		if ( Pos.vx < minX )
		{
			Pos.vx = minX;
			m_currentSpeed = 0;
		}
		else if ( Pos.vx > maxX )
		{
			Pos.vx = maxX;
			m_currentSpeed = 0;
		}
	}
}
/*=========================================================================

	psbarrel.h

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __PLATFORM_PSBARREL_H__
#include "platform\psbarrel.h"
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

void CNpcSteerableBarrelPlatform::processMovement( int _frames )
{
	s32 maxHeight = 20;
	s32 fallSpeed = 3;
	s8 yMovement = fallSpeed * _frames;
	s32 groundHeight;

	s32 moveX = 0;
	s32 moveY = 0;

	if ( m_contact )
	{
		CPlayer *player = GameScene.getPlayer();

		DVECTOR playerPos = player->getPos();

		s32 playerX = playerPos.vx - this->Pos.vx;

		if ( playerX > 5 )
		{
			// roll barrel right

			moveX = m_speed * _frames;
		}
		else if ( playerX < -5 )
		{
			moveX = -m_speed * _frames;
		}

		// check for collision

		if ( m_layerCollision->getHeightFromGround( Pos.vx + moveX, Pos.vy ) < -maxHeight )
		{
			moveX = 0;
		}

		if ( moveX > 0 )
		{
			m_rotation += 30 * _frames;
			m_rotation &= 4095;
		}
		else if ( moveX < 0 )
		{
			m_rotation -= 30 * _frames;
			m_rotation &= 4095;
		}
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

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcSteerableBarrelPlatform::render()
{
	if ( m_isActive )
	{
		CPlatformThing::render();

		if (canRender())
		{
			DVECTOR &renderPos=getRenderPos();
			DVECTOR	offset = CLevel::getCameraPos();

			SVECTOR rotation;
			rotation.vx = 0;
			rotation.vy = 0;
			rotation.vz = m_rotation;

			VECTOR scale;
			scale.vx = ONE;
			scale.vy = ONE;
			scale.vz = ONE;

			m_modelGfx->Render(renderPos,&rotation,&scale);
		}
	}
}

/*=========================================================================

	hbarrel.cpp

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __HAZARD_HBARREL_H__
#include "hazard\hbarrel.h"
#endif

#ifndef __LAYER_COLLISION_H__
#include "level\layercollision.h"
#endif

#include	"game\game.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcBarrelHazard::init()
{
	CNpcHazard::init();

	m_npcPath.setPathType( CNpcPath::SINGLE_USE_PATH );
	m_rotation = 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcBarrelHazard::processMovement( int _frames )
{
	s32 maxHeight = 20;
	s32 distX, distY;
	s32 fallSpeed = 3;
	s8 yMovement = fallSpeed * _frames;
	s32 groundHeight;

	s32 moveX = 0;
	s32 moveY = 0;

	// ignore y component of waypoint, since we are stuck to the ground

	bool pathComplete;

	if ( m_npcPath.thinkFlat( Pos, &pathComplete, &distX, &distY, &m_heading ) )
	{
		if ( pathComplete )
		{
			// reset

			Pos = m_base;
			m_npcPath.resetPath();

			return;
		}
		else
		{
			// check for vertical movement

			groundHeight = CGameScene::getCollision()->getHeightFromGround( Pos.vx, Pos.vy, yMovement + 16 );

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
		}
	}
	else
	{
		// check for collision

		distX = distX / abs( distX );

		if ( CGameScene::getCollision()->getHeightFromGround( Pos.vx + ( distX * 3 * _frames ), Pos.vy ) < -maxHeight )
		{
			// there is an obstacle in the way, increment the path point (hopefully this will resolve the problem)

			m_npcPath.incPath();
		}
		else
		{
			// check for vertical movement

			groundHeight = CGameScene::getCollision()->getHeightFromGround( Pos.vx, Pos.vy, yMovement + 16 );

			moveX = distX * 3 * _frames;

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
		}
	}

	if ( moveX < 0 )
	{
		m_rotation -= 64 * _frames;
		m_rotation &= 4095;
	}
	else if ( moveX > 0 )
	{
		m_rotation += 64 * _frames;
		m_rotation &= 4095;
	}

	Pos.vx += moveX;
	Pos.vy += moveY;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcBarrelHazard::render()
{
	CHazardThing::render();

	if (canRender())
	{
		DVECTOR &renderPos=getRenderPos();

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

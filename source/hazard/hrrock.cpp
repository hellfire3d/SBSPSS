/*=========================================================================

	hrrock.cpp

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __HAZARD_HRROCK_H__
#include "hazard\hrrock.h"
#endif

#ifndef __LAYER_COLLISION_H__
#include "level\layercollision.h"
#endif

#include	"game\game.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcRollingRockHazard::init()
{
	CNpcHazard::init();

	m_npcPath.setPathType( CNpcPath::SINGLE_USE_PATH );
	m_rotation = 0;
	m_jump = false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcRollingRockHazard::processMovement( int _frames )
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
	bool waypointChange;

	m_npcPath.think( Pos, &pathComplete, &waypointChange, &distX, &distY );

	if ( pathComplete )
	{
		// reset

		Pos = m_base;
		m_npcPath.resetPath();
		m_jump = false;

		return;
	}
	else
	{
		if ( m_soundId == NOT_PLAYING )
		{
			m_soundId = (int) CSoundMediator::playSfx( CSoundMediator::SFX_ROLLING_ROCK, true, true );
		}

		if ( distX )
		{
			// move along ground

			s32 groundSpeed = 4 * _frames;

			moveX = distX;

			if ( moveX > groundSpeed )
			{
				moveX = groundSpeed;
			}
			else if ( moveX < -groundSpeed )
			{
				moveX = -groundSpeed;
			}

			if ( m_jump )
			{
				// process jump movement

				if ( m_jumpVel < ( 16 << 8 ) )
				{
					m_jumpVel += 128 * _frames;
				}

				moveY += m_jumpVel >> 8;

				groundHeight = CGameScene::getCollision()->getHeightFromGround( Pos.vx + moveX, Pos.vy + moveY, 16 );

				if ( groundHeight < 16 )
				{
					// have hit ground

					m_jump = false;
					moveY += groundHeight;
				}
			}
			else
			{
				// check for vertical movement

				groundHeight = CGameScene::getCollision()->getHeightFromGround( Pos.vx + moveX, Pos.vy, 64 );

				if ( groundHeight < 64 )
				{
					// groundHeight <= yMovement indicates either just above ground or on or below ground

					moveY = groundHeight;
				}
				else
				{
					// cannot find ground immediately below

					m_jump = true;

					m_jumpVel = 0;
					moveY = 0;
				}
			}
		}
		else
		{
			// drop vertically

			// check for vertical movement

			groundHeight = CGameScene::getCollision()->getHeightFromGround( Pos.vx, Pos.vy, yMovement + 16 );

			if ( groundHeight <= yMovement )
			{
				// groundHeight <= yMovement indicates either just above ground or on or below ground

				moveY = groundHeight;

				// increment path if we hit the ground at this stage
				
				m_npcPath.incPath();
				m_jump = false;
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

void CNpcRollingRockHazard::render()
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

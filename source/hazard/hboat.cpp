/*=========================================================================

	hboat.cpp

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __HAZARD_HBOAT_H__
#include "hazard\hboat.h"
#endif

#ifndef __LAYER_COLLISION_H__
#include "level\layercollision.h"
#endif

#ifndef __GAME_GAME_H__
#include	"game\game.h"
#endif

#ifndef __VID_HEADER_
#include "system\vid.h"
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcBoatHazard::init()
{
	CNpcHazard::init();

	m_npcPath.setPathType( CNpcPath::PONG_PATH );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcBoatHazard::processMovement( int _frames )
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
		// path has finished, waypoint has changed, or there are no waypoints - do not move horizontally

		// check for vertical movement

		groundHeight = m_layerCollision->getHeightFromGround( Pos.vx, Pos.vy, yMovement + 16 );

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
	else
	{
		// check for collision

		distX = distX / abs( distX );

		if ( distX > 0 )
		{
			m_reversed = false;
		}
		else
		{
			m_reversed = true;
		}

		if ( m_layerCollision->getHeightFromGround( Pos.vx + ( distX * 3 * _frames ), Pos.vy ) < -maxHeight )
		{
			// there is an obstacle in the way, increment the path point (hopefully this will resolve the problem)

			m_npcPath.incPath();
		}
		else
		{
			// check for vertical movement

			groundHeight = m_layerCollision->getHeightFromGround( Pos.vx, Pos.vy, yMovement + 16 );

			if ( groundHeight <= yMovement )
			{
				// groundHeight <= yMovement indicates either just above ground or on or below ground

				moveX = distX * 3 * _frames;
				moveY = groundHeight;
			}
			else
			{
				// fall

				moveY = yMovement;
			}
		}
	}

	Pos.vx += moveX;
	Pos.vy += moveY;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcBoatHazard::render()
{
	if ( m_isActive )
	{
		CHazardThing::render();

		// Render
		DVECTOR renderPos;
		DVECTOR	offset = CLevel::getCameraPos();

		renderPos.vx = Pos.vx - offset.vx;
		renderPos.vy = Pos.vy - offset.vy;

		CRECT collisionRect = getCollisionArea();
		collisionRect.x1 -= Pos.vx;
		collisionRect.x2 -= Pos.vx;
		collisionRect.y1 -= Pos.vy;
		collisionRect.y2 -= Pos.vy;

		if ( renderPos.vx + collisionRect.x2 >= 0 && renderPos.vx + collisionRect.x1 <= VidGetScrW() )
		{
			if ( renderPos.vy + collisionRect.y2 >= 0 && renderPos.vy + collisionRect.y1 <= VidGetScrH() )
			{
				VECTOR flip;

				if ( m_reversed )
				{
					flip.vx = ONE;
				}
				else
				{
					flip.vx = -ONE;
				}

				flip.vy = ONE;
				flip.vz = ONE;

				m_modelGfx->Render( renderPos, NULL, &flip );
				//m_actorGfx->Render(renderPos,0,0,0);
			}
		}
	}
}

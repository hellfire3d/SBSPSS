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

#ifndef	__PLAYER_PLAYER_H__
#include "player\player.h"
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

		if ( CGameScene::getCollision()->getHeightFromGround( Pos.vx + ( distX * 3 * _frames ), Pos.vy ) < -maxHeight )
		{
			// there is an obstacle in the way, increment the path point (hopefully this will resolve the problem)

			m_npcPath.incPath();
		}
		else
		{
			// check for vertical movement

			groundHeight = CGameScene::getCollision()->getHeightFromGround( Pos.vx, Pos.vy, yMovement + 16 );

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

		if (canRender())
		{
			DVECTOR &renderPos=getRenderPos();

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

			m_modelGfx->RenderNoClip( renderPos, NULL, &flip );
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcBoatHazard::collidedWith( CThing *_thisThing )
{
	if ( m_isActive )
	{
		switch(_thisThing->getThingType())
		{
			case TYPE_PLAYER:
			{
				CPlayer *player = (CPlayer *) _thisThing;

				player->takeDamage( DAMAGE__HIT_ENEMY, REACT__GET_DIRECTION_FROM_THING, this );

				break;
			}

			default:
				break;
		}
	}
}

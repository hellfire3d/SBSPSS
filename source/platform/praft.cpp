/*=========================================================================

	praft.cpp

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __PLATFORM_PRAFT_H__
#include "platform\praft.h"
#endif

#ifndef	__UTILS_HEADER__
#include	"utils\utils.h"
#endif

#ifndef __GAME_GAME_H__
#include "game\game.h"
#endif


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcRaftPlatform::postInit()
{
	CNpcPlatform::postInit();

	m_npcPath.setPathType( CNpcPath::SINGLE_USE_PATH );

	m_isActivated = false;
	m_isSinking = false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcRaftPlatform::processMovement( int _frames )
{
	s32 groundHeight;
	s32 maxHeight = 20;
	s32 distX, distY;
	s32 fallSpeed = 3;
	s8 yMovement = fallSpeed * _frames;

	s32 moveX = 0;
	s32 moveY = 0;

	if ( m_isActivated )
	{
		if ( m_isSinking )
		{
			Pos.vy += _frames;

			groundHeight = CGameScene::getCollision()->getHeightFromGround( Pos.vx, Pos.vy - 32 );

			if ( groundHeight <= 0 )
			{
				m_isActive = false;
				m_timer = getRnd() % ( 2 * GameState::getOneSecondInFrames() );
				m_timerType = NPC_PLATFORM_TIMER_RESPAWN;
				m_isActivated = false;
				m_npcPath.resetPath();
				m_isSinking = false;
			}
		}
		else
		{
			// ignore y component of waypoint, since we are stuck to the ground

			bool pathComplete;

			m_npcPath.thinkFlat( Pos, &pathComplete, &distX, &distY, &m_heading );

			if ( pathComplete )
			{
				m_isSinking = true;
			}
			else
			{
				// check for collision

				distX = distX / abs( distX );

				if ( CGameScene::getCollision()->getHeightFromGround( Pos.vx + ( distX * m_speed * _frames ), Pos.vy ) < -maxHeight )
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

						moveX = distX * m_speed * _frames;
						moveY = groundHeight;
					}
					else
					{
						// fall

						moveX = distX * m_speed * _frames;
						moveY = yMovement;
					}
				}
			}

			Pos.vx += moveX;
			Pos.vy += moveY;
		}
	}
	else
	{
		groundHeight = CGameScene::getCollision()->getHeightFromGround( Pos.vx, Pos.vy, yMovement + 16 );

		if ( groundHeight <= yMovement )
		{
			moveY = groundHeight;
		}
		else
		{
			// fall

			moveY = yMovement;
		}

		Pos.vy += moveY;

		if ( m_contact )
		{
			m_isActivated = true;
		}
	}

	// sort out rotation
	
	DVECTOR testPos1, testPos2;

	testPos1 = testPos2 = Pos;
	testPos1.vx -= 10;
	testPos2.vx += 10;

	s16 heightDiff;
	bool levelOff = false;

	heightDiff = CGameScene::getCollision()->getHeightFromGround( testPos1.vx, testPos1.vy, 16 );

	if ( heightDiff == 16 )
	{
		levelOff = true;
	}

	testPos1.vy += heightDiff;

	heightDiff = CGameScene::getCollision()->getHeightFromGround( testPos2.vx, testPos2.vy, 16 );

	if ( heightDiff == 16 )
	{
		levelOff = true;
	}

	testPos2.vy += heightDiff;

	s16 heading;

	if ( levelOff )
	{
		heading = getCollisionAngle();

		if ( heading )
		{
			groundHeight = CGameScene::getCollision()->getHeightFromGround( Pos.vx, Pos.vy, 56 );

			if ( groundHeight < 32 && groundHeight > 4 )
			{
				int reverseAngle = -heading;
				int maxRotate = 32 * _frames;

				if ( reverseAngle > maxRotate )
				{
					reverseAngle = maxRotate;
				}
				else if ( reverseAngle < -maxRotate )
				{
					reverseAngle = -maxRotate;
				}

				heading += reverseAngle;
			}
		}
	}
	else
	{
		s32 xDist = testPos2.vx - testPos1.vx;
		s32 yDist = testPos2.vy - testPos1.vy;

		heading = ratan2( yDist, xDist );
	}

	/*if ( heading > 512 )
	{
		heading = 512;
	}
	else if ( heading < -512 )
	{
		heading = -512;
	}*/

	setCollisionAngle( heading );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcRaftPlatform::render()
{
	if ( m_isActive )
	{
		CPlatformThing::render();

		if (canRender())
		{
			DVECTOR &renderPos=getRenderPos();
			SVECTOR rotation;

			rotation.vx = 0;
			rotation.vy = 0;
			rotation.vz = getCollisionAngle();

			VECTOR scale;
			scale.vx = ONE;
			scale.vy = ONE;
			scale.vz = ONE;

			m_modelGfx->Render(renderPos,&rotation,&scale);
		}
	}
}

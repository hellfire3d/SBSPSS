/*=========================================================================

	pghost.cpp

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __PLATFORM_PGHOST_H__
#include "platform\pghost.h"
#endif

#ifndef __GAME_GAME_H__
#include	"game\game.h"
#endif

#ifndef __VID_HEADER_
#include "system\vid.h"
#endif


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcGhostTrainPlatform::postInit()
{
	CNpcCartPlatform::postInit();

	m_speedSetting = GO_DEFAULT;

	m_carSpeed >>= 8;
	m_trackContact = false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcGhostTrainPlatform::processMovement( int _frames )
{
	s32 fallSpeed = 2;
	s8 yMovement = fallSpeed * _frames;
	s32 distX, distY, heading;
	s32 groundHeight;
	s32 moveX = 0;
	s32 moveY = 0;

	bool pathComplete;

	if ( !m_playerAttached && !m_falling )
	{
		m_playerAttached = true;
		CPlayer *player = GameScene.getPlayer();

		DVECTOR newPos = Pos;
		CRECT const &collisionArea=getCollisionArea();
		newPos.vy = collisionArea.y1;

		player->setPos( newPos );
		player->setPlatform( this );
		m_contact = true;
	}

	m_trackContact = false;

	if ( m_isActivated )
	{
		if ( m_falling )
		{
			m_vertSpeed += 192;

			if ( m_vertSpeed > ( 8 << 8 ) )
			{
				m_vertSpeed = 8 << 8;
			}

			moveY = ( m_vertSpeed >> 8 ) * _frames;

			Pos.vy += moveY;

			DVECTOR const &offset = CLevel::getCameraPos();

			s32 yPos = Pos.vy - offset.vy;

			if ( yPos < 0 || yPos > VidGetScrH() )
			{
				setToShutdown();
			}
		}
		else if ( m_rebound )
		{
			moveX = -4 * _frames;

			m_vertSpeed += 96 * _frames;

			if ( m_vertSpeed > ( 8 << 8 ) )
			{
				m_vertSpeed = 8 << 8;
			}
			else if ( m_vertSpeed < -( 6 << 8 ) )
			{
				m_vertSpeed = -( 6 << 8 );
			}

			moveY = ( m_vertSpeed >> 8 ) * _frames;

			groundHeight = CGameScene::getCollision()->getHeightFromGroundCart( Pos.vx + moveX, Pos.vy + moveY, 16 );

			if ( groundHeight < 0 )
			{
				// have touched down

				m_rebound = false;
				m_trackContact = true;
				moveY += groundHeight;
			}

			Pos.vx += moveX;
			Pos.vy += moveY;
		}
		else
		{
			m_npcPath.thinkFlat( Pos, &pathComplete, &distX, &distY, &heading );

			switch( m_speedSetting )
			{
				case GO_DEFAULT:
				{
					m_carSpeed = m_speed;

					break;
				}

				case GO_SLOW_DOWN:
				{
					m_carSpeed = m_speed - 2;
					m_speedSetting = GO_DEFAULT;

					break;
				}

				case GO_SPEED_UP:
				{
					m_carSpeed = m_speed + 2;
					m_speedSetting = GO_DEFAULT;

					break;
				}
			}

			if ( !pathComplete )
			{
				moveX = m_carSpeed * _frames;

				if ( heading == 2048 )
				{
					moveX = -moveX;
				}
			}

			if ( m_inJump )
			{
				m_vertSpeed += 80 * _frames;

				if ( m_vertSpeed > ( 5 << 8 ) )
				{
					m_vertSpeed = 5 << 8;
				}
				else if ( m_vertSpeed < -( 6 << 8 ) )
				{
					m_vertSpeed = -( 6 << 8 );
				}

				moveY = ( m_vertSpeed >> 8 ) * _frames;

				groundHeight = CGameScene::getCollision()->getHeightFromGroundCart( Pos.vx + moveX, Pos.vy, moveY + 16 );

				if ( groundHeight < moveY )
				{
					// have touched down

					m_inJump = false;
					m_trackContact = true;
					moveY += groundHeight;
				}
			}
			else
			{
				// check for vertical movement

				s32 checkDist = yMovement + 50;

				groundHeight = CGameScene::getCollision()->getHeightFromGroundCart( Pos.vx + moveX, Pos.vy, checkDist );

				if ( groundHeight < checkDist )
				{
					// groundHeight <= yMovement indicates either just above ground or on or below ground

					moveY = groundHeight;
					m_trackContact = true;
				}
				else
				{
					// fall

					moveY = yMovement;
				}
			}

			Pos.vx += moveX;
			Pos.vy += moveY;
		}

		// sort out draw rotation

		DVECTOR testPos1, testPos2;

		testPos1 = testPos2 = Pos;
		testPos1.vx -= 10;
		testPos2.vx += 10;

		u8 sensorDist = 16;

		s32 yDiff1, yDiff2;

		yDiff1 = CGameScene::getCollision()->getHeightFromGroundCart( testPos1.vx, testPos1.vy, sensorDist + 1 );

		if ( abs( yDiff1 ) <= sensorDist )
		{
			// only use if there is ground present

			yDiff2 = CGameScene::getCollision()->getHeightFromGroundCart( testPos2.vx, testPos2.vy, sensorDist + 1 );

			if ( abs( yDiff2 ) <= sensorDist )
			{
				// only use if there is ground present

				testPos1.vy += yDiff1;
				testPos2.vy += yDiff2;
			}
		}

		s32 xDist = testPos2.vx - testPos1.vx;
		s32 yDist = testPos2.vy - testPos1.vy;

		heading = ratan2( yDist, xDist );

		setCollisionAngle( heading );

		testPos2 = Pos;
		testPos2.vx += 32;

		yDiff1 = CGameScene::getCollision()->getHeightFromGroundCart( testPos2.vx, testPos2.vy, sensorDist + 1 );

		if ( yDiff1 <= sensorDist )
		{
			// only use if there is ground present

			testPos2.vy += yDiff1;
		}

		switch ( CGameScene::getCollision()->getCollisionBlock( testPos2.vx, testPos2.vy - 8 ) & COLLISION_TYPE_MASK )
		{
			case COLLISION_TYPE_FLAG_DAMAGE:
			{
				if ( !m_rebound )
				{
					m_vertSpeed = -6 << 8;
					m_rebound = true;
					Pos.vy -= 8;
				}

				break;
			}

			default:
				break;
		}

		switch ( CGameScene::getCollision()->getCollisionBlock( Pos.vx, Pos.vy ) & COLLISION_TYPE_MASK )
		{
			case COLLISION_TYPE_FLAG_DEATH_FALL:
			{
				m_playerAttached = false;
				m_falling = true;
				CPlayer *player = GameScene.getPlayer();
				if ( player->getMode() != PLAYER_MODE_DEAD )
				{
					player->dieYouPorousFreak();
				}

				break;
			}

			default:
				break;
		}
	}
	else
	{
		groundHeight = CGameScene::getCollision()->getHeightFromGroundCart( Pos.vx, Pos.vy, yMovement + 16 );

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
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcGhostTrainPlatform::slowDown()
{
	m_speedSetting = GO_SLOW_DOWN;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcGhostTrainPlatform::speedUp()
{
	m_speedSetting = GO_SPEED_UP;
}

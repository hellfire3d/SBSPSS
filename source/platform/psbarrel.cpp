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

void CNpcSteerableBarrelPlatform::postInit()
{
	CNpcPlatform::postInit();

	m_currentSpeed = 0;
	m_moveXHighRes = 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcSteerableBarrelPlatform::processMovement( int _frames )
{
	s32 maxHeight = 8;
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

		if ( !playerMovement )
		{
			playerMovement = player->getMoveLeftRight();
		}

		m_currentSpeed = -playerMovement;
	}
	else
	{
		m_currentSpeed = 0;
	}

	moveX = m_currentSpeed;

	if ( m_currentSpeed )
	{
		if(m_soundId==NOT_PLAYING)
		{
			m_soundId=CSoundMediator::playSfx(CSoundMediator::SFX_ROLLING_BARREL,true);
		}
	}
	else
	{
		if( m_soundId != NOT_PLAYING )
		{
			CSoundMediator::stopAndUnlockSfx( (xmPlayingId) m_soundId );
			m_soundId = NOT_PLAYING;
		}
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

	// check for collision

	if ( moveX )
	{
		int remainder = abs( moveX );
		int sign = moveX / abs( moveX );
		int testX = 0;

		while ( remainder )
		{
			if ( remainder > 16 )
			{
				remainder -= 16;
				testX += 16 * sign;
			}
			else
			{
				testX += remainder * sign;
				remainder = 0;
			}

			if ( CGameScene::getCollision()->getHeightFromGround( Pos.vx + testX + ( sign * 32 ), Pos.vy ) < -maxHeight )
			{
				moveX = 0;
				m_currentSpeed = 0;
			}
		}
	}

	/*if ( CGameScene::getCollision()->getHeightFromGround( Pos.vx + moveX, Pos.vy ) < -maxHeight )
	{
		moveX = 0;
		m_currentSpeed = 0;
	}*/

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

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcSteerableBarrelPlatform::render()
{
	if ( m_isActive )
	{
		CPlatformThing::render();

		if (canRender())
		{
			DVECTOR &renderPos=getRenderPos();
			DVECTOR const &offset = CLevel::getCameraPos();

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

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcSteerableBarrelPlatform::collidedWith( CThing *_thisThing )
{
	switch(_thisThing->getThingType())
	{
		case TYPE_PLAYER:
		{
			CPlayer *player;
//			DVECTOR	playerPos;
//			CRECT	collisionArea;
//			CRECT	playerCollisionArea;

			// Only interested in SBs feet colliding with the box (pkg)
			player=(CPlayer*)_thisThing;
DVECTOR const &playerPos=player->getPos();
CRECT const &playerCollisionArea = player->getCollisionArea();
CRECT const &collisionArea=getCollisionArea();

			//if( playerPos.vx >= collisionArea.x1 && playerPos.vx <= collisionArea.x2 )
			if( playerCollisionArea.x2 >= collisionArea.x1 && playerCollisionArea.x1 <= collisionArea.x2 )
			{
				if ( checkCollisionDelta( _thisThing, 0, collisionArea ) )
				{
					player->setPlatform( this );

					m_contact = true;
				}
				else
				{
					if( playerPos.vy >= collisionArea.y1 && playerPos.vy <= collisionArea.y2 )
					{
						int height = getHeightFromPlatformAtPosition( playerPos.vx, playerPos.vy );

						if ( height >= 0 && height < 1 )
						{
							player->setPlatform( this );

							m_contact = true;
						}
					}
				}
			}

			break;
		}

		case TYPE_NPC:
		case TYPE_HAZARD:
			break;

		default:
			ASSERT(0);
			break;
	}
}


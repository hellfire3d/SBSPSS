/*=========================================================================

	pfblock.h

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __PLATFORM_PFBLOCK_H__
#include "platform\pfblock.h"
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

void CNpcFallingBlockPlatform::postInit()
{
	CNpcPlatform::postInit();

	m_isTriggered = false;
	m_isFalling = false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcFallingBlockPlatform::trigger()
{
	m_isTriggered = true;
	m_timer = GameState::getOneSecondInFrames();

// Clear collision, flagged with normal - cos rest is solid!
	CLevel	&level=GameScene.GetLevel();
	level.destroyMapArea(Pos,COLLISION_TYPE_SLIPPERY);

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcFallingBlockPlatform::processMovement( int _frames )
{
	if ( m_isTriggered )
	{
		if ( m_timer > 0 )
		{
			m_timer -= _frames;

			if ( m_timer <= 0 )
			{
				Pos = m_base;
				m_isFalling = true;

				if( m_soundId != NOT_PLAYING )
				{
					CSoundMediator::stopAndUnlockSfx( (xmPlayingId) m_soundId );
				}

				m_soundId = (int) CSoundMediator::playSfx( CSoundMediator::SFX_ANY_OBJECT_FALLING, true );
			}
			else
			{
				Pos.vx = m_base.vx + ( -3 + ( getRnd() % 7 ) );
				Pos.vy = m_base.vy + ( -3 + ( getRnd() % 7 ) );

				if ( m_soundId == NOT_PLAYING )
				{
					m_soundId = (int) CSoundMediator::playSfx( CSoundMediator::SFX_HAZARD__STALACTITE_RATTLE, true );
				}
			}
		}
		else
		{
			DVECTOR const &offset = CLevel::getCameraPos();

			s32 yPos = Pos.vy - offset.vy;

			if ( yPos > VidGetScrH() )
			{
				setToShutdown();
			}

			s32 moveY = m_speed * _frames;

			Pos.vy += moveY;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcFallingBlockPlatform::collidedWith( CThing *_thisThing )
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

			s32 threshold = abs( collisionArea.y2 - collisionArea.y1 );

			if ( threshold > 16 )
			{
				threshold = 16;
			}

			//if( playerPos.vx >= collisionArea.x1 && playerPos.vx <= collisionArea.x2 )
			if( playerCollisionArea.x2 >= collisionArea.x1 && playerCollisionArea.x1 <= collisionArea.x2 )
			{
				if ( m_isFalling )
				{
					int height = getHeightFromPlatformAtPosition( playerPos.vx, playerPos.vy );

					if ( height < -threshold )
					{
						player->takeDamage( DAMAGE__HIT_ENEMY, REACT__GET_DIRECTION_FROM_THING, this );
					}
				}
				else
				{
					if ( checkCollisionDelta( _thisThing, threshold, collisionArea ) )
					{
						player->setPlatform( this );

						m_contact = true;
					}
					else
					{
						if( playerPos.vy >= collisionArea.y1 && playerPos.vy <= collisionArea.y2 )
						{
							int height = getHeightFromPlatformAtPosition( playerPos.vx, playerPos.vy );

							if ( height >= -threshold && height < 1 )
							{
								player->setPlatform( this );

								m_contact = true;
							}
						}
					}
				}
			}

			break;
		}

		default:
			break;
	}
}

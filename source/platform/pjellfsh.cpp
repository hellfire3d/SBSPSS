/*=========================================================================

	pjellfsh.h

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __PLATFORM_PJELLFSH_H__
#include "platform\pjellfsh.h"
#endif

#ifndef __LEVEL_LEVEL_H__
#include "level\level.h"
#endif

#ifndef __VID_HEADER_
#include "system\vid.h"
#endif

#ifndef	__UTILS_HEADER__
#include	"utils\utils.h"
#endif

#ifndef __GAME_GAME_H__
#include	"game\game.h"
#endif



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcJellyfishPlatform::postInit()
{
	sBBox boundingBox = m_modelGfx->GetBBox();
	boundingBox.YMin += 8;
	setCollisionSize( ( boundingBox.XMax - boundingBox.XMin ) - 16, ( boundingBox.YMax - boundingBox.YMin ) );
	setCollisionCentreOffset( ( boundingBox.XMax + boundingBox.XMin ) >> 1, ( boundingBox.YMax + boundingBox.YMin ) >> 1 );

	calculateNonRotatedCollisionData();
	setCollisionAngle( m_tiltAngle >> 8 );

	m_npcPath.setPathType( CNpcPath::PONG_PATH );

	m_vertScale = 0;
	m_dipCount = 0;
	m_dipOffset = 0;
	m_contactTimeout = 0;
	m_silent = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcJellyfishPlatform::collidedWith( CThing *_thisThing )
{
	switch(_thisThing->getThingType())
	{
		case TYPE_PLAYER:
		{
			if ( m_detectCollision && m_isActive )
			{
				CPlayer *player;
//				DVECTOR	playerPos;
//				CRECT	collisionArea;
//				CRECT	playerArea;

				// Only interested in SBs feet colliding with the box (pkg)
				player=(CPlayer*)_thisThing;
DVECTOR const	&playerPos=player->getPos();
CRECT const		&playerArea=player->getCollisionArea();
CRECT const		&collisionArea=getCollisionArea();

				s32 height = getHeightFromPlatformAtPosition(playerPos.vx,playerPos.vy);

				if(playerArea.x2>=collisionArea.x1&&playerArea.x1<=collisionArea.x2&&
				   playerPos.vy>=collisionArea.y1&&playerPos.vy<=collisionArea.y2)
				//if(((playerArea.x1>=collisionArea.x1&&playerArea.x1<=collisionArea.x2)||(playerArea.x2>=collisionArea.x1&&playerArea.x2<=collisionArea.x2)||(playerArea.x1<=collisionArea.x1&&playerArea.x2>=collisionArea.x2))&&
				   //((playerArea.y1>=thatRect.y1&&playerArea.y1<=thatRect.y2)||(playerArea.y2>=thatRect.y1&&playerArea.y2<=thatRect.y2)||(playerArea.y1<=thatRect.y1&&playerArea.y2>=thatRect.y2)))
				{
					player->setPlatform( this );

					if( height == 0 )
					{
						m_contact = true;

						DVECTOR playerDelta;

						playerDelta = player->getPosDelta();

						if ( playerDelta.vy == 0 )
						{
							// force dip to complete since player is not dropping onto jellyfish

							m_dipCount = GameState::getOneSecondInFrames();
						}

						if ( m_contactTimeout <= 0 )
						{
							if ( m_soundId == NOT_PLAYING )
							{
								m_soundId = (int) CSoundMediator::playSfx( CSoundMediator::SFX_LAND_ON_JELLYFISH, true );
							}
						}

						m_contactTimeout = 2;
					}
				}
			}

			break;
		}

		default:
			break;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcJellyfishPlatform::think( int _frames )
{
	/*if ( m_contact )
	{
		if ( m_vertScale > -2048 )
		{
			m_vertScale -= 10;
		}

		Pos.vy += 3;
	}
	else if ( m_vertScale < 0 )
	{
		m_vertScale += 10;
	}*/

	if ( m_contactTimeout > 0 )
	{
		m_contactTimeout--;
	}

	if ( m_contact )
	{
		if ( m_dipCount < GameState::getOneSecondInFrames() )
		{
			s16 sineVal = ( m_dipCount << 10 ) / GameState::getOneSecondInFrames();

			m_dipOffset = ( 4 * rcos( sineVal ) ) >> 12;

			m_dipCount += _frames;
		}
		else
		{
			m_dipOffset = 0;
		}
	}
	else
	{
		m_dipCount = 0;
		m_dipOffset = 0;
	}

	Pos.vy += m_dipOffset;

	s32 minY, maxY;

	m_npcPath.getPathYExtents( &minY, &maxY );

	if ( Pos.vy > ( maxY + 16 ) )
	{
		Pos.vy = maxY + 16;
	}

	CNpcLinearPlatform::think( _frames );
}

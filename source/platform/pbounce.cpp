/*=========================================================================

	pbounce.cpp

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __PLATFORM_PBOUNCE_H__
#include "platform\pbounce.h"
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

void CNpcBouncePlatform::postInit()
{
	CNpcPlatform::postInit();

	m_vertScale = 0;
	m_vertVelocity = 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcBouncePlatform::think( int _frames )
{
	if ( m_contact )
	{
		CPlayer *player = GameScene.getPlayer();

		player->springPlayerUp();

		s16 vertForce = -180 * _frames;

		m_vertVelocity += vertForce;

		m_contact = false;
	}

	s32 resistance = -( 10 * _frames * m_vertScale ) >> 8;

	if ( m_vertScale > 0 && resistance > -1 )
	{
		resistance = -1;
	}
	else if ( m_vertScale < 0 && resistance < 1 )
	{
		resistance = 1;
	}

	// get direction of resistance

	m_vertVelocity += resistance;

	m_vertScale += m_vertVelocity;

	if ( m_vertVelocity )
	{
		s32 absVertVelocity = abs( m_vertVelocity );

		m_vertVelocity += -m_vertVelocity / abs( m_vertVelocity );

		if ( abs( m_vertVelocity ) < 10 )
		{
			m_vertVelocity = 0;
		}

		if ( m_vertVelocity > 60 )
		{
			m_vertVelocity = 60;
		}
		else if ( m_vertVelocity < -60 )
		{
			m_vertVelocity = -60;
		}
	}

	sBBox boundingBox = m_modelGfx->GetBBox();
	boundingBox.XMin -= 12;
	boundingBox.XMax += 12;

	if ( ( boundingBox.YMax - boundingBox.YMin ) < 24 )
	{
		boundingBox.YMax = 24;
		boundingBox.YMin = 0;
	}

	s32 newCollisionHeight = ( ( ONE + m_vertScale ) * ( boundingBox.YMax - boundingBox.YMin ) ) >> 12;
	setCollisionSize( ( boundingBox.XMax - boundingBox.XMin ), newCollisionHeight - 2 );

	calculateNonRotatedCollisionData();

	CPlatformThing::think(_frames);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcBouncePlatform::render()
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
			rotation.vz = 0;

			VECTOR scale;
			scale.vx = ONE;
			scale.vy = ONE + m_vertScale;
			scale.vz = ONE;

			m_modelGfx->Render(renderPos,&rotation,&scale);
		}

	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcBouncePlatform::collidedWith( CThing *_thisThing )
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
DVECTOR const	&playerPos=player->getPos();
CRECT	const	&playerCollisionArea = player->getCollisionArea();
CRECT	const	&collisionArea=getCollisionArea();

			s32 threshold = abs( collisionArea.y2 - collisionArea.y1 );

			if ( threshold > 16 )
			{
				threshold = 16;
			}

			//if( playerPos.vx >= collisionArea.x1 && playerPos.vx <= collisionArea.x2 )
			if( playerCollisionArea.x2 >= collisionArea.x1 && playerCollisionArea.x1 <= collisionArea.x2 )
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

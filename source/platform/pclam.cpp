/*=========================================================================

	pclam.cpp

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __PLATFORM_PCLAM_H__
#include "platform\pclam.h"
#endif

#ifndef __LEVEL_LEVEL_H__
#include "level\level.h"
#endif

#ifndef	__UTILS_HEADER__
#include	"utils\utils.h"
#endif


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcClamPlatform::postInit()
{
	sBBox boundingBox = m_modelGfx->GetBBox();
	setCollisionSize( 80, 34 );
	setCollisionCentreOffset( 0, -15 );

	calculateNonRotatedCollisionData();
	setCollisionAngle( m_tiltAngle >> 8 );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcClamPlatform::render()
{
	CPlatformThing::render();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcClamPlatform::collidedWith( CThing *_thisThing )
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
CRECT const		&playerCollisionArea = player->getCollisionArea();
CRECT const		&collisionArea=getCollisionArea();

			s32 threshold = abs( collisionArea.y2 - collisionArea.y1 );

			if ( threshold > 16 )
			{
				threshold = 16;
			}

			if( playerCollisionArea.x2 >= collisionArea.x1 && playerCollisionArea.x1 <= collisionArea.x2 )
			{
				if ( checkCollisionDelta( _thisThing, threshold, collisionArea ) )
				{
					player->setPlatform( this );

					m_clam->stoodOn();
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

							m_clam->stoodOn();
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

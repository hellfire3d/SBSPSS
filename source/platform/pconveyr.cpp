/*=========================================================================

	pconveyr.cpp

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __PLATFORM_PCONVEYR_H__
#include "platform\pconveyr.h"
#endif

#ifndef	__UTILS_HEADER__
#include	"utils\utils.h"
#endif

#ifndef __GAME_GAME_H__
#include "game\game.h"
#endif

#ifndef __PLATFORM_PFALLNOR_H__
#include "platform\pfallnor.h"
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcConveyorPlatformGenerator::collidedWith(CThing *_thisThing)
{
	// do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcConveyorPlatformGenerator::render()
{
	// no rendering
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcConveyorPlatformGenerator::think( int _frames )
{
	m_timer -= _frames;

	if ( m_timer < 0 )
	{
		s32 second = GameState::getOneSecondInFrames();

		m_timer = second;

		m_timer += m_dataPtr->initTimer * second;

		m_timer += second - ( getRnd() % ( 2 * second ) );

		//m_timer = ( 3 * GameState::getOneSecondInFrames() ) + ( getRnd() % ( ( m_data[m_type].initTimer - 1 ) * GameState::getOneSecondInFrames() ) );

		// generate new falling platform

		CNpcPlatform *newPlatform = NULL;
		newPlatform = CNpcPlatform::Create( NPC_CONVEYOR_PLATFORM );

		ASSERT(newPlatform);

		newPlatform->setThingSubType( NPC_CONVEYOR_PLATFORM );
		newPlatform->setGraphic( m_graphicNum );

		CNpcWaypoint *sourceWaypoint = m_npcPath.getWaypointList();

		if ( sourceWaypoint )
		{
			DVECTOR startPos;
			startPos.vx = sourceWaypoint->pos.vx;
			startPos.vy = sourceWaypoint->pos.vy;
			//startPos.vx = 100;
			//startPos.vy = 100;

			newPlatform->init( startPos );

			while( sourceWaypoint )
			{
				newPlatform->addWaypoint( sourceWaypoint->pos.vx >> 4, sourceWaypoint->pos.vy >> 4 );
				sourceWaypoint = sourceWaypoint->nextWaypoint;
			}
		}

		newPlatform->setTiltable( false );
		newPlatform->postInit();
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const CRECT *CNpcConveyorPlatformGenerator::getThinkBBox()
{
	CRECT objThinkBox = getCollisionArea();

	sBBox &thinkBBox = CThingManager::getThinkBBox();
	objThinkBox.x1 = thinkBBox.XMin;
	objThinkBox.x2 = thinkBBox.XMax;
	objThinkBox.y1 = thinkBBox.YMin;
	objThinkBox.y2 = thinkBBox.YMax;

	return &objThinkBox;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcConveyorPlatform::postInit()
{
	CNpcPlatform::postInit();

	m_npcPath.setPathType( CNpcPath::SINGLE_USE_PATH );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcConveyorPlatform::processMovement( int _frames )
{
	s32 moveX = 0, moveY = 0;
	s32 distX, distY;
	bool pathComplete, waypointChange;

	m_npcPath.think( Pos, &pathComplete, &waypointChange, &distX, &distY );

	if ( pathComplete )
	{
		setToShutdown();
	}
	else
	{
		if ( distX )
		{
			moveX = distX;

			if ( moveX > 1 )
			{
				moveX = 1;
			}
			else if ( moveX < -1 )
			{
				moveX = -1;
			}
		}
		else
		{
			moveY = distY;

			s32 verticalMovement = 3 * _frames;

			if ( moveY > verticalMovement )
			{
				moveY = verticalMovement;
			}
			else if ( moveY < -verticalMovement )
			{
				moveY = -verticalMovement;
			}
		}
	}

	Pos.vx += moveX;
	Pos.vy += moveY;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const CRECT *CNpcConveyorPlatform::getThinkBBox()
{
	CRECT objThinkBox = getCollisionArea();

	sBBox &thinkBBox = CThingManager::getThinkBBox();
	objThinkBox.x1 = thinkBBox.XMin;
	objThinkBox.x2 = thinkBBox.XMax;
	objThinkBox.y1 = thinkBBox.YMin;
	objThinkBox.y2 = thinkBBox.YMax;

	return &objThinkBox;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcConveyorPlatform::collidedWith( CThing *_thisThing )
{
	switch(_thisThing->getThingType())
	{
		case TYPE_PLAYER:
		{
			CPlayer *player;
			DVECTOR	playerPos;
			CRECT	collisionArea;

			// Only interested in SBs feet colliding with the box (pkg)
			player=(CPlayer*)_thisThing;
			playerPos=player->getPos();
			collisionArea=getCollisionArea();

			s32 threshold = abs( collisionArea.y2 - collisionArea.y1 );

			if ( threshold > 16 )
			{
				threshold = 16;
			}

			if( playerPos.vx >= collisionArea.x1 && playerPos.vx <= collisionArea.x2 )
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
			break;

		case TYPE_HAZARD:
		{
			// needs to explode or something

			setToShutdown();

			break;
		}

		default:
			ASSERT(0);
			break;
	}
}

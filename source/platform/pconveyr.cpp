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

#ifndef __VID_HEADER_
#include "system\vid.h"
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

		// generate new conveyor platform

		CNpcPlatform *newPlatform = NULL;
		newPlatform = CNpcPlatform::Create( NPC_CONVEYOR_PLATFORM );

		ASSERT(newPlatform);

		newPlatform->setThingSubType( NPC_CONVEYOR_PLATFORM );
		newPlatform->setGraphic( m_graphicNum );

		DVECTOR startPos = Pos;
		newPlatform->init( startPos );

		newPlatform->setWaypointCount( m_npcPath.getWaypointCount() );
		newPlatform->setWaypointPtr( m_npcPath.getWaypointPtr() );

		newPlatform->setTiltable( false );
		newPlatform->postInit();
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcConveyorPlatform::postInit()
{
	CNpcPlatform::postInit();

	m_npcPath.setPathType( CNpcPath::SINGLE_USE_PATH );

	m_spinFinish = false;
	m_rotation = 0;

	s32 minX, maxX, minY, maxY;

	m_npcPath.getPathXExtents( &minX, &maxX );
	m_npcPath.getPathYExtents( &minY, &maxY );

	m_thinkArea.x1 = minX;
	m_thinkArea.x2 = maxX;
	m_thinkArea.y1 = minY;
	m_thinkArea.y2 = maxY;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcConveyorPlatform::processMovement( int _frames )
{
	s32 moveX = 0, moveY = 0;
	s32 distX, distY;
	bool pathComplete, waypointChange;

	if ( m_spinFinish )
	{
		m_rotation += 64 * _frames;
		m_rotation &= 4095;

		DVECTOR const &offset = CLevel::getCameraPos();

		Pos.vy += 3 * _frames;

		s32 yPos = Pos.vy - offset.vy;

		if ( yPos < 0 || yPos > VidGetScrH() )
		{
			setToShutdown();
		}
	}
	else
	{
		m_npcPath.think( Pos, &pathComplete, &waypointChange, &distX, &distY );

		if ( pathComplete )
		{
			m_spinFinish = true;
		}
		else
		{
			if ( distX )
			{
				int Time = GameState::getFramesSinceLast();

				moveX = distX;

				if ( moveX > Time )
				{
					moveX = Time;
				}
				else if ( moveX < -Time )
				{
					moveX = -Time;
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
	}

	Pos.vx += moveX;
	Pos.vy += moveY;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcConveyorPlatform::collidedWith( CThing *_thisThing )
{
	switch(_thisThing->getThingType())
	{
		case TYPE_PLAYER:
		{
			CPlayer *player;
//			DVECTOR	playerPos;
//			CRECT	collisionArea;

			// Only interested in SBs feet colliding with the box (pkg)
			player=(CPlayer*)_thisThing;
DVECTOR const &playerPos=player->getPos();
CRECT const &collisionArea=getCollisionArea();

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
						else
						{
							int distLeft, distRight;

							DVECTOR const &playerCollisionSize = player->getCollisionSize();

							distLeft = collisionArea.x1 - playerPos.vx - ( playerCollisionSize.vx >> 1 );
							distRight = collisionArea.x2 - playerPos.vx + ( playerCollisionSize.vx >> 1 );

							DVECTOR playerShove;
							playerShove.vy = 0;

							if ( abs( distLeft ) < abs( distRight ) )
							{
								playerShove.vx = distLeft;
							}
							else
							{
								playerShove.vx = distRight;
							}

							player->shove( playerShove );
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
			if ( !m_spinFinish )
			{
				m_spinFinish = true;
			}

			break;
		}

		default:
			ASSERT(0);
			break;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcConveyorPlatform::render()
{
	if ( m_isActive )
	{
		CPlatformThing::render();

		// Render
		if (canRender())
		{
			DVECTOR &renderPos=getRenderPos();

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

int CNpcConveyorPlatform::checkCollisionAgainst(CThing *_thisThing, int _frames)
{
	if ( m_spinFinish )
	{
		return( false );
	}
	else
	{
		return( CNpcPlatform::checkCollisionAgainst( _thisThing, _frames ) );
	}
}

/*=========================================================================

	pdual.cpp

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __PLATFORM_PDUAL_H__
#include "platform\pdual.h"
#endif

#ifndef __HAZARD_HAZARD_H__
#include "hazard\hazard.h"
#endif

#ifndef	__UTILS_HEADER__
#include	"utils\utils.h"
#endif

#ifndef __VID_HEADER_
#include "system\vid.h"
#endif

#ifndef __GAME_GAME_H__
#include	"game\game.h"
#endif


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcDualPlatform::setMaster( u8 isMaster )
{
	m_isMaster = isMaster;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcDualPlatform::setOtherPlatform( CNpcDualPlatform *other )
{
	m_otherPlatform = other;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcDualPlatform::setWaypoints( sThingPlatform *ThisPlatform )
{
	ASSERT( ThisPlatform->PointCount >= 3 );

	u16	*PntList=(u16*)MakePtr(ThisPlatform,sizeof(sThingPlatform));

	u16 newXPos, newYPos;

	// get master platform init pos

	newXPos = (u16) *PntList;
	PntList++;
	newYPos = (u16) *PntList;
	PntList++;

	DVECTOR startPos;
	startPos.vx = newXPos << 4;
	startPos.vy = newYPos << 4;

	init( startPos );

	m_extension = 0;

	// get master platform max vertical extension

	newXPos = (u16) *PntList;
	PntList++;
	newYPos = (u16) *PntList;
	PntList++;

	m_maxExtension = ( newYPos << 4 ) - startPos.vy;

	ASSERT( m_maxExtension >= 0 );

	// get slave platform init x pos

	newXPos = (u16) *PntList;
	PntList++;
	newYPos = (u16) *PntList;
	PntList++;

	DVECTOR slavePos;
	slavePos.vx = newXPos << 4;
	slavePos.vy = startPos.vy + m_maxExtension;

	m_otherPlatform->init( slavePos );
	m_otherPlatform->postInit();

	if ( ThisPlatform->PointCount > 3 )
	{
		DVECTOR base;

		base.vx = (u16) *PntList;
		base.vx <<= 4;
		PntList++;
		base.vy = (u16) *PntList;
		base.vy <<= 4;
		PntList++;

		m_otherPlatform->setLineBase( base );

		base.vx = (u16) *PntList;
		base.vx <<= 4;
		PntList++;
		base.vy = (u16) *PntList;
		base.vy <<= 4;
		PntList++;

		setLineBase( base );
	}
	else
	{
		setLineBase( Pos );
		m_otherPlatform->setLineBase( slavePos );
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

u8 CNpcDualPlatform::canDrop()
{
	if ( m_isMaster )
	{
		s32 extensionLeft = m_maxExtension - m_extension;

		if ( extensionLeft == 0 )
		{
			return( false );
		}
		else
		{
			return( true );
		}
	}
	else
	{
		return( false );
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcDualPlatform::processMovement( int _frames )
{
	if ( m_isMaster )
	{
		s32 extensionChange = 0;
		s32 extensionLeft;

		if ( m_contact )
		{
			extensionLeft = m_maxExtension - m_extension;

			extensionChange = 3 * _frames;

			if ( extensionChange > extensionLeft )
			{
				extensionChange = extensionLeft;
			}
		}
		else
		{
			extensionLeft = m_extension;

			extensionChange = -3 * _frames;

			if ( -extensionChange > extensionLeft )
			{
				extensionChange = -extensionLeft;
			}
		}

		m_extension += extensionChange;

		Pos.vy = m_base.vy + m_extension;

		DVECTOR slaveMove;
		slaveMove.vx = 0;
		slaveMove.vy = -extensionChange;

		m_otherPlatform->setMovement( slaveMove );
		m_otherPlatform->think(_frames);
		m_otherPlatform->updateCollisionArea();
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcDualPlatform::setMovement( DVECTOR move )
{
	Pos.vx += move.vx;
	Pos.vy += move.vy;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcDualPlatform::render()
{
	int		x1,y1,x2,y2;

	if ( m_isActive )
	{
		CPlatformThing::render();

		if (canRender())
		{
			DVECTOR &renderPos=getRenderPos();

			m_modelGfx->Render(renderPos);
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const CRECT *CNpcDualPlatform::getThinkBBox()
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

void CNpcDualPlatform::collidedWith( CThing *_thisThing )
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
			m_contact = true;

			CNpcHazard *hazard;

			hazard = (CNpcHazard *)_thisThing;

			hazard->setPlatform( this );

			break;
		}

		default:
			ASSERT(0);
			break;
	}
}

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

#ifndef __FRIEND_FRIEND_H__
#include "friend\friend.h"
#endif

#ifndef	__GFX_OTPOS_H__
#include "gfx\otpos.h"
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
	startPos.vx = ( newXPos << 4 ) + 8;
	startPos.vy = ( newYPos << 4 ) + 16;

	init( startPos );

	m_extension = 0;

	// get master platform max vertical extension

	newXPos = (u16) *PntList;
	PntList++;
	newYPos = (u16) *PntList;
	PntList++;

	m_maxExtension = ( ( newYPos << 4 ) + 16 ) - startPos.vy;

	ASSERT( m_maxExtension >= 0 );

	// get slave platform init x pos

	newXPos = (u16) *PntList;
	PntList++;
	newYPos = (u16) *PntList;
	PntList++;

	DVECTOR slavePos;
	slavePos.vx = ( newXPos << 4 ) + 8;
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

	m_thinkArea.x1 = startPos.vx;
	m_thinkArea.x2 = slavePos.vx;

	if ( m_thinkArea.x1 > m_thinkArea.x2 )
	{
		int temp = m_thinkArea.x2;
		m_thinkArea.x2 = m_thinkArea.x1;
		m_thinkArea.x1 = temp;
	}

	m_thinkArea.y1 = startPos.vy;
	m_thinkArea.y2 = startPos.vy + m_maxExtension;

	m_otherPlatform->setThinkArea( m_thinkArea );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcDualPlatform::setThinkArea( CRECT &newArea )
{
	m_thinkArea.x1 = newArea.x1;
	m_thinkArea.y1 = newArea.y1;
	m_thinkArea.x2 = newArea.x2;
	m_thinkArea.y2 = newArea.y2;
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

		if ( extensionChange && m_soundId == NOT_PLAYING )
		{
			m_soundId = (int) CSoundMediator::playSfx( CSoundMediator::SFX_PULLEY, true, true );
		}

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

	DVECTOR const &offset = CLevel::getCameraPos();

	if ( m_isActive )
	{
		CPlatformThing::render();

		if (canRender())
		{
			DVECTOR &renderPos=getRenderPos();

			m_modelGfx->Render(renderPos);
		}

		x1 = Pos.vx - offset.vx;
		x2 = m_lineBase.vx - offset.vx;

		if ( x1 > x2 )
		{
			int tempX = x1;
			x1 = x2;
			x2 = tempX;
		}

		y1 = Pos.vy - offset.vy;
		y2 = m_lineBase.vy - offset.vy;

		if ( y1 > y2 )
		{
			int tempY = y1;
			y1 = y2;
			y2 = tempY;
		}

		if ( y1 < 0 )
		{
			y1 = 0;
		}
		
		if ( y2 > VidGetScrH() )
		{
			y2 = VidGetScrH();
		}

		if ( x2 >= 0 && x1 <= VidGetScrW() )
		{
			if ( y2 >= 0 && y1 <= VidGetScrH() )
			{
				DrawLine( x1, y1, x2, y2, 0, 0, 0, OTPOS__ACTOR_POS+1 );
			}
		}

		if ( m_isMaster )
		{
			DVECTOR otherLineBase = m_otherPlatform->getLineBase();

			x1 = otherLineBase.vx - offset.vx;
			x2 = m_lineBase.vx - offset.vx;

			if ( x1 > x2 )
			{
				int tempX = x1;
				x1 = x2;
				x2 = tempX;
			}

			y1 = otherLineBase.vy - offset.vy;
			y2 = m_lineBase.vy - offset.vy;

			if ( y1 > y2 )
			{
				int tempY = y1;
				y1 = y2;
				y2 = tempY;
			}

			if ( x1 < 0 )
			{
				x1 = 0;
			}
			
			if ( x2 > VidGetScrW() )
			{
				x2 = VidGetScrW();
			}

			if ( x2 >= 0 && x1 <= VidGetScrW() )
			{
				if ( y2 >= 0 && y1 <= VidGetScrH() )
				{
					DrawLine( x1, y1, x2, y2, 0, 0, 0, OTPOS__ACTOR_POS+1 );
				}
			}
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcDualPlatform::collidedWith( CThing *_thisThing )
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
		{
			CNpcFriend *friendNpc;
//			DVECTOR	friendPos;
//			CRECT	collisionArea;

			friendNpc = (CNpcFriend*) _thisThing;
DVECTOR	const	&friendPos = friendNpc->getPos();
CRECT const &friendCollisionArea = friendNpc->getCollisionArea();
CRECT const		&collisionArea=getCollisionArea();

			s32 threshold = abs( collisionArea.y2 - collisionArea.y1 );

			if ( threshold > 16 )
			{
				threshold = 16;
			}

			if( friendCollisionArea.x2 >= collisionArea.x1 && friendCollisionArea.x1 <= collisionArea.x2 )
			{
				if ( checkCollisionDelta( _thisThing, threshold, collisionArea ) )
				{
					int height = getHeightFromPlatformAtPosition( friendPos.vx, friendPos.vy );

					friendNpc->setPlatform( this );

					m_contact = true;
				}
				else
				{
					if( friendPos.vy >= collisionArea.y1 && friendPos.vy <= collisionArea.y2 )
					{
						int height = getHeightFromPlatformAtPosition( friendPos.vx, friendPos.vy );

						if ( height >= -threshold && height < 1 )
						{
							friendNpc->setPlatform( this );

							m_contact = true;
						}
					}
				}
			}

			break;
		}

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

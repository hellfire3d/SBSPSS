/*=========================================================================

	plift.cpp

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __PLATFORM_PLIFT_H__
#include "platform\plift.h"
#endif

#ifndef	__UTILS_HEADER__
#include	"utils\utils.h"
#endif

#ifndef __GAME_GAME_H__
#include	"game\game.h"
#endif

#ifndef __FRIEND_FRIEND_H__
#include "friend\friend.h"
#endif


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcLiftPlatform::postInit()
{
	CNpcPlatform::postInit();

	m_isActivated = false;
	m_returning = false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcLiftPlatform::setWaypoints( sThingPlatform *ThisPlatform )
{
	int pointNum;

	u16	*PntList=(u16*)MakePtr(ThisPlatform,sizeof(sThingPlatform));

	u16 newXPos, newYPos;

	newXPos = (u16) *PntList;
	PntList++;
	newYPos = (u16) *PntList;
	PntList++;

	DVECTOR startPos;
	startPos.vx = ( newXPos << 4 ) + 8;
	startPos.vy = ( newYPos << 4 ) + 16;

	init( startPos );

	newXPos = (u16) *PntList;
	PntList++;
	newYPos = (u16) *PntList;
	PntList++;

	m_maxExtension = ( ( newYPos << 4 ) + 16 ) - startPos.vy;

	/*s32 minX, maxX, minY, maxY;

	m_npcPath.getPathXExtents( &minX, &maxX );
	m_npcPath.getPathYExtents( &minY, &maxY );*/

	m_thinkArea.x1 = startPos.vx;
	m_thinkArea.x2 = startPos.vx + 1;
	m_thinkArea.y1 = startPos.vy;
	m_thinkArea.y2 = startPos.vy + m_maxExtension;

	if ( m_thinkArea.y1 > m_thinkArea.y2 )
	{
		s32 temp = m_thinkArea.y2;
		m_thinkArea.y2 = m_thinkArea.y1;
		m_thinkArea.y1 = temp;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcLiftPlatform::processMovement( int _frames )
{
	if ( m_contact )
	{
		if ( !m_isActivated )
		{
			m_isActivated = true;
			m_timer = GameState::getOneSecondInFrames();
		}
	}

	if ( m_isActivated )
	{
		if ( m_timer <= 0 )
		{
			s32 extension;

			if ( m_returning )
			{
				extension = -m_extension;
			}
			else
			{
				extension = m_maxExtension - m_extension;
			}

			s32 maxMove = m_speed * _frames;

			if ( extension > maxMove )
			{
				extension = maxMove;
			}
			else if ( extension < -maxMove )
			{
				extension = -maxMove;
			}

			if ( extension )
			{
				m_extension += extension;

				if(m_soundId==NOT_PLAYING)
				{
					m_soundId=CSoundMediator::playSfx(CSoundMediator::SFX_ELEVATOR_LOOP,true,true);
				}
			}
			else
			{
				if ( m_soundId != NOT_PLAYING )
				{
					CSoundMediator::stopAndUnlockSfx( (xmPlayingId) m_soundId );
				}

				m_soundId=CSoundMediator::playSfx(CSoundMediator::SFX_ELEVATOR_STOP,true,true);

				if ( m_returning )
				{
					m_isActivated = false;
					m_returning = false;
				}
				else
				{
					m_timer = GameState::getOneSecondInFrames();
					m_returning = true;
				}
			}
		}

		Pos.vy = m_base.vy + m_extension;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcLiftPlatform::processTimer( int _frames )
{
	if ( m_timer > 0 )
	{
		m_timer -= _frames;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcLiftPlatform::collidedWith( CThing *_thisThing )
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
DVECTOR const &friendPos = friendNpc->getPos();
CRECT const &friendCollisionArea = friendNpc->getCollisionArea();
CRECT const &collisionArea=getCollisionArea();

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
				}
				else
				{
					if( friendPos.vy >= collisionArea.y1 && friendPos.vy <= collisionArea.y2 )
					{
						int height = getHeightFromPlatformAtPosition( friendPos.vx, friendPos.vy );

						if ( height >= -threshold && height < 1 )
						{
							friendNpc->setPlatform( this );
						}
					}
				}
			}

			break;
		}

		case TYPE_HAZARD:
			break;

		default:
			ASSERT(0);
			break;
	}
}

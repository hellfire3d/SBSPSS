/*=========================================================================

	hbrock.h

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __HAZARD_HBROCK_H__
#include "hazard\hbrock.h"
#endif

#ifndef __TRIGGERS_THAZWALK_H__
#include "triggers\thazwalk.h"
#endif

#ifndef	__UTILS_HEADER__
#include	"utils\utils.h"
#endif

#ifndef	__PLAYER_PLAYER_H__
#include	"player\player.h"
#endif


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcBouncingRockHazard::init()
{
	CNpcBouncingBarrelHazard::init();

	m_isTriggered = false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcBouncingRockHazard::setWaypoints( sThingHazard *ThisHazard )
{
	int pointNum;

	u16	*PntList=(u16*)MakePtr(ThisHazard,sizeof(sThingHazard));

	u16 newXPos, newYPos;

	m_npcPath.setWaypointCount( ThisHazard->PointCount - 2 );

	newXPos = (u16) *PntList;
	setWaypointPtr( PntList );
	PntList++;
	newYPos = (u16) *PntList;
	PntList++;

	DVECTOR startPos;
	startPos.vx = ( newXPos << 4 ) + 8;
	startPos.vy = ( newYPos << 4 ) + 16;

	Pos = startPos;
	m_base = Pos;

	for ( int i = 2 ; i < ThisHazard->PointCount ; i++ )
	{
		PntList++;
		PntList++;
	}

	newXPos = (u16) *PntList;
	PntList++;
	newYPos = (u16) *PntList;

	CHazardTrigger *trigger;

	trigger=(CHazardTrigger*)CTrigger::Create(CTrigger::TRIGGER_HAZARD_WALK);
	trigger->setPositionAndSize( ( newXPos << 4 ) + 8, ( newYPos << 4 ) + 16 - 100, 128, 128 );
	trigger->setHazard( this );

	s32 minX, maxX, minY, maxY;

	m_npcPath.getPathXExtents( &minX, &maxX );
	m_npcPath.getPathYExtents( &minY, &maxY );

	m_thinkArea.x1 = minX;
	m_thinkArea.x2 = maxX;
	m_thinkArea.y1 = minY;
	m_thinkArea.y2 = maxY;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcBouncingRockHazard::render()
{
	if ( m_isTriggered )
	{
		CNpcBouncingBarrelHazard::render();
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcBouncingRockHazard::processMovement( int _frames )
{
	if ( m_isTriggered )
	{
		s32 moveX = 0, moveY = 0;
		s32 moveVel = 0;
		s32 moveDist = 0;
		s32 waypointXDist;
		s32 waypointYDist;
		s32 waypointHeading;
		s32 groundHeight;

		// deal with horizontal

		if ( m_soundId == NOT_PLAYING )
		{
			m_soundId = (int) CSoundMediator::playSfx( CSoundMediator::SFX_ROLLING_ROCK, true, true );
		}

		bool pathComplete;
		
		if ( m_npcPath.thinkFlat( Pos, &pathComplete, &waypointXDist, &waypointYDist, &waypointHeading, 1 ) )
		{
			if ( pathComplete )
			{
				setToShutdown();
			}

			m_lastWaypoint = Pos;
		}

		moveX = 3 * _frames;

		if ( moveX > abs( waypointXDist ) )
		{
			moveX = abs( waypointXDist );
		}

		if ( waypointHeading == 2048 )
		{
			moveX = -moveX;
			m_rotation -= 256 * _frames;
			m_rotation &= 4095;
		}
		else
		{
			m_rotation += 256 * _frames;
			m_rotation &= 4095;
		}

		if ( m_rockDir )
		{
			m_rockRotation += 30 * _frames;

			if ( m_rockRotation > 256 )
			{
				m_rockDir = false;
			}
		}
		else
		{
			m_rockRotation -= 30 * _frames;

			if ( m_rockRotation < -256 )
			{
				m_rockDir = true;
			}
		}

		Pos.vx += moveX;

		// deal with vertical

		DVECTOR nextWaypoint;
		
		nextWaypoint.vx = waypointXDist + Pos.vx;
		nextWaypoint.vy = waypointYDist + Pos.vy;

		s32 waypointDist = abs( nextWaypoint.vx - m_lastWaypoint.vx );

		if ( waypointDist < 1 )
		{
			waypointDist = 1;
		}

		if ( waypointYDist > 0 )
		{
			s32 sineVal = ( abs( Pos.vx - nextWaypoint.vx ) * 1024 ) / waypointDist;

			Pos.vy = nextWaypoint.vy - ( ( abs( nextWaypoint.vy - m_lastWaypoint.vy ) * rsin( sineVal ) ) >> 12 );
		}
		else if ( waypointYDist < 0 )
		{
			s32 sineVal = ( abs( Pos.vx - m_lastWaypoint.vx ) * 1024 ) / waypointDist;

			Pos.vy = m_lastWaypoint.vy - ( ( abs( nextWaypoint.vy - m_lastWaypoint.vy ) * rsin( sineVal ) ) >> 12 );
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcBouncingRockHazard::trigger()
{
	m_isTriggered = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcBouncingRockHazard::collidedWith( CThing *_thisThing )
{
	if ( m_isActive )
	{
		switch(_thisThing->getThingType())
		{
			case TYPE_PLAYER:
			{
				CPlayer *player = (CPlayer *) _thisThing;
				player->takeDamage( DAMAGE__KILL_OUTRIGHT );

				break;
			}

			default:
				break;
		}
	}
}

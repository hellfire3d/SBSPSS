/*=========================================================================

	npcpath.cpp

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/

#ifndef __ENEMY_NPCPATH_H__
#include "enemy\npcpath.h"
#endif


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool CNpcPath::isPointNear( DVECTOR const &testPos, s32 *xDist, s32 *yDist )
{
	s32 xDistSqr, yDistSqr;

	u16 *waypoint = waypointPtr;
	waypoint += 2 * currentWaypoint;

	*xDist = ( ( *waypoint << 4 ) + 8 ) - testPos.vx;
	xDistSqr = (*xDist) * (*xDist);

	waypoint++;

	*yDist = ( ( *waypoint << 4 ) + 16 ) - testPos.vy;
	yDistSqr = (*yDist) * (*yDist);

	if ( xDistSqr + yDistSqr < 100 )
	{
		return( true );
	}
	else
	{
		return( false );
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcPath::getCurrentWaypointPos( DVECTOR *waypointPos )
{
	u16 *waypoint = waypointPtr;
	waypoint += 2 * currentWaypoint;

	waypointPos->vx = ( *waypoint << 4 ) + 8;

	waypoint++;

	waypointPos->vy = ( *waypoint << 4 ) + 16;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcPath::initPath()
{
	//waypoint = NULL;
	pathType = SINGLE_USE_PATH;
	currentWaypoint = 0;
	lastWaypoint = 0;
	waypointCount = 0;
	reversePath = false;
	minX = maxX = minY = maxY = 0;
	waypointPtr = NULL;
	decLockout = false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcPath::resetPath()
{
	lastWaypoint = currentWaypoint = 0;
	decLockout = false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcPath::getPathXExtents( s32 *minExtent, s32 *maxExtent )
{
	*minExtent = minX;
	*maxExtent = maxX;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcPath::getPathYExtents( s32 *minExtent, s32 *maxExtent )
{
	*minExtent = minY;
	*maxExtent = maxY;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcPath::setPathType( u8 newPathType )
{
	pathType = (NPC_PATH_TYPE) newPathType;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

u8 CNpcPath::getPathType()
{
	return( pathType );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcPath::setWaypointPtr( u16 *newPtr )
{
	waypointPtr = newPtr;

	setPathExtents();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcPath::setPathExtents()
{
	u8 tempWaypoint;
	u16 *tempPtr = waypointPtr;
	DVECTOR mapPos;

	if ( tempPtr )
	{
		mapPos.vx = ( *tempPtr << 4 ) + 8;
		*tempPtr++;
		mapPos.vy = ( *tempPtr << 4 ) + 16;
		*tempPtr++;

		minX = maxX = mapPos.vx;
		minY = maxY = mapPos.vy;

		for ( tempWaypoint = 1 ; tempWaypoint <= waypointCount ; tempWaypoint++ )
		{
			mapPos.vx = ( *tempPtr << 4 ) + 8;
			*tempPtr++;
			mapPos.vy = ( *tempPtr << 4 ) + 16;
			*tempPtr++;

			if ( mapPos.vx < minX )
			{
				minX = mapPos.vx;
			}
			else if ( mapPos.vx > maxX )
			{
				maxX = mapPos.vx;
			}

			if ( mapPos.vy < minY )
			{
				minY = mapPos.vy;
			}
			else if ( mapPos.vy > maxY )
			{
				maxY = mapPos.vy;
			}
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool CNpcPath::incPath()
{
	decLockout = false;

	if ( !reversePath )
	{
		if ( currentWaypoint < waypointCount )
		{
			lastWaypoint = currentWaypoint;
			currentWaypoint++;
		}
		else
		{
			switch( pathType )
			{
				case SINGLE_USE_PATH:
					// path is completed

					return( true );

				case REPEATING_PATH:
					// go back to start

					lastWaypoint = currentWaypoint;
					currentWaypoint = 0;

					break;

				case PONG_PATH:
					// reverse path

					reversePath = !reversePath;

					if ( currentWaypoint > 0 )
					{
						lastWaypoint = currentWaypoint;
						currentWaypoint--;
					}

					break;
			}
		}
	}
	else
	{
		// must be pong path if reversed

		if ( currentWaypoint > 0 )
		{
			lastWaypoint = currentWaypoint;
			currentWaypoint--;
		}
		else
		{
			reversePath = !reversePath;

			if ( currentWaypoint < waypointCount )
			{
				lastWaypoint = currentWaypoint;
				currentWaypoint++;
			}
		}
	}

	return( false );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool CNpcPath::decPath()
{
	if ( !decLockout )
	{
		if ( currentWaypoint > 0 )
		{
			lastWaypoint = currentWaypoint;
			currentWaypoint--;
		}
		else
		{
			lastWaypoint = currentWaypoint;
			currentWaypoint = waypointCount;
		}

		decLockout = true;

		return( true );
	}

	return( false );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcPath::reversePathDir()
{
	if ( lastWaypoint )
	{
		u8 tempWaypoint;

		tempWaypoint = currentWaypoint;
		currentWaypoint = lastWaypoint;
		lastWaypoint = tempWaypoint;

		if ( pathType == PONG_PATH )
		{
			reversePath = !reversePath;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool CNpcPath::getDistToNextWaypoint( DVECTOR const &currentPos, s32 *distX, s32 *distY )
{
	return( isPointNear( currentPos, distX, distY ) );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

s32 CNpcPath::think( DVECTOR const &currentPos, bool *pathComplete, bool *waypointChange, s32 *distX, s32 *distY )
{
	if ( !waypointPtr )
	{
		return( 0 );
	}

	*pathComplete = false;
	*waypointChange = false;

	if ( isPointNear( currentPos, distX, distY ) )
	{
		*pathComplete = incPath();
		*waypointChange = true;

		u16 *waypoint = waypointPtr;
		waypoint += 2 * currentWaypoint;

		*distX = ( ( *waypoint << 4 ) + 8 ) - currentPos.vx;
		waypoint++;
		*distY = ( ( *waypoint << 4 ) + 16 ) - currentPos.vy;
	}

	s32 headingToTarget = ratan2( *distY, *distX ) & 4095;

	return( headingToTarget );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool CNpcPath::thinkFlat( DVECTOR const &currentPos, bool *pathComplete, s32 *distX, s32 *distY, s32 *heading, u8 waypointDist )
{
	bool pointChange = false;

	*pathComplete = false;

	if ( !waypointPtr )
	{
		return( true );
	}

	u16 *waypoint = waypointPtr;
	waypoint += 2 * currentWaypoint;

	*distX = ( ( *waypoint << 4 ) + 8 ) - currentPos.vx;
	waypoint++;
	*distY = ( ( *waypoint << 4 ) + 16 ) - currentPos.vy;

	if ( abs( *distX ) < waypointDist )
	{
		pointChange = true;
		*pathComplete = incPath();
	}

	waypoint = waypointPtr;
	waypoint += 2 * currentWaypoint;

	*distX = ( ( *waypoint << 4 ) + 8 ) - currentPos.vx;
	waypoint++;
	*distY = ( ( *waypoint << 4 ) + 16 ) - currentPos.vy;

	if ( *distX > 0 )
	{
		*heading = 0;
	}
	else
	{
		*heading = 2048;
	}

	return( pointChange );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool CNpcPath::thinkVertical( DVECTOR const &currentPos, bool *pathComplete, s32 *distX, s32 *distY, s32 *heading )
{
	bool pointChange = false;

	*pathComplete = false;

	if ( !waypointPtr )
	{
		return( true );
	}

	u16 *waypoint = waypointPtr;
	waypoint += 2 * currentWaypoint;

	*distX = ( ( *waypoint << 4 ) + 8 ) - currentPos.vx;
	waypoint++;
	*distY = ( ( *waypoint << 4 ) + 16 ) - currentPos.vy;

	if ( abs( *distY ) < 10 )
	{
		pointChange = true;
		*pathComplete = incPath();
	}

	waypoint = waypointPtr;
	waypoint += 2 * currentWaypoint;

	*distX = ( ( *waypoint << 4 ) + 8 ) - currentPos.vx;
	waypoint++;
	*distY = ( ( *waypoint << 4 ) + 16 ) - currentPos.vy;

	if ( *distY > 0 )
	{
		*heading = 1024;
	}
	else
	{
		*heading = 3072;
	}

	return( pointChange );
}

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

bool CNpcWaypoint::isPointNear( DVECTOR testPos, s32 *xDist, s32 *yDist )
{
	s32 xDistSqr, yDistSqr;

	*xDist = this->pos.vx - testPos.vx;
	xDistSqr = (*xDist) * (*xDist);

	*yDist = this->pos.vy - testPos.vy;
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

void CNpcPath::initPath()
{
	waypoint = NULL;
	pathType = SINGLE_USE_PATH;
	currentWaypoint = NULL;
	lastWaypoint = NULL;
	waypointCount = 0;
	reversePath = false;
	minX = maxX = minY = maxY = 0;
}

void CNpcPath::resetPath()
{
	currentWaypoint = waypoint;
	lastWaypoint = NULL;
}

void CNpcPath::addWaypoint( DVECTOR newPos )
{
	CNpcWaypoint *testWaypoint;
	CNpcWaypoint *newWaypoint;

	testWaypoint = this->waypoint;

	if ( testWaypoint )
	{
		// find end of path

		while ( testWaypoint->nextWaypoint )
		{
			testWaypoint = testWaypoint->nextWaypoint;
		}

		newWaypoint = new( "waypoint" ) CNpcWaypoint;
		newWaypoint->pos = newPos;
		newWaypoint->nextWaypoint = NULL;
		newWaypoint->prevWaypoint = testWaypoint;

		testWaypoint->nextWaypoint = newWaypoint;
		waypointCount++;

		if ( newPos.vx < minX )
		{
			minX = newPos.vx;
		}
		else if ( newPos.vx > maxX )
		{
			maxX = newPos.vx;
		}

		if ( newPos.vy < minY )
		{
			minY = newPos.vy;
		}
		else if ( newPos.vy > maxY )
		{
			maxY = newPos.vy;
		}
	}
	else
	{
		// no waypoints exist in this path, create

		newWaypoint = new( "waypoint" ) CNpcWaypoint;
		newWaypoint->pos = newPos;
		newWaypoint->nextWaypoint = NULL;
		newWaypoint->prevWaypoint = NULL;

		this->waypoint = newWaypoint;
		waypointCount++;

		currentWaypoint = this->waypoint;

		minX = maxX = newPos.vx;
		minY = maxY = newPos.vy;
	}
}

void CNpcPath::getPathXExtents( s32 *minExtent, s32 *maxExtent )
{
	*minExtent = minX;
	*maxExtent = maxX;
}

void CNpcPath::getPathYExtents( s32 *minExtent, s32 *maxExtent )
{
	*minExtent = minY;
	*maxExtent = maxY;
}

void CNpcPath::removeAllWaypoints()
{
	CNpcWaypoint *testWaypoint;
	CNpcWaypoint *lastWaypoint;

	testWaypoint = this->waypoint;

	while ( testWaypoint )
	{
		lastWaypoint = testWaypoint;
		testWaypoint = testWaypoint->nextWaypoint;

		delete lastWaypoint;
	}

	this->waypoint = NULL;
}

void CNpcPath::setPathType( u8 newPathType )
{
	pathType = (NPC_PATH_TYPE) newPathType;
}

u8 CNpcPath::getPathType()
{
	return( pathType );
}

bool CNpcPath::incPath()
{
	if ( !reversePath )
	{
		if ( currentWaypoint->nextWaypoint )
		{
			lastWaypoint = currentWaypoint;
			currentWaypoint = currentWaypoint->nextWaypoint;
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
					currentWaypoint = this->waypoint;

					break;

				case PONG_PATH:
					// reverse path

					reversePath = !reversePath;

					if ( currentWaypoint->prevWaypoint )
					{
						lastWaypoint = currentWaypoint;
						currentWaypoint = currentWaypoint->prevWaypoint;
					}

					break;
			}
		}
	}
	else
	{
		// must be pong path if reversed

		if ( currentWaypoint->prevWaypoint )
		{
			lastWaypoint = currentWaypoint;
			currentWaypoint = currentWaypoint->prevWaypoint;
		}
		else
		{
			reversePath = !reversePath;

			if ( currentWaypoint->nextWaypoint )
			{
				lastWaypoint = currentWaypoint;
				currentWaypoint = currentWaypoint->nextWaypoint;
			}
		}
	}

	return( false );
}

void CNpcPath::reversePathDir()
{
	if ( lastWaypoint )
	{
		CNpcWaypoint *tempWaypoint;

		tempWaypoint = currentWaypoint;
		currentWaypoint = lastWaypoint;
		lastWaypoint = tempWaypoint;

		if ( pathType == PONG_PATH )
		{
			reversePath = !reversePath;
		}
	}
}

bool CNpcPath::getDistToNextWaypoint( DVECTOR currentPos, s32 *distX, s32 *distY )
{
	return( currentWaypoint->isPointNear( currentPos, distX, distY ) );
}

s32 CNpcPath::think( DVECTOR currentPos, bool *pathComplete, bool *waypointChange, s32 *distX, s32 *distY )
{
	if ( !this->waypoint )
	{
		return( 0 );
	}

	if ( !currentWaypoint )
	{
		// if no currentWaypoint set, start it off

		currentWaypoint = this->waypoint;
	}

	*pathComplete = false;
	*waypointChange = false;

	if ( currentWaypoint->isPointNear( currentPos, distX, distY ) )
	{
		*pathComplete = incPath();
		*waypointChange = true;

		*distX = currentWaypoint->pos.vx - currentPos.vx;
		*distY = currentWaypoint->pos.vy - currentPos.vy;
	}

	s32 headingToTarget = ratan2( *distY, *distX );

	return( headingToTarget );
}

bool CNpcPath::thinkFlat( DVECTOR currentPos, bool *pathComplete, s32 *distX, s32 *distY, s32 *heading, u8 waypointDist )
{
	bool pointChange = false;

	*pathComplete = false;

	if ( !this->waypoint )
	{
		return( true );
	}

	if ( !currentWaypoint )
	{
		// if no currentWaypoint set, start it off

		currentWaypoint = this->waypoint;
	}

	*distX = currentWaypoint->pos.vx - currentPos.vx;
	*distY = currentWaypoint->pos.vy - currentPos.vy;

	if ( abs( *distX ) < waypointDist )
	{
		pointChange = true;
		*pathComplete = incPath();
	}

	*distX = currentWaypoint->pos.vx - currentPos.vx;
	*distY = currentWaypoint->pos.vy - currentPos.vy;

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

bool CNpcPath::thinkVertical( DVECTOR currentPos, bool *pathComplete, s32 *distX, s32 *distY, s32 *heading )
{
	bool pointChange = false;

	*pathComplete = false;

	if ( !this->waypoint )
	{
		return( true );
	}

	if ( !currentWaypoint )
	{
		// if no currentWaypoint set, start it off

		currentWaypoint = this->waypoint;
	}

	*distX = currentWaypoint->pos.vx - currentPos.vx;
	*distY = currentWaypoint->pos.vy - currentPos.vy;

	if ( abs( *distY ) < 10 )
	{
		pointChange = true;
		*pathComplete = incPath();
	}

	*distX = currentWaypoint->pos.vx - currentPos.vx;
	*distY = currentWaypoint->pos.vy - currentPos.vy;

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

CNpcWaypoint *CNpcPath::getWaypointList()
{
	return( waypoint );
}

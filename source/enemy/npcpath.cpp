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
	waypointCount = 0;
	reversePath = false;
}

void CNpcPath::resetPath()
{
	currentWaypoint = NULL;
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
	}
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

void CNpcPath::setPathType( NPC_PATH_TYPE newPathType )
{
	pathType = newPathType;
}

bool CNpcPath::incPath()
{
	if ( !reversePath )
	{
		if ( currentWaypoint->nextWaypoint )
		{
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

					currentWaypoint = this->waypoint;

					break;

				case PONG_PATH:
					// reverse path

					reversePath = !reversePath;

					if ( currentWaypoint->prevWaypoint )
					{
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
			currentWaypoint = currentWaypoint->prevWaypoint;
		}
		else
		{
			reversePath = !reversePath;

			if ( currentWaypoint->nextWaypoint )
			{
				currentWaypoint = currentWaypoint->nextWaypoint;
			}
		}
	}

	return( false );
}

bool CNpcPath::getDistToNextWaypoint( DVECTOR currentPos, s32 *distX, s32 *distY )
{
	return( currentWaypoint->isPointNear( currentPos, distX, distY ) );
}

s32 CNpcPath::think( DVECTOR currentPos, bool *pathComplete, bool *waypointChange )
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

	s32 xDist, yDist;

	*pathComplete = false;
	*waypointChange = false;

	if ( currentWaypoint->isPointNear( currentPos, &xDist, &yDist ) )
	{
		*pathComplete = incPath();
		*waypointChange = true;
	}

	s32 headingToTarget = ratan2( yDist, xDist );

	return( headingToTarget );
}
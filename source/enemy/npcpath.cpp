/*=========================================================================

	npcpath.cpp

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/

#ifndef __ENEMY_NCPPATH_H__
#include "enemy\ncppath.h"
#endif

bool CNpcWaypoint::isPointNear( DVECTOR testPos )
{
	s32 xDistSqr, yDistSqr;

	xDistSqr = testPos.vx - this->pos.vx;
	xDistSqr *= xDistSqr;

	yDistSqr = testPos.vy - this->pos.vy;
	yDistSqr *= yDistSqr;

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
	int loop;

	for ( loop = 0 ; loop < NPC_MAX_WAYPOINTS ; loop++ )
	{
		waypoint[loop].pos.vx = 0;
		waypoint[loop].pox.vy = 0;
	}

	pathType = SINGLE_USE_PATH;
	currentWaypoint = 0;
	waypointCount = 0;
	reversePath = false;
}

void CNpcPath::addWaypoint( DVECTOR newPos )
{
	if ( waypointCount < NPC_MAX_WAYPOINTS )
	{
		waypoint[waypointCount] = newPos;
		waypointCount++;
	}
}

void CNpcPath::setPathType( NPC_PATH_TYPE newPathType )
{
	pathType = newPathType;
}

bool CNpcPath::incPath()
{
	if ( !reversePath )
	{
		if ( currentWaypoint < waypointCount )
		{
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

					currentWaypoint = 0;

					break;

				case PONG_PATH:
					// reverse path

					reversePath = !reversePath;
					currentWaypoint--;

					break;
			}
		}
	}
	else
	{
		// must be pong path if reversed

		if ( currentWaypoint > 0 )
		{
			currentWaypoint--;
		}
		else
		{
			reversePath = !reversePath;
			currentWaypoint++;
		}
	}

	return( false );
}

void CNpcPath::think( DVECTOR currentPos )
{
	CNpcWaypoint *currentWaypoint;

	currentWaypoint = &waypoint[currentWaypoint]

	if ( currentWaypoint->isPointNear( currentPos ) )
	{
		incPath();
	}
}
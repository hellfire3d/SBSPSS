/*=========================================================================

	npcpath.h

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/

#ifndef __ENEMY_NPCPATH_H__
#define __ENEMY_NPCPATH_H__

#include "system\global.h"

class CNpcWaypoint
{
public:
	DVECTOR	pos;
	CNpcWaypoint *nextWaypoint;
	CNpcWaypoint *prevWaypoint;

	bool	isPointNear( DVECTOR testPos, s32 *xDist, s32 *yDist );
};

class CNpcPath
{
public:
	enum NPC_PATH_TYPE
	{
		SINGLE_USE_PATH = 0,
		REPEATING_PATH = 1,
		PONG_PATH = 2,
	};

	void			initPath();
	void			addWaypoint( DVECTOR newPos );
	void			removeAllWaypoints();
	void			setPathType( u8 newPathType );
	u8				getPathType();
	bool			incPath();
	void			resetPath();
	void			reversePathDir();
	s32				think( DVECTOR currentPos, bool *pathComplete, bool *waypointChange, s32 *distX, s32 *distY );
	bool			thinkFlat( DVECTOR currentPos, bool *pathComplete, s32 *distX, s32 *distY, s32 *heading );
	bool			thinkVertical( DVECTOR currentPos, bool *pathComplete, s32 *distX, s32 *distY, s32 *heading );
	bool			getDistToNextWaypoint( DVECTOR currentPos, s32 *distX, s32 *distY );
	void			getPathXExtents( s32 *minExtent, s32 *maxExtent );
	void			getPathYExtents( s32 *minExtent, s32 *maxExtent );
	CNpcWaypoint	*getWaypointList();

private:
	CNpcWaypoint	*waypoint;
	NPC_PATH_TYPE	pathType;
	u8				waypointCount;
	bool			reversePath;
	CNpcWaypoint	*currentWaypoint;
	CNpcWaypoint	*lastWaypoint;
	s32				minX, maxX;
	s32				minY, maxY;
};

#endif
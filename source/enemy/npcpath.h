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

enum NPC_PATH_TYPE
{
	SINGLE_USE_PATH = 0,
	REPEATING_PATH = 1,
	PONG_PATH = 2,
};

class CNpcPath
{
private:
	CNpcWaypoint	*waypoint;
	NPC_PATH_TYPE	pathType;
	u8				waypointCount;
	bool			reversePath;
	CNpcWaypoint	*currentWaypoint;
	CNpcWaypoint	*lastWaypoint;

public:
	void			initPath();
	void			addWaypoint( DVECTOR newPos );
	void			removeAllWaypoints();
	void			setPathType( NPC_PATH_TYPE newPathType );
	bool			incPath();
	void			resetPath();
	void			reversePathDir();
	s32				think( DVECTOR currentPos, bool *pathComplete, bool *waypointChange );
	bool			think2D( DVECTOR currentPos, s32 *distX, s32 *distY, s32 *heading );
	bool			getDistToNextWaypoint( DVECTOR currentPos, s32 *distX, s32 *distY );
};

#endif
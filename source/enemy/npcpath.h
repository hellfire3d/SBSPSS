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
	void			setPathType( u8 newPathType );
	u8				getPathType();
	bool			incPath();
	bool			decPath();
	void			resetPath();
	void			reversePathDir();
	s32				think( DVECTOR const &currentPos, bool *pathComplete, bool *waypointChange, s32 *distX, s32 *distY );
	bool			thinkFlat( DVECTOR const &currentPos, bool *pathComplete, s32 *distX, s32 *distY, s32 *heading, u8 waypointDist = 10 );
	bool			thinkVertical( DVECTOR const &currentPos, bool *pathComplete, s32 *distX, s32 *distY, s32 *heading );
	bool			getDistToNextWaypoint( DVECTOR const &currentPos, s32 *distX, s32 *distY );
	void			getPathXExtents( s32 *minExtent, s32 *maxExtent );
	void			getPathYExtents( s32 *minExtent, s32 *maxExtent );
	u8				getWaypointCount()						{ return( waypointCount ); }
	void			setWaypointCount( u8 newCount )			{ waypointCount = newCount; }
	void			setWaypointPtr( u16 *newPtr );
	void			setPathExtents();
	u16				*getWaypointPtr()						{ return( waypointPtr ); }
	bool			isPointNear( DVECTOR const &testPos, s32 *xDist, s32 *yDist );
	void			getCurrentWaypointPos( DVECTOR *waypointPos );

private:
	NPC_PATH_TYPE	pathType;
	u8				waypointCount;
	bool			reversePath;
	u8				currentWaypoint;
	u8				lastWaypoint;
	s32				minX, maxX;
	s32				minY, maxY;
	bool			decLockout;

	u16				*waypointPtr;
};

#endif
/*=========================================================================

	npcpath.cpp

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/

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

void CNpcPath::addWaypoint( DVECTOR newPos )
{
	if ( waypointCount < NPC_MAX_WAYPOINTS )
	{
		waypoint[waypointCount] = newPos;
		waypointCount++;
	}
}

	void			setPathType( NPC_PATH_TYPE newPathType );
	bool			incPath();
bool CNpcPath::incPath
/*=========================================================================

	hbbarrel.h

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __HAZARD_HBBARREL_H__
#include "hazard\hbbarrel.h"
#endif

#ifndef __LAYER_COLLISION_H__
#include "level\layercollision.h"
#endif

#ifndef __VID_HEADER_
#include "system\vid.h"
#endif

#ifndef __LEVEL_LEVEL_H__
#include "level\level.h"
#endif


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcBouncingBarrelHazard::init()
{
	CNpcHazard::init();

	m_lastWaypoint = Pos;

	m_rotation = 0;
	m_rockRotation = 0;
	m_rockDir = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcBouncingBarrelHazard::processMovement( int _frames )
{
	s32 moveX = 0, moveY = 0;
	s32 moveVel = 0;
	s32 moveDist = 0;
	s32 waypointXDist;
	s32 waypointYDist;
	s32 waypointHeading;
	s32 groundHeight;

	// deal with horizontal

	bool pathComplete;
	
	if ( m_npcPath.thinkFlat( Pos, &pathComplete, &waypointXDist, &waypointYDist, &waypointHeading, 1 ) )
	{
		if ( pathComplete )
		{
			Pos = m_base;
			m_npcPath.resetPath();
		}

		m_lastWaypoint = Pos;
	}

	moveX = 4 * _frames;

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

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcBouncingBarrelHazard::render()
{
	CHazardThing::render();

	// Render
	DVECTOR renderPos;
	DVECTOR	offset = CLevel::getCameraPos();

	renderPos.vx = Pos.vx - offset.vx;
	renderPos.vy = Pos.vy - offset.vy;

	CRECT collisionRect = getCollisionArea();
	collisionRect.x1 -= Pos.vx;
	collisionRect.x2 -= Pos.vx;
	collisionRect.y1 -= Pos.vy;
	collisionRect.y2 -= Pos.vy;

	if ( renderPos.vx + collisionRect.x2 >= 0 && renderPos.vx + collisionRect.x1 <= VidGetScrW() )
	{
		if ( renderPos.vy + collisionRect.y2 >= 0 && renderPos.vy + collisionRect.y1 <= VidGetScrH() )
		{
			SVECTOR rotation;
			rotation.vx = m_rockRotation;
			rotation.vy = 0;
			rotation.vz = m_rotation;

			VECTOR scale;
			scale.vx = ONE;
			scale.vy = ONE;
			scale.vz = ONE;

			m_modelGfx->Render(renderPos,&rotation,&scale);
		}
	}
}

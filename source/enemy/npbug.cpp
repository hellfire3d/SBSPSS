/*=========================================================================

	npbug.cpp

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __ENEMY_NPBUG_H__
#include "enemy\npbug.h"
#endif

#ifndef __LAYER_COLLISION_H__
#include	"level\layercollision.h"
#endif

#ifndef __VID_HEADER_
#include "system\vid.h"
#endif

#include	"game/game.h"


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcPricklyBugEnemy::processMovementModifier( int _frames, s32 distX, s32 distY, s32 dist, s16 headingChange )
{
	Pos.vx += distX;
	Pos.vy += distY;

	// sort out draw rotation

	DVECTOR testPos1, testPos2;

	testPos1 = testPos2 = Pos;
	testPos1.vx -= 10;
	testPos2.vx += 10;

	int groundDist = CGameScene::getCollision()->getHeightFromGround( testPos1.vx, testPos1.vy, 16 );

	if ( abs( groundDist ) > 15 )
	{
		m_drawRotation = 0;
		return;
	}

	testPos1.vy += groundDist;

	groundDist = CGameScene::getCollision()->getHeightFromGround( testPos2.vx, testPos2.vy, 16 );

	if ( abs( groundDist ) > 15 )
	{
		m_drawRotation = 0;
		return;
	}

	testPos2.vy += groundDist;

	s32 xDist = testPos2.vx - testPos1.vx;
	s32 yDist = testPos2.vy - testPos1.vy;

	s16 heading = ratan2( yDist, xDist ) & 4095;

	m_drawRotation = heading;
}

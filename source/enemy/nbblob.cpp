/*=========================================================================

	nbblob.cpp

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __ENEMY_NPC_H__
#include "enemy\npc.h"
#endif

#ifndef __ENEMY_NBBLOB_H__
#include "enemy\nbblob.h"
#endif

#ifndef __GAME_GAME_H__
#include	"game\game.h"
#endif

#ifndef __ANIM_BALLBLOB_HEADER__
#include <ACTOR_BALLBLOB_ANIM.h>
#endif

void CNpcBallBlobEnemy::postInit()
{
	m_heading = m_fireHeading = 128;

	m_npcPath.setPathType( CNpcPath::PONG_PATH );
}

void CNpcBallBlobEnemy::processMovement( int _frames )
{
	s32 moveX = 0, moveY = 0;
	s32 moveVel = 0;
	s32 moveDist = 0;
	s32 waypointXDist;
	s32 waypointYDist;
	s32 waypointHeading;

	if ( !m_animPlaying && m_frame != 0 )
	{
		m_animNo = ANIM_BALLBLOB_WOBBLE;
		m_frame = 0;
	}
	else if ( m_animNo == ANIM_BALLBLOB_BOUNCE )
	{
		moveX = 0;
		moveY = 0;

		processMovementModifier( _frames, moveX, moveY, moveVel, moveDist );

		return;
	}

	// deal with horizontal

	bool pathComplete;
	
	if ( m_npcPath.thinkFlat( Pos, &pathComplete, &waypointXDist, &waypointYDist, &waypointHeading ) )
	{
		// increment waypoint

		if ( waypointHeading == 0 )
		{
			// head right

			if ( m_heading >= 1024 && m_heading <= 3072 )
			{
				// currently heading in the wrong direction, reverse horizontal

				s32 headingDiff = m_heading - 2048;

				headingDiff = -headingDiff;

				headingDiff += 4096;

				m_heading = headingDiff % 4096;

				m_animPlaying = true;
				m_animNo = ANIM_BALLBLOB_WOBBLE;
				m_frame = 0;
			}
		}
		else
		{
			// head left

			if ( m_heading < 1024 || m_heading > 3072 )
			{
				// currently heading in the wrong direction, reverse horizontal

				s32 headingDiff = m_heading;

				headingDiff = -headingDiff;

				headingDiff += 6144;

				m_heading = headingDiff % 4096;

				m_animPlaying = true;
				m_animNo = ANIM_BALLBLOB_WOBBLE;
				m_frame = 0;
			}
		}
	}

	s32 preShiftX = _frames * m_data[m_type].speed * rcos( m_heading );
	s32 preShiftY = _frames * m_data[m_type].speed * rsin( m_heading );

	moveX = preShiftX >> 12;
	if ( !moveX && preShiftX )
	{
		moveX = preShiftX / abs( preShiftX );
	}

	moveY = preShiftY >> 12;
	if ( !moveY && preShiftY )
	{
		moveY = preShiftY / abs( preShiftY );
	}

	// deal with vertical

	if ( Pos.vy < 0 || m_layerCollision->Get( ( Pos.vx + moveX ) >> 4, ( Pos.vy + moveY ) >> 4 ) )
	{
		// reflect heading in vertical

		m_heading = 4096 - m_heading;

		m_animPlaying = true;
		m_animNo = ANIM_BALLBLOB_BOUNCE;
		m_frame = 0;

		// get new movement values

		preShiftX = _frames * m_data[m_type].speed * rcos( m_heading );
		preShiftY = _frames * m_data[m_type].speed * rsin( m_heading );

		moveX = preShiftX >> 12;
		if ( !moveX && preShiftX )
		{
			moveX = preShiftX / abs( preShiftX );
		}

		moveY = preShiftY >> 12;
		if ( !moveY && preShiftY )
		{
			moveY = preShiftY / abs( preShiftY );
		}
	}

	processMovementModifier( _frames, moveX, moveY, moveVel, moveDist );
}
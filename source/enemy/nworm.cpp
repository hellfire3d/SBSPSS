/*=========================================================================

	nworm.cpp

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __ENEMY_NPC_H__
#include "enemy\npc.h"
#endif

//#ifndef __GAME_GAME_H__
//#include "game\game.h"
//#endif

#ifndef	__PLAYER_PLAYER_H__
#include	"player\player.h"
#endif

void CNpcEnemy::processParasiticWormMovement( int _frames )
{
	s32 moveX = 0, moveY = 0;
	s32 moveVel = 0;
	s32 moveDist = 0;
	DVECTOR oldPos = Pos;
	u8 skipCounter;

	processGenericFixedPathMove( _frames, &moveX, &moveY, &moveVel, &moveDist );

	Pos.vx += moveX;
	Pos.vy += moveY;

	m_extension += 256;
	m_extension &= 4095;

	m_positionHistory = m_positionHistory->prev;
	m_positionHistory->pos = oldPos;

	CNpcPositionHistory *newPos;
	newPos = m_positionHistory;
	for ( skipCounter = 1 ; skipCounter < NPC_PARASITIC_WORM_SPACING ; skipCounter++ )
	{
		newPos = newPos->next;
	}

	CThing	*List=Next;

	oldPos = Pos;

	s32 extension = m_extension;
	u8 downShift = 2;

	while( List )
	{
		CNpcEnemy *segment = (CNpcEnemy *) List;

		s32 xDist = oldPos.vx - newPos->pos.vx;
		s32 yDist = oldPos.vy - newPos->pos.vy;

		s16 headingToTarget = ratan2( yDist, xDist );

		segment->setHeading( headingToTarget );

		DVECTOR sinPos;

		sinPos = newPos->pos;
		s32 diff = ( ( 10 >> downShift ) * rsin( extension ) ) >> 12;
		sinPos.vx += ( diff * rcos( headingToTarget + 1024 ) ) >> 12;
		sinPos.vy += ( diff * rsin( headingToTarget + 1024 ) ) >> 12;

		List->setPos( sinPos );
		oldPos = newPos->pos;

		List = List->getNext();

		if ( List )
		{
			for ( skipCounter = 0 ; skipCounter < NPC_PARASITIC_WORM_SPACING ; skipCounter++ )
			{
				newPos = newPos->next;
			}
		}

		extension += 1024;
		extension &= 4095;

		if ( downShift > 0 )
		{
			downShift--;
		}
	}

	/*// add new (old) position onto list head
	CNpcPositionHistory *newPos;
	newPos = new ("position history") CNpcPositionHistory;
	newPos->pos = oldPos;
	newPos->next = m_positionHistory;
	m_positionHistory = newPos;

	// remove list end
	CNpcPositionHistory *last;
	last = newPos;

	while( newPos->next )
	{
		last = newPos;
		newPos = newPos->next;
	}

	delete newPos;
	last->next = NULL;

	// assign positions
	newPos = m_positionHistory;
	for ( skipCounter = 1 ; skipCounter < NPC_PARASITIC_WORM_SPACING ; skipCounter++ )
	{
		newPos = newPos->next;
	}

	CThing	*List=Next;

	oldPos = Pos;

	s32 extension = m_extension;
	u8 downShift = 2;

	while( List )
	{
		CNpcEnemy *segment = (CNpcEnemy *) List;

		s32 xDist = oldPos.vx - newPos->pos.vx;
		s32 yDist = oldPos.vy - newPos->pos.vy;

		s16 headingToTarget = ratan2( yDist, xDist );

		segment->setHeading( headingToTarget );

		DVECTOR sinPos;

		sinPos = newPos->pos;
		s32 diff = ( ( 10 >> downShift ) * rsin( extension ) ) >> 12;
		sinPos.vx += ( diff * rcos( headingToTarget + 1024 ) ) >> 12;
		sinPos.vy += ( diff * rsin( headingToTarget + 1024 ) ) >> 12;

		List->setPos( sinPos );
		oldPos = newPos->pos;

		List = List->getNext();

		if ( List )
		{
			for ( skipCounter = 0 ; skipCounter < NPC_PARASITIC_WORM_SPACING ; skipCounter++ )
			{
				newPos = newPos->next;
			}
		}

		extension += 1024;
		extension &= 4095;

		if ( downShift > 0 )
		{
			downShift--;
		}
	}*/
}
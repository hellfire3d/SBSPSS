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

#ifndef	__ENEMY_NWORM_H__
#include "enemy\nworm.h"
#endif

#ifndef __GAME_GAME_H__
#include "game\game.h"
#endif

#ifndef	__PLAYER_PLAYER_H__
#include	"player\player.h"
#endif

void CNpcParasiticWormEnemy::postInit()
{
	DVECTOR newPos;

	newPos.vx = 100;
	//newPos.vy = 10;
	newPos.vy = 100;

	m_npcPath.addWaypoint( newPos );

	newPos.vx = 500;
	//newPos.vy = 10;
	newPos.vy = 100;

	m_npcPath.addWaypoint( newPos );

	newPos.vx = 500;
	//newPos.vy = 100;
	newPos.vy = 300;

	m_npcPath.addWaypoint( newPos );

	newPos.vx = 100;
	//newPos.vy = 100;
	newPos.vy = 300;

	m_npcPath.addWaypoint( newPos );

	m_npcPath.setPathType( CNpcPath::PONG_PATH );

	// create start of list
	CNpcPositionHistory *newPosition;
	newPosition = new ("position history") CNpcPositionHistory;
	newPosition->pos = Pos;
	m_positionHistory = newPosition;

	CNpcPositionHistory *currentPosition = m_positionHistory;

	// create rest of list

	for ( int histLength = 1 ; histLength < ( NPC_PARASITIC_WORM_LENGTH * NPC_PARASITIC_WORM_SPACING ) ; histLength++ )
	{
		newPosition = new ("position history") CNpcPositionHistory;
		newPosition->pos = Pos;
		newPosition->next = NULL;
		newPosition->prev = currentPosition;

		currentPosition->next = newPosition;
		currentPosition = newPosition;
	}

	// link ends together for circular list

	currentPosition->next = m_positionHistory;
	m_positionHistory->prev = currentPosition;

	for ( int segCount = 0 ; segCount < NPC_PARASITIC_WORM_LENGTH ; segCount++ )
	{
		CNpcEnemy *segment;
		segment = new ("segment") CNpcParasiticWormSegment;
		segment->setType( CNpcEnemy::NPC_PARASITIC_WORM_SEGMENT );
		segment->init();
		segment->setLayerCollision( m_layerCollision );
		segment->postInit();

		this->addChild( segment );
	}

	m_movementTimer = 2 * GameState::getOneSecondInFrames();
}

void CNpcParasiticWormSegment::postInit()
{
}

bool CNpcParasiticWormEnemy::processSensor()
{
	if ( playerXDistSqr + playerYDistSqr < 40000 )
	{
		m_controlFunc = NPC_CONTROL_CLOSE;

		return( true );
	}
	else
	{
		return( false );
	}
}

void CNpcParasiticWormEnemy::processMovement( int _frames )
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
	u8 timeShift;

	if ( m_movementTimer > 0 )
	{
		m_movementTimer -= _frames;

		if ( m_movementTimer < 0 )
		{
			m_movementTimer = 0;
		}
	}

	timeShift = m_movementTimer / GameState::getOneSecondInFrames();

	while( List )
	{
		CNpcEnemy *segment = (CNpcEnemy *) List;

		s32 xDist = oldPos.vx - newPos->pos.vx;
		s32 yDist = oldPos.vy - newPos->pos.vy;

		s16 headingToTarget = ratan2( yDist, xDist );

		segment->setHeading( headingToTarget );

		DVECTOR sinPos;

		sinPos = newPos->pos;
		s32 diff = ( ( ( 10 >> downShift ) * rsin( extension ) ) >> 12 ) >> timeShift;
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
}

void CNpcParasiticWormEnemy::resetParasiticWormHeadToTail()
{
	DVECTOR startPos;
	DVECTOR endPos;
	int posCounter;
	CNpcPositionHistory *currentPos;

	startPos = Pos;

	currentPos = m_positionHistory;

	for ( posCounter = 0 ; posCounter < ( NPC_PARASITIC_WORM_LENGTH * NPC_PARASITIC_WORM_SPACING ) - 1 ; posCounter++ )
	{
		currentPos = currentPos->next;
	}

	endPos = currentPos->pos;

	currentPos = m_positionHistory;

	for ( posCounter = 0 ; posCounter < NPC_PARASITIC_WORM_LENGTH * NPC_PARASITIC_WORM_SPACING ; posCounter++ )
	{
		currentPos->pos.vx = startPos.vx + ( posCounter * ( endPos.vx - startPos.vx ) ) / ( ( NPC_PARASITIC_WORM_LENGTH * NPC_PARASITIC_WORM_SPACING ) - 1 );
		currentPos->pos.vy = startPos.vy + ( posCounter * ( endPos.vy - startPos.vy ) ) / ( ( NPC_PARASITIC_WORM_LENGTH * NPC_PARASITIC_WORM_SPACING ) - 1 );

		currentPos = currentPos->next;
	}

	CNpcPositionHistory *newPos;
	newPos = m_positionHistory;

	u8 skipCounter;
	for ( skipCounter = 1 ; skipCounter < NPC_PARASITIC_WORM_SPACING ; skipCounter++ )
	{
		newPos = newPos->next;
	}

	CThing	*List=Next;

	DVECTOR oldPos = Pos;

	s32 extension = m_extension;

	while( List )
	{
		CNpcEnemy *segment = (CNpcEnemy *) List;

		s32 xDist = oldPos.vx - newPos->pos.vx;
		s32 yDist = oldPos.vy - newPos->pos.vy;

		s16 headingToTarget = ratan2( yDist, xDist );

		segment->setHeading( headingToTarget );

		List->setPos( newPos->pos );
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
	}
}

void CNpcParasiticWormEnemy::processClose( int _frames )
{
	resetParasiticWormHeadToTail();

	m_movementTimer = 2 * GameState::getOneSecondInFrames();

	m_controlFunc = NPC_CONTROL_MOVEMENT;
	m_timerFunc = NPC_TIMER_ATTACK_DONE;
	m_timerTimer = GameState::getOneSecondInFrames();
	m_sensorFunc = NPC_SENSOR_NONE;
}


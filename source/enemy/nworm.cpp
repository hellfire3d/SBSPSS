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

#ifndef __VID_HEADER_
#include "system\vid.h"
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcParasiticWormEnemy::postInit()
{
	m_npcPath.setPathType( CNpcPath::REPEATING_PATH );

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

	u16 segScale;
	int initLength = NPC_PARASITIC_WORM_LENGTH / 3;
	int remLength = NPC_PARASITIC_WORM_LENGTH - initLength;

	for ( int segCount = 0 ; segCount < NPC_PARASITIC_WORM_LENGTH ; segCount++ )
	{
		CNpcEnemy *segment;
		CNpcParasiticWormSegment *wormSegment;
		wormSegment = new ("segment") CNpcParasiticWormSegment;
		wormSegment->setType( CNpcEnemy::NPC_PARASITIC_WORM_SEGMENT );
		wormSegment->init();
		wormSegment->setLayerCollision( m_layerCollision );
		wormSegment->postInit();

		if ( segCount < initLength )
		{
			u16 sum = ONE;
			u16 start = ONE >> 1;
			u16 end = sum - start;

			segScale = start + ( ( end * segCount ) / initLength );
		}
		else
		{
			u16 sum = ONE;
			u16 start = ONE >> 3;
			u16 end = sum - start;

			segScale = start + ( ( end * ( NPC_PARASITIC_WORM_LENGTH - segCount ) ) / remLength );
		}

		wormSegment->setScale( segScale );
		segment = wormSegment;

		this->addChild( segment );
	}

	m_movementTimer = 2 * GameState::getOneSecondInFrames();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcParasiticWormEnemy::shutdown()
{
	deleteAllChild();
	CNpcEnemy::shutdown();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcParasiticWormSegment::think( int _frames )
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcParasiticWormSegment::postInit()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool CNpcParasiticWormEnemy::processSensor()
{
	/*if ( playerXDistSqr + playerYDistSqr < 40000 )
	{
		m_controlFunc = NPC_CONTROL_CLOSE;

		return( true );
	}
	else
	{
		return( false );
	}*/

	return( false );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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

		//segment->setHeading( headingToTarget );

		DVECTOR sinPos;

		sinPos = newPos->pos;
		s32 diff = ( ( ( 5 >> downShift ) * rsin( extension ) ) >> 12 ) >> timeShift;
		sinPos.vx += ( diff * rcos( headingToTarget + 1024 ) ) >> 12;
		sinPos.vy += ( diff * rsin( headingToTarget + 1024 ) ) >> 12;

		List->setPos( sinPos );
		oldPos = sinPos;

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

	List=Next;

	oldPos = Pos;

	while( List )
	{
		CNpcEnemy *segment = (CNpcEnemy *) List;
		DVECTOR currentPos = segment->getPos();

		s32 xDist = oldPos.vx - currentPos.vx;
		s32 yDist = oldPos.vy - currentPos.vy;

		s16 headingToPrev = ratan2( yDist, xDist );
		s16 headingFromNext;
		s16 heading = headingToPrev;

		oldPos = currentPos;

		List = List->getNext();

		if ( List )
		{
			DVECTOR nextPos = List->getPos();
			xDist = currentPos.vx - nextPos.vx;
			yDist = currentPos.vy - nextPos.vy;
			headingFromNext = ratan2( yDist, xDist );

			s16 decDir, incDir, moveDist;

			decDir = headingFromNext - headingToPrev;

			if ( decDir < 0 )
			{
				decDir += ONE;
			}

			incDir = headingToPrev - headingFromNext;

			if ( incDir < 0 )
			{
				incDir += ONE;
			}

			if ( decDir < incDir )
			{
				moveDist = -decDir;
			}
			else
			{
				moveDist = incDir;
			}

			heading -= moveDist >> 1;
		}

		segment->setHeading( heading );
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcParasiticWormEnemy::processClose( int _frames )
{
	resetParasiticWormHeadToTail();

	m_movementTimer = 2 * GameState::getOneSecondInFrames();

	m_controlFunc = NPC_CONTROL_MOVEMENT;
	m_timerFunc = NPC_TIMER_ATTACK_DONE;
	m_timerTimer = GameState::getOneSecondInFrames();
	m_sensorFunc = NPC_SENSOR_NONE;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcParasiticWormEnemy::processEnemyCollision( CThing *thisThing )
{
	// do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcParasiticWormSegment::processEnemyCollision( CThing *thisThing )
{
	// do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcParasiticWormEnemy::render()
{
	SprFrame = NULL;

	if ( m_isActive )
	{
		CEnemyThing::render();

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
				SprFrame = m_actorGfx->Render(renderPos,m_animNo,( m_frame >> 8 ),m_reversed);
				m_actorGfx->RotateScale( SprFrame, renderPos, m_heading, 4096, 4096 );

				sBBox boundingBox = m_actorGfx->GetBBox();
				setCollisionSize( ( boundingBox.XMax - boundingBox.XMin ), ( boundingBox.YMax - boundingBox.YMin ) );
				setCollisionCentreOffset( ( boundingBox.XMax + boundingBox.XMin ) >> 1, ( boundingBox.YMax + boundingBox.YMin ) >> 1 );
			}
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcParasiticWormSegment::render()
{
	SprFrame = NULL;

	if ( m_isActive )
	{
		CEnemyThing::render();

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
				SprFrame = m_actorGfx->Render(renderPos,m_animNo,( m_frame >> 8 ),m_reversed);
				m_actorGfx->RotateScale( SprFrame, renderPos, m_heading, 4096, m_scale );

				sBBox boundingBox = m_actorGfx->GetBBox();
				setCollisionSize( ( boundingBox.XMax - boundingBox.XMin ), ( boundingBox.YMax - boundingBox.YMin ) );
				setCollisionCentreOffset( ( boundingBox.XMax + boundingBox.XMin ) >> 1, ( boundingBox.YMax + boundingBox.YMin ) >> 1 );
			}
		}
	}
}

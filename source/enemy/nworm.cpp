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

#ifdef	SHOW_BBOX
#include "gfx\prim.h"
#endif

#ifndef	__ANIM_GIANTWORM_HEADER__
#include <ACTOR_GIANTWORM_ANIM.h>
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcParasiticWormSegment::init()
{
	m_actorGfx=CActorPool::GetActor( (FileEquate) ACTORS_GIANTWORM_SBK );

	m_heading = 0;
	m_nextSegment = NULL;

	setCollisionSize( 20, 20 );
	setCollisionCentreOffset( 10, 10 );
	updateCollisionArea();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcParasiticWormSegment::updateCollisionArea()
{
	m_collisionCentre.vx=Pos.vx+m_collisionCentreOffset.vx;
	m_collisionCentre.vy=Pos.vy+m_collisionCentreOffset.vy;
	m_collisionArea.x1=m_collisionCentre.vx-(m_collisionSize.vx/2);
	m_collisionArea.x2=m_collisionArea.x1+m_collisionSize.vx;
	m_collisionArea.y1=m_collisionCentre.vy-(m_collisionSize.vy/2);
	m_collisionArea.y2=m_collisionArea.y1+m_collisionSize.vy;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcParasiticWormSegment::setCollisionSize(int _w,int _h)
{
	m_collisionSize.vx=_w;
	m_collisionSize.vy=_h;
	if(m_collisionSize.vx>m_collisionSize.vy)
	{
		m_collisionRadius=m_collisionSize.vx;
	}
	else
	{
		m_collisionRadius=m_collisionSize.vy;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcParasiticWormEnemy::postInit()
{
	m_npcPath.setPathType( CNpcPath::REPEATING_PATH );

	s16 maxArraySize = NPC_PARASITIC_WORM_LENGTH * NPC_PARASITIC_WORM_SPACING;

	m_positionHistoryArray[0].pos = Pos;
	m_positionHistoryArray[0].next = &m_positionHistoryArray[1];
	m_positionHistoryArray[0].prev = &m_positionHistoryArray[maxArraySize - 1];

	for ( int histLength = 1 ; histLength < maxArraySize - 1 ; histLength++ )
	{
		m_positionHistoryArray[histLength].pos = Pos;
		m_positionHistoryArray[histLength].next = &m_positionHistoryArray[histLength + 1];
		m_positionHistoryArray[histLength].prev = &m_positionHistoryArray[histLength - 1];
	}

	m_positionHistoryArray[maxArraySize - 1].pos = Pos;
	m_positionHistoryArray[maxArraySize - 1].next = &m_positionHistoryArray[0];
	m_positionHistoryArray[maxArraySize - 1].prev = &m_positionHistoryArray[maxArraySize - 2];

	m_positionHistory = &m_positionHistoryArray[0];

	u16 segScale;
	int initLength = NPC_PARASITIC_WORM_LENGTH / 3;
	int remLength = NPC_PARASITIC_WORM_LENGTH - initLength;

	for ( int segCount = 0 ; segCount < NPC_PARASITIC_WORM_LENGTH ; segCount++ )
	{
		m_segmentArray[segCount].init();

		if ( segCount < initLength )
		{
			/*u16 sum = ONE;
			u16 start = ONE >> 1;
			u16 end = sum - start;

			segScale = start + ( ( end * segCount ) / initLength );*/
			segScale = ONE;
		}
		else
		{
			u16 sum = ONE;
			u16 start = ONE >> 3;
			u16 end = sum - start;

			segScale = start + ( ( end * ( NPC_PARASITIC_WORM_LENGTH - segCount ) ) / remLength );
		}

		m_segmentArray[segCount].setScale( segScale );

		if ( segCount == NPC_PARASITIC_WORM_LENGTH - 1 )
		{
			m_segmentArray[segCount].setAnim( ANIM_GIANTWORM_TAIL );
		}
		else
		{
			m_segmentArray[segCount].setAnim( ANIM_GIANTWORM_BODYSTATIC );
		}

		// attach snake segment

		if ( segCount < NPC_PARASITIC_WORM_LENGTH - 1 )
		{
			m_segmentArray[segCount].m_nextSegment = &m_segmentArray[segCount + 1];
		}
	}

	m_movementTimer = 2 * GameState::getOneSecondInFrames();
	m_collTimer = 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcParasiticWormSegment::shutdown()
{
	delete m_actorGfx;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcParasiticWormEnemy::shutdown()
{
	// delete worm segments

	for ( int segCount = 0 ; segCount < NPC_PARASITIC_WORM_LENGTH ; segCount++ )
	{
		m_segmentArray[segCount].shutdown();
	}

	CNpcEnemy::shutdown();
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

	int segmentCount;

	for ( segmentCount = 0 ; segmentCount < NPC_PARASITIC_WORM_LENGTH ; segmentCount++ )
	{
		s32 xDist = oldPos.vx - newPos->pos.vx;
		s32 yDist = oldPos.vy - newPos->pos.vy;

		s16 headingToTarget = ratan2( yDist, xDist ) & 4095;

		DVECTOR sinPos;

		sinPos = newPos->pos;
		s32 diff = ( ( ( 2 >> downShift ) * rsin( extension ) ) >> 12 ) >> timeShift;
		sinPos.vx += ( diff * rcos( headingToTarget + 1024 ) ) >> 12;
		sinPos.vy += ( diff * rsin( headingToTarget + 1024 ) ) >> 12;

		m_segmentArray[segmentCount].setPos( sinPos );
		oldPos = sinPos;

		for ( skipCounter = 0 ; skipCounter < NPC_PARASITIC_WORM_SPACING ; skipCounter++ )
		{
			newPos = newPos->next;
		}

		extension += 256;
		extension &= 4095;

		if ( downShift > 0 )
		{
			downShift--;
		}
	}

	oldPos = Pos;

	for ( segmentCount = 0 ; segmentCount < NPC_PARASITIC_WORM_LENGTH ; segmentCount++ )
	{
		DVECTOR const &currentPos = m_segmentArray[segmentCount].getPos();

		s32 xDist = oldPos.vx - currentPos.vx;
		s32 yDist = oldPos.vy - currentPos.vy;

		s16 headingToPrev = ratan2( yDist, xDist ) & 4095;
		s16 headingFromNext;
		s16 heading = headingToPrev;

		oldPos = currentPos;

		if ( segmentCount < NPC_PARASITIC_WORM_LENGTH - 1 )
		{
			DVECTOR const &nextPos = m_segmentArray[segmentCount + 1].getPos();
			xDist = currentPos.vx - nextPos.vx;
			yDist = currentPos.vy - nextPos.vy;
			headingFromNext = ratan2( yDist, xDist ) & 4095;

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

		m_segmentArray[segmentCount].setHeading( heading );
		m_segmentArray[segmentCount].updateCollisionArea();
	}

	if ( m_collTimer > 0 )
	{
		m_collTimer -= _frames;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*void CNpcParasiticWormEnemy::resetParasiticWormHeadToTail()
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
}*/

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcParasiticWormEnemy::processClose( int _frames )
{
	//resetParasiticWormHeadToTail();

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

		if (canRender())
		{
			DVECTOR &renderPos=getRenderPos();

			SprFrame = m_actorGfx->Render(renderPos,m_animNo,( m_frame >> 8 ),0);
			m_actorGfx->RotateScale( SprFrame, renderPos, m_heading, 4096, 4096 );

			sBBox boundingBox = m_actorGfx->GetBBox();
			setCollisionSize( ( boundingBox.XMax - boundingBox.XMin ), ( boundingBox.YMax - boundingBox.YMin ) );
			setCollisionCentreOffset( ( boundingBox.XMax + boundingBox.XMin ) >> 1, ( boundingBox.YMax + boundingBox.YMin ) >> 1 );
		}

		for ( int segmentCount = 0 ; segmentCount < NPC_PARASITIC_WORM_LENGTH ; segmentCount++ )
		{
			m_segmentArray[segmentCount].render();
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int CNpcParasiticWormEnemy::checkCollisionAgainst( CThing *_thisThing, int _frames )
{
//	DVECTOR	pos,thisThingPos;
	int		radius;
	int		collided;

DVECTOR const	&pos=getCollisionCentre();
DVECTOR const	&thisThingPos=_thisThing->getCollisionCentre();

	radius=getCollisionRadius()+_thisThing->getCollisionRadius();
	collided=false;
	if(abs(pos.vx-thisThingPos.vx)<radius&&
	   abs(pos.vy-thisThingPos.vy)<radius)
	{
//		CRECT	thisRect,thatRect;

CRECT const	&thisRect=getCollisionArea();
CRECT const	&thatRect=_thisThing->getCollisionArea();

		if(((thisRect.x1>=thatRect.x1&&thisRect.x1<=thatRect.x2)||(thisRect.x2>=thatRect.x1&&thisRect.x2<=thatRect.x2)||(thisRect.x1<=thatRect.x1&&thisRect.x2>=thatRect.x2))&&
		   ((thisRect.y1>=thatRect.y1&&thisRect.y1<=thatRect.y2)||(thisRect.y2>=thatRect.y1&&thisRect.y2<=thatRect.y2)||(thisRect.y1<=thatRect.y1&&thisRect.y2>=thatRect.y2)))
		{
			collided=true;
		}
	}

	// go through segments

	for ( int segmentCount = 0 ; segmentCount < NPC_PARASITIC_WORM_LENGTH ; segmentCount++ )
	{
		if ( m_segmentArray[segmentCount].checkCollisionAgainst( _thisThing, _frames ) )
		{
			collided = true;
		}
	}

	return collided;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcParasiticWormSegment::render()
{
	POLY_FT4 *SprFrame = NULL;

	sBBox			&ScrBBox=CThingManager::getRenderBBox();
	DVECTOR	const	&CamPos=CLevel::getCameraPos();

	DVECTOR renderPos;
	renderPos.vx = Pos.vx - CamPos.vx;
	renderPos.vy = Pos.vy - CamPos.vy;

	u8 renderFlag = true;
	if ( m_collisionArea.x2 < ScrBBox.XMin || m_collisionArea.x1 > ScrBBox.XMax ) renderFlag=false;
	if ( m_collisionArea.y2 < ScrBBox.YMin || m_collisionArea.y1 > ScrBBox.YMax ) renderFlag=false;

	if ( renderFlag )
	{
		SprFrame = m_actorGfx->Render(renderPos,m_animNo,0,0);
		m_actorGfx->RotateScale( SprFrame, renderPos, m_heading, 4096, m_scale );

		sBBox boundingBox = m_actorGfx->GetBBox();
		setCollisionSize( ( boundingBox.XMax - boundingBox.XMin ), ( boundingBox.YMax - boundingBox.YMin ) );
		setCollisionCentreOffset( ( boundingBox.XMax + boundingBox.XMin ) >> 1, ( boundingBox.YMax + boundingBox.YMin ) >> 1 );

#if defined (__USER_charles__)
		DVECTOR	const	&ofs=CLevel::getCameraPos();
		CRECT	area;

		area=getCollisionArea();
		area.x1-=ofs.vx;
		area.y1-=ofs.vy;
		area.x2-=ofs.vx;
		area.y2-=ofs.vy;

		if(area.x1<=511&&area.x2>=0 && area.y1<=255&&area.y2>=0)
		{
			DrawLine(area.x1,area.y1,area.x2,area.y1,255,255,255,0);
			DrawLine(area.x2,area.y1,area.x2,area.y2,255,255,255,0);
			DrawLine(area.x2,area.y2,area.x1,area.y2,255,255,255,0);
			DrawLine(area.x1,area.y2,area.x1,area.y1,255,255,255,0);

			area.x1=Pos.vx-10-ofs.vx;
			area.y1=Pos.vy-10-ofs.vy;
			area.x2=Pos.vx+10-ofs.vx;
			area.y2=Pos.vy+10-ofs.vy;
			DrawLine(area.x1,area.y1,area.x2,area.y2,255,0,0,0);
			DrawLine(area.x2,area.y1,area.x1,area.y2,255,0,0,0);
		}
#endif
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int CNpcParasiticWormSegment::checkCollisionAgainst( CThing *_thisThing, int _frames )
{
//	DVECTOR	pos,thisThingPos;
	int		radius;
	int		collided;

DVECTOR const	&pos = getCollisionCentre();
DVECTOR const	&thisThingPos = _thisThing->getCollisionCentre();

	radius = getCollisionRadius() + _thisThing->getCollisionRadius();
	collided = false;
	if(abs(pos.vx-thisThingPos.vx)<radius&&
	   abs(pos.vy-thisThingPos.vy)<radius)
	{
//		CRECT	thisRect,thatRect;

CRECT const	&thisRect=getCollisionArea();
CRECT const	&thatRect=_thisThing->getCollisionArea();

		if(((thisRect.x1>=thatRect.x1&&thisRect.x1<=thatRect.x2)||(thisRect.x2>=thatRect.x1&&thisRect.x2<=thatRect.x2)||(thisRect.x1<=thatRect.x1&&thisRect.x2>=thatRect.x2))&&
		   ((thisRect.y1>=thatRect.y1&&thisRect.y1<=thatRect.y2)||(thisRect.y2>=thatRect.y1&&thisRect.y2<=thatRect.y2)||(thisRect.y1<=thatRect.y1&&thisRect.y2>=thatRect.y2)))
		{
			collided=true;
		}
	}

	return collided;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcParasiticWormEnemy::processShotDeathEnd( int _frames )
{
	CNpcEnemy::processShotDeathEnd( _frames );

	// go through segments

	for ( int segCount = 0 ; segCount < NPC_PARASITIC_WORM_LENGTH ; segCount++ )
	{
		DVECTOR segPos = m_segmentArray[segCount].getPos();
		segPos.vy += m_speed * _frames;
		m_segmentArray[segCount].setPos( segPos );
		m_segmentArray[segCount].updateCollisionArea();
	}
}


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

#ifndef	__ANIM_SEASNAKE_HEADER__
#include <ACTOR_SEASNAKE_ANIM.h>
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcParasiticWormSegment::init()
{
	m_actorGfx=CActorPool::GetActor( (FileEquate) ACTORS_SEASNAKE_SBK );

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

	m_segment = NULL;

	CNpcParasiticWormSegment *currentSegment;

	for ( int segCount = 0 ; segCount < NPC_PARASITIC_WORM_LENGTH ; segCount++ )
	{
		CNpcParasiticWormSegment *wormSegment;
		wormSegment = new ("segment") CNpcParasiticWormSegment;
		wormSegment->init();
		//wormSegment->setLayerCollision( m_layerCollision );

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

		// attach worm segment

		if ( m_segment )
		{
			currentSegment = m_segment;

			while( currentSegment->m_nextSegment )
			{
				currentSegment = currentSegment->m_nextSegment;
			}

			currentSegment->m_nextSegment = wormSegment;
		}
		else
		{
			// no previous segments

			m_segment = wormSegment;
		}
	}

	m_movementTimer = 2 * GameState::getOneSecondInFrames();
	m_collTimer = 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcParasiticWormEnemy::shutdown()
{
	// delete worm segments

	CNpcParasiticWormSegment *currentSegment = m_segment;

	while( currentSegment )
	{
		CNpcParasiticWormSegment *oldSegment;

		oldSegment = currentSegment;
		currentSegment = currentSegment->m_nextSegment;
		delete oldSegment;
	}

	// remove position history

	CNpcPositionHistory *currentPosition;
	CNpcPositionHistory *oldPosition;

	currentPosition = m_positionHistory;

	while( currentPosition )
	{
		oldPosition = currentPosition;
		currentPosition = currentPosition->next;

		oldPosition->prev->next = NULL;
		delete oldPosition;
	}

	m_positionHistory = NULL;

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

	CNpcParasiticWormSegment *List = m_segment;

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
		s32 xDist = oldPos.vx - newPos->pos.vx;
		s32 yDist = oldPos.vy - newPos->pos.vy;

		s16 headingToTarget = ratan2( yDist, xDist );

		DVECTOR sinPos;

		sinPos = newPos->pos;
		s32 diff = ( ( ( 5 >> downShift ) * rsin( extension ) ) >> 12 ) >> timeShift;
		sinPos.vx += ( diff * rcos( headingToTarget + 1024 ) ) >> 12;
		sinPos.vy += ( diff * rsin( headingToTarget + 1024 ) ) >> 12;

		List->setPos( sinPos );
		oldPos = sinPos;

		List = List->m_nextSegment;

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

	List = m_segment;

	oldPos = Pos;

	while( List )
	{
		DVECTOR currentPos = List->getPos();

		s32 xDist = oldPos.vx - currentPos.vx;
		s32 yDist = oldPos.vy - currentPos.vy;

		s16 headingToPrev = ratan2( yDist, xDist );
		s16 headingFromNext;
		s16 heading = headingToPrev;

		oldPos = currentPos;

		CNpcParasiticWormSegment *oldList = List;

		List = List->m_nextSegment;

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

		oldList->setHeading( heading );
		oldList->updateCollisionArea();
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

			SprFrame = m_actorGfx->Render(renderPos,m_animNo,( m_frame >> 8 ),m_reversed);
			m_actorGfx->RotateScale( SprFrame, renderPos, m_heading, 4096, 4096 );

			sBBox boundingBox = m_actorGfx->GetBBox();
			setCollisionSize( ( boundingBox.XMax - boundingBox.XMin ), ( boundingBox.YMax - boundingBox.YMin ) );
			setCollisionCentreOffset( ( boundingBox.XMax + boundingBox.XMin ) >> 1, ( boundingBox.YMax + boundingBox.YMin ) >> 1 );
		}

		CNpcParasiticWormSegment *segment = m_segment;

		while( segment )
		{
			segment->render();
			segment = segment->m_nextSegment;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int CNpcParasiticWormEnemy::checkCollisionAgainst( CThing *_thisThing, int _frames )
{
	DVECTOR	pos,thisThingPos;
	int		radius;
	int		collided;

	pos=getCollisionCentre();
	thisThingPos=_thisThing->getCollisionCentre();

	radius=getCollisionRadius()+_thisThing->getCollisionRadius();
	collided=false;
	if(abs(pos.vx-thisThingPos.vx)<radius&&
	   abs(pos.vy-thisThingPos.vy)<radius)
	{
		CRECT	thisRect,thatRect;

		thisRect=getCollisionArea();

		thatRect=_thisThing->getCollisionArea();

		if(((thisRect.x1>=thatRect.x1&&thisRect.x1<=thatRect.x2)||(thisRect.x2>=thatRect.x1&&thisRect.x2<=thatRect.x2)||(thisRect.x1<=thatRect.x1&&thisRect.x2>=thatRect.x2))&&
		   ((thisRect.y1>=thatRect.y1&&thisRect.y1<=thatRect.y2)||(thisRect.y2>=thatRect.y1&&thisRect.y2<=thatRect.y2)||(thisRect.y1<=thatRect.y1&&thisRect.y2>=thatRect.y2)))
		{
			collided=true;
		}
	}

	// go through segments

	CNpcParasiticWormSegment *segment = m_segment;

	while( segment )
	{
		if ( segment->checkCollisionAgainst( _thisThing, _frames ) )
		{
			collided = true;
		}

		segment = segment->m_nextSegment;
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
		SprFrame = m_actorGfx->Render(renderPos,ANIM_SEASNAKE_BODYSTATIC,0,0);
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
	DVECTOR	pos,thisThingPos;
	int		radius;
	int		collided;

	pos = getCollisionCentre();
	thisThingPos = _thisThing->getCollisionCentre();

	radius = getCollisionRadius() + _thisThing->getCollisionRadius();
	collided = false;
	if(abs(pos.vx-thisThingPos.vx)<radius&&
	   abs(pos.vy-thisThingPos.vy)<radius)
	{
		CRECT	thisRect,thatRect;

		thisRect=getCollisionArea();

		thatRect=_thisThing->getCollisionArea();

		if(((thisRect.x1>=thatRect.x1&&thisRect.x1<=thatRect.x2)||(thisRect.x2>=thatRect.x1&&thisRect.x2<=thatRect.x2)||(thisRect.x1<=thatRect.x1&&thisRect.x2>=thatRect.x2))&&
		   ((thisRect.y1>=thatRect.y1&&thisRect.y1<=thatRect.y2)||(thisRect.y2>=thatRect.y1&&thisRect.y2<=thatRect.y2)||(thisRect.y1<=thatRect.y1&&thisRect.y2>=thatRect.y2)))
		{
			collided=true;
		}
	}

	return collided;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcParasiticWormEnemy::processShot( int _frames )
{
	if ( !m_segment )
	{
		m_drawRotation += 64 * _frames;
		m_drawRotation &= 4095;

		Pos.vy += m_speed * _frames;

		if ( m_speed < 5 )
		{
			m_speed++;
		}

		DVECTOR	offset = CLevel::getCameraPos();

		if ( Pos.vy - offset.vy > VidGetScrH() )
		{
			setToShutdown();
		}
	}
	else
	{
		if ( m_collTimer <= 0 )
		{
			// knock segment off end of list

			CNpcParasiticWormSegment *segment = m_segment;
			CNpcParasiticWormSegment *oldSegment = segment;

			while( segment->m_nextSegment )
			{
				oldSegment = segment;

				segment = segment->m_nextSegment;
			}

			delete segment;

			if ( segment == m_segment )
			{
				m_segment = NULL;
			}
			else
			{
				oldSegment->m_nextSegment = NULL;
			}

			m_collTimer = GameState::getOneSecondInFrames();
		}

		m_controlFunc = NPC_CONTROL_MOVEMENT;
	}
}

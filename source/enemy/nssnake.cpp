/*=========================================================================

	nssnake.cpp

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __ENEMY_NPC_H__
#include "enemy\npc.h"
#endif

#ifndef	__ENEMY_NSSNAKE_H__
#include "enemy\nssnake.h"
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

#ifndef __PROJECTL_PROJECTL_H__
#include "projectl\projectl.h"
#endif

#ifndef __SPR_SPRITES_H__
#include <sprites.h>
#endif

#include "fx\fx.h"
#include "fx\fxnrgbar.h"


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcSeaSnakeSegment::init()
{
	m_actorGfx=CActorPool::GetActor( (FileEquate) ACTORS_SEASNAKE_SBK );

	m_heading = 0;
	m_nextSegment = NULL;

	setCollisionSize( 20, 20 );
	setCollisionCentreOffset( 10, 10 );
	updateCollisionArea();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcSeaSnakeSegment::updateCollisionArea()
{
	m_collisionCentre.vx=Pos.vx+m_collisionCentreOffset.vx;
	m_collisionCentre.vy=Pos.vy+m_collisionCentreOffset.vy;
	m_collisionArea.x1=m_collisionCentre.vx-(m_collisionSize.vx/2);
	m_collisionArea.x2=m_collisionArea.x1+m_collisionSize.vx;
	m_collisionArea.y1=m_collisionCentre.vy-(m_collisionSize.vy/2);
	m_collisionArea.y2=m_collisionArea.y1+m_collisionSize.vy;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcSeaSnakeSegment::setCollisionSize(int _w,int _h)
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

void CNpcSeaSnakeEnemy::postInit()
{
	m_npcPath.setPathType( CNpcPath::REPEATING_PATH );

	s16 maxArraySize = NPC_SEA_SNAKE_LENGTH * NPC_SEA_SNAKE_SPACING;

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
	int initLength = NPC_SEA_SNAKE_LENGTH / 3;
	int remLength = NPC_SEA_SNAKE_LENGTH - initLength;

	m_health = NPC_SEA_SNAKE_LENGTH;

	if ( CLevel::getIsBossRespawn() )
	{
		m_health = CLevel::getBossHealth();
		m_speed = m_data[m_type].speed + ( ( 3 * ( NPC_SEA_SNAKE_LENGTH - m_health ) ) / NPC_SEA_SNAKE_LENGTH );
	}

	for ( int segCount = 0 ; segCount < NPC_SEA_SNAKE_LENGTH ; segCount++ )
	{
		m_segmentArray[segCount].init();

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

			segScale = start + ( ( end * ( NPC_SEA_SNAKE_LENGTH - segCount ) ) / remLength );
		}

		m_segmentArray[segCount].setScale( segScale );

		// attach snake segment

		if ( segCount < NPC_SEA_SNAKE_LENGTH - 1 )
		{
			m_segmentArray[segCount].m_nextSegment = &m_segmentArray[segCount + 1];
		}
	}

	m_segmentCount = m_health;

	m_movementTimer = 2 * GameState::getOneSecondInFrames();
	m_collTimer = 0;
	m_meterOn=false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcSeaSnakeSegment::shutdown()
{
	delete m_actorGfx;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcSeaSnakeEnemy::shutdown()
{
	if ( m_state != NPC_GENERIC_HIT_DEATH_END )
	{
		CLevel::setIsBossRespawn( true );
		CLevel::setBossHealth( m_health );
	}

	// delete snake segments

	for ( int segCount = 0 ; segCount < NPC_SEA_SNAKE_LENGTH ; segCount++ )
	{
		m_segmentArray[segCount].shutdown();
	}

	CNpcEnemy::shutdown();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool CNpcSeaSnakeEnemy::processSensor()
{
	if ( m_sensorFunc == NPC_SENSOR_NONE )
	{
		return( false );
	}

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

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcSeaSnakeEnemy::processMovement( int _frames )
{
	s32 moveX = 0, moveY = 0;
	s32 moveVel = 0;
	s32 moveDist = 0;
	DVECTOR oldPos = Pos;
	u8 skipCounter;

	if ( m_snapTimer > 0 )
	{
		m_snapTimer -= _frames;

		if ( m_snapTimer <= 0 )
		{
			m_animNo = ANIM_SEASNAKE_HEADSTATICMOUTHSHUT;
			m_animPlaying = true;
			m_frame = 0;
		}
		else
		{
			m_openTimer -= _frames;

			if ( m_openTimer <= 0 )
			{
				if ( m_animNo == ANIM_SEASNAKE_HEADSTATIC )
				{
					m_animNo = ANIM_SEASNAKE_HEADSTATICMOUTHSHUT;
				}
				else
				{
					m_animNo = ANIM_SEASNAKE_HEADSTATIC;
				}

				m_animPlaying = true;
				m_frame = 0;

				m_openTimer = GameState::getOneSecondInFrames() >> 2;
			}
		}
	}

	if ( m_data[m_type].moveSfx < CSoundMediator::NUM_SFXIDS )
	{
		if ( m_soundId == NOT_PLAYING )
		{
			m_soundId = (int) CSoundMediator::playSfx( m_data[m_type].moveSfx, true );
		}
	}

	processGenericFixedPathMove( _frames, &moveX, &moveY, &moveVel, &moveDist );

	Pos.vx += moveX;
	Pos.vy += moveY;

	m_extension += 256;
	m_extension &= 4095;

	m_positionHistory = m_positionHistory->prev;
	m_positionHistory->pos = oldPos;

	CNpcPositionHistory *newPos;
	newPos = m_positionHistory;
	for ( skipCounter = 1 ; skipCounter < NPC_SEA_SNAKE_SPACING ; skipCounter++ )
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

	for ( segmentCount = 0 ; segmentCount < m_segmentCount ; segmentCount++ )
	{
		s32 xDist = oldPos.vx - newPos->pos.vx;
		s32 yDist = oldPos.vy - newPos->pos.vy;

		s16 headingToTarget = ratan2( yDist, xDist );

		DVECTOR sinPos;

		sinPos = newPos->pos;
		s32 diff = ( ( ( 5 >> downShift ) * rsin( extension ) ) >> 12 ) >> timeShift;
		sinPos.vx += ( diff * rcos( headingToTarget + 1024 ) ) >> 12;
		sinPos.vy += ( diff * rsin( headingToTarget + 1024 ) ) >> 12;

		m_segmentArray[segmentCount].setPos( sinPos );
		oldPos = sinPos;

		for ( skipCounter = 0 ; skipCounter < NPC_SEA_SNAKE_SPACING ; skipCounter++ )
		{
			newPos = newPos->next;
		}

		extension += 1024;
		extension &= 4095;

		if ( downShift > 0 )
		{
			downShift--;
		}
	}

	oldPos = Pos;

	for ( segmentCount = 0 ; segmentCount < m_segmentCount ; segmentCount++ )
	{
		DVECTOR currentPos = m_segmentArray[segmentCount].getPos();

		s32 xDist = oldPos.vx - currentPos.vx;
		s32 yDist = oldPos.vy - currentPos.vy;

		s16 headingToPrev = ratan2( yDist, xDist );
		s16 headingFromNext;
		s16 heading = headingToPrev;

		oldPos = currentPos;

		if ( segmentCount < m_segmentCount - 1 )
		{
			DVECTOR nextPos = m_segmentArray[segmentCount + 1].getPos();
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

		m_segmentArray[segmentCount].setHeading( heading );
		m_segmentArray[segmentCount].updateCollisionArea();
	}

	if ( m_collTimer > 0 )
	{
		m_collTimer -= _frames;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*void CNpcSeaSnakeEnemy::resetSeaSnakeHeadToTail()
{
	DVECTOR startPos;
	DVECTOR endPos;
	int posCounter;
	CNpcPositionHistory *currentPos;

	startPos = Pos;

	currentPos = m_positionHistory;

	for ( posCounter = 0 ; posCounter < ( NPC_SEA_SNAKE_LENGTH * NPC_SEA_SNAKE_SPACING ) - 1 ; posCounter++ )
	{
		currentPos = currentPos->next;
	}

	endPos = currentPos->pos;

	currentPos = m_positionHistory;

	for ( posCounter = 0 ; posCounter < NPC_SEA_SNAKE_LENGTH * NPC_SEA_SNAKE_SPACING ; posCounter++ )
	{
		currentPos->pos.vx = startPos.vx + ( posCounter * ( endPos.vx - startPos.vx ) ) / ( ( NPC_SEA_SNAKE_LENGTH * NPC_SEA_SNAKE_SPACING ) - 1 );
		currentPos->pos.vy = startPos.vy + ( posCounter * ( endPos.vy - startPos.vy ) ) / ( ( NPC_SEA_SNAKE_LENGTH * NPC_SEA_SNAKE_SPACING ) - 1 );

		currentPos = currentPos->next;
	}

	CNpcPositionHistory *newPos;
	newPos = m_positionHistory;

	u8 skipCounter;
	for ( skipCounter = 1 ; skipCounter < NPC_SEA_SNAKE_SPACING ; skipCounter++ )
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
			for ( skipCounter = 0 ; skipCounter < NPC_SEA_SNAKE_SPACING ; skipCounter++ )
			{
				newPos = newPos->next;
			}
		}

		extension += 1024;
		extension &= 4095;
	}
}*/

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcSeaSnakeEnemy::processClose( int _frames )
{
	// fire at player

	s16 heading = ratan2( playerYDist, playerXDist ) & 4095;

	CProjectile *projectile;
	projectile = CProjectile::Create();
	DVECTOR newPos = Pos;
	projectile->init( newPos, heading );
	projectile->setGraphic( FRM__LIGHTNING1 );

	//resetSeaSnakeHeadToTail();

	m_movementTimer = 2 * GameState::getOneSecondInFrames();

	m_controlFunc = NPC_CONTROL_MOVEMENT;
	m_timerFunc = NPC_TIMER_ATTACK_DONE;
	m_timerTimer = GameState::getOneSecondInFrames();
	m_sensorFunc = NPC_SENSOR_NONE;

	m_snapTimer = m_movementTimer;
	m_openTimer = GameState::getOneSecondInFrames() >> 2;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcSeaSnakeEnemy::processEnemyCollision( CThing *thisThing )
{
	// do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcSeaSnakeSegment::processEnemyCollision( CThing *thisThing )
{
	// do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcSeaSnakeEnemy::render()
{
	SprFrame = NULL;

	if ( m_isActive )
	{
		CEnemyThing::render();

		if (canRender())
		{
			if (!m_meterOn)
			{
				CFXNRGBar	*T=(CFXNRGBar*)CFX::Create(CFX::FX_TYPE_NRG_BAR,this);
				T->SetMax( NPC_SEA_SNAKE_LENGTH );
				m_meterOn=true;
			}

			DVECTOR &renderPos=getRenderPos();

			SprFrame = m_actorGfx->Render(renderPos,m_animNo,( m_frame >> 8 ),0);
			m_actorGfx->RotateScale( SprFrame, renderPos, m_heading, 4096, 4096 );

			sBBox boundingBox = m_actorGfx->GetBBox();
			setCollisionSize( ( boundingBox.XMax - boundingBox.XMin ), ( boundingBox.YMax - boundingBox.YMin ) );
			setCollisionCentreOffset( ( boundingBox.XMax + boundingBox.XMin ) >> 1, ( boundingBox.YMax + boundingBox.YMin ) >> 1 );
		}

		for ( int segmentCount = 0 ; segmentCount < m_segmentCount ; segmentCount++ )
		{
			m_segmentArray[segmentCount].render();
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int CNpcSeaSnakeEnemy::checkCollisionAgainst( CThing *_thisThing, int _frames )
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

	for ( int segmentCount = 0 ; segmentCount < m_segmentCount ; segmentCount++ )
	{
		if ( m_segmentArray[segmentCount].checkCollisionAgainst( _thisThing, _frames ) )
		{
			collided = true;
		}
	}

	return collided;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcSeaSnakeSegment::render()
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

int CNpcSeaSnakeSegment::checkCollisionAgainst( CThing *_thisThing, int _frames )
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

void CNpcSeaSnakeEnemy::processShot( int _frames )
{
	if ( !m_segmentCount )
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
			CGameScene::setBossHasBeenKilled();
			m_state = NPC_GENERIC_HIT_DEATH_END;
		}
	}
	else
	{
		if ( m_collTimer <= 0 )
		{
			// knock segment off end of list

			m_segmentCount--;
			m_health--;
			m_speed = m_data[m_type].speed + ( ( 3 * ( NPC_SEA_SNAKE_LENGTH - m_health ) ) / NPC_SEA_SNAKE_LENGTH );

			m_collTimer = GameState::getOneSecondInFrames();
		}

		m_controlFunc = NPC_CONTROL_MOVEMENT;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const CRECT *CNpcSeaSnakeEnemy::getThinkBBox()
{
	CRECT objThinkBox = getCollisionArea();

	sBBox &thinkBBox = CThingManager::getThinkBBox();
	objThinkBox.x1 = thinkBBox.XMin;
	objThinkBox.x2 = thinkBBox.XMax;
	objThinkBox.y1 = thinkBBox.YMin;
	objThinkBox.y2 = thinkBBox.YMax;

	return &objThinkBox;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcSeaSnakeEnemy::processUserCollision( CThing *thisThing )
{
}
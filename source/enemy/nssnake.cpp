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
	int histLength;

	m_npcPath.setPathType( CNpcPath::REPEATING_PATH );

	s16 maxArraySize = NPC_SEA_SNAKE_LENGTH * NPC_SEA_SNAKE_SPACING * 2;

	m_positionHistoryArray[0].pos = Pos;
	m_positionHistoryArray[0].next = &m_positionHistoryArray[1];
	m_positionHistoryArray[0].prev = &m_positionHistoryArray[maxArraySize - 1];

	for ( histLength = 1 ; histLength < maxArraySize - 1 ; histLength++ )
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

	m_health = NPC_SEA_SNAKE_LENGTH + 1;

	if ( CLevel::getIsBossRespawn() )
	{
		m_health = CLevel::getBossHealth();
		m_speed = m_data[m_type].speed + ( ( 3 * ( NPC_SEA_SNAKE_LENGTH - ( m_health - 1 ) ) ) / NPC_SEA_SNAKE_LENGTH );
	}

	for ( int segCount = 0 ; segCount < NPC_SEA_SNAKE_LENGTH ; segCount++ )
	{
		m_segmentArray[segCount].init();

		if ( segCount < initLength )
		{
			u16 sum = ONE << 1;
			u16 start = ONE;
			u16 end = sum - start;

			segScale = start + ( ( end * segCount ) / initLength );
		}
		else
		{
			u16 sum = ONE << 1;
			u16 start = ONE >> 1;
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

	m_segmentCount = m_health - 1;

	m_movementTimer = 2 * GameState::getOneSecondInFrames();
	m_collTimer = 0;
	m_turnDir = 0;
	m_waitTimer = 0;
	m_collCount = 0;
	m_sign = 1;

	CNpcBossEnemy::postInit();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcSeaSnakeSegment::shutdown()
{
	delete m_actorGfx;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcSeaSnakeEnemy::shutdown()
{
	CLevel::setIsBossRespawn( true );

	if ( m_state != NPC_GENERIC_HIT_DEATH_END )
	{
		CLevel::setBossHealth( m_health );
	}
	else
	{
		CLevel::setBossHealth( 0 );
	}

	// delete snake segments

	for ( int segCount = 0 ; segCount < NPC_SEA_SNAKE_LENGTH ; segCount++ )
	{
		m_segmentArray[segCount].shutdown();
	}

	CNpcBossEnemy::shutdown();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool CNpcSeaSnakeEnemy::processSensor()
{
	if ( m_sensorFunc == NPC_SENSOR_NONE )
	{
		return( false );
	}

	if ( playerXDistSqr + playerYDistSqr < 50000 )
	{
		m_controlFunc = NPC_CONTROL_CLOSE;
		m_state = SEA_SNAKE_VERTICAL_LINEUP;

		return( true );
	}
	else
	{
		return( false );
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

u8 CNpcSeaSnakeEnemy::processPathMove( int _frames, s32 *moveX, s32 *moveY, s32 *moveVel, s32 *moveDist )
{
	bool pathComplete;
	bool waypointChange;
	s32 xDist, yDist;

	s16 headingToTarget = m_npcPath.think( Pos, &pathComplete, &waypointChange, &xDist, &yDist );

	/*if ( waypointChange )
	{
		m_movementTimer = 0;
	}*/

	if ( !pathComplete )
	{
		s16 decDir, incDir;
		s16 maxTurnRate = m_data[m_type].turnSpeed;

		decDir = m_heading - headingToTarget;

		if ( decDir < 0 )
		{
			decDir += ONE;
		}

		incDir = headingToTarget - m_heading;

		if ( incDir < 0 )
		{
			incDir += ONE;
		}

		if ( decDir < incDir )
		{
			*moveDist = -decDir;
		}
		else
		{
			*moveDist = incDir;
		}

		if ( *moveDist < -maxTurnRate )
		{
			*moveDist = -maxTurnRate;
		}
		else if ( *moveDist > maxTurnRate )
		{
			*moveDist = maxTurnRate;
		}

		m_heading += *moveDist;
		m_heading &= 4095;

		s32 speed = m_speed;

		if ( abs( *moveDist ) > 10 )
		{
			speed = 3;
		}

		s32 preShiftX = _frames * speed * rcos( m_heading );
		s32 preShiftY = _frames * speed * rsin( m_heading );

		*moveX = preShiftX >> 12;
		if ( !(*moveX) && preShiftX )
		{
			*moveX = preShiftX / abs( preShiftX );
		}

		if ( xDist > 0 )
		{
			if ( *moveX > xDist )
			{
				*moveX = xDist;
			}
		}
		else if ( xDist < 0 )
		{
			if ( *moveX < xDist )
			{
				*moveX = xDist;
			}
		}
		else
		{
			*moveX = 0;
		}

		*moveY = preShiftY >> 12;
		if ( !(*moveY) && preShiftY )
		{
			*moveY = preShiftY / abs( preShiftY );
		}

		if ( yDist > 0 )
		{
			if ( *moveY > yDist )
			{
				*moveY = yDist;
			}
		}
		else if ( yDist < 0 )
		{
			if ( *moveY < yDist )
			{
				*moveY = yDist;
			}
		}
		else
		{
			*moveY = 0;
		}

		*moveVel = ( _frames * m_speed ) << 8;

		//processGroundCollisionReverse( moveX, moveY );
	}

	return( waypointChange );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcSeaSnakeEnemy::processFrameMovement()
{
	s32 moveX = 0, moveY = 0;
	s32 moveVel = 0;
	s32 moveDist = 0;
	DVECTOR oldPos = Pos;

	if ( m_snapTimer > 0 )
	{
		m_snapTimer -= 1;

		if ( m_snapTimer > 0 )
		{
			if ( !m_animPlaying )
			{
				m_animNo = ANIM_SEASNAKE_HEADSNAP;
				m_animPlaying = true;
				m_frame = 0;

				CSoundMediator::playSfx( CSoundMediator::SFX_WORM___CHOMP );
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

	if ( m_waitTimer > 0 )
	{
		m_waitTimer -= 1;
	}
	else
	{
		switch( m_turnDir )
		{
			case NPC_SEA_SNAKE_CIRCLE_CLOCKWISE:
			{
				m_circleHeading += m_data[m_type].turnSpeed;

				if ( m_circleHeading > 4096 )
				{
					m_circleHeading = 0;
					m_turnDir = 0;
				}

				m_heading = ( m_origHeading + m_circleHeading ) & 4095;

				s32 preShiftX = 3 * rcos( m_heading );
				s32 preShiftY = 3 * rsin( m_heading );

				s32 moveX = preShiftX >> 12;
				if ( !moveX && preShiftX )
				{
					moveX = preShiftX / abs( preShiftX );
				}

				s32 moveY = preShiftY >> 12;
				if ( !moveY && preShiftY )
				{
					moveY = preShiftY / abs( preShiftY );
				}

				Pos.vx += moveX;
				Pos.vy += moveY;

				break;
			}

			case NPC_SEA_SNAKE_CIRCLE_ANTICLOCKWISE:
			{
				m_circleHeading -= m_data[m_type].turnSpeed;

				if ( m_circleHeading < -4096 )
				{
					m_circleHeading = 0;
					m_turnDir = 0;
				}

				m_heading = ( m_origHeading + m_circleHeading ) & 4095;

				s32 preShiftX = 3 * rcos( m_heading );
				s32 preShiftY = 3 * rsin( m_heading );

				s32 moveX = preShiftX >> 12;
				if ( !moveX && preShiftX )
				{
					moveX = preShiftX / abs( preShiftX );
				}

				s32 moveY = preShiftY >> 12;
				if ( !moveY && preShiftY )
				{
					moveY = preShiftY / abs( preShiftY );
				}

				Pos.vx += moveX;
				Pos.vy += moveY;

				break;
			}

			default:
			{
				DVECTOR waypointPos;
				m_npcPath.getCurrentWaypointPos( &waypointPos );
				waypointPos.vy -= 8;

				if ( CGameScene::getCollision()->getHeightFromGround( waypointPos.vx, waypointPos.vy ) < 0 )
				{
					// waypoint is either start or end waypoint

					s32 distX, distY;

					distX = waypointPos.vx - Pos.vx;
					distY = waypointPos.vy - Pos.vy;

					if( !distX && !distY )
					{
						if ( isSnakeStopped() )
						{
							m_npcPath.incPath();

							m_npcPath.getCurrentWaypointPos( &waypointPos );
							waypointPos.vy -= 8;

							if ( CGameScene::getCollision()->getHeightFromGround( waypointPos.vx, waypointPos.vy ) < 0 )
							{
								// if next waypoint is ALSO a start/end waypoint, teleport directly to it

								moveEntireSnake( waypointPos );
								m_waitTimer = GameState::getOneSecondInFrames();
								oldPos.vx = waypointPos.vx;
								oldPos.vy = waypointPos.vy;

								// increment path
								m_npcPath.incPath();

								// point snake in correct direction
								m_npcPath.getCurrentWaypointPos( &waypointPos );

								m_heading = ratan2( waypointPos.vy - Pos.vy, waypointPos.vx - Pos.vx ) & 4095;
							}
						}
					}
					else
					{
						processGenericGotoTarget( 1, distX, distY, m_speed );
					}
				}
				else
				{
					if ( processPathMove( 1, &moveX, &moveY, &moveVel, &moveDist ) )
					{
						// path has changed

						DVECTOR newWaypointPos;

						m_npcPath.getCurrentWaypointPos( &newWaypointPos );
						newWaypointPos.vy -= 8;

						if ( newWaypointPos.vy == waypointPos.vy )
						{
							int testDir = newWaypointPos.vx - waypointPos.vx;

							if ( testDir > 0 && testDir <= 16 )
							{
								// clockwise

								m_turnDir = NPC_SEA_SNAKE_CIRCLE_CLOCKWISE;
								m_circleHeading = 0;
								m_origHeading = m_heading;
								m_npcPath.incPath();
							}
							else if ( testDir < 0 && testDir >= -16 )
							{
								// anticlockwise

								m_turnDir = NPC_SEA_SNAKE_CIRCLE_ANTICLOCKWISE;
								m_circleHeading = 0;
								m_origHeading = m_heading;
								m_npcPath.incPath();
							}
						}
					}
				}

				break;
			}
		}
	}

	Pos.vx += moveX;
	Pos.vy += moveY;

	// check for hitting ground

	if ( CGameScene::getCollision()->Get( Pos.vx >> 4, Pos.vy >> 4 ) )
	{
		switch ( CGameScene::getCollision()->getCollisionBlock( Pos.vx, Pos.vy ) & COLLISION_TYPE_MASK )
		{
			case COLLISION_TYPE_FLAG_SOLID:
			{
				Pos = oldPos;

				if ( m_collCount > 4 )
				{
					m_collCount = 0;
					m_sign = -m_sign;
				}
				else
				{
					m_collCount++;
				}

				m_heading += m_sign * 1024;
				m_heading &= 4095;

				bool dec = m_npcPath.decPath();

				if ( dec )
				{
					DVECTOR waypointPos;
					m_npcPath.getCurrentWaypointPos( &waypointPos );
					waypointPos.vy -= 8;

					if ( CGameScene::getCollision()->getHeightFromGround( waypointPos.vx, waypointPos.vy ) < 0 )
					{
						// one of the special 'teleport' waypoints

						m_npcPath.incPath();
					}
				}

				break;
			}

			default:
				break;
		}
	}

	updateTail( oldPos, 1 );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcSeaSnakeEnemy::processMovement( int _frames )
{
	for ( int frameCount = 0 ; frameCount < _frames ; frameCount++ )
	{
		processFrameMovement();
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcSeaSnakeEnemy::updateTail( DVECTOR &oldPos, int _frames )
{
	u8 skipCounter;

	m_extension += 256;
	m_extension &= 4095;

	m_positionHistory = m_positionHistory->prev;
	m_positionHistory->pos = oldPos;


	CNpcPositionHistory *newPos;
	newPos = m_positionHistory;

	//m_speed = m_data[m_type].speed + ( ( 3 * ( NPC_SEA_SNAKE_LENGTH - ( m_health - 1 ) ) ) / NPC_SEA_SNAKE_LENGTH );

	int skipDist;

	skipDist = NPC_SEA_SNAKE_SPACING;
	skipDist -= m_speed - m_data[m_type].speed;

	//for ( skipCounter = 1 ; skipCounter < NPC_SEA_SNAKE_SPACING ; skipCounter++ )
	for ( skipCounter = 1 ; skipCounter < skipDist ; skipCounter++ )
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

		s16 headingToTarget = ratan2( yDist, xDist ) & 4095;

		DVECTOR sinPos;

		sinPos = newPos->pos;
		s32 diff = ( ( ( 5 >> downShift ) * rsin( extension ) ) >> 12 ) >> timeShift;
		sinPos.vx += ( diff * rcos( headingToTarget + 1024 ) ) >> 12;
		sinPos.vy += ( diff * rsin( headingToTarget + 1024 ) ) >> 12;

		m_segmentArray[segmentCount].setPos( sinPos );

		if ( segmentCount > 3 )
		{
			if ( segmentCount == m_segmentCount - 1 )
			{
				m_segmentArray[segmentCount].setAnim( ANIM_SEASNAKE_TAILSTATIC );
			}
			else
			{
				if ( segmentCount % 2 )
				{
					m_segmentArray[segmentCount].setAnim( ANIM_SEASNAKE_BODY2STATIC );
				}
				else
				{
					m_segmentArray[segmentCount].setAnim( ANIM_SEASNAKE_BODY3STATIC );
				}
			}
		}
		else
		{
			m_segmentArray[segmentCount].setAnim( ANIM_SEASNAKE_BODY1STATIC );
		}
		oldPos = sinPos;

		//for ( skipCounter = 0 ; skipCounter < NPC_SEA_SNAKE_SPACING ; skipCounter++ )
		for ( skipCounter = 0 ; skipCounter < skipDist ; skipCounter++ )
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
		DVECTOR const &currentPos = m_segmentArray[segmentCount].getPos();

		s32 xDist = oldPos.vx - currentPos.vx;
		s32 yDist = oldPos.vy - currentPos.vy;

		s16 headingToPrev = ratan2( yDist, xDist ) & 4095;
		s16 headingFromNext;
		s16 heading = headingToPrev;

		oldPos = currentPos;

		if ( segmentCount < m_segmentCount - 1 )
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

void CNpcSeaSnakeEnemy::processFrameClose()
{
	DVECTOR oldPos = Pos;

	switch( m_state )
	{
		case SEA_SNAKE_VERTICAL_LINEUP:
		{
			if ( playerYDistSqr > 100 )
			{
				processGenericGotoTarget( 1, 0, playerYDist, m_speed );
			}
			else
			{
				m_state = SEA_SNAKE_CHARGE;
			}

			break;
		}

		case SEA_SNAKE_CHARGE:
		{
			if ( playerXDistSqr > 4000 )
			{
				processGenericGotoTarget( 1, playerXDist, 0, m_speed );
			}
			else
			{
				s16 heading = m_heading;

				CProjectile *projectile;
				projectile = CProjectile::Create();
				DVECTOR newPos = Pos;
				newPos.vx += 50 * ( rcos( m_heading ) >> 12 );
				newPos.vy += 50 * ( rsin( m_heading ) >> 12 );

				int perpHeading = ( heading - 1024 ) & 4095;

				newPos.vx += 20 * ( rcos( perpHeading ) >> 12 );
				newPos.vy += 20 * ( rsin( perpHeading ) >> 12 );

				projectile->init( newPos, heading );
				projectile->setGraphic( FRM__SNAKEBILE );

				m_movementTimer = GameState::getOneSecondInFrames();

				m_controlFunc = NPC_CONTROL_MOVEMENT;
				m_timerFunc = NPC_TIMER_ATTACK_DONE;
				m_timerTimer = 5 * GameState::getOneSecondInFrames();
				m_sensorFunc = NPC_SENSOR_NONE;

				m_snapTimer = m_movementTimer;

				CSoundMediator::playSfx( CSoundMediator::SFX_WORM___HISS );
			}

			if ( m_soundId == NOT_PLAYING )
			{
				m_soundId = (int) CSoundMediator::playSfx( CSoundMediator::SFX_SEASNAKE_ATTACK, true );
			}

			break;
		}
	}

	// check for hitting ground

	if ( CGameScene::getCollision()->Get( Pos.vx >> 4, Pos.vy >> 4 ) )
	{
		switch ( CGameScene::getCollision()->getCollisionBlock( Pos.vx, Pos.vy ) & COLLISION_TYPE_MASK )
		{
			case COLLISION_TYPE_FLAG_SOLID:
			{
				m_movementTimer = GameState::getOneSecondInFrames();

				m_controlFunc = NPC_CONTROL_MOVEMENT;
				m_timerFunc = NPC_TIMER_ATTACK_DONE;
				m_timerTimer = 10 * GameState::getOneSecondInFrames();
				m_sensorFunc = NPC_SENSOR_NONE;

				Pos = oldPos;

				if ( m_collCount > 4 )
				{
					m_collCount = 0;
					m_sign = -m_sign;
				}
				else
				{
					m_collCount++;
				}

				m_heading += m_sign * 1024;
				m_heading &= 4095;

				bool dec = m_npcPath.decPath();

				if ( dec )
				{
					DVECTOR waypointPos;
					m_npcPath.getCurrentWaypointPos( &waypointPos );
					waypointPos.vy -= 8;

					if ( CGameScene::getCollision()->getHeightFromGround( waypointPos.vx, waypointPos.vy ) < 0 )
					{
						// one of the special 'teleport' waypoints

						m_npcPath.incPath();
					}
				}

				break;
			}

			default:
				break;
		}
	}

	updateTail( oldPos, 1 );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcSeaSnakeEnemy::processClose( int _frames )
{
	for ( int frameCount = 0 ; frameCount < _frames ; frameCount++ )
	{
		processFrameClose();
	}
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
			DVECTOR &renderPos=getRenderPos();

			SprFrame = m_actorGfx->Render(renderPos,m_animNo,( m_frame >> 8 ),0);
			m_actorGfx->RotateScale( SprFrame, renderPos, m_heading, 4096, 4096, true );

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

void CNpcSeaSnakeEnemy::renderOnMapScreen( DVECTOR drawPos )
{
	drawPos.vx += 120;

	POLY_FT4 *headSprFrame;

	headSprFrame = m_actorGfx->Render(drawPos,ANIM_SEASNAKE_HEADSNAP,0,0);
	m_actorGfx->RotateScale( headSprFrame, drawPos, 0, 4096, 4096,true );

	int segmentCount;
	int segmentType;
	u16 segScale;
	int initLength = NPC_SEA_SNAKE_LENGTH / 3;
	int remLength = NPC_SEA_SNAKE_LENGTH - initLength;

	for ( segmentCount = 0 ; segmentCount < NPC_SEA_SNAKE_LENGTH ; segmentCount++ )
	{
		drawPos.vx -= 30;

		if ( segmentCount > 3 )
		{
			if ( segmentCount == NPC_SEA_SNAKE_LENGTH - 1 )
			{
				segmentType = ANIM_SEASNAKE_TAILSTATIC;
			}
			else
			{
				if ( segmentCount % 2 )
				{
					segmentType = ANIM_SEASNAKE_BODY2STATIC;
				}
				else
				{
					segmentType = ANIM_SEASNAKE_BODY3STATIC;
				}
			}
		}
		else
		{
			segmentType = ANIM_SEASNAKE_BODY1STATIC;
		}

		if ( segmentCount < initLength )
		{
			u16 sum = ONE << 1;
			u16 start = ONE;
			u16 end = sum - start;

			segScale = start + ( ( end * segmentCount ) / initLength );
		}
		else
		{
			u16 sum = ONE << 1;
			u16 start = ONE >> 1;
			u16 end = sum - start;

			segScale = start + ( ( end * ( NPC_SEA_SNAKE_LENGTH - segmentCount ) ) / remLength );
		}

		POLY_FT4 *Ft4= m_actorGfx->Render(drawPos,segmentType,0,0);
		m_actorGfx->RotateScale( Ft4, drawPos, 0, 4096, segScale,true );
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int CNpcSeaSnakeEnemy::checkCollisionAgainst( CThing *_thisThing, int _frames )
{
//	DVECTOR	pos,thisThingPos;
	int		radius;
	int		collided;

DVECTOR const &pos=getCollisionCentre();
DVECTOR const &thisThingPos=_thisThing->getCollisionCentre();

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
		SprFrame = m_actorGfx->Render(renderPos,m_anim,0,0);
		m_actorGfx->RotateScale( SprFrame, renderPos, m_heading, 4096, m_scale, true );

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

void CNpcSeaSnakeEnemy::processShot( int _frames )
{
	if ( !m_segmentCount )
	{
		if ( m_collTimer <= 0 )
		{
			m_drawRotation += 64 * _frames;
			m_drawRotation &= 4095;

			Pos.vy += m_speed * _frames;

			if ( m_speed < 5 )
			{
				m_speed++;
			}

			m_state = NPC_GENERIC_HIT_DEATH_END;

			if ( !CGameScene::getBossHasBeenKilled() )
			{
				CGameScene::setBossHasBeenKilled();
			}

			DVECTOR const &offset = CLevel::getCameraPos();

			if ( Pos.vy - offset.vy > VidGetScrH() )
			{
				setToShutdown();
			}
		}
		else
		{
			m_controlFunc = NPC_CONTROL_MOVEMENT;
		}
	}
	else
	{
		if ( m_collTimer <= 0 )
		{
			// knock segment off end of list

			m_segmentCount--;
			m_health--;
			m_speed = m_data[m_type].speed + ( ( 3 * ( NPC_SEA_SNAKE_LENGTH - ( m_health - 1 ) ) ) / NPC_SEA_SNAKE_LENGTH );

			m_collTimer = GameState::getOneSecondInFrames();
		}

		m_invulnerableTimer = 2 * GameState::getOneSecondInFrames();

		m_controlFunc = NPC_CONTROL_MOVEMENT;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcSeaSnakeEnemy::processUserCollision( CThing *thisThing )
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

s32 CNpcSeaSnakeEnemy::getFrameShift( int _frames )
{
	return( ( _frames << 8 ) >> 3 );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

u8 CNpcSeaSnakeEnemy::isSnakeStopped()
{
	if ( !m_segmentCount )
	{
		return( true );
	}

	DVECTOR const &tailPos = m_segmentArray[m_segmentCount - 1].getPos();

	if ( tailPos.vx == Pos.vx && tailPos.vy == Pos.vy )
	{
		return( true );
	}
	else
	{
		return( false );
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcSeaSnakeEnemy::moveEntireSnake( DVECTOR const &newPos )
{
	Pos.vx = newPos.vx;
	Pos.vy = newPos.vy;

	int segmentCount;

	for ( segmentCount = 0 ; segmentCount < m_segmentCount ; segmentCount++ )
	{
		m_segmentArray[segmentCount].setPos( Pos );
	}

	s16 maxArraySize = NPC_SEA_SNAKE_LENGTH * NPC_SEA_SNAKE_SPACING;

	for ( int histLength = 0 ; histLength < maxArraySize ; histLength++ )
	{
		m_positionHistoryArray[histLength].pos = Pos;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcSeaSnakeEnemy::addHealthMeter()
{
	if (!m_meterOn)
	{
		m_energyBar=(CFXNRGBar*)CFX::Create(CFX::FX_TYPE_NRG_BAR,this);
		m_energyBar->SetMax( NPC_SEA_SNAKE_LENGTH + 1 );
		m_meterOn=true;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcSeaSnakeEnemy::collidedWith( CThing *_thisThing )
{
	if ( m_isActive && !m_isCaught && !m_isDying )
	{
		switch(_thisThing->getThingType())
		{
			case TYPE_PLAYER:
			{
				if ( m_invulnerableTimer <= 0 )
				{
					CPlayer *player = (CPlayer *) _thisThing;

					ATTACK_STATE playerState = player->getAttackState();

					if(playerState==ATTACK_STATE__NONE)
					{
						if ( !player->isRecoveringFromHit() )
						{
							switch( m_data[m_type].detectCollision )
							{
								case DETECT_NO_COLLISION:
								{
									// ignore

									break;
								}

								case DETECT_ALL_COLLISION:
								{
									if ( m_controlFunc != NPC_CONTROL_COLLISION )
									{
										m_oldControlFunc = m_controlFunc;
										m_controlFunc = NPC_CONTROL_COLLISION;
									}

									processUserCollision( _thisThing );

									break;
								}

								case DETECT_ATTACK_COLLISION_GENERIC:
								{
									processAttackCollision();
									processUserCollision( _thisThing );

									break;
								}
							}
						}
					}
					else
					{
						// player is attacking, respond appropriately

						if ( m_controlFunc != NPC_CONTROL_SHOT )
						{
							if(playerState==ATTACK_STATE__BUTT_BOUNCE)
							{
								player->justButtBouncedABadGuy();
							}
							m_controlFunc = NPC_CONTROL_SHOT;
							m_state = NPC_GENERIC_HIT_CHECK_HEALTH;

							drawAttackEffect();
						}
					}
				}

				break;
			}

			case TYPE_ENEMY:
			{
				CNpcEnemy *enemy = (CNpcEnemy *) _thisThing;

				if ( canCollideWithEnemy() && enemy->canCollideWithEnemy() )
				{
					processEnemyCollision( _thisThing );
				}

				break;
			}

			default:
				ASSERT(0);
				break;
		}
	}
}

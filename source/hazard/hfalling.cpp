/*=========================================================================

	hfalling.cpp

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __HAZARD_HFALLING_H__
#include "hazard\hfalling.h"
#endif

#ifndef __GAME_GAME_H__
#include	"game\game.h"
#endif

#ifndef	__UTILS_HEADER__
#include	"utils\utils.h"
#endif

#ifndef __VID_HEADER_
#include "system\vid.h"
#endif


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcFallingHazard::init()
{
	CNpcHazard::init();

	m_movementTimer = 2 * GameState::getOneSecondInFrames();

	m_respawnRate = 4;
	m_bounceFinish = false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcFallingHazard::processMovement( int _frames )
{
	s8 groundHeight;
	s8 yMovement;

	if ( m_bounceFinish )
	{
		if ( m_bounceDir )
		{
			Pos.vx += 2 * _frames;
		}
		else
		{
			Pos.vx -= 2 * _frames;
		}

		Pos.vy += m_speed * _frames;

		if ( Pos.vy > ( m_bouncePos.vy + 32 ) )
		{
			m_bounceFinish = false;
		}
		else
		{
			if ( m_speed < 3 )
			{
				m_speed++;
			}
		}
	}
	else
	{
		if ( m_movementTimer > 0 )
		{
			m_movementTimer -= _frames;

			if ( m_movementTimer <= 0 )
			{
				Pos = m_base;
			}
			else
			{
				Pos.vx = m_base.vx + ( -3 + ( getRnd() % 7 ) );
				Pos.vy = m_base.vy + ( -3 + ( getRnd() % 7 ) );
			}
		}
		else
		{
			yMovement = 3 * _frames;

			groundHeight = CGameScene::getCollision()->getHeightFromGround( Pos.vx, Pos.vy, yMovement + 16 );

			if ( groundHeight < yMovement )
			{
				// colliding with ground

				Pos.vy += groundHeight;

				m_bounceFinish = true;
				m_speed = -5;
				m_bounceDir = getRnd() % 2;

				m_bouncePos = Pos;
			}
			else
			{
				// drop down

				Pos.vy += yMovement;
			}
		}
	}

	DVECTOR	offset = CLevel::getCameraPos();

	s32 yPos = Pos.vy - offset.vy;

	if ( yPos > VidGetScrH() )
	{
		m_isActive = false;
		m_timerActive = true;
		m_timer = ( m_respawnRate - 1 ) * GameState::getOneSecondInFrames();
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcFallingHazard::processTimer( int _frames )
{
	m_timer -= _frames;

	if ( m_timer < 0 )
	{
		m_timerActive = false;
		m_isActive = true;
		Pos = m_base;
		m_movementTimer = 2 * GameState::getOneSecondInFrames();
		m_bounceFinish = false;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcFallingHazard::collidedWith( CThing *_thisThing )
{
	if (m_movementTimer<=0) CNpcHazard::collidedWith(_thisThing);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const CRECT *CNpcFallingHazard::getThinkBBox()
{
	CRECT objThinkBox = getCollisionArea();

	sBBox &thinkBBox = CThingManager::getThinkBBox();
	objThinkBox.y2 = thinkBBox.YMin + 1;

	return &objThinkBox;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcFallingHazard::setWaypoints( sThingHazard *ThisHazard )
{
	int pointNum;

	u16	*PntList=(u16*)MakePtr(ThisHazard,sizeof(sThingHazard));

	u16 newXPos, newYPos;

	newXPos = (u16) *PntList;
	PntList++;
	newYPos = (u16) *PntList;
	PntList++;

	DVECTOR startPos;
	startPos.vx = newXPos << 4;
	//startPos.vy = newYPos << 4;
	startPos.vy = -10;

	Pos = startPos;
	m_base = Pos;

	addWaypoint( newXPos, newYPos );

	if ( ThisHazard->PointCount > 1 )
	{
		for ( pointNum = 1 ; pointNum < ThisHazard->PointCount ; pointNum++ )
		{
			newXPos = (u16) *PntList;
			PntList++;
			newYPos = (u16) *PntList;
			PntList++;

			addWaypoint( newXPos, newYPos );
		}
	}
}

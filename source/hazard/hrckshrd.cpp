/*=========================================================================

	hrckshrd.cpp

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __HAZARD_HRCKSHRD_H__
#include "hazard\hrckshrd.h"
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

void CNpcRockShardHazard::init()
{
	CNpcHazard::init();

	m_movementTimer = 2 * GameState::getOneSecondInFrames();

	m_respawnRate = 4;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcRockShardHazard::processMovement( int _frames )
{
	s8 groundHeight;
	s8 yMovement;

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

			if ( canRender() && m_soundId == NOT_PLAYING )
			{
				m_soundId = (int) CSoundMediator::playSfx( CSoundMediator::SFX_HAZARD__STALACTITE_RATTLE, true, true );
			}
		}
	}
	else
	{
		yMovement = 6 * _frames;

		groundHeight = CGameScene::getCollision()->getHeightFromGround( Pos.vx, Pos.vy, yMovement + 16 );

		if ( groundHeight < yMovement )
		{
			// colliding with ground

			Pos.vy += groundHeight;

			m_isActive = false;
			m_timerActive = true;
			m_timer = ( m_respawnRate - 1 ) * GameState::getOneSecondInFrames();

			if ( canRender() && m_soundId == NOT_PLAYING )
			{
				m_soundId = (int) CSoundMediator::playSfx( CSoundMediator::SFX_HAZARD__STALACTITE_LAND, true );
			}
		}
		else
		{
			// drop down

			Pos.vy += yMovement;
		}

		DVECTOR const &offset = CLevel::getCameraPos();

		s32 yPos = Pos.vy - offset.vy;

		if ( yPos > VidGetScrH() )
		{
			m_isActive = false;
			m_timerActive = true;
			m_timer = ( m_respawnRate - 1 ) * GameState::getOneSecondInFrames();
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcRockShardHazard::processTimer( int _frames )
{
	m_timer -= _frames;

	if ( m_timer < 0 )
	{
		m_timerActive = false;
		m_isActive = true;
		Pos = m_base;
		m_movementTimer = 2 * GameState::getOneSecondInFrames();
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const CRECT *CNpcRockShardHazard::getThinkBBox()
{
	CRECT objThinkBox = getCollisionArea();

	sBBox &thinkBBox = CThingManager::getThinkBBox();
	objThinkBox.y2 = thinkBBox.YMin + 1;

	return &objThinkBox;
}

/*=========================================================================

	pdrop.cpp

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __PLATFORM_PDROP_H__
#include "platform\pdrop.h"
#endif

#ifndef __GAME_GAME_H__
#include "game\game.h"
#endif

#ifndef __PAD_VIBE_H__
#include "pad\vibe.h"
#endif


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcDropPlatform::postInit()
{
	m_dropHit = true;

	CNpcPlatform::postInit();

	m_initTimer = 2 * GameState::getOneSecondInFrames();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcDropPlatform::processMovement( int _frames )
{
	if ( m_initTimer > 0 )
	{
		m_initTimer -= _frames;
	}

	s32 moveY = m_speed * _frames;

	s32 groundHeight = CGameScene::getCollision()->getHeightFromGround( Pos.vx, Pos.vy, moveY );

	if ( groundHeight < moveY )
	{
		if ( !m_dropHit && m_initTimer <= 0 )
		{
			if( m_soundId != NOT_PLAYING )
			{
				CSoundMediator::stopAndUnlockSfx( (xmPlayingId) m_soundId );
			}

			m_soundId = (int) CSoundMediator::playSfx( CSoundMediator::SFX_HAZARD__FALLING_ROCK_LAND, true );

			CGameScene::setCameraShake(0,8);
			CPadVibrationManager::setVibration(0,CPadVibrationManager::VIBE_MEDIUM);
		}

		m_dropHit = true;

		moveY = groundHeight;
	}
	else
	{
		m_dropHit = false;
	}

	Pos.vy += moveY;
}

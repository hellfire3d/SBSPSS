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


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcDropPlatform::postInit()
{
	m_dropHit = false;

	CNpcPlatform::postInit();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcDropPlatform::processMovement( int _frames )
{
	s32 moveY = m_speed * _frames;

	s32 groundHeight = CGameScene::getCollision()->getHeightFromGround( Pos.vx, Pos.vy + moveY, 16 );

	if ( groundHeight < moveY )
	{
		if ( !m_dropHit )
		{
			if( m_soundId != NOT_PLAYING )
			{
				CSoundMediator::stopAndUnlockSfx( (xmPlayingId) m_soundId );
			}

			m_soundId = (int) CSoundMediator::playSfx( CSoundMediator::SFX_HAZARD__FALLING_ROCK_LAND, true );
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

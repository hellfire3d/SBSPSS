/*=========================================================================

	pplayer.cpp

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __PLATFORM_PPLAYER_H__
#include "platform\pplayer.h"
#endif

#ifndef __GAME_GAME_H__
#include	"game\game.h"
#endif

#ifndef __SPR_SPRITES_H__
#include <sprites.h>
#endif


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcPlayerBubblePlatform::processLifetime( int _frames )
{
	m_lifetime -= _frames;

	if ( m_pop )
	{
		if ( m_lifetime <= 0 )
		{
			setToShutdown();
		}
	}
	else
	{
		if ( m_lifetime <= 0 )
		{
			m_pop = true;

			if ( m_soundId == NOT_PLAYING )
			{
				m_soundId = (int) CSoundMediator::playSfx( CSoundMediator::SFX_BALLOON_POP, true );
			}

			m_lifetime = GameState::getOneSecondInFrames() >> 2;
		}
		else if ( m_lifetime <= ( GameState::getOneSecondInFrames() >> 1 ) )
		{
			m_scale = ONE + ( ( 256 * rsin( ( m_lifetime << 14 ) / ( GameState::getOneSecondInFrames() >> 1 ) ) ) >> 12 );
		}
	}
}

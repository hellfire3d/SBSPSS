/*=========================================================================

	pcbubble.cpp

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __PLATFORM_PCBUBBLE_H__
#include "platform\pcbubble.h"
#endif

#ifndef __GAME_GAME_H__
#include	"game\game.h"
#endif

#ifndef __SPR_SPRITES_H__
#include <sprites.h>
#endif


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcCollapsingBubblePlatform::postInit()
{
	CNpcBubblePlatform::postInit();

	m_startCollapse = false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcCollapsingBubblePlatform::processLifetime( int _frames )
{
	switch( m_lifetimeType )
	{
		case NPC_PLATFORM_FINITE_LIFE:
		{
			m_lifetime -= _frames;

			if ( m_lifetime <= 0 )
			{
				setToShutdown();

				return;
			}

			break;
		}

		case NPC_PLATFORM_FINITE_LIFE_RESPAWN:
		{
			m_lifetime -= _frames;

			if ( m_lifetime <= 0 )
			{
				reinit();
			}

			break;
		}

		case NPC_PLATFORM_INFINITE_LIFE_COLLAPSIBLE:
		{
			if ( m_pop )
			{
				m_lifetime -= _frames;

				if ( m_lifetime <= 0 )
				{
					m_isActive = false;
					m_timer = 3 * GameState::getOneSecondInFrames();
					m_timerType = NPC_PLATFORM_TIMER_RESPAWN;
					m_pop = false;
					m_scale = ONE;
					m_startCollapse = false;
				}
			}
			else
			{
				if ( m_contact || m_startCollapse )
				{
					m_lifetime -= _frames;

					if ( m_lifetime <= 0 )
					{
						m_lifetime = GameState::getOneSecondInFrames() >> 2;
						m_pop = true;

						if ( m_soundId == NOT_PLAYING )
						{
							m_soundId = (int) CSoundMediator::playSfx( CSoundMediator::SFX_BALLOON_POP, true );
						}
					}
					else if ( m_lifetime <= ( GameState::getOneSecondInFrames() >> 1 ) )
					{
						m_startCollapse = true;

						m_scale = ONE + ( ( 256 * rsin( ( m_lifetime << 14 ) / ( GameState::getOneSecondInFrames() >> 1 ) ) ) >> 12 );
					}
				}
			}

			break;
		}

		default:
			break;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcCollapsingAcridPlatform::render()
{

	CNpcCollapsingBubblePlatform::render();
	if (SprFrame)
	{
		setRGB0( SprFrame, 255, 255, 128 );
	}
}

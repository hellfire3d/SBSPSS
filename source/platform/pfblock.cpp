/*=========================================================================

	pfblock.h

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __PLATFORM_PFBLOCK_H__
#include "platform\pfblock.h"
#endif

#ifndef __GAME_GAME_H__
#include	"game\game.h"
#endif

#ifndef __VID_HEADER_
#include "system\vid.h"
#endif

#ifndef	__UTILS_HEADER__
#include	"utils\utils.h"
#endif


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcFallingBlockPlatform::postInit()
{
	CNpcPlatform::postInit();

	m_isTriggered = false;
	m_isFalling = false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcFallingBlockPlatform::processMovement( int _frames )
{
	if ( m_isTriggered )
	{
		if ( m_timer > 0 )
		{
			m_timer -= _frames;

			if ( m_timer <= 0 )
			{
				Pos = m_base;
				m_isFalling = true;
			}
			else
			{
				Pos.vx = m_base.vx + ( -3 + ( getRnd() % 7 ) );
				Pos.vy = m_base.vy + ( -3 + ( getRnd() % 7 ) );

				if ( m_soundId == NOT_PLAYING )
				{
					m_soundId = (int) CSoundMediator::playSfx( CSoundMediator::SFX_HAZARD__STALACTITE_RATTLE, true );
				}
			}
		}
		else
		{
			DVECTOR	offset = CLevel::getCameraPos();

			s32 yPos = Pos.vy - offset.vy;

			if ( yPos > VidGetScrH() )
			{
				setToShutdown();
			}

			s32 moveY = m_speed * _frames;

			Pos.vy += moveY;
		}
	}
	else if ( m_contact )
	{
		m_isTriggered = true;
		m_timer = GameState::getOneSecondInFrames();
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int CNpcFallingBlockPlatform::checkCollisionAgainst(CThing *_thisThing, int _frames)
{
	if ( m_isFalling )
	{
		return( false );
	}
	else
	{
		return( CNpcPlatform::checkCollisionAgainst( _thisThing, _frames ) );
	}
}

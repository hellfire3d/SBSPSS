/*=========================================================================

	pretract.cpp

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __PLATFORM_PRETRACT_H__
#include "platform\pretract.h"
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

#include "fx\fx.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcRetractingPlatform::postInit()
{
	CNpcPlatform::postInit();

	m_timer = NPC_PLATFORM_TIMER_RETRACT;

	m_extension = ONE;

	m_initDelay = 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcRetractingPlatform::setSpeed( s16 newSpeed )
{
	m_speed = newSpeed;

	m_initDelay = ( m_speed * GameState::getOneSecondInFrames() ) >> 2;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcRetractingPlatform::reinit()
{
	CNpcPlatform::reinit();

	m_initDelay = ( m_speed * GameState::getOneSecondInFrames() ) >> 2;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcRetractingPlatform::processTimer( int _frames )
{
	if ( m_initDelay > 0 )
	{
		m_initDelay -= _frames;

		return;
	}

	switch( m_timerType )
	{
		case NPC_PLATFORM_TIMER_RETRACT:
		{
			if ( m_timer > 0 )
			{
				m_timer -= _frames;

				if ( m_timer <= 0 )
				{
					if ( m_soundId == NOT_PLAYING )
					{
						m_soundId = (int) CSoundMediator::playSfx( CSoundMediator::SFX_RETRACTING_PLATFORM, true, true );
					}
				}
			}
			else
			{
				if ( m_extension > 1 )
				{
					m_extension -= 128 * _frames;

					if ( m_extension <= 1 )
					{
						m_extension = 1;

						m_timer = 4 * GameState::getOneSecondInFrames();
						m_timerType = NPC_PLATFORM_TIMER_EXTEND;
						m_detectCollision = false;
					}
				}
			}

			break;
		}

		case NPC_PLATFORM_TIMER_EXTEND:
		{
			if ( m_timer > 0 )
			{
				m_timer -= _frames;

				if ( m_timer <= 0 )
				{
					if ( m_soundId == NOT_PLAYING )
					{
						m_soundId = (int) CSoundMediator::playSfx( CSoundMediator::SFX_RETRACTING_PLATFORM, true, true );
					}
				}
			}
			else
			{
				if ( m_extension < ONE )
				{
					m_extension += 128 * _frames;

					if ( m_extension >= ONE )
					{
						m_extension = ONE;

						m_timer = 4 * GameState::getOneSecondInFrames();
						m_timerType = NPC_PLATFORM_TIMER_RETRACT;
						m_detectCollision = true;
					}
				}
			}

			break;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcRetractingPlatform::render()
{
	if ( m_isActive )
	{
		CPlatformThing::render();

		// Render
		CPlatformThing::render();

		if (canRender())
		{
			DVECTOR &renderPos=getRenderPos();
			SVECTOR rotation;

			rotation.vx = 0;
			rotation.vy = 0;
			rotation.vz = 0;

			VECTOR scale;
			scale.vx = ONE;
			scale.vy = ( ONE >> 1 ) + ( ( m_extension * ( ONE >> 1 ) ) / ONE );
			scale.vz = m_extension;

			m_modelGfx->Render(renderPos,&rotation,&scale);
		}
	}
}

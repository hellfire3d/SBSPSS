/*=========================================================================

	pgeyser.cpp

	Author:		CRB
	Created:
	Project:	Spongebob
	Purpose:

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __PLATFORM_PGEYSER_H__
#include "platform\pgeyser.h"
#endif

#ifndef	__UTILS_HEADER__
#include	"utils\utils.h"
#endif

#include	"fx\fxgeyser.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const int	DodgyFXOffset=16;
void CNpcGeyserPlatform::postInit()
{
	CNpcPlatform::postInit();

	m_isFiring = false;

	m_state = GEYSER_READY;
// Add GEYSER stuff

DVECTOR	Pos=getPos();
		Pos.vy+=DodgyFXOffset;
CThing	*FX=CFX::Create(CFX::FX_TYPE_GEYSER_OIL,Pos);
		addChild(FX);

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcGeyserPlatform::processMovement( int _frames )
{
	if ( m_isFiring )
	{
		s32 minY, maxY;
		m_npcPath.getPathYExtents( &minY, &maxY );

		switch ( m_state )
		{
			case GEYSER_DROPPING:
			{
				if ( maxY - Pos.vy == 0 )
				{
					m_isFiring = false;
					reinit();
					m_state = GEYSER_READY;
				}
				else
				{
					s32 moveY = m_speed * _frames;

					if ( Pos.vy < ( minY + 64 ) )
					{
						s32 multiplier = Pos.vy - minY;

						moveY = ( multiplier * moveY ) >> 6;

						if ( moveY < 1 )
						{
							moveY = 1;
						}
					}

					Pos.vy += moveY;

					if ( Pos.vy > maxY )
					{
						Pos.vy = maxY;
					}
				}


				break;
			}

			case GEYSER_RISING:
			{
				if ( minY - Pos.vy == 0 )
				{
					m_state = GEYSER_DROPPING;
				}
				else
				{
					s32 moveY = m_speed * _frames;

					if ( Pos.vy < ( minY + 64 ) )
					{
						s32 multiplier = Pos.vy - minY;

						moveY = ( multiplier * moveY ) >> 6;

						if ( moveY < 1 )
						{
							moveY = 1;
						}
					}

					Pos.vy -= moveY;

					if ( Pos.vy < minY )
					{
						Pos.vy = minY;
					}
				}

				break;
			}
		}
	}
	else
	{
		Pos.vx = m_base.vx + ( -3 + ( getRnd() % 7 ) );
		Pos.vy = m_base.vy + ( -3 + ( getRnd() % 7 ) );
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcGeyserPlatform::processTimer( int _frames )
{
	if ( m_timer > 0 )
	{
		m_timer -= _frames;
	}
	else
	{
		if ( m_state == GEYSER_READY )
		{
			m_isFiring = true;
			Pos = m_base;
			m_state = GEYSER_RISING;

			if ( m_soundId == NOT_PLAYING )
			{
				m_soundId = (int) CSoundMediator::playSfx( CSoundMediator::SFX_WORLD_OBJECT__GEYSER, true );
			}
		}
	}
}

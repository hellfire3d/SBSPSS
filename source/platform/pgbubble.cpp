/*=========================================================================

	pgbubble.cpp

	Author:		CRB
	Created:
	Project:	Spongebob
	Purpose:

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __PLATFORM_PGBUBBLE_H__
#include "platform\pgbubble.h"
#endif

#ifndef __GAME_GAME_H__
#include	"game\game.h"
#endif

#ifndef __SPR_SPRITES_H__
#include <sprites.h>
#endif


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcGeyserBubblePlatform::postInit()
{
	CNpcBubblePlatform::postInit();

	s32 minX, maxX, minY, maxY;

	m_npcPath.getPathXExtents( &minX, &maxX );
	m_npcPath.getPathYExtents( &minY, &maxY );

	m_thinkArea.x1 = minX;
	m_thinkArea.x2 = maxX;
	m_thinkArea.y1 = minY;
	m_thinkArea.y2 = maxY;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcGeyserBubblePlatform::render()
{
	SprFrame=0;
	if ( m_isActive || m_pop )
	{
		CPlatformThing::render();

		// Render
		if (canRender())
		{
			DVECTOR &renderPos=getRenderPos();

			if ( m_pop )
			{
				SprFrame = CGameScene::getSpriteBank()->printRotatedScaledSprite( FRM__BALLOONBURST, renderPos.vx, renderPos.vy - 16, 4096 << 1, 4096 << 1, 0, 6 );
				setRGB0( SprFrame, 128, 128, 255 );
			}
			else
			{
				// Evil hard coded Offsets
				POLY_FT4 *SprFrame = CGameScene::getSpriteBank()->printRotatedScaledSprite( FRM__BUBBLE_1, renderPos.vx, renderPos.vy - 16, m_scale, ONE, 0, 6 );
				setRGB0( SprFrame, 128, 128, 255 );
				SprFrame->u1++; SprFrame->u3++;
				SprFrame->v2++; SprFrame->v3++;
			}
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcGeyserBubblePlatform::processMovement( int _frames )
{
	if ( !isSetToShutdown() && !m_pop )
	{
		Pos.vy -= m_speed * _frames;

		if ( m_npcPath.getWaypointCount() )
		{
			s32 minY, maxY;

			m_npcPath.getPathYExtents( &minY, &maxY );

			if ( Pos.vy < minY )
			{
				m_lifetime = GameState::getOneSecondInFrames() >> 2;
				m_pop = true;

				if ( m_soundId == NOT_PLAYING )
				{
					m_soundId = (int) CSoundMediator::playSfx( CSoundMediator::SFX_BALLOON_POP, true, true );
				}
			}
		}
		else
		{
			if ( Pos.vy < 0 )
			{
				m_lifetime = GameState::getOneSecondInFrames() >> 2;
				m_pop = true;

				if ( m_soundId == NOT_PLAYING )
				{
					m_soundId = (int) CSoundMediator::playSfx( CSoundMediator::SFX_BALLOON_POP, true, true );
				}
			}
		}

		DVECTOR const &offset = CLevel::getCameraPos();

		s32 yPos = Pos.vy - offset.vy;

		if ( yPos < 0 )
		{
			setToShutdown();
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcGeyserBubblePlatform::processLifetime( int _frames )
{
	if ( m_pop )
	{
		if ( m_lifetime <= 0 )
		{
			setToShutdown();
		}
		else
		{
			m_lifetime = 0;
		}
	}
}

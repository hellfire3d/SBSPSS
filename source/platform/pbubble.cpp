/*=========================================================================

	pbubble.cpp

	Author:		CRB
	Created:
	Project:	Spongebob
	Purpose:

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __PLATFORM_PBUBBLE_H__
#include "platform\pbubble.h"
#endif

#ifndef __GAME_GAME_H__
#include	"game\game.h"
#endif

#ifndef __SPR_SPRITES_H__
#include <sprites.h>
#endif


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcBubblePlatform::postInit()
{
	CNpcPlatform::postInit();

	m_pop = false;

	s32 minX, maxX, minY, maxY;

	m_npcPath.getPathXExtents( &minX, &maxX );
	m_npcPath.getPathYExtents( &minY, &maxY );

	m_thinkArea.x1 = minX;
	m_thinkArea.x2 = maxX;
	m_thinkArea.y1 = minY;
	m_thinkArea.y2 = maxY;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcBubblePlatform::render()
{
	if ( m_isActive || m_pop )
	{
		CPlatformThing::render();

		// Render
		if (canRender())
		{
			DVECTOR &renderPos=getRenderPos();

			if ( m_pop )
			{
				POLY_FT4 *SprFrame = CGameScene::getSpriteBank()->printRotatedScaledSprite( FRM__BALLOONBURST, renderPos.vx, renderPos.vy - 16, 4096 << 1, 4096 << 1, 0, 10 );
				setRGB0( SprFrame, 128, 128, 255 );
			}
			else
			{
				// Evil hard coded Offsets
				POLY_FT4 *SprFrame = CGameScene::getSpriteBank()->printFT4( FRM__BUBBLE_1, renderPos.vx-16, renderPos.vy-32, 0, 0, 10 );
				setRGB0( SprFrame, 128, 128, 255 );
//				m_modelGfx->Render(renderPos);
			}
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcBubblePlatform::processMovement( int _frames )
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
			}
		}
		else
		{
			if ( Pos.vy < 0 )
			{
				m_lifetime = GameState::getOneSecondInFrames() >> 2;
				m_pop = true;
			}
		}

		DVECTOR	offset = CLevel::getCameraPos();

		s32 yPos = Pos.vy - offset.vy;

		if ( yPos < 0 )
		{
			setToShutdown();
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcBubblePlatform::processLifetime( int _frames )
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

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*const CRECT *CNpcBubblePlatform::getThinkBBox()
{
	CRECT objThinkBox = getCollisionArea();

	sBBox &thinkBBox = CThingManager::getThinkBBox();
	objThinkBox.x1 = thinkBBox.XMin;
	objThinkBox.x2 = thinkBBox.XMax;
	objThinkBox.y1 = thinkBBox.YMin;
	objThinkBox.y2 = thinkBBox.YMax;

	return &objThinkBox;
}*/
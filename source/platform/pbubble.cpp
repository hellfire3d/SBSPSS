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
				m_modelGfx->Render(renderPos);
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

const CRECT *CNpcBubblePlatform::getThinkBBox()
{
	CRECT objThinkBox = getCollisionArea();

	sBBox &thinkBBox = CThingManager::getThinkBBox();
	objThinkBox.x1 = thinkBBox.XMin;
	objThinkBox.x2 = thinkBBox.XMax;
	objThinkBox.y1 = thinkBBox.YMin;
	objThinkBox.y2 = thinkBBox.YMax;

	return &objThinkBox;
}
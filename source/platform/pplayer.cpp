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

void CNpcPlayerBubblePlatform::postInit()
{
	CNpcPlatform::postInit();

	m_pop = false;
}

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

			m_lifetime = GameState::getOneSecondInFrames() >> 1;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcPlayerBubblePlatform::render()
{
	if ( m_isActive )
	{
		CPlatformThing::render();

		// Render
		if (canRender())
		{
			DVECTOR &renderPos=getRenderPos();

			if ( m_pop )
			{
				CGameScene::getSpriteBank()->printRotatedScaledSprite( FRM__BALLOONBURST, renderPos.vx, renderPos.vy - 16, 4096 << 1, 4096 << 1, 0, 10 );
			}
			else
			{
				m_modelGfx->Render(renderPos);
			}
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int CNpcPlayerBubblePlatform::checkCollisionAgainst(CThing *_thisThing, int _frames)
{
	switch(_thisThing->getThingType())
	{
		case TYPE_PLAYERPROJECTILE:
			return( false );

		default:
		{
			int collided = false;

			if ( m_detectCollision && m_isActive && !isSetToShutdown() && !m_pop )
			{
				CRECT thisRect, thatRect;

				thisRect = getCollisionArea();
				thatRect = _thisThing->getCollisionArea();

				DVECTOR posDelta = getPosDelta();

				thisRect.y1 -= abs( posDelta.vy ) >> 1;
				thisRect.y2 += abs( posDelta.vy ) >> 1;

				posDelta = _thisThing->getPosDelta();

				thatRect.y1 -= abs( posDelta.vy ) >> 1;
				thatRect.y2 += abs( posDelta.vy ) >> 1;

				if(((thisRect.x1>=thatRect.x1&&thisRect.x1<=thatRect.x2)||(thisRect.x2>=thatRect.x1&&thisRect.x2<=thatRect.x2)||(thisRect.x1<=thatRect.x1&&thisRect.x2>=thatRect.x2))&&
				   ((thisRect.y1>=thatRect.y1&&thisRect.y1<=thatRect.y2)||(thisRect.y2>=thatRect.y1&&thisRect.y2<=thatRect.y2)||(thisRect.y1<=thatRect.y1&&thisRect.y2>=thatRect.y2)))
				{
					collided = true;
				}
			}

			return( collided );
		}
	}
}

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
	CNpcPlatform::postInit();

	m_pop = false;
	m_scale = ONE;
	m_startCollapse = false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CNpcCollapsingBubblePlatform::render()
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
				SprFrame = CGameScene::getSpriteBank()->printRotatedScaledSprite( FRM__BALLOONBURST, renderPos.vx, renderPos.vy - 16, 4096 << 1, 4096 << 1, 0, 10 );
				setRGB0( SprFrame, 128, 128, 255 );
			}
			else
			{
				// Evil hard coded Offsets
				POLY_FT4 *SprFrame = CGameScene::getSpriteBank()->printRotatedScaledSprite( FRM__BUBBLE_1, renderPos.vx, renderPos.vy - 16, m_scale, ONE, 0, 10 );
				setRGB0( SprFrame, 128, 128, 255 );
			}
		}
	}
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

int CNpcCollapsingBubblePlatform::checkCollisionAgainst(CThing *_thisThing, int _frames)
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

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcCollapsingAcridPlatform::render()
{

	CNpcCollapsingBubblePlatform::render();
	if (SprFrame)
	{
		setRGB0( SprFrame, 255, 255, 128 );
	}
}

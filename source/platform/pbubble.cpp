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
	setCollisionSize( 30, 30 );
	setCollisionCentreOffset( 0, -15 );

	calculateNonRotatedCollisionData();
	setCollisionAngle( m_tiltAngle >> 8 );

	m_pop = false;
	m_scale = ONE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcBubblePlatform::shutdown()
{
	if ( m_soundId != NOT_PLAYING )
	{
		CSoundMediator::stopAndUnlockSfx( (xmPlayingId) m_soundId );
	}

	CPlatformThing::shutdown();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcBubblePlatform::render()
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

int CNpcBubblePlatform::checkCollisionAgainst(CThing *_thisThing, int _frames)
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


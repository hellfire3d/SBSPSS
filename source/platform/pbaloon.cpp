/*=========================================================================

	pbaloon.h

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __PLATFORM_PBALOON_H__
#include "platform\pbaloon.h"
#endif

#ifndef __PLATFORM_PLATFORM_H__
#include "platform\platform.h"
#endif

#ifndef __GAME_GAME_H__
#include "game\game.h"
#endif

#ifndef __SPR_SPRITES_H__
#include <sprites.h>
#endif

#ifndef	__UTILS_HEADER__
#include	"utils\utils.h"
#endif


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CBridgeBalloon::render()
{
	sBBox			&ScrBBox=CThingManager::getRenderBBox();
	DVECTOR	const	&CamPos=CLevel::getCameraPos();

	u8 renderFlag = true;
	if ( Pos.vx < ScrBBox.XMin || Pos.vx > ScrBBox.XMax ) renderFlag=false;
	if ( Pos.vy < ScrBBox.YMin || Pos.vy > ScrBBox.YMax ) renderFlag=false;

	if ( renderFlag )
	{
		DVECTOR renderPos;
		renderPos.vx = Pos.vx - CamPos.vx;
		renderPos.vy = Pos.vy - CamPos.vy;

		CGameScene::getSpriteBank()->printFT4(FRM__BALLOON,renderPos.vx,renderPos.vy,0,0,10);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CBridgeBalloon::init()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CBridgeBalloon::shutdown()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int CBridgeBalloon::checkCollisionAgainst(CThing *_thisThing, int _frames)
{
	DVECTOR const &projectilePos = _thisThing->getPos();

	s32 xDist = projectilePos.vx - Pos.vx;
	s32 yDist = projectilePos.vy - Pos.vy;

	s32 dist = ( xDist * xDist ) + ( yDist * yDist );

	if ( dist < 5000 )
	{
		return( true );
	}
	else
	{
		return( false );
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcBalloonBridgePlatform::postInit()
{
	CNpcPlatform::postInit();

	m_balloonCount = BRIDGE_NUM_BALLOONS;

	for ( int i = 0 ; i < BRIDGE_NUM_BALLOONS ; i++ )
	{
		m_balloon[i] = new ("bridge balloon") CBridgeBalloon;
		m_balloon[i]->init();
	}

	m_targetHeight = m_base.vy + ( ( ( BRIDGE_NUM_BALLOONS - m_balloonCount ) * m_maxExtension ) / BRIDGE_NUM_BALLOONS );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcBalloonBridgePlatform::shutdown()
{
	for ( int i = 0 ; i < BRIDGE_NUM_BALLOONS ; i++ )
	{
		if ( m_balloon[i] )
		{
			m_balloon[i]->shutdown();

			delete( m_balloon[i] );

			m_balloon[i] = NULL;
		}
	}

	CNpcPlatform::shutdown();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcBalloonBridgePlatform::setWaypoints( sThingPlatform *ThisPlatform )
{
	u16	*PntList=(u16*)MakePtr(ThisPlatform,sizeof(sThingPlatform));

	u16 newXPos, newYPos;

	// get master platform init pos

	newXPos = (u16) *PntList;
	PntList++;
	newYPos = (u16) *PntList;
	PntList++;

	DVECTOR startPos;
	startPos.vx = ( newXPos << 4 ) + 8;
	startPos.vy = ( newYPos << 4 ) + 8;

	init( startPos );

	m_extension = 0;

	// get master platform max vertical extension

	newXPos = (u16) *PntList;
	PntList++;
	newYPos = (u16) *PntList;
	PntList++;

	m_maxExtension = abs( ( ( newYPos << 4 ) + 8 ) - startPos.vy );

	s32 minX, maxX, minY, maxY;

	m_npcPath.getPathXExtents( &minX, &maxX );
	m_npcPath.getPathYExtents( &minY, &maxY );

	m_thinkArea.x1 = minX;
	m_thinkArea.x2 = maxX;
	m_thinkArea.y1 = minY;
	m_thinkArea.y2 = maxY;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcBalloonBridgePlatform::processMovement( int _frames )
{
	if ( m_balloonCount )
	{
		m_velocity = m_targetHeight - Pos.vy;

		if ( m_velocity > 3 * _frames )
		{
			m_velocity = 3 * _frames;
		}
		else if ( m_velocity < -3 * _frames )
		{
			m_velocity = -3 * _frames;
		}
	}
	else
	{
		m_velocity = 3 * _frames;
	}

	Pos.vy += m_velocity;

	DVECTOR balloonPos;

	for ( int i = 0 ; i < BRIDGE_NUM_BALLOONS ; i++ )
	{
		balloonPos = Pos;
		balloonPos.vx += -20 + ( 40 * ( i > 1 ) );
		balloonPos.vy -= 30 + ( 20 * ( i % 2 ) );

		if ( m_balloon[i] )
		{
			m_balloon[i]->setPos( balloonPos );
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcBalloonBridgePlatform::render()
{
	if ( m_isActive )
	{
		CPlatformThing::render();

		for ( int i = 0 ; i < BRIDGE_NUM_BALLOONS ; i++ )
		{
			if ( m_balloon[i] )
			{
				m_balloon[i]->render();
			}
		}

		// Render
		if (canRender())
		{
			DVECTOR &renderPos=getRenderPos();

			m_modelGfx->Render(renderPos);
		}
		else if ( !m_balloonCount )
		{
			setToShutdown();
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int CNpcBalloonBridgePlatform::checkCollisionAgainst(CThing *_thisThing, int _frames)
{
	switch(_thisThing->getThingType())
	{
		case TYPE_PLAYERPROJECTILE:
		{
			for ( int i = 0 ; i < BRIDGE_NUM_BALLOONS ; i++ )
			{
				if ( m_balloon[i] )
				{
					if ( m_balloon[i]->checkCollisionAgainst( _thisThing, _frames ) )
					{
						m_balloon[i]->shutdown();

						delete( m_balloon[i] );

						m_balloon[i] = NULL;

						_thisThing->setToShutdown();

						m_balloonCount--;

						m_targetHeight = m_base.vy + ( ( ( BRIDGE_NUM_BALLOONS - m_balloonCount ) * m_maxExtension ) / BRIDGE_NUM_BALLOONS );

						if ( m_soundId == NOT_PLAYING )
						{
							m_soundId = (int) CSoundMediator::playSfx( CSoundMediator::SFX_BALLOON_POP, true );
						}

						break;
					}
				}
			}

			return( false );
		}

		default:
		{
			int collided = false;

			if ( m_detectCollision && m_isActive && !isSetToShutdown() )
			{
				CRECT thisRect, thatRect;

				thisRect = getCollisionArea();
				thatRect = _thisThing->getCollisionArea();

				DVECTOR const &thisPosDelta = getPosDelta();
				int	ThisAbsY=abs(thisPosDelta.vy)>>1;
				thisRect.y1 -= ThisAbsY;
				thisRect.y2 += ThisAbsY;

				DVECTOR const &ThatPosDelta = _thisThing->getPosDelta();
				int	ThatAbsY=abs(ThatPosDelta.vy)>>1;
				thatRect.y1 -= ThatAbsY;
				thatRect.y2 += ThatAbsY;

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

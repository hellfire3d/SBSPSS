/*=========================================================================

	hfalling.cpp

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __HAZARD_HFALLING_H__
#include "hazard\hfalling.h"
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

#ifndef	__PLAYER_PLAYER_H__
#include	"player\player.h"
#endif


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcFallingHazard::init()
{
	CNpcHazard::init();

	m_movementTimer = 2 * GameState::getOneSecondInFrames();

	m_respawnRate = 4;
	m_bounceFinish = false;
	m_spinFinish = false;
	m_rotation = 0;
	m_growing = true;
	m_scale = 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcFallingHazard::processMovement( int _frames )
{
	if ( m_growing )
	{
		m_scale = ( ( ( 2 * GameState::getOneSecondInFrames() ) - m_movementTimer ) << 12 ) / ( 2 * GameState::getOneSecondInFrames() );

		if ( m_movementTimer > 0 )
		{
			m_movementTimer -= _frames;

			if ( m_movementTimer < 0 )
			{
				m_movementTimer = 0;
			}
		}
		else
		{
			m_growing = false;

			m_movementTimer = 2 * GameState::getOneSecondInFrames();
		}
	}
	else
	{
		s8 groundHeight;
		s8 yMovement;

		if ( m_bounceFinish )
		{
			if ( m_bounceDir )
			{
				Pos.vx += 2 * _frames;
			}
			else
			{
				Pos.vx -= 2 * _frames;
			}

			Pos.vy += m_speed * _frames;

			/*if ( Pos.vy > ( m_bouncePos.vy + 32 ) )
			{
				m_bounceFinish = false;
			}
			else*/
			{
				if ( m_speed < 3 )
				{
					m_speed++;
				}
			}

			if ( m_spinFinish )
			{
				m_rotation += 64 * _frames;
				m_rotation &= 4095;
			}
		}
		else
		{
			if ( m_movementTimer > 0 )
			{
				m_movementTimer -= _frames;

				if ( m_movementTimer <= 0 )
				{
					Pos = m_base;
				}
				else
				{
					Pos.vx = m_base.vx + ( -3 + ( getRnd() % 7 ) );
					Pos.vy = m_base.vy + ( -3 + ( getRnd() % 7 ) );
				}
			}
			else
			{
				yMovement = 3 * _frames;

				groundHeight = CGameScene::getCollision()->getHeightFromGround( Pos.vx, Pos.vy, yMovement + 16 );

				if ( groundHeight < yMovement )
				{
					if ( m_soundId == NOT_PLAYING )
					{
						m_soundId = (int) CSoundMediator::playSfx( CSoundMediator::SFX_HAZARD__ACORN_LAND, true );
					}

					// colliding with ground

					Pos.vy += groundHeight;

					m_bounceFinish = true;
					m_speed = -5;
					m_bounceDir = getRnd() % 2;

					m_bouncePos = Pos;
				}
				else
				{
					// drop down

					Pos.vy += yMovement;
				}
			}
		}

		DVECTOR const &offset = CLevel::getCameraPos();

		s32 yPos = Pos.vy - offset.vy;

		if ( yPos > VidGetScrH() )
		{
			m_isActive = false;
			m_timerActive = true;
			m_timer = ( m_respawnRate - 1 ) * GameState::getOneSecondInFrames();
			m_growing = true;
			m_scale = 0;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcFallingHazard::processTimer( int _frames )
{
	m_timer -= _frames;

	if ( m_timer < 0 )
	{
		m_timerActive = false;
		m_isActive = true;
		Pos = m_base;
		m_movementTimer = 2 * GameState::getOneSecondInFrames();
		m_bounceFinish = false;
		m_spinFinish = false;
		m_rotation = 0;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcFallingHazard::collidedWith( CThing *_thisThing )
{
	if ( m_movementTimer <= 0 && m_isActive )
	{
		switch(_thisThing->getThingType())
		{
			case TYPE_PLAYER:
			{
				CPlayer *player = (CPlayer *) _thisThing;

				if ( !player->isRecoveringFromHit() )
				{
					player->takeDamage( DAMAGE__HIT_ENEMY );
				}

				m_bounceFinish = true;
				m_spinFinish = true;
				m_speed = -5;
				m_bounceDir = getRnd() % 2;

				if( m_soundId != NOT_PLAYING )
				{
					CSoundMediator::stopAndUnlockSfx( (xmPlayingId) m_soundId );
				}

				m_soundId = (int) CSoundMediator::playSfx( CSoundMediator::SFX_HAZARD__ACORN_LAND, true );

				m_bouncePos = Pos;

				break;
			}

			default:
				break;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcFallingHazard::setWaypoints( sThingHazard *ThisHazard )
{
	int pointNum;

	u16	*PntList=(u16*)MakePtr(ThisHazard,sizeof(sThingHazard));

	u16 newXPos, newYPos;

	m_npcPath.setWaypointCount( ThisHazard->PointCount - 1 );

	newXPos = (u16) *PntList;
	setWaypointPtr( PntList );
	PntList++;
	newYPos = (u16) *PntList;
	PntList++;

	DVECTOR startPos;
	startPos.vx = ( newXPos << 4 ) + 8;
	startPos.vy = ( newYPos << 4 ) + 16;

	Pos = startPos;
	m_base = Pos;

	s32 minX, maxX, minY, maxY;

	m_npcPath.getPathXExtents( &minX, &maxX );
	m_npcPath.getPathYExtents( &minY, &maxY );

	m_thinkArea.x1 = minX;
	m_thinkArea.x2 = maxX;
	m_thinkArea.y1 = minY;
	m_thinkArea.y2 = maxY;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcFallingHazard::render()
{
	CHazardThing::render();

	if (canRender())
	{
		DVECTOR &renderPos=getRenderPos();

		sBBox boundingBox = m_modelGfx->GetBBox();
		renderPos.vy -= boundingBox.YMax - boundingBox.YMin;

		SVECTOR rotation;
		rotation.vx = 0;
		rotation.vy = 0;
		rotation.vz = m_rotation;

		VECTOR scale;
		scale.vx = m_scale;
		scale.vy = -m_scale;
		scale.vz = m_scale;

		m_modelGfx->RenderNoClip(renderPos,&rotation,&scale);
	}
}

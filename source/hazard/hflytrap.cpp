/*=========================================================================

	hflytrap.cpp

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __HAZARD_HFLYTRAP_H__
#include "hazard\hflytrap.h"
#endif

#ifndef __VID_HEADER_
#include "system\vid.h"
#endif

#ifndef __LEVEL_LEVEL_H__
#include "level\level.h"
#endif

#ifndef __GAME_GAME_H__
#include	"game\game.h"
#endif

#ifndef	__PLAYER_PLAYER_H__
#include	"player\player.h"
#endif


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcFlyTrapHazard::init()
{
	CNpcHazard::init();

	m_rotation = -1024;
	m_shut = true;
	m_contact = false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcFlyTrapHazard::processMovement( int _frames )
{
	if ( m_shut )
	{
		if ( m_contact )
		{
			if ( m_timer <= 0 )
			{
				m_rotation += 64 * _frames;

				if ( m_rotation > -256 )
				{
					m_rotation = -256;
					m_shut = false;
					m_contact = false;
					m_timer = GameState::getOneSecondInFrames();

					if ( m_soundId == NOT_PLAYING )
					{
						m_soundId = (int) CSoundMediator::playSfx( CSoundMediator::SFX_CLAM_ATTACK, true );
					}
				}
			}
			else
			{
				m_timer -= _frames;
			}
		}
	}
	else if ( !m_shut )
	{
		if ( m_timer <= 0 )
		{
			m_rotation -= 64 * _frames;

			if ( m_rotation < -1024 )
			{
				m_rotation = -1024;
				m_shut = true;
			}
		}
		else
		{
			if ( m_contact )
			{
				m_contact = false;
			}
			else
			{
				if ( m_timer > 0 )
				{
					m_timer -= _frames;
				}
			}
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcFlyTrapHazard::render()
{
	CHazardThing::render();

	if (canRender())
	{
		DVECTOR &renderPos=getRenderPos();

		SVECTOR rotation;
		rotation.vx = m_rotation;
		rotation.vy = 0;
		rotation.vz = 0;

		VECTOR scale;
		scale.vx = ONE;
		scale.vy = ONE;
		scale.vz = ONE;

		m_modelGfx->Render(renderPos,&rotation,&scale);

		sBBox boundingBox = m_modelGfx->GetBBox();

		s32 vertSize = ( ( boundingBox.YMax - boundingBox.YMin ) * rsin( 1024 - m_rotation ) ) >> 12;
		if ( vertSize < 20 )
		{
			vertSize = 20;
		}

		setCollisionSize( ( boundingBox.XMax - boundingBox.XMin ), vertSize );
		//setCollisionSize( ( boundingBox.XMax - boundingBox.XMin ), ( boundingBox.YMax - boundingBox.YMin ) );
		setCollisionCentreOffset( ( boundingBox.XMax + boundingBox.XMin ) >> 1, -vertSize >> 1 );
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcFlyTrapHazard::collidedWith( CThing *_thisThing )
{
	if ( m_isActive )
	{
		switch(_thisThing->getThingType())
		{
			case TYPE_PLAYER:
			{
				if ( !m_contact )
				{
					if ( m_shut )
					{
						m_contact = true;

						m_timer = GameState::getOneSecondInFrames();
					}
					else
					{
						if ( m_timer > 0 )
						{
							m_contact = true;

							m_timer = GameState::getOneSecondInFrames();
						}
					}
				}

				if ( m_rotation > -512 )
				{
					CPlayer *player = (CPlayer *) _thisThing;

					if ( !player->isRecoveringFromHit() )
					{
						player->takeDamage( DAMAGE__HIT_ENEMY );
					}
				}

				break;
			}

			default:
				break;
		}
	}
}

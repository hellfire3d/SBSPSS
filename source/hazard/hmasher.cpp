/*=========================================================================

	hmasher.cpp

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __HAZARD_HMASHER_H__
#include "hazard\hmasher.h"
#endif

#ifndef __LAYER_COLLISION_H__
#include "level\layercollision.h"
#endif

#include	"game\game.h"


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcMasherHazard::init()
{
	CNpcHazard::init();

	m_state = MASHER_DROPPING;
	m_pauseTimer = 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcMasherHazard::processMovement( int _frames )
{
	switch ( m_state )
	{
		case MASHER_DROPPING:
		{
			if ( m_pauseTimer <= 0 )
			{
				s8 yMovement = 3 * _frames;

				s8 groundHeight;

				groundHeight = CGameScene::getCollision()->getHeightFromGround( Pos.vx, Pos.vy, yMovement + 16 );

				if ( groundHeight < yMovement )
				{
					// colliding with ground

					Pos.vy += groundHeight;

					// pause and change direction

					m_state = MASHER_RISING;

					CSoundMediator::playSfx( CSoundMediator::SFX_HAZARD__FALLING_ROCK_LAND, false, true );
				}
				else
				{
					// drop down

					Pos.vy += yMovement;
				}
			}
			else
			{
				m_pauseTimer -= _frames;
			}

			break;
		}

		case MASHER_RISING:
		{
			if ( m_base.vx - Pos.vx == 0 && m_base.vy - Pos.vy == 0 )
			{
				m_state = MASHER_DROPPING;

				m_pauseTimer = GameState::getOneSecondInFrames();
			}
			else
			{
				// return to original position

				Pos.vy -= 3 * _frames;

				if ( Pos.vy < m_base.vy )
				{
					Pos.vy = m_base.vy;
				}
			}

			break;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcMasherHazard::setGraphic( sThingHazard *ThisHazard )
{
	m_modelGfx = new ("ModelGfx") CModelGfx;
	m_modelGfx->SetModel( ThisHazard->Gfx );

	sBBox boundingBox = m_modelGfx->GetBBox();
	boundingBox.YMin = boundingBox.YMax - 18;
	setCollisionSize( ( boundingBox.XMax - boundingBox.XMin ), ( boundingBox.YMax - boundingBox.YMin ) );
	setCollisionCentreOffset( ( boundingBox.XMax + boundingBox.XMin ) >> 1, ( boundingBox.YMax + boundingBox.YMin ) >> 1 );
}

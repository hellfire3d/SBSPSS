/*=========================================================================

	praft.cpp

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __PLATFORM_PRAFT_H__
#include "platform\praft.h"
#endif

#ifndef	__UTILS_HEADER__
#include	"utils\utils.h"
#endif

#ifndef __GAME_GAME_H__
#include "game\game.h"
#endif


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcRaftPlatform::postInit()
{
	m_npcPath.setPathType( CNpcPath::SINGLE_USE_PATH );

	m_isActivated = false;

	sBBox boundingBox = m_modelGfx->GetBBox();
	setCollisionSize( ( boundingBox.XMax - boundingBox.XMin ), ( boundingBox.YMax - boundingBox.YMin ) );
	setCollisionCentreOffset( ( boundingBox.XMax + boundingBox.XMin ) >> 1, ( ( boundingBox.YMax + boundingBox.YMin ) >> 1 ) - 16 );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcRaftPlatform::processMovement( int _frames )
{
	if ( m_isActivated )
	{
		s32 maxHeight = 20;
		s32 distX, distY;
		s32 fallSpeed = 3;
		s8 yMovement = fallSpeed * _frames;
		s32 groundHeight;

		s32 moveX = 0;
		s32 moveY = 0;

		// ignore y component of waypoint, since we are stuck to the ground

		bool pathComplete;

		m_npcPath.thinkFlat( Pos, &pathComplete, &distX, &distY, &m_heading );

		if ( pathComplete )
		{
			m_isActive = false;
			m_timer = getRnd() % ( 4 * GameState::getOneSecondInFrames() );
			m_timerType = NPC_PLATFORM_TIMER_RESPAWN;
			m_isActivated = false;
			m_npcPath.resetPath();
		}
		else
		{
			// check for collision

			distX = distX / abs( distX );

			if ( m_layerCollision->getHeightFromGround( Pos.vx + ( distX * m_data[m_type].speed * _frames ), Pos.vy ) < -maxHeight )
			{
				// there is an obstacle in the way, increment the path point (hopefully this will resolve the problem)

				m_npcPath.incPath();
			}
			else
			{
				// check for vertical movement

				groundHeight = m_layerCollision->getHeightFromGround( Pos.vx, Pos.vy, yMovement + 16 );

				if ( groundHeight <= yMovement )
				{
					// groundHeight <= yMovement indicates either just above ground or on or below ground

					moveX = distX * m_data[m_type].speed * _frames;
					moveY = groundHeight;
				}
				else
				{
					// fall

					moveY = yMovement;
				}
			}
		}

		Pos.vx += moveX;
		Pos.vy += moveY;
	}
	else
	{
		if ( m_contact )
		{
			m_isActivated = true;
		}
	}
}
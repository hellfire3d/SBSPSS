/*=========================================================================

	nprojjf.cpp

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/

#ifndef __ENEMY_NPC_H__
#include "enemy\npc.h"
#endif

#ifndef __ENEMY_NPROJJF_H__
#include "enemy\nprojjf.h"
#endif

#ifndef	__UTILS_HEADER__
#include "utils\utils.h"
#endif

#ifndef __GAME_GAME_H__
#include	"game\game.h"
#endif

#ifndef	__PLAYER_PLAYER_H__
#include	"player\player.h"
#endif

#ifndef __VID_HEADER_
#include "system\vid.h"
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcSmallJellyfishProjectileEnemy::postInit()
{
	CNpcEnemy::postInit();

	m_lifetime = 8 * GameState::getOneSecondInFrames();

	m_RGB = 255 + ( 128 << 8 ) + ( 255 << 16 );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcSmallJellyfishProjectileEnemy::think(int _frames)
{
	int moveFrames = _frames;

	if ( moveFrames > 2 )
	{
		// make sure enemies don't go berserk if too many frames are dropped

		moveFrames = 2;
	}

	CEnemyThing::think(_frames);

	processGenericGetUserDist( moveFrames, &playerXDist, &playerYDist );
	playerXDistSqr = playerXDist * playerXDist;
	playerYDistSqr = playerYDist * playerYDist;

	if ( m_isCaught )
	{
		processCoralBlower( moveFrames );
	}
	else
	{
		if ( m_isActive )
		{
			if ( m_animPlaying )
			{
				s32 frameCount;

				frameCount = getFrameCount();

				s32 frameShift = ( moveFrames << 8 ) >> 1;

				if ( ( frameCount << 8 ) - m_frame > frameShift )
				{
					m_frame += frameShift;
				}
				else
				{
					m_frame = ( frameCount - 1 ) << 8;
					m_animPlaying = false;
				}
			}

			switch ( this->m_controlFunc )
			{
				case NPC_CONTROL_NONE:
					return;

				case NPC_CONTROL_MOVEMENT:
					if ( !processSensor() )
					{
						processMovement( moveFrames );
					}
					else
					{
						processClose( moveFrames );
					}

					break;

				case NPC_CONTROL_SHOT:
					processShot( moveFrames );

					break;

				case NPC_CONTROL_CLOSE:
					processClose( moveFrames );

					break;

				case NPC_CONTROL_COLLISION:
					processCollision();

					break;
			}

			processGraphicFlipping();
		}
	}

	if ( !m_isCaught )
	{
		processTimer( moveFrames );
	}

	m_lifetime -= _frames;

	if ( m_lifetime < 0 )
	{
		setToShutdown();
	}
}

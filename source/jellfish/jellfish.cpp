/*=========================================================================

	jellfish.h

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __JELLFISH_JELLFISH_H__
#include "jellfish\jellfish.h"
#endif

#ifndef __GAME_GAME_H__
#include	"game\game.h"
#endif

#ifndef __ENEMY_NSJBACK_H__
#include "enemy\nsjback.h"
#endif

#ifndef __ENEMY_NPC_H__
#include "enemy\npc.h"
#endif

#ifndef	__UTILS_HEADER__
#include	"utils\utils.h"
#endif

#ifndef __VID_HEADER_
#include "system\vid.h"
#endif


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

u8 CJellyfishGenerator::m_jellyfishCount;
s32 CJellyfishGenerator::m_timer;
u8 CJellyfishGenerator::m_on;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CJellyfishGenerator::init()
{
	m_timer = 1 * GameState::getOneSecondInFrames();
	m_jellyfishCount = 0;

	m_on = true;

	switch( CLevel::getCurrentChapter() )
	{
		case 1:
		{
			switch( CLevel::getCurrentChapterLevel() )
			{
				case 2:
				{
					m_on = false;
				}
			}

			break;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CJellyfishGenerator::think( int _frames, CLevel *level )
{
	if ( m_on )
	{
		if ( m_timer <= 0 )
		{
			if ( m_jellyfishCount < 10 )
			{
				// add jellyfish

				m_timer = 1 * GameState::getOneSecondInFrames();

				m_jellyfishCount++;

				CNpcEnemy *enemy;
				enemy = new( "jellyfish" ) CNpcSmallJellyfishBackgroundEnemy;
				ASSERT(enemy);
				enemy->setType( CNpcEnemy::NPC_SMALL_JELLYFISH_BACKGROUND );
				enemy->init();
				enemy->setLayerCollision( level->getCollisionLayer() );

				DVECTOR	offset = CLevel::getCameraPos();

				DVECTOR startPos;
				if ( ( getRnd() % 10 ) > 4 )
				{
					CNpcSmallJellyfishBackgroundEnemy *jfish = ( CNpcSmallJellyfishBackgroundEnemy * ) enemy;
					jfish->setTargetHeading( 0 );
					startPos.vx = offset.vx - 20;
				}
				else
				{
					CNpcSmallJellyfishBackgroundEnemy *jfish = ( CNpcSmallJellyfishBackgroundEnemy * ) enemy;
					jfish->setTargetHeading( 2048 );
					startPos.vx = offset.vx + VidGetScrW() + 20;
				}

				startPos.vy = offset.vy + ( getRnd() % VidGetScrH() );

				enemy->setStartPos( startPos.vx >> 4, startPos.vy >> 4 );
				enemy->postInit();
			}
		}
		else
		{
			m_timer -= _frames;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CJellyfishGenerator::decCounter()
{
	if ( m_jellyfishCount > 0 )
	{
		m_jellyfishCount--;
	}
}

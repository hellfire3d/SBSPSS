/*=========================================================================

	pfallnor.cpp

	Author:		CRB
	Created:
	Project:	Spongebob
	Purpose:

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __PLATFORM_PFALLNOR_H__
#include "platform\pfallnor.h"
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


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcFallingNoRespawnPlatform::postInit()
{
	CNpcPlatform::postInit();

	m_npcPath.setPathType( CNpcPath::SINGLE_USE_PATH );

	m_spinFinish = false;
	m_rotation = 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcFallingNoRespawnPlatform::processMovement( int _frames )
{
	s32 moveX = 0, moveY = 0;
	s32 distX, distY, heading;
	bool pathComplete;

	DVECTOR const &offset = CLevel::getCameraPos();

	if ( m_spinFinish )
	{
		m_rotation += 64 * _frames;
		m_rotation &= 4095;

		if ( m_bounceDir )
		{
			Pos.vx += 2 * _frames;
		}
		else
		{
			Pos.vx -= 2 * _frames;
		}

		Pos.vy += m_speed * _frames;

		if ( m_speed < 12 )
		{
			m_speed++;
		}

		s32 yPos = Pos.vy - offset.vy;

		if ( yPos < 0 || yPos > VidGetScrH() )
		{
			setToShutdown();
		}
	}
	else
	{
		m_npcPath.thinkVertical( Pos, &pathComplete, &distX, &distY, &heading );

		if ( pathComplete )
		{
			m_spinFinish = true;
			m_speed = -5;
			m_bounceDir = getRnd() % 2;

			if( m_soundId != NOT_PLAYING )
			{
				CSoundMediator::stopAndUnlockSfx( (xmPlayingId) m_soundId );
			}

			m_soundId = (int) CSoundMediator::playSfx( CSoundMediator::SFX_HAZARD__ACORN_LAND, true );
		}
		else
		{
			moveX = 0;
			moveY = m_speed * _frames;

			if ( heading == 3072 )
			{
				moveY = -moveY;
			}

			s32 groundHeight = CGameScene::getCollision()->getHeightFromGround( Pos.vx + moveX, Pos.vy + moveY, 16 );

			if ( groundHeight < moveY )
			{
				if ( ( CGameScene::getCollision()->getCollisionBlock( Pos.vx, Pos.vy + groundHeight + 8 ) & COLLISION_TYPE_MASK ) != COLLISION_TYPE_FLAG_DEATH_FALL )
				{
					moveY = groundHeight;
					moveX = 2 * _frames;
				}
			}
		}

		s32 yPos = Pos.vy - offset.vy;

		if ( yPos > VidGetScrH() + 100 )
		{
			setToShutdown();
		}
	}

	Pos.vx += moveX;
	Pos.vy += moveY;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcFallingNoRespawnPlatform::render()
{
	if ( m_isActive )
	{
		CPlatformThing::render();

		// Render
		if (canRender())
		{
			DVECTOR &renderPos=getRenderPos();

			SVECTOR rotation;
			rotation.vx = 0;
			rotation.vy = 0;
			rotation.vz = m_rotation;

			VECTOR scale;
			scale.vx = ONE;
			scale.vy = ONE;
			scale.vz = ONE;

			m_modelGfx->Render(renderPos,&rotation,&scale);
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int CNpcFallingNoRespawnPlatform::checkCollisionAgainst(CThing *_thisThing, int _frames)
{
	if ( m_spinFinish )
	{
		return( false );
	}
	else
	{
		return( CNpcPlatform::checkCollisionAgainst( _thisThing, _frames ) );
	}
}

/*=========================================================================

	pseesaw.cpp

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __PLATFORM_PSEESAW_H__
#include "platform\pseesaw.h"
#endif

#ifndef __GAME_GAME_H__
#include	"game\game.h"
#endif

#ifndef __VID_HEADER_
#include "system\vid.h"
#endif

#ifndef	__UTILS_HEADER__
#include	"utils\utils.h"
#endif


void CNpcSeesawPlatform::postInit()
{
	CNpcPlatform::postInit();

	m_angularVelocity = 0;
	m_currentAngle = 0;
}

void CNpcSeesawPlatform::processMovement( int _frames )
{
	s32 forceActing = 0;

	if ( m_contact )
	{
		// find if user is to left or right of platform centre

		CPlayer *player = GameScene.getPlayer();

		DVECTOR const &playerPos = player->getPos();

		s32 distX = playerPos.vx - this->Pos.vx;

		if ( distX > 0 )
		{
			forceActing = 64 * _frames;

			m_angularVelocity += forceActing;
		}
		else if ( distX < 0 )
		{
			forceActing = -64 * _frames;

			m_angularVelocity += forceActing;
		}
	}

	if ( !forceActing && m_angularVelocity )
	{
		// reduce velocity until 0

		s32 m_angularResistance = -( 64 * _frames * m_angularVelocity ) / abs( m_angularVelocity );

		if ( m_angularResistance < -m_angularVelocity )
		{
			m_angularResistance = -m_angularVelocity;
		}
		else if ( m_angularResistance > m_angularVelocity )
		{
			m_angularResistance = m_angularVelocity;
		}

		m_angularVelocity += m_angularResistance;
	}

	if ( m_angularVelocity > ( 20 << 8 ) )
	{
		m_angularVelocity = 20 << 8;
	}
	else if ( m_angularVelocity < -( 20 << 8 ) )
	{
		m_angularVelocity = -( 20 << 8 );
	}

	s32 newAngle = m_currentAngle + m_angularVelocity;

	if ( newAngle > ( 340 << 8 ) )
	{
		newAngle = 340 << 8;
		m_angularVelocity = 0;
	}
	else if ( newAngle < -( 340 << 8 ) )
	{
		newAngle = -( 340 << 8 );
		m_angularVelocity = 0;
	}

	if ( m_currentAngle != newAngle )
	{
		m_currentAngle = newAngle;

		if ( m_soundId == NOT_PLAYING )
		{
			m_soundId = (int) CSoundMediator::playSfx( CSoundMediator::SFX_WORLD_OBJECT__SEESAW, true );
		}
	}

	setCollisionAngle( newAngle >> 8 );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcSeesawPlatform::render()
{
	if ( m_isActive )
	{
		CPlatformThing::render();

		if (canRender())
		{
			DVECTOR &renderPos=getRenderPos();
			SVECTOR rotation;

			rotation.vx = 0;
			rotation.vy = 0;
			rotation.vz = getCollisionAngle();

			VECTOR scale;
			scale.vx = ONE;
			scale.vy = ONE;
			scale.vz = ONE;

			m_modelGfx->Render(renderPos,&rotation,&scale);
		}
	}
}

/*=========================================================================

	pbounce.cpp

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __PLATFORM_PBOUNCE_H__
#include "platform\pbounce.h"
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


void CNpcBouncePlatform::postInit()
{
	CNpcPlatform::postInit();

	m_vertScale = 0;
	m_vertVelocity = 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcBouncePlatform::think( int _frames )
{
	if ( m_contact )
	{
		CPlayer *player = GameScene.getPlayer();

		if ( m_vertVelocity > 20 && m_vertScale < -100 )
		{
			player->springPlayerUp();
		}

		s16 vertForce = -30 * _frames;

		m_vertVelocity += vertForce;

		m_contact = false;
	}

	s32 resistance = -( 10 * _frames * m_vertScale ) >> 8;

	if ( m_vertScale > 0 && resistance > -1 )
	{
		resistance = -1;
	}
	else if ( m_vertScale < 0 && resistance < 1 )
	{
		resistance = 1;
	}

	// get direction of resistance

	m_vertVelocity += resistance;

	m_vertScale += m_vertVelocity;

	if ( m_vertVelocity )
	{
		s32 absVertVelocity = abs( m_vertVelocity );

		m_vertVelocity += -m_vertVelocity / abs( m_vertVelocity );

		if ( abs( m_vertVelocity ) < 10 )
		{
			m_vertVelocity = 0;
		}

		if ( m_vertVelocity > 60 )
		{
			m_vertVelocity = 60;
		}
		else if ( m_vertVelocity < -60 )
		{
			m_vertVelocity = -60;
		}
	}

	sBBox boundingBox = m_modelGfx->GetBBox();

	s32 newCollisionHeight = ( ( ONE + m_vertScale ) * ( boundingBox.YMax - boundingBox.YMin ) ) >> 12;
	setCollisionSize( ( boundingBox.XMax - boundingBox.XMin ), newCollisionHeight - 2 );

	calculateNonRotatedCollisionData();

	CPlatformThing::think(_frames);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcBouncePlatform::render()
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
			rotation.vz = 0;

			VECTOR scale;
			scale.vx = ONE;
			scale.vy = ONE + m_vertScale;
			scale.vz = ONE;

			m_modelGfx->Render(renderPos,&rotation,&scale);
		}

	}
}
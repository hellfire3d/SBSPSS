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

	CPlatformThing::think(_frames);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcBouncePlatform::render()
{
	if ( m_isActive )
	{
		CPlatformThing::render();

		// Render
		DVECTOR renderPos;
		DVECTOR	offset = CLevel::getCameraPos();

		renderPos.vx = Pos.vx - offset.vx;
		renderPos.vy = Pos.vy - offset.vy;

		CRECT collisionRect = getCollisionArea();
		collisionRect.x1 -= Pos.vx;
		collisionRect.x2 -= Pos.vx;
		collisionRect.y1 -= Pos.vy;
		collisionRect.y2 -= Pos.vy;

		if ( renderPos.vx + collisionRect.x2 >= 0 && renderPos.vx + collisionRect.x1 <= VidGetScrW() )
		{
			if ( renderPos.vy + collisionRect.y2 >= 0 && renderPos.vy + collisionRect.y1 <= VidGetScrH() )
			{
				SVECTOR rotation;
				rotation.vx = 0;
				rotation.vy = 0;
				rotation.vz = 0;

				VECTOR scale;
				scale.vx = ONE;
				scale.vy = ONE + m_vertScale;
				scale.vz = ONE;

				m_modelGfx->Render(renderPos,&rotation,&scale);

#if defined (__USER_paul__) || defined (__USER_charles__)
	DVECTOR	centre;
	DVECTOR size;
	int		halfLength;
	int		x1,y1,x2,y2;

	centre=getCollisionCentre();
	size=getCollisionSize();
	halfLength=size.vx>>1;

	x1=-halfLength*mcos(getCollisionAngle()&4095)>>12;
	y1=-halfLength*msin(getCollisionAngle()&4095)>>12;
	x2=+halfLength*mcos(getCollisionAngle()&4095)>>12;
	y2=+halfLength*msin(getCollisionAngle()&4095)>>12;

	centre.vx-=offset.vx;
	centre.vy-=offset.vy;
	x1+=centre.vx;
	y1+=centre.vy;
	x2+=centre.vx;
	y2+=centre.vy;

	DrawLine(x1,y1,x2,y2,0,255,0,0);
#endif
			}
		}
	}
}
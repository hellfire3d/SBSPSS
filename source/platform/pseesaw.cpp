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

		DVECTOR playerPos = player->getPos();

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

	m_currentAngle = newAngle;

	setCollisionAngle( newAngle >> 8 );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcSeesawPlatform::render()
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
				rotation.vz = getCollisionAngle();

				VECTOR scale;
				scale.vx = ONE;
				scale.vy = ONE;
				scale.vz = ONE;

				m_modelGfx->Render(renderPos,&rotation,&scale);

#if defined (__USER_paul__) || defined (__USER_charles__)
	DVECTOR size;
	DVECTOR	centre;
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

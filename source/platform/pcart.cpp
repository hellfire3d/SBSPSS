/*=========================================================================

	pcart.cpp

	Author:		CRB
	Created:
	Project:	Spongebob
	Purpose:

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __PLATFORM_PCART_H__
#include "platform\pcart.h"
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

void CNpcCartPlatform::postInit()
{
	CNpcPlatform::postInit();

	m_npcPath.setPathType( CNpcPath::SINGLE_USE_PATH );

	m_carSpeed = m_data[m_type].speed << 8;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcCartPlatform::processMovement( int _frames )
{
	s32 fallSpeed = 2;
	s8 yMovement = fallSpeed * _frames;
	s32 distX, distY, heading;
	s32 groundHeight;
	s32 moveX = 0;
	s32 moveY = 0;

	bool pathComplete;

	m_npcPath.thinkFlat( Pos, &pathComplete, &distX, &distY, &heading );

	if ( !pathComplete )
	{
		moveX = ( m_carSpeed >> 8 ) * _frames;

		if ( heading == 2048 )
		{
			moveX = -moveX;
		}
	}

	// check for vertical movement

	s32 checkDist = yMovement + 50;

	groundHeight = m_layerCollision->getHeightFromGround( Pos.vx + moveX, Pos.vy, checkDist );

	if ( groundHeight < checkDist )
	{
		// groundHeight <= yMovement indicates either just above ground or on or below ground

		moveY = groundHeight;
	}
	else
	{
		// fall

		moveY = yMovement;
	}

	if ( moveY < 0 )
	{
		m_carSpeed -= 20;

		if ( m_carSpeed < ( 2 << 8 ) )
		{
			m_carSpeed = ( 2 << 8 );
		}
	}
	else if ( moveY > 0 )
	{
		m_carSpeed += 20;

		if ( m_carSpeed > ( 6 << 8 ) )
		{
			m_carSpeed = ( 6 << 8 );
		}
	}

	Pos.vx += moveX;
	Pos.vy += moveY;

	// sort out draw rotation

	DVECTOR testPos1, testPos2;

	testPos1 = testPos2 = Pos;
	testPos1.vx -= 10;
	testPos2.vx += 10;

	u8 sensorDist = 16;

	s32 yDiff;

	yDiff = m_layerCollision->getHeightFromGround( testPos1.vx, testPos1.vy, sensorDist + 1 );

	if ( yDiff <= sensorDist )
	{
		// only use if there is ground present

		testPos1.vy += yDiff;
	}

	yDiff = m_layerCollision->getHeightFromGround( testPos2.vx, testPos2.vy, sensorDist + 1 );

	if ( yDiff <= sensorDist )
	{
		// only use if there is ground present

		testPos2.vy += yDiff;
	}

	s32 xDist = testPos2.vx - testPos1.vx;
	s32 yDist = testPos2.vy - testPos1.vy;

	heading = ratan2( yDist, xDist );

	setCollisionAngle( heading );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcCartPlatform::render()
{
	if ( m_isActive )
	{
		CPlatformThing::render();

		// Render
		DVECTOR renderPos;
		DVECTOR	offset = CLevel::getCameraPos();

		renderPos.vx = Pos.vx - offset.vx ;
		renderPos.vy = Pos.vy - offset.vy ;

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

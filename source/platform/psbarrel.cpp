/*=========================================================================

	psbarrel.h

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __PLATFORM_PSBARREL_H__
#include "platform\psbarrel.h"
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


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcSteerableBarrelPlatform::postInit()
{
	CNpcPlatform::postInit();

	sBBox boundingBox = m_modelGfx->GetBBox();
	setCollisionSize( ( boundingBox.XMax - boundingBox.XMin ), ( boundingBox.YMax - boundingBox.YMin ) );
	setCollisionCentreOffset( ( boundingBox.XMax + boundingBox.XMin ) >> 1, ( ( boundingBox.YMax + boundingBox.YMin ) >> 1 ) - 32 );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcSteerableBarrelPlatform::processMovement( int _frames )
{
	s32 maxHeight = 20;
	s32 fallSpeed = 3;
	s8 yMovement = fallSpeed * _frames;
	s32 groundHeight;

	s32 moveX = 0;
	s32 moveY = 0;

	if ( m_contact )
	{
		CPlayer *player = GameScene.getPlayer();

		DVECTOR playerPos = player->getPos();

		s32 playerX = playerPos.vx - this->Pos.vx;

		if ( playerX > 5 )
		{
			// roll barrel right

			moveX = m_data[m_type].speed * _frames;
		}
		else if ( playerX < -5 )
		{
			moveX = -m_data[m_type].speed * _frames;
		}

		// check for collision

		if ( m_layerCollision->getHeightFromGround( Pos.vx + moveX, Pos.vy ) < -maxHeight )
		{
			moveX = 0;
		}

		if ( moveX > 0 )
		{
			m_rotation += 30 * _frames;
			m_rotation &= 4095;
		}
		else if ( moveX < 0 )
		{
			m_rotation -= 30 * _frames;
			m_rotation &= 4095;
		}
	}

	// check for vertical movement

	groundHeight = m_layerCollision->getHeightFromGround( Pos.vx + moveX, Pos.vy, yMovement + 16 );

	if ( groundHeight <= yMovement )
	{
		// groundHeight <= yMovement indicates either just above ground or on or below ground

		moveY = groundHeight;
	}
	else
	{
		// fall

		moveY = yMovement;
	}

	Pos.vx += moveX;
	Pos.vy += moveY;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcSteerableBarrelPlatform::render()
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
				rotation.vz = m_rotation;

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

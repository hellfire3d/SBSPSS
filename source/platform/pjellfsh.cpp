/*=========================================================================

	pjellfsh.h

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __PLATFORM_PJELLFSH_H__
#include "platform\pjellfsh.h"
#endif

#ifndef __LEVEL_LEVEL_H__
#include "level\level.h"
#endif

#ifndef __VID_HEADER_
#include "system\vid.h"
#endif

#ifndef	__UTILS_HEADER__
#include	"utils\utils.h"
#endif

#ifndef __GAME_GAME_H__
#include	"game\game.h"
#endif



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcJellyfishPlatform::postInit()
{
	CNpcLinearPlatform::postInit();

	m_vertScale = 0;
	m_dipCount = 0;
	m_dipOffset = 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcJellyfishPlatform::collidedWith( CThing *_thisThing )
{
	switch(_thisThing->getThingType())
	{
		case TYPE_PLAYER:
		{
			if ( m_detectCollision && m_isActive )
			{
				CPlayer *player;
				DVECTOR	playerPos;
				CRECT	collisionArea;
				CRECT	playerArea;

				// Only interested in SBs feet colliding with the box (pkg)
				player=(CPlayer*)_thisThing;
				playerPos=player->getPos();
				playerArea=player->getCollisionArea();
				collisionArea=getCollisionArea();

				s32 height = getHeightFromPlatformAtPosition(playerPos.vx,playerPos.vy);

				if(playerPos.vx>=collisionArea.x1&&playerPos.vx<=collisionArea.x2&&
				   playerPos.vy>=collisionArea.y1&&playerPos.vy<=collisionArea.y2)
				//if(((playerArea.x1>=collisionArea.x1&&playerArea.x1<=collisionArea.x2)||(playerArea.x2>=collisionArea.x1&&playerArea.x2<=collisionArea.x2)||(playerArea.x1<=collisionArea.x1&&playerArea.x2>=collisionArea.x2))&&
				   //((playerArea.y1>=thatRect.y1&&playerArea.y1<=thatRect.y2)||(playerArea.y2>=thatRect.y1&&playerArea.y2<=thatRect.y2)||(playerArea.y1<=thatRect.y1&&playerArea.y2>=thatRect.y2)))
				{
					player->setPlatform( this );

					if( height == 0 )
					{
						m_contact = true;
					}
				}

				if ( height < -30 )
				{
					// shock player

					player->takeDamage( DAMAGE__SHOCK_ENEMY, REACT__GET_DIRECTION_FROM_THING, (CThing*)this );
				}
			}

			break;
		}

		default:
			break;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcJellyfishPlatform::think( int _frames )
{
	/*if ( m_contact )
	{
		if ( m_vertScale > -2048 )
		{
			m_vertScale -= 10;
		}

		Pos.vy += 3;
	}
	else if ( m_vertScale < 0 )
	{
		m_vertScale += 10;
	}*/

	if ( m_contact )
	{
		if ( m_dipCount < GameState::getOneSecondInFrames() )
		{
			s16 sineVal = ( m_dipCount << 10 ) / GameState::getOneSecondInFrames();

			m_dipOffset = ( 4 * rcos( sineVal ) ) >> 12;

			m_dipCount += _frames;
		}
		else
		{
			m_dipOffset = 0;
		}
	}
	else
	{
		m_dipCount = 0;
		m_dipOffset = 0;
	}

	Pos.vy += m_dipOffset;

	CNpcLinearPlatform::think( _frames );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcJellyfishPlatform::render()
{
	if ( m_isActive )
	{
		CPlatformThing::render();

		if (canRender())
		{
			DVECTOR &renderPos=getRenderPos();
			//renderPos.vy += m_dipOffset;
			SVECTOR rotation;
			rotation.vx = 0;
			rotation.vy = 0;
			rotation.vz = 0;

			VECTOR scale;
			scale.vx = ONE;
			//scale.vy = ONE + m_vertScale;
			scale.vy = ONE;
			scale.vz = ONE;

			m_modelGfx->Render(renderPos,&rotation,&scale);

#if defined (__USER_paul__) || defined (__USER_charles__)
	DVECTOR	offset = CLevel::getCameraPos();
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

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const CRECT *CNpcJellyfishPlatform::getThinkBBox()
{
	CRECT objThinkBox = getCollisionArea();

	sBBox &thinkBBox = CThingManager::getThinkBBox();
	objThinkBox.x1 = thinkBBox.XMin;
	objThinkBox.x2 = thinkBBox.XMax;
	objThinkBox.y1 = thinkBBox.YMin;
	objThinkBox.y2 = thinkBBox.YMax;

	return &objThinkBox;
}
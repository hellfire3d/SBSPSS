/*=========================================================================

	pfishhk.cpp

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __PLATFORM_PFISHHK_H__
#include "platform\pfishhk.h"
#endif

#ifndef	__UTILS_HEADER__
#include	"utils\utils.h"
#endif

#ifndef __VID_HEADER_
#include "system\vid.h"
#endif

#ifndef __GAME_GAME_H__
#include "game\game.h"
#endif


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcFishHookPlatform::postInit()
{
	CNpcPlatform::postInit();

	m_isMoving = false;
	m_isShuttingDown = false;
	m_lineBase.vx = Pos.vx;
	m_lineBase.vy = 0;

	sBBox boundingBox = m_modelGfx->GetBBox();
	setCollisionCentreOffset( ( boundingBox.XMax + boundingBox.XMin ) >> 1, boundingBox.YMin + 15 );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcFishHookPlatform::processLifetime( int _frames )
{
	if ( m_contact )
	{
		m_isMoving = true;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcFishHookPlatform::processMovement( int _frames )
{
	if ( m_isMoving )
	{
		Pos.vy -= m_data[m_type].speed * _frames;

		if ( Pos.vy < 0 )
		{
			m_isActive = false;
			m_timer = getRnd() % ( 4 * GameState::getOneSecondInFrames() );
			m_timerType = NPC_PLATFORM_TIMER_RESPAWN;
			m_isMoving = false;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcFishHookPlatform::render()
{
	int		x1,y1,x2,y2;

	setOnScreenFlag(false);
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
				setOnScreenFlag(true);
				m_modelGfx->Render(renderPos);
//				POLY_F4	*F4=GetPrimF4();
//				setXYWH(F4,renderPos.vx-32,renderPos.vy-32,64,16);
//				setRGB0(F4,127,127,64);
//				AddPrimToList(F4,2);

#if defined (__USER_paul__) || defined (__USER_charles__)
	DVECTOR size;
	DVECTOR	centre;
	int		halfLength;

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

		x1 = Pos.vx - offset.vx;
		x2 = m_lineBase.vx - offset.vx;

		if ( x1 > x2 )
		{
			int tempX = x1;
			x1 = x2;
			x2 = tempX;
		}

		y1 = Pos.vy - offset.vy;
		y2 = m_lineBase.vy - offset.vy;

		if ( y1 > y2 )
		{
			int tempY = y1;
			y1 = y2;
			y2 = tempY;
		}

		if ( y1 < 0 )
		{
			y1 = 0;
		}
		
		if ( y2 > VidGetScrH() )
		{
			y2 = VidGetScrH();
		}

		if ( x2 >= 0 && x1 <= VidGetScrW() )
		{
			if ( y2 >= 0 && y1 <= VidGetScrH() )
			{
				DrawLine( x1, y1, x2, y2, 0, 0, 0, 0 );
			}
		}
	}
}

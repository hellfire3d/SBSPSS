/*=========================================================================

	hpendulm.cpp

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __HAZARD_HPENDULM_H__
#include "hazard\hpendulm.h"
#endif

#ifndef	__UTILS_HEADER__
#include	"utils\utils.h"
#endif

#ifndef __VID_HEADER_
#include "system\vid.h"
#endif

#ifndef __GAME_GAME_H__
#include	"game\game.h"
#endif

#ifndef	__GFX_OTPOS_H__
#include "gfx\otpos.h"
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcPendulumHazard::init()
{
	CNpcHazard::init();

	m_extendDir = EXTEND_LEFT;
	m_extension = 0;
	m_heading = 1024;
	m_length = 200;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcPendulumHazard::setWaypoints( sThingHazard *ThisHazard )
{
	int pointNum;

	u16	*PntList=(u16*)MakePtr(ThisHazard,sizeof(sThingHazard));

	u16 newXPos, newYPos;

	newXPos = (u16) *PntList;
	PntList++;
	newYPos = (u16) *PntList;
	PntList++;

	DVECTOR startPos;
	startPos.vx = ( newXPos << 4 ) + 8;
	startPos.vy = ( newYPos << 4 ) + 16;

	if ( ThisHazard->PointCount > 1 )
	{
		newXPos = (u16) *PntList;
		PntList++;
		newYPos = (u16) *PntList;
		PntList++;

		DVECTOR pivotPos;
		pivotPos.vx = ( newXPos << 4 ) + 8;
		pivotPos.vy = ( newYPos << 4 ) + 16;

		s32 xDist = startPos.vx - pivotPos.vx;
		s32 yDist = startPos.vy - pivotPos.vy;

		m_maxExtension = 1024 - ratan2( abs( yDist ), abs( xDist ) );

		m_length = isqrt2( ( xDist * xDist ) + ( yDist * yDist ) );

		Pos = pivotPos;
		m_base = pivotPos;
	}
	else
	{
		Pos = startPos;
		m_base = Pos;
	}

	m_thinkArea.x1 = Pos.vx - m_length;
	m_thinkArea.x2 = Pos.vx + m_length;
	m_thinkArea.y1 = Pos.vy - m_length;
	m_thinkArea.y2 = Pos.vy + m_length;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcPendulumHazard::processMovement( int _frames )
{
	if ( m_extendDir == EXTEND_LEFT )
	{
		if ( m_extension > m_maxExtension )
		{
			m_extendDir = EXTEND_RIGHT;
		}
		else
		{
			m_extension += _frames << 3;
		}
	}
	else
	{
		if ( m_extension < -m_maxExtension )
		{
			m_extendDir = EXTEND_LEFT;
		}
		else
		{
			m_extension -= _frames << 3;
		}
	}

	Pos.vx = m_base.vx + ( ( m_length * rcos( m_heading + m_extension ) ) >> 12 );
	Pos.vy = m_base.vy + ( ( m_length * rsin( m_heading + m_extension ) ) >> 12 );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcPendulumHazard::render()
{
	int		x1,y1,x2,y2;
	int		minX, maxX, minY, maxY;

	DVECTOR const &offset = CLevel::getCameraPos();

	CHazardThing::render();

	if (canRender())
	{
		DVECTOR &renderPos=getRenderPos();

		m_modelGfx->Render(renderPos);
	}

	x1 = Pos.vx - offset.vx;
	x2 = m_base.vx - offset.vx;

	y1 = Pos.vy - offset.vy;
	y2 = m_base.vy - offset.vy;

	minX = x1;
	maxX = x2;

	if ( minX > maxX )
	{
		minX = x2;
		maxX = x1;
	}

	minY = y1;
	maxY = y2;

	if ( minY > maxY )
	{
		minY = y2;
		maxY = y1;
	}

	if ( maxX >= 0 && minX <= VidGetScrW() )
	{
		if ( maxY >= 0 && minY <= VidGetScrH() )
		{
			DrawLine( x1, y1, x2, y2, 0, 0, 0, OTPOS__ACTOR_POS+1 );
		}
	}
}

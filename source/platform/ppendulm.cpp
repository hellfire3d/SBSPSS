/*=========================================================================

	ppendulm.cpp

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __PLATFORM_PPENDULM_H__
#include "platform\ppendulm.h"
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

#ifndef	__GFX_OTPOS_H__
#include "gfx\otpos.h"
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcPendulumPlatform::postInit()
{
	sBBox boundingBox = m_modelGfx->GetBBox();
	boundingBox.YMin += 18;
	setCollisionSize( ( boundingBox.XMax - boundingBox.XMin ), ( boundingBox.YMax - boundingBox.YMin ) );
	setCollisionCentreOffset( ( boundingBox.XMax + boundingBox.XMin ) >> 1, ( boundingBox.YMax + boundingBox.YMin ) >> 1 );

	calculateNonRotatedCollisionData();
	setCollisionAngle( m_tiltAngle >> 8 );

	m_extendDir = EXTEND_LEFT;
	m_extension = 0;
	m_heading = 1024;
	m_lineBase.vx = Pos.vx;
	m_lineBase.vy = 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcPendulumPlatform::setWaypoints( sThingPlatform *ThisPlatform )
{
	int pointNum;

	u16	*PntList=(u16*)MakePtr(ThisPlatform,sizeof(sThingPlatform));

	u16 newXPos, newYPos;

	newXPos = (u16) *PntList;
	PntList++;
	newYPos = (u16) *PntList;
	PntList++;

	DVECTOR startPos;
	startPos.vx = ( newXPos << 4 ) + 8;
	startPos.vy = ( newYPos << 4 ) + 16;

	if ( ThisPlatform->PointCount > 1 )
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

		init( pivotPos );
	}
	else
	{
		init( startPos );

		m_length = 200;
	}

	m_thinkArea.x1 = Pos.vx - m_length;
	m_thinkArea.x2 = Pos.vx + m_length;
	m_thinkArea.y1 = Pos.vy - m_length;
	m_thinkArea.y2 = Pos.vy + m_length;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcPendulumPlatform::processMovement( int _frames )
{
	if ( m_extendDir == EXTEND_LEFT )
	{
		if ( m_extension > m_maxExtension )
		{
			m_extendDir = EXTEND_RIGHT;
		}
		else
		{
			m_extension += m_speed * _frames;
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
			m_extension -= m_speed * _frames;
		}
	}

	Pos.vx = m_base.vx + ( ( m_length * rcos( m_heading + m_extension ) ) >> 12 );
	Pos.vy = m_base.vy + ( ( m_length * rsin( m_heading + m_extension ) ) >> 12 );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcPendulumPlatform::render()
{
	int		x1,y1,x2,y2;
	int		x1Boundary,y1Boundary,x2Boundary,y2Boundary;

	DVECTOR const &offset = CLevel::getCameraPos();

	if ( m_isActive )
	{
		CPlatformThing::render();

		if (canRender())
		{
			DVECTOR &renderPos=getRenderPos();

			m_modelGfx->Render(renderPos);
		}

		x1 = x1Boundary = Pos.vx - offset.vx;
		x2 = x2Boundary = m_lineBase.vx - offset.vx;

		y1 = y1Boundary = Pos.vy - offset.vy;
		y2 = y2Boundary = m_lineBase.vy - offset.vy;

		int angle = ratan2( x1 - x2, y1 - y2 );

		if ( y2 < 0 )
		{
			int yDiff = -y2;
			y2 = y2Boundary = 0;
			int hyp = ( yDiff << 12 ) / rcos( angle );
			x2 += ( hyp * rsin( angle ) ) >> 12;
		}
		
		if ( y1 > VidGetScrH() )
		{
			int yDiff = y1 - VidGetScrH();
			y1 = y1Boundary = VidGetScrH();
			int hyp = ( yDiff << 12 ) / rcos( angle );
			x1 -= ( hyp * rsin( angle ) ) >> 12;
		}

		if ( x1Boundary > x2Boundary )
		{
			int tempX = x1Boundary;
			x1Boundary = x2Boundary;
			x2Boundary = tempX;
		}

		if ( y1Boundary > y2Boundary )
		{
			int tempY = y1Boundary;
			y1Boundary = y2Boundary;
			y2Boundary = tempY;
		}

		if ( x2Boundary >= 0 && x1Boundary <= VidGetScrW() )
		{
			if ( y2Boundary >= 0 && y1Boundary <= VidGetScrH() )
			{
				DrawLine( x1, y1, x2, y2, 0, 0, 0, OTPOS__ACTOR_POS+1 );
			}
		}
	}
}

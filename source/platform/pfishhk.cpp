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
	m_isResetting = false;
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
		if ( m_isResetting )
		{
			Pos.vy += 2 * _frames;

			if ( Pos.vy > m_base.vy )
			{
				Pos.vy = m_base.vy;

				m_isResetting = false;
				m_isMoving = false;
			}
		}
		else
		{
			Pos.vy -= m_speed * _frames;

			if ( Pos.vy < 0 )
			{
				m_isResetting = true;
			}
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcFishHookPlatform::render()
{
	int		x1,y1,x2,y2;

	if ( m_isActive )
	{
		CPlatformThing::render();

		DVECTOR &renderPos=getRenderPos();
		if (canRender())
		{
			m_modelGfx->Render(renderPos);
		}
// draw Line (Literally!!)
		if (renderPos.vx>0 && renderPos.vx<512)
		{
			DVECTOR	const	&CamPos=CLevel::getCameraPos();
		
			x1 = renderPos.vx;
			x2 = m_lineBase.vx - CamPos.vx;

			if ( x1 > x2 )
			{
				int tempX = x1;
				x1 = x2;
				x2 = tempX;
			}

			y1 = renderPos.vy;
			y2 = m_lineBase.vy - CamPos.vy;

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
			DrawLine( x1, y1, x2, y2, 0, 0, 0, 0 );
		}
	}

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const CRECT *CNpcFishHookPlatform::getThinkBBox()
{
	CRECT objThinkBox = getCollisionArea();

	sBBox &thinkBBox = CThingManager::getThinkBBox();
	objThinkBox.y2 = thinkBBox.YMin + 1;

	return &objThinkBox;
}
/*=========================================================================

	hflytrap.cpp

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __HAZARD_HFLYTRAP_H__
#include "hazard\hflytrap.h"
#endif

#ifndef __VID_HEADER_
#include "system\vid.h"
#endif

#ifndef __LEVEL_LEVEL_H__
#include "level\level.h"
#endif


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcFlyTrapHazard::init()
{
	CNpcHazard::init();

	m_rotation = 0;
	m_shut = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcFlyTrapHazard::processMovement( int _frames )
{
	if ( m_shut )
	{
		m_rotation += 64 * _frames;

		if ( m_rotation > 1024 )
		{
			m_rotation = 1024;
			m_shut = false;
		}
	}
	else
	{
		m_rotation -= 64 * _frames;

		if ( m_rotation < 0 )
		{
			m_rotation = 0;
			m_shut = true;
		}
	}
	//m_rotation &= 4095;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcFlyTrapHazard::render()
{
	CHazardThing::render();

	if (canRender())
	{
		DVECTOR &renderPos=getRenderPos();

		SVECTOR rotation;
		rotation.vx = m_rotation;
		rotation.vy = 0;
		rotation.vz = 0;

		VECTOR scale;
		scale.vx = ONE;
		scale.vy = ONE;
		scale.vz = ONE;

		m_modelGfx->Render(renderPos,&rotation,&scale);
	}
}

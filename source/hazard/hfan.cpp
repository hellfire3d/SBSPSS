/*=========================================================================

	hfan.cpp

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __HAZARD_HFAN_H__
#include "hazard\hfan.h"
#endif

#ifndef __VID_HEADER_
#include "system\vid.h"
#endif

#ifndef __LEVEL_LEVEL_H__
#include "level\level.h"
#endif


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcFanHazard::init()
{
	CNpcHazard::init();

	m_rotation = 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcFanHazard::processMovement( int _frames )
{
	m_rotation += 256 * _frames;
	m_rotation &= 4095;

	if ( m_soundId == NOT_PLAYING )
	{
		m_soundId = (int) CSoundMediator::playSfx( CSoundMediator::SFX_ELECTRIC_FAN, true, true );
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcFanHazard::render()
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

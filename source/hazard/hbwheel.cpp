/*=========================================================================

	hbwheel.cpp

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __HAZARD_HBWHEEL_H__
#include "hazard\hbwheel.h"
#endif

#ifndef __VID_HEADER_
#include "system\vid.h"
#endif

#ifndef __LEVEL_LEVEL_H__
#include "level\level.h"
#endif


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcBigWheelHazard::init()
{
	CNpcHazard::init();

	m_rotation = 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcBigWheelHazard::processMovement( int _frames )
{
	m_rotation += 5 * _frames;
	m_rotation &= 4095;

	if ( m_soundId == NOT_PLAYING )
	{
		m_soundId = (int) CSoundMediator::playSfx( CSoundMediator::SFX_WORLD_OBJECT__BIG_WHEEL, true );
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcBigWheelHazard::render()
{
	CHazardThing::render();

	if (canRender())
	{
		DVECTOR &renderPos=getRenderPos();

		SVECTOR rotation;
		rotation.vx = 0;
		rotation.vy = 0;
		rotation.vz = m_rotation;

		VECTOR scale;
		scale.vx = ONE;
		scale.vy = ONE;
		scale.vz = ONE;

		m_modelGfx->Render(renderPos,&rotation,&scale);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcBigWheelHazard::collidedWith( CThing *_thisThing )
{
	// do not collide
}

/*=========================================================================

	hinert.cpp

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __HAZARD_HINERT_H__
#include "hazard\hinert.h"
#endif

#ifndef __VID_HEADER_
#include "system\vid.h"
#endif

#ifndef __LEVEL_LEVEL_H__
#include "level\level.h"
#endif


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcInertHazard::render()
{
	CHazardThing::render();

	if (canRender())
	{
		DVECTOR &renderPos=getRenderPos();

		m_modelGfx->Render( renderPos );
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcInertHazard::collidedWith( CThing *_thisThing )
{
	// do not collide
}

/*=========================================================================

	pclam.cpp

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __PLATFORM_PCLAM_H__
#include "platform\pclam.h"
#endif

#ifndef __LEVEL_LEVEL_H__
#include "level\level.h"
#endif

#ifndef	__UTILS_HEADER__
#include	"utils\utils.h"
#endif


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcClamPlatform::postInit()
{
	sBBox boundingBox = m_modelGfx->GetBBox();
	setCollisionSize( 80, 30 );
	setCollisionCentreOffset( 0, -15 );

	calculateNonRotatedCollisionData();
	setCollisionAngle( m_tiltAngle >> 8 );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcClamPlatform::render()
{
	CPlatformThing::render();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*void CNpcClamPlatform::setBBox()
{
	//setCollisionSize( 54, PLATFORMCOLLISIONHEIGHT);
	//setCollisionSize( 54, 10 );
	setCollisionCentreOffset( 50, -100 );
}*/
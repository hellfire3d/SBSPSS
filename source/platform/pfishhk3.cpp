/*=========================================================================

	pfishhk3.cpp

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __PLATFORM_PFISHHK3_H__
#include "platform\pfishhk3.h"
#endif

void CNpcFishHook3Platform::postInit()
{
	sBBox boundingBox = m_modelGfx->GetBBox();
	boundingBox.YMin += 12;
	setCollisionSize( ( boundingBox.XMax - boundingBox.XMin ), ( boundingBox.YMax - boundingBox.YMin ) );
	setCollisionCentreOffset( ( boundingBox.XMax + boundingBox.XMin ) >> 1, ( boundingBox.YMax + boundingBox.YMin ) >> 1 );

	calculateNonRotatedCollisionData();
	setCollisionAngle( m_tiltAngle >> 8 );

	m_isMoving = false;
	m_isResetting = false;
	m_lineBase.vx = Pos.vx;
	m_lineBase.vy = 0;
}

/*=========================================================================

	plurve.cpp

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __PLATFORM_PLURVE_H__
#include "platform\plurve.h"
#endif


void CNpcLoveBoatPlatform::postInit()
{
	CNpcGhostTrainPlatform::postInit();

	sBBox boundingBox = m_modelGfx->GetBBox();
	boundingBox.YMin += 32;
	setCollisionSize( ( boundingBox.XMax - boundingBox.XMin ), ( boundingBox.YMax - boundingBox.YMin ) );
	setCollisionCentreOffset( ( boundingBox.XMax + boundingBox.XMin ) >> 1, ( boundingBox.YMax + boundingBox.YMin ) >> 1 );
	calculateNonRotatedCollisionData();
}

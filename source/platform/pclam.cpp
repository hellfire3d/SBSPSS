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

void CNpcClamPlatform::render()
{
	if (canRender())
	{
		//CNpcPlatform::render();

		DVECTOR &renderPos=getRenderPos();

	#if defined (__USER_paul__) || defined (__USER_charles__)
		DVECTOR	offset = CLevel::getCameraPos();
		DVECTOR size;
		DVECTOR	centre;
		int		halfLength;
		int		x1,y1,x2,y2;

		centre=getCollisionCentre();
		size=getCollisionSize();
		halfLength=size.vx>>1;

		x1=-halfLength*mcos(getCollisionAngle()&4095)>>12;
		y1=-halfLength*msin(getCollisionAngle()&4095)>>12;
		x2=+halfLength*mcos(getCollisionAngle()&4095)>>12;
		y2=+halfLength*msin(getCollisionAngle()&4095)>>12;

		centre.vx-=offset.vx;
		centre.vy-=offset.vy;
		x1+=centre.vx;
		y1+=centre.vy;
		x2+=centre.vx;
		y2+=centre.vy;

		DrawLine(x1,y1,x2,y2,0,255,0,0);
	#endif
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcClamPlatform::setBBox()
{
	setCollisionSize( 54, PLATFORMCOLLISIONHEIGHT);
	setCollisionCentreOffset( 0, -30 );
}
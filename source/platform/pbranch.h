/*=========================================================================

	pbranch.h

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __PLATFORM_PBRANCH_H__
#define __PLATFORM_PBRANCH_H__

#ifndef __PLATFORM_PLATFORM_H__
#include "platform\platform.h"
#endif

class CNpcBranchPlatform : public CNpcPlatform
{
public:
	virtual void		postInit();
	virtual void		render();
	virtual int			getHeightFromPlatformAtPosition(int _x,int _y, int offsetX = 0, int offsetY = 0);
protected:
	virtual void		setWaypoints( sThingPlatform *ThisPlatform );
	virtual void		processMovement( int _frames );
	virtual sBBox		&getBBox();

	//virtual void		collidedWith(CThing *_thisThing);
	//virtual void		calculateBoundingBoxSize();
	//virtual int			getHeightFromPlatformAtPosition(int _x,int _y, int offsetX = 0, int offsetY = 0);

	s32					m_angularVelocity;
	u8					m_reversed;
	sBBox				m_boundingBox;
};

#endif
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
	void				postInit();
	void				render();
	CRECT const			*getThinkBBox()												{return( CThing::getThinkBBox() );}
protected:
	void				setWaypoints( sThingPlatform *ThisPlatform );
	void				processMovement( int _frames );

	s32					m_angularVelocity;
	u8					m_reversed;
	sBBox				m_boundingBox;
};

#endif
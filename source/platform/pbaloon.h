/*=========================================================================

	pbaloon.h

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __PLATFORM_PBALOON_H__
#define __PLATFORM_PBALOON_H__

#ifndef __PLATFORM_PLATFORM_H__
#include "platform\platform.h"
#endif

class CBridgeBalloon
{
public:
	void				setPos( DVECTOR const &newPos )			{Pos = newPos;}
	void				render();
	void				init();
	void				shutdown();
	int					checkCollisionAgainst(CThing *_thisThing, int _frames);
protected:
	DVECTOR							Pos;
};

class CNpcBalloonBridgePlatform : public CNpcPlatform
{
public:
	void				postInit();
	void				render();
	void				shutdown();
	CRECT const			*getThinkBBox()									{return( CThing::getThinkBBox() );}
protected:
	void				setWaypoints( sThingPlatform *ThisPlatform );
	void				processMovement( int _frames );
	int					checkCollisionAgainst(CThing *_thisThing, int _frames);

	s32					m_maxExtension;
	s8					m_balloonCount;
	s32					m_targetHeight;

	enum
	{
		BRIDGE_NUM_BALLOONS = 4,
	};

	CBridgeBalloon		*m_balloon[BRIDGE_NUM_BALLOONS];
};

#endif
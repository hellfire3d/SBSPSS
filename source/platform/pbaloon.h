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
	virtual void		setPos( DVECTOR newPos )						{Pos = newPos;}
	void				render();
	void				init();
	void				shutdown();
	virtual int			checkCollisionAgainst(CThing *_thisThing, int _frames);
protected:
	DVECTOR							Pos;
	SpriteBank						*m_spriteBank;
};

class CNpcBalloonBridgePlatform : public CNpcPlatform
{
public:
	virtual void		postInit();
	virtual void		render();
	virtual void		shutdown();
protected:
	virtual void		setWaypoints( sThingPlatform *ThisPlatform );
	virtual void		processMovement( int _frames );
	virtual int			checkCollisionAgainst(CThing *_thisThing, int _frames);

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
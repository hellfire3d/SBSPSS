/*=========================================================================

	pfishhk.h

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __PLATFORM_PFISHHK_H__
#define __PLATFORM_PFISHHK_H__

#ifndef __PLATFORM_PLATFORM_H__
#include "platform\platform.h"
#endif

class CNpcFishHookPlatform : public CNpcPlatform
{
public:
	virtual void		postInit();
	void				render();
	CRECT const			*getThinkBBox();
	bool				alwaysThink()								{return(false);}
	bool				allowOffMap()								{return (true);}

protected:
	void				processLifetime( int _frames );
	void				processMovement( int _frames );

	u8					m_isMoving;
	u8					m_isResetting;
	DVECTOR				m_lineBase;
	s32					m_bobTimer;
};

#endif
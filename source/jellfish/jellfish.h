/*=========================================================================

	jellfish.h

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __JELLFISH_JELLFISH_H__
#define __JELLFISH_JELLFISH_H__

#ifndef _GLOBAL_HEADER_
#include "system\global.h"
#endif

#ifndef __LEVEL_LEVEL_H__
#include "level\level.h"
#endif

class CJellyfishGenerator
{
public:
	static void			init();
	static void			think( int _frames, CLevel *level );
	static void			decCounter();
	static void			start()										{m_on=true;}
protected:
	static u8			m_jellyfishCount;
	static s32			m_timer;
	static u8			m_on;
	static u8			m_level;
};

#endif
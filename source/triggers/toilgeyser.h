/*=========================================================================

	toilgeysor

	Author:		Dave - based on the original story by CB
	Created:	
	Project:	Spongebob
	Purpose:	

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef	__TRIGGERS_TOILGEYSER_H__
#define __TRIGGERS_TOILGEYSER_H__

/*----------------------------------------------------------------------
	Includes
	-------- */

#ifndef __THING_THING_H__
#include "thing/thing.h"
#endif

#ifndef __TRIGGER_TRIGGER_HEADER__
#include "triggers\trigger.h"
#endif

#include "fx\fx.h"

/*	Std Lib
	------- */

/*----------------------------------------------------------------------
	Tyepdefs && Defines
	------------------- */

/*----------------------------------------------------------------------
	Structure defintions
	-------------------- */
class	CFXGeyser;
class COilGeyserTrigger: public CTrigger
{
public:
	virtual void		think(int _frames);
	virtual void		setPositionAndSize(int _x,int _y,int _w,int _h);
protected:
	s32					m_timer;
	CFXGeyser			*m_FX;
	int					m_Height;
};

#endif

/*===========================================================================
 end */

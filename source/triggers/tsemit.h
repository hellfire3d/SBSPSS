/*=========================================================================

	tsemit.h

	Author:		CRB
	Created:	
	Project:	Spongebob
	Purpose:	

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef	__TRIGGERS_TSEMIT_H__
#define __TRIGGERS_TSEMIT_H__

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
#include "fx\fxbaseanim.h"

/*	Std Lib
	------- */

/*----------------------------------------------------------------------
	Tyepdefs && Defines
	------------------- */

/*----------------------------------------------------------------------
	Structure defintions
	-------------------- */

class CSteamEmitterTrigger : public CTrigger
{
public:
	void				init();
	void				think(int _frames);
	void				shutdown();
	virtual void		setPositionAndSize(int _x,int _y,int _w,int _h);
protected:
	CFX					*m_effect;
	int					m_soundId;
};

/*----------------------------------------------------------------------
	Globals
	------- */

/*----------------------------------------------------------------------
	Functions
	--------- */

/*---------------------------------------------------------------------- */

#endif

/*===========================================================================
 end */

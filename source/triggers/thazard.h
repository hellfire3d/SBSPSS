/*=========================================================================

	thazard.h

	Author:		Charles Blair
	Created:	
	Project:	Spongebob
	Purpose:	

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef	__TRIGGERS_THAZARD_H__
#define __TRIGGERS_THAZARD_H__

/*----------------------------------------------------------------------
	Includes
	-------- */

#ifndef __THING_THING_H__
#include "thing/thing.h"
#endif

#ifndef __TRIGGERS_TRIGGER_H__
#include "triggers\trigger.h"
#endif

#ifndef __HAZARD_HAZARD_H__
#include "hazard\hazard.h"
#endif


/*	Std Lib
	------- */

/*----------------------------------------------------------------------
	Tyepdefs && Defines
	------------------- */

/*----------------------------------------------------------------------
	Structure defintions
	-------------------- */

class CHazardTrigger : public CTrigger
{
public:
	virtual void		init();
	virtual void		think(int _frames);
	virtual void		setHazard( CNpcHazard *hazard )			{m_hazard = hazard;}

protected:
	virtual void		collidedWith(CThing *_thisThing);

	CNpcHazard			*m_hazard;
	s32					m_timeout;
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

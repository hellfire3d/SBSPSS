/*=========================================================================

	twater.h

	Author:		PKG
	Created:	
	Project:	Spongebob
	Purpose:	

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef	__TRIGGERS_TWATER_H__
#define __TRIGGERS_TWATER_H__

/*----------------------------------------------------------------------
	Includes
	-------- */

#ifndef __THING_THING_H__
#include "thing/thing.h"
#endif

#ifndef __TRIGGERS_TRIGGER_H__
#include "triggers\trigger.h"
#endif


/*	Std Lib
	------- */

/*----------------------------------------------------------------------
	Tyepdefs && Defines
	------------------- */

/*----------------------------------------------------------------------
	Structure defintions
	-------------------- */

class CInWaterTrigger : public CTrigger
{
protected:
	virtual void		collidedWith(CThing *_thisThing);

};

class COutOfWaterTrigger : public CTrigger
{
protected:
	virtual void		collidedWith(CThing *_thisThing);

};

/*----------------------------------------------------------------------
	Globals
	------- */

/*----------------------------------------------------------------------
	Functions
	--------- */

/*---------------------------------------------------------------------- */

#endif	/* __TRIGGERS_TWATER_H__ */

/*===========================================================================
 end */

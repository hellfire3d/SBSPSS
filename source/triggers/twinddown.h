/*=========================================================================

	twinddown.h

	Author:		Charles Blair
	Created:	
	Project:	Spongebob
	Purpose:	

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef	__TRIGGERS_TWINDDOWN_H__
#define __TRIGGERS_TWINDDOWN_H__

/*----------------------------------------------------------------------
	Includes
	-------- */

#ifndef __TRIGGER_TRIGGER_HEADER__
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

class CWindDownTrigger : public CTrigger
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

#endif

/*===========================================================================
 end */

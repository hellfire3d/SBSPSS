/*=========================================================================

	tlevexit.h

	Author:		PKG
	Created:	
	Project:	Spongebob
	Purpose:	

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef	__TRIGGERS_TLEVEXIT_H__
#define __TRIGGERS_TLEVEXIT_H__

/*----------------------------------------------------------------------
	Includes
	-------- */

#ifndef __THING_THING_H__
#include "thing/thing.h"
#endif


/*	Std Lib
	------- */

/*----------------------------------------------------------------------
	Tyepdefs && Defines
	------------------- */

/*----------------------------------------------------------------------
	Structure defintions
	-------------------- */

class CLevelExitTrigger : public CTriggerThing
{
public:
	void				setExitPosition(int _x,int _y,int _w,int _h);

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

#endif	/* __TRIGGERS_TLEVEXIT_H__ */

/*===========================================================================
 end */

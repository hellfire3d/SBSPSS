/*=========================================================================

	tgstop.h

	Author:		Charles Blair
	Created:	
	Project:	Spongebob
	Purpose:	

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef	__TRIGGERS_TGSTOP_H__
#define __TRIGGERS_TGSTOP_H__

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

class CGaryStopTrigger : public CTrigger
{
public:
#if defined (__USER_art__) || defined (__USER_sbart__)
	virtual void		render();
#endif
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

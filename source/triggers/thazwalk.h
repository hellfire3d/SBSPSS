/*=========================================================================

	thazwalk.h

	Author:		Charles Blair
	Created:	
	Project:	Spongebob
	Purpose:	

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef	__TRIGGERS_THAZWALK_H__
#define __TRIGGERS_THAZWALK_H__

/*----------------------------------------------------------------------
	Includes
	-------- */

#ifndef __TRIGGERS_THAZARD_H__
#include "triggers\thazard.h"
#endif

/*	Std Lib
	------- */

/*----------------------------------------------------------------------
	Tyepdefs && Defines
	------------------- */

/*----------------------------------------------------------------------
	Structure defintions
	-------------------- */

class CHazardWalkTrigger : public CHazardTrigger
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

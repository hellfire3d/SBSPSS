/*=========================================================================

	tbgeyser.h

	Author:		CRB
	Created:	
	Project:	Spongebob
	Purpose:	

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef	__TRIGGERS_TBGEYSER_H__
#define __TRIGGERS_TBGEYSER_H__

/*----------------------------------------------------------------------
	Includes
	-------- */

#ifndef __THING_THING_H__
#include "thing/thing.h"
#endif

#ifndef __TRIGGERS_TBEMIT_H__
#include "triggers\tbemit.h"
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

class CBubbleGeyserEmitterTrigger : public CBubbleEmitterTrigger
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

/*=========================================================================

	tobemit.h

	Author:		CRB
	Created:	
	Project:	Spongebob
	Purpose:	

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef	__TRIGGERS_TOBEMIT_H__
#define __TRIGGERS_TOBEMIT_H__

/*----------------------------------------------------------------------
	Includes
	-------- */

#ifndef __THING_THING_H__
#include "thing/thing.h"
#endif

#ifndef __TRIGGERS_TBEMIT_H__
#include "triggers\tbemit.h"
#endif

/*	Std Lib
	------- */

/*----------------------------------------------------------------------
	Tyepdefs && Defines
	------------------- */

/*----------------------------------------------------------------------
	Structure defintions
	-------------------- */

class COilBubbleEmitterTrigger : public CBubbleEmitterTrigger
{
public:
	virtual void		think(int _frames);
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
